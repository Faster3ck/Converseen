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

#include <QDir>
#include "translator.h"
#include "whereiam.h"

#define INIFILENAME ".converseen.conf"

QString dataDir;
QString language;

Translator::Translator()
{
    WhereIAm w;
    dataDir = w.dataDir();

    QString iniPath = QDir::homePath() + "/" + INIFILENAME;

#ifdef Q_WS_WIN
    if ((QSysInfo::windowsVersion() == QSysInfo::WV_VISTA) || (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7)) {
        iniPath = QDir::homePath() + "/AppData/Roaming/Converseen/" + INIFILENAME;
    }
#endif

    settings = new QSettings(iniPath, QSettings::IniFormat);
}

QTranslator *Translator::translation()
{
    QTranslator *transl = new QTranslator;
    QString translationFile = QString("%1/converseen/"+loadCurrentTranslationName()).arg(dataDir);
    transl->load(translationFile);

    return(transl);
}

QString Translator::loadCurrentTranslationName()
{
    QString language = "English";
    if (settings->contains("Options/language")) {
        language = settings->value("Options/language").value<QString>();
    }
    return(language);
}

void Translator::saveSettings(QString language)
{
    if (language != "English") {
        QString lang = language;

        settings->setValue("Options/language", lang);
    }
    else
        settings->setValue("Options/language", "English");
}

QList<QPair<QString, QString> > Translator::loadTranslationFiles()
{
    language = tr("English");
    QTranslator transl;

    QDir trPath(QString("%1/converseen").arg(dataDir));
    QStringList fileNames = trPath.entryList(QStringList() << "*.qm", QDir::Files, QDir::Name);

    QList<QPair<QString, QString> > langsList;
    QPair<QString, QString> lang;

    for (int i = 0; i < fileNames.count(); i++) {
        lang.first = fileNames.at(i);

        transl.load(fileNames.at(i), QString("%1/converseen").arg(dataDir));
        lang.second = transl.translate("Translator", "English");

        langsList << lang;
    }
    return langsList;
}

QString Translator::currentLanguage()
{
    return language;
}
