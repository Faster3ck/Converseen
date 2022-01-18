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

#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <QSettings>
#include <QTranslator>
#include <QList>
#include <QPair>

class Translator : public QObject
{
    Q_OBJECT
public:
        Translator();
        QTranslator *translation();
        void saveSettings(QString language);
        QList<QPair<QString, QString> > loadTranslationFiles();
        QString currentLanguage();
        QString loadCurrentTranslationName();
private:
        QSettings *settings;
		QString m_loc;
};

#endif // __TRANSLATOR_H__
