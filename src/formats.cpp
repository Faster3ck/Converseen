/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2014
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

#include <QDebug>
#include <QImageWriter>
#include <QFileInfo>
#include <QTextStream>
#include "formats.h"

using namespace Magick;
using namespace std;

QString Formats::s_readableFiltersString;
QStringList Formats::s_readableFilters;
QStringList Formats::s_writableFilters;

void Formats::loadFormats()
{
#ifdef Q_OS_LINUX || Q_OS_FREEBSD
    list<CoderInfo> coderList;
    coderInfoList(&coderList,
                  CoderInfo::TrueMatch,
                  CoderInfo::AnyMatch,
                  CoderInfo::AnyMatch);

    list<CoderInfo>::iterator entry = coderList.begin();
    QString readableExts;

    QStringList readableFiltersList;
    QStringList blacklist;

    blacklist << "pdf" << "ico" << "icon";

    while(entry != coderList.end())
    {
        if (entry->isReadable()) {
            QString currFormat = QString::fromStdString(entry->name()).toLower();
            if (!(blacklist.contains(currFormat))) {
                readableFiltersList << QString(";;%3 [*.%1] (*.%1 *.%2 )")
                                       .arg(QString::fromStdString(entry->name()).toLower())   // Lower and upper filters for gnome compatibility (?)
                                       .arg(QString::fromStdString(entry->name()))
                                       .arg(QString::fromStdString(entry->description()));

                s_readableFilters << QString::fromStdString(entry->name()).toLower();

                readableExts += QString("*.%1 *.%2 ")
                        .arg(QString::fromStdString(entry->name()).toLower())
                        .arg(QString::fromStdString(entry->name()));
            }
        }

        if (entry->isWritable()) {
            s_writableFilters << QString("%1 - (%2)")
                                 .arg(QString::fromStdString(entry->name()))
                                 .arg(QString::fromStdString(entry->description()));
        }

        ++entry;
    }

    ////////////////////
    // Correct the leak of tif format
    readableFiltersList << ";;Tagged Image File Format [*.tif] (*.tif *.TIF )";
    readableFiltersList = sortNonCaseSensitive(readableFiltersList);

    readableExts += "*.tif *.TIF";
    ////////////////////

    s_readableFiltersString = readableFiltersList.join("");
    s_readableFiltersString.prepend(tr("All Supported Filters (%1)").arg(readableExts));
#else
    QFile data1(QDir::cleanPath("FileFormats/Readableformats.txt"));
    if (data1.open(QFile::ReadOnly)) {
        QTextStream in(&data1);

        s_readableFilters = in.readLine().split(" ");
        s_readableFiltersString = in.readLine();
    }

    QFile data2(QDir::cleanPath("FileFormats/WritableFormats.txt"));
    if (data2.open(QFile::ReadOnly)) {
        QTextStream in(&data2);
        QString line;
        do {
            line = in.readLine();
            s_writableFilters << line;
        } while (!line.isNull());
    }
    s_writableFilters.removeLast();
#endif

    s_writableFilters.prepend(tr("Don't change the format"));
}

QStringList Formats::nativeReadableFormats()
{
    QList<QByteArray> nativeRFormats = QImageWriter::supportedImageFormats();

    QStringList listNativeFormats;

    foreach(QByteArray format, nativeRFormats) {
        listNativeFormats.append(*new QString(format));
    }

    return listNativeFormats;
}

bool Formats::isNativeReadable(QString fileName)
{
    QFileInfo fi(fileName);
    QString format = fi.suffix();

    QStringList listNativeFormats = nativeReadableFormats();

    return listNativeFormats.contains(format, Qt::CaseInsensitive);
}

QString Formats::readableFiltersString()
{
    return s_readableFiltersString;
}

QStringList Formats::readableFilters()
{
    return s_readableFilters;
}

QStringList Formats::writableFilters()
{
    return s_writableFilters;
}

// Sorts the string list case-insentitively
QStringList  Formats::sortNonCaseSensitive(QStringList list) {
    QMap<QString, QString> map;

    foreach ( QString str, list ) {
        map.insert( str.toLower(), str );
    }
    list = map.values();

    return list;
}
