/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2018
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
#include "inisettings.h"

#define INIFILENAME ".converseen.conf"

QString dataDir;
QString language;

Translator::Translator()
{
    IniSettings::init();

    WhereIAm w;
    dataDir = w.dataDir();

#ifdef Q_OS_LINUX
	m_loc = QString("%1/converseen/loc").arg(dataDir);
#else
	m_loc = QString("%1/loc").arg(dataDir);
#endif
}

QTranslator *Translator::translation()
{
    QTranslator *transl = new QTranslator;
    QString translationFile = QString("%1/%2").arg(m_loc).arg(loadCurrentTranslationName());
    transl->load(translationFile);

    return(transl);
}

QString Translator::loadCurrentTranslationName()
{
    return IniSettings::language();
}

void Translator::saveSettings(QString language)
{
    if (language != "English") {
        QString lang = language;

		IniSettings::setLanguage(lang);
    }
    else
		IniSettings::setLanguage("English");
}

QList<QPair<QString, QString> > Translator::loadTranslationFiles()
{
    language = tr("English");
    QTranslator transl;

    QDir trPath(m_loc);
    QStringList fileNames = trPath.entryList(QStringList() << "*.qm", QDir::Files, QDir::Name);

    QList<QPair<QString, QString> > langsList;
    QPair<QString, QString> lang;

    for (int i = 0; i < fileNames.count(); i++) {
        lang.first = fileNames.at(i);

        transl.load(fileNames.at(i), m_loc);
        lang.second = transl.translate("Translator", "English");

        langsList << lang;
    }
    return langsList;
}

QString Translator::currentLanguage()
{
    return language;
}
