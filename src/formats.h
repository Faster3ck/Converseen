/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2021
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

#ifndef FORMATS_H
#define FORMATS_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <QDir>
#include <Magick++.h>
#include <string>
#include <iostream>

class Formats : public QObject
{
    Q_OBJECT
public:
    static void loadFormats();

    static QString readableFiltersString();
    static QStringList readableFilters();
    static QStringList writableFilters();
    static QStringList nativeReadableFormats();
    static bool isNativeReadable(QString fileName);

private:
    static QString s_readableFiltersString;
    static QStringList s_readableFilters;
    static QStringList s_writableFilters;
    static QStringList sortNonCaseSensitive(QStringList list);

signals:

public slots:

};

#endif // FORMATS_H
