/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2013
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Faster <faster3ck@gmail.com>
*
*/

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <QtGui>
#include <QObject>
#include <Magick++.h>
#include <string>
#include <iostream>

using namespace Magick;
using namespace std;

enum FlipOrientation { VERTICAL, HORIZONTAL };

class Converter : public QThread
{
    Q_OBJECT
public:
    Converter(QObject *parent);
    void reset();
    ~Converter();
    void run();

    void setInputPicture(QString fileName);
    void setOutputPictureName(QString fileName);
    void setFormat(QString format);
    void setQuality(int quality);
    void setOutputDir(QString outputDir);
    void setResize(QString resizingStr);
    void setRotation(double deg);
    void setFlip(FlipOrientation orientation);
    void setDensity(QString densityStr);
    void setBackgroundColor(QString bg_color, bool changeBg_color);
    void setOverwrite(bool overwrite);
    void setNewBasename(QString newBaseName, bool ok);
    void setResamplingFilter(FilterTypes resamplingFilter);
    void stopProcess();

    QMutex mutex;
private:
    void resize(Image &my_image);
    void changeDensity(Image &my_image);
    void rotate(Image &my_image);
    void flip(Image &my_image);
    bool writeImage(Image &my_image, QString format, int quality, QString out);
    QString overwriteOldFileName(QString out);

    QString m_fileNameIn;
    QString m_fileNameOut;
    QString m_format;
    int m_quality;
    QString m_outputDir;
    QString m_bg_color;
    bool m_changeBg_color;
    double m_rotation_deg;
    bool m_flip;
    FlipOrientation m_orientation;

    bool m_zoom;
    bool m_overwrite;
    bool m_allow_rename;
    bool m_density;
    bool m_rotation;
    bool m_process_stopped;

    int m_conv_status;  // 1 = processed; 2 = jump/unchecked; -1 = Error;

    QString m_renamingString;

    QString resizingString;
    QString m_densityString;

    QString m_newBaseName;

    QWaitCondition imageCondition;

    FilterTypes m_resamplingFilter;
signals:
    void nextConversion(int);
    void requestOverwrite(QString);
    void errorMessage(QString);
private slots:
    void conversionCompleted();
};

#endif // __CONVERTER_H__
