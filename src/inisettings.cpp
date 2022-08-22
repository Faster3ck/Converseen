/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2022
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

#include <QDir>
#include <QStandardPaths>
#include "inisettings.h"

#define INIFILENAME ".converseen.conf"

QSettings *IniSettings::settings;

IniSettings::IniSettings(QObject *parent) :
    QObject(parent)
{

}

void IniSettings::init()
{
    QString configLocationPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QString iniPath = QString("%1/converseen/%2").arg(configLocationPath).arg(INIFILENAME);

#if defined(Q_OS_WIN)
	QDir portableConfigDir(QDir::currentPath() + "/settings");

	if (portableConfigDir.exists())
        configLocationPath = portableConfigDir.absolutePath();

    iniPath = QString("%1/%2").arg(configLocationPath).arg(INIFILENAME);
#endif

    settings = new QSettings(iniPath, QSettings::IniFormat);
}

QString IniSettings::outputDir()
{
    QString myDir = QDir::homePath();
    if (settings->contains("General/Save directory"))
        myDir = settings->value("General/Save directory").toString();

    return myDir;
}

QByteArray IniSettings::windowGeometry()
{
    QByteArray geometry = "";
    if (settings->contains("Options/WindowSize"))
        geometry = settings->value("Options/WindowSize").toByteArray();

    return geometry;
}

int IniSettings::latestWrittenFormatIndex()
{
    return settings->value("General/Last Write Format").toInt();
}

bool IniSettings::isOverwriteMode()
{
    return settings->value("Options/Overwrite mode").toBool();
}

int IniSettings::jpgQuality()
{
    int jpgQuality = 75;

    if (settings->contains("Quality/Jpeg quality"))
        jpgQuality = settings->value("Quality/Jpeg quality").toInt();

    return jpgQuality;
}

int IniSettings::pngQuality()
{
    int pngQuality = 7;

    if (settings->contains("Quality/Png quality"))
        pngQuality = settings->value("Quality/Png quality").toInt();

    return pngQuality;
}

QString IniSettings::latestOpenedDir()
{
    QString path = QDir::homePath();
    if (settings->contains("General/Last Opened"))
        path = settings->value("General/Last Opened").toString();

    return path;
}

QString IniSettings::backgroundColor()
{
    QString bgColor = "#000000";
    if (settings->contains("Colors/background"))
        bgColor = settings->value("Colors/background").toString();

    return bgColor;
}

bool IniSettings::previewChecked()
{
    bool checked = true;
    if (settings->contains("Options/Preview checked"))
        checked = settings->value("Options/Preview checked").toBool();

    return checked;
}

bool IniSettings::scaleChecked()
{
    return settings->value("Options/Scale checked").toBool();
}

int IniSettings::measure()    // get px or %
{
    int idx = 0;
    if (settings->contains("Options/Measure"))
        idx = settings->value("Options/Measure").toInt();

    return idx;
}

bool IniSettings::aspectRatioChecked()
{
    return settings->value("Options/Aspectratio checked").toBool();
}

bool IniSettings::resolutionChecked()
{
    return settings->value("Options/Resolution checked").toBool();
}

bool IniSettings::imageDirChecked()
{
    return settings->value("Options/ImageDir checked").toBool();
}

bool IniSettings::renameChecked()
{
    return settings->value("Options/Rename checked").toBool();
}

bool IniSettings::bgColorChecked()
{
    return settings->value("Options/BgColor checked").toBool();
}

int IniSettings::latestInterpFiltIdx()
{
    int idx = 0;
    if (settings->contains("Options/Interpolation filter"))
        idx = settings->value("Options/Interpolation filter").toInt();

    return idx;
}

int IniSettings::currentVersion()
{
    int version = 0;

    if (settings->contains("General/Current version"))
        version = settings->value("General/Current version").toInt();

    return version;
}

QString IniSettings::language()
{
	QString language = "English";
    if (settings->contains("Options/language")) {
        language = settings->value("Options/language").value<QString>();
    }

    return language;
}

bool IniSettings::isAutoChechUpdates()
{
    bool enabled;

    if (settings->contains("Options/Check updates")) {
        enabled = settings->value("Options/Check updates").value<bool>();
    }
    else
        enabled = true;

    return enabled;
}

//

void IniSettings::setOutputDir(QString path)
{
    settings->setValue("General/Save directory", path);
}

void IniSettings::setWindowGeometry(QByteArray geometry)
{
    settings->setValue("Options/WindowSize", geometry);
}

void IniSettings::setLatestWrittenFormatIndex(int index)
{
    settings->setValue("General/Last Write Format", index);
}

void IniSettings::setOverwriteMode(bool enabled)
{
    settings->setValue("Options/Overwrite mode", enabled);
}

void IniSettings::setJpgQuality(int quality)
{
    settings->setValue("Quality/Jpeg quality", quality);
}

void IniSettings::setPngQuality(int quality)
{
    settings->setValue("Quality/Png quality", quality);
}

void IniSettings::setLatestOpenedDir(QString path)
{
    settings->setValue("General/Last Opened", path);
}

void IniSettings::setBackgroundColor(QString bgColor)
{
    settings->setValue("Colors/background", bgColor);
}

void IniSettings::setPreviewChecked(bool state)
{
    settings->setValue("Options/Preview checked", state);
}

void IniSettings::setScaleChecked(bool state)
{
    settings->setValue("Options/Scale checked", state);
}

void IniSettings::setMeasure(int index)    // set px or %
{
    settings->setValue("Options/Measure", index);
}

void IniSettings::setAspectRatioChecked(bool state)
{
    settings->setValue("Options/Aspectratio checked", state);
}

void IniSettings::setResolutionChecked(bool state)
{
    settings->setValue("Options/Resolution checked", state);
}

void IniSettings::setImageDirChecked(bool state)
{
    settings->setValue("Options/ImageDir checked", state);
}

void IniSettings::setRenameChecked(bool state)
{
    settings->setValue("Options/Rename checked", state);
}

void IniSettings::setBgColorChecked(bool state)
{
    settings->setValue("Options/BgColor checked", state);
}

void IniSettings::setLatestInterpFiltIdx(int index)
{
    settings->setValue("Options/Interpolation filter", index);
}

void IniSettings::setCurrentVersion(int version)
{
    settings->setValue("General/Current version", version);
}

void IniSettings::setLanguage(QString lang)
{
	settings->setValue("Options/language", lang);
}

void IniSettings::setAutoChechUpdates(bool enabled)
{
    settings->setValue("Options/Check updates", enabled);
}
