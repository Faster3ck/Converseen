/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2015
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

#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <QObject>
#include <QSettings>

class IniSettings : public QObject
{
    Q_OBJECT
public:
    explicit IniSettings(QObject *parent = 0);

    static void init();

    static QString outputDir();
    static QByteArray windowGeometry();
    static int latestWrittenFormatIndex();
    static bool isOverwriteMode();
    static int jpgQuality();
    static int pngQuality();
    static QString latestOpenedDir();
    static QString backgroundColor();

    static bool previewChecked();
    static bool scaleChecked();
    static int measure();    // get px or %
    static bool aspectRatioChecked();
    static bool resolutionChecked();
    static bool imageDirChecked();
    static bool renameChecked();
    static bool bgColorChecked();
    static int latestInterpFiltIdx();
    static int currentVersion();
	static QString language();
    static bool isAutoChechUpdates();

    static void setOutputDir(QString path);
    static void setWindowGeometry(QByteArray geometry);
    static void setLatestWrittenFormatIndex(int index);
    static void setOverwriteMode(bool status);
    static void setJpgQuality(int quality);
    static void setPngQuality(int quality);
    static void setLatestOpenedDir(QString path);
    static void setBackgroundColor(QString bgColor);
    static void setPreviewChecked(bool state);
    static void setScaleChecked(bool state);
    static void setMeasure(int index);
    static void setAspectRatioChecked(bool state);
    static void setResolutionChecked(bool state);
    static void setImageDirChecked(bool state);
    static void setRenameChecked(bool state);
    static void setBgColorChecked(bool state);
    static void setLatestInterpFiltIdx(int index);
    static void setCurrentVersion(int version);
	static void setLanguage(QString lang);
    static void setAutoChechUpdates(bool enabled);

//private:
    static QSettings *settings;
signals:

public slots:

};

#endif // INISETTINGS_H
