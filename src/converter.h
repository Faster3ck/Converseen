/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2025
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
* Contact e-mail: Francesco Mondello <faster3ck@gmail.com>
*
*/

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDialog>
#include <QObject>
#include <Magick++.h>
#include <string>
#include <iostream>

#include "globals.h"
#include "magickdefine.h"

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
    void setResize(const double &width, const double &height, const bool &percent, const bool &maintainAspectRatio);
    void setRotation(double deg);
    void setFlip(FlipOrientation orientation);
    void setDensity(QString densityStr);
    void setBackgroundColor(QString bg_color, bool changeBg_color);
    void setOverwrite(bool overwrite);
    void setNewBasename(QString newBaseName, bool ok);
    void setResamplingFilter(IMFilterType resamplingFilter);
    void setMagickDefines(const QList<MagickDefine> &magickDefines);
    void setRemoveMetadata(const bool &value);
    void stopProcess();

    QMutex mutex;
private:
    void resize(Image &my_image);
    void rotate(Image &my_image);
    void flip(Image &my_image);
    bool writeImage(Image &my_image, const QString &format, const int &quality, const QString &out, QString &error_status);
    Image convertPDFtoImage(const Image &my_image);
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

    double m_width;
    double m_height;
    bool m_percent;
    bool m_maintainAspectRatio;

    bool m_resize;
    bool m_overwrite;
    bool m_allow_rename;
    bool m_density;
    bool m_rotation;
    bool m_process_stopped;

    int m_conv_status;  // 1 = processed; 2 = jump/unchecked; -1 = Error;

    QString m_renamingString;
    QString m_densityString;

    QString m_newBaseName;

    QWaitCondition imageCondition;

    IMFilterType m_resamplingFilter;
    QList<MagickDefine> m_magickDefines;
    bool m_removeMetadata;

signals:
    void nextConversion(int);
    void requestOverwrite(QString);
    void errorMessage(QString);
private slots:
    void conversionCompleted();
};

#endif // __CONVERTER_H__
