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

#include <QCoreApplication>
#include "whereiam.h"

WhereIAm::WhereIAm()
{

}

QString WhereIAm::dataDir()
{
    QString binDir = QCoreApplication::applicationDirPath();
    QString dataDir = binDir;

#ifdef Q_OS_LINUX
    if (binDir == QDir::cleanPath ("/usr/bin")) // If linux /usr installed
        dataDir = "/usr/share/";
    else
        if (binDir == "/usr/local/bin")
            dataDir = "/usr/local/share/";	// If linux /usr/local installed
#else   // Windows, Haiku etc...
        dataDir = dataDir + "\\";
#endif

    return(dataDir);
}
