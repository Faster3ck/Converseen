/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2023
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

#include <QImageWriter>
#include <QFileInfo>
#include <QTextStream>
#include "formats.h"

QString Formats::s_readableFiltersString;
QStringList Formats::s_readableFilters;
QStringList Formats::s_readableFormattedFilters;
QStringList Formats::s_writableFilters;

void Formats::loadFormats()
{
    list<CoderInfo> coderList;
    try {
        coderInfoList(&coderList,
                      CoderInfo::TrueMatch,
                      CoderInfo::AnyMatch,
                      CoderInfo::AnyMatch);
    }
    catch (ErrorModule &e) {
        cerr << e.what() << endl;
    }
    list<CoderInfo>::iterator entry = coderList.begin();
    QString readableExts;

    QStringList readableFiltersList;
    QStringList blacklist;

    blacklist << "pdf" << "ico" << "icon";

    QString uuu = "";

    while(entry != coderList.end())
    {
        // Readable formats
        if ((entry->isReadable()) && (!isVideo(entry))) {
            QString currFormat = QString::fromStdString(entry->name()).toLower();
            if (!(blacklist.contains(currFormat))) {
                readableFiltersList << QString(";;%3 [*.%1] (*.%1 *.%2 )")
                                       .arg(QString::fromStdString(entry->name()).toLower())   // Lower and upper filters for gnome compatibility (?)
                                       .arg(QString::fromStdString(entry->name()))
                                       .arg(QString::fromStdString(entry->description()));

                QString c_ext = QString::fromStdString(entry->name()).toLower();

                s_readableFilters << c_ext;
                s_readableFormattedFilters << QString("*.%1").arg(c_ext);

                readableExts += QString("*.%1 *.%2 ")
                        .arg(QString::fromStdString(entry->name()).toLower())
                        .arg(QString::fromStdString(entry->name()));
            }
        }

        // Writable formats
        if ((entry->isWritable()) && (!isVideo(entry))) {
            s_writableFilters << QString("%1 - (%2)")
                                 .arg(QString::fromStdString(entry->name()))
                                 .arg(QString::fromStdString(entry->description()));
        }

        ++entry;
    }

    // Fixme: Temporary fix (It avoids to import txt files when dropping folders)
    s_readableFormattedFilters.removeAll("*.txt");

    // Add missing but supported formats
    s_readableFormattedFilters.append(QStringList() <<  "*.tif" << "*.jfif");

    ////////////////////
    // Adding manually some missing but supported formats
    readableFiltersList << ";;Tagged Image File Format [*.tif] (*.tif *.TIF )";
    readableFiltersList << ";;Joint Photographic Experts Group JFIF format [*.jfif] (*.jfif *.JFIF )";
    readableFiltersList = sortNonCaseSensitive(readableFiltersList);

    readableExts += "*.tif *.TIF *.jfif *.JFIF";
    ////////////////////

    s_readableFiltersString = readableFiltersList.join("");
    s_readableFiltersString.prepend(tr("All Supported Filters (%1)").arg(readableExts));

    // Adding manually some missing but supported formats
    s_writableFilters << "TIF - (Tagged Image File Format)";
    s_writableFilters << "JFIF - (Joint Photographic Experts Group JFIF format)";
    s_writableFilters = sortNonCaseSensitive(s_writableFilters);

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

QStringList Formats::readableFormattedFilters()
{
    return s_readableFormattedFilters;
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

bool Formats::isVideo(list<CoderInfo>::iterator entry)
{
#if MagickLibVersion < 0x700
    // Support for ImageMagick-6. This will be removed sooner as Linux distros drop the support!
    QString currFormat = QString::fromStdString(entry->name());
    QStringList videoFormats(QStringList() << "3G2" << "3GP" << "APNG" << "AVI" << "FLV" << "M2V" << "M4V" << "MKV" << "MOV" << "MP4" << "MPEG" << "MPG" << "WEBM" << "WMV");

    return videoFormats.contains(currFormat);
#else
    return (QString::fromStdString(entry->module()) == "VIDEO") ? true : false;
#endif
}

void Formats::printSupportedFormats()
{
    QTextStream out(stdout);

    int n_readable = 0;
    int n_writable = 0;

    out << "+----------------------------------------------------------------------------------------------+\n";
    out << "|                                         READABLE                                             |\n";
    out << "+----------------------------------------------------------------------------------------------+\n";

    list<CoderInfo> coderList;
    try {
        coderInfoList(&coderList,
                      CoderInfo::TrueMatch,
                      CoderInfo::AnyMatch,
                      CoderInfo::AnyMatch);
    }
    catch (ErrorModule &e) {
        cerr << e.what() << endl;
    }

    list<CoderInfo>::iterator entry = coderList.begin();
    QString readableExts;

    while(entry != coderList.end())
    {
        if (entry->isReadable()) {
            QString name = QString::fromStdString(entry->name());
            QString descr = QString::fromStdString(entry->description());

            out << QString("|%1\t|%2\t\t|%3\n").arg(QString::number(n_readable + 1), name, descr);
            n_readable++;
        }

        ++entry;
    }

    out << QString("|Total readable:\t%1\n\n").arg(QString::number(n_readable));


    out << "+----------------------------------------------------------------------------------------------+\n";
    out << "|                                         WRITABLE                                             |\n";
    out << "+----------------------------------------------------------------------------------------------+\n";

    entry = coderList.begin();

    while(entry != coderList.end())
    {
        if (entry->isWritable()) {
            QString name = QString::fromStdString(entry->name());
            QString descr = QString::fromStdString(entry->description());

            out << QString("|%1\t\t|%2\t\t|%3\n").arg(QString::number(n_writable + 1), name, descr);

            n_writable++;
        }
        ++entry;
    }

    out << QString("|Total writable:\t%1\n\n").arg(QString::number(n_writable));
}


