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

#include "sizeutil.h"

QString SizeUtil::simplifyFileSize(qint64 size)
{
    QString sSize = "0";

    sSize = QString("%1 Bytes").arg(size);
    if (size > 1024)
        sSize = QString("%1 KiB").arg(QString::number( ((double)size / 1024), 'f', 1) );
    if (size / 1024 > 1024)
        sSize = QString("%1 MiB").arg(QString::number( ((double)size / 1024 / 1024), 'f', 1) );

    return sSize;
}
