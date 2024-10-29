/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2024
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

#include <QLibraryInfo>
#include <QLocale>
#include <QDir>
#include <QStorageInfo>
#include <QCoreApplication>
#include <QStandardPaths>

#include "translator.h"
#include "inisettings.h"

#define INIFILENAME ".converseen.conf"

QString dataDir;
QString language;

Translator::Translator()
{
    IniSettings::init();
    m_loc = findLangDir();
}

QTranslator *Translator::translation()
{
    QTranslator *transl = new QTranslator;
    QString translationFile = QString("%1/%2").arg(m_loc).arg(loadCurrentTranslationName());
    //QString translationFileQt = QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "qt_" + QLocale::system().name();

    QFileInfo fi(translationFile);

    if (fi.exists()) {
        transl->load(translationFile);
        return transl;
    }
    else
        return NULL;

    //
    /*transl->load("qt_" + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath));*/

    //qDebug() << "Locations " << QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);


}

QString Translator::loadCurrentTranslationName()
{
    return IniSettings::language();
}

QString Translator::findLangDir()
{
    QString binDir = QCoreApplication::applicationDirPath();
    QString langDir = "";

    QStorageInfo storage(qApp->applicationDirPath());
    QString rootPath = storage.rootPath();

#if defined(Q_OS_LINUX)
    QStringList langDirs;

    langDirs.append(QString("%1/usr/share/converseen/loc").arg(rootPath));
    langDirs.append(QString("%1/usr/local/share/converseen/loc").arg(rootPath));
    langDirs.append(QString("%1/share/converseen/loc").arg(rootPath));
    langDirs.append(QString("%1/local/share/converseen/loc").arg(rootPath));
    langDirs.append(QString("%1/usr/share/locale/converseen/loc").arg(rootPath));
    langDirs.append(QString("%1/usr/share/converseen/loc").arg(QString::fromLocal8Bit(qgetenv("SNAP"))));   // For snapcraft loc search

    foreach (langDir, langDirs) {
        if (qmFilesFound(langDir))
            return langDir;
    }
#elif defined(Q_OS_MACOS)
    langDir = QString("%1/../Resources/loc").arg(binDir);
#else
    langDir = QString("%1/loc").arg(binDir);
#endif

    return langDir;
}

bool Translator::qmFilesFound(const QString &path)
{
    QDir d(path);
    QStringList entryList = d.entryList(QStringList() << "*.qm", QDir::Files);

    if (entryList.count() > 0)
        return true;
    else
        return false;
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
