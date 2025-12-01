/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2025
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <Magick++.h>
#include <string>
#include <iostream>

using namespace Magick;
using namespace std;

#if MagickLibVersion < 0x700
    typedef FilterTypes IMFilterType;
#else
    typedef FilterType IMFilterType;
#endif

namespace globals
{
    const QString PROGRAM_NAME = "Converseen";
    const int CURRENT_INTERNAL_VERSION = 1613;
    const QString VERSION = "0.15.1.3";

    class Globals
    {
    public:
        static void setSignature(const QString &signature);
        static QString signature();
        static QString magickVersion();
        static qreal scaleFactor();

    private:
        static QString m_sig;
    };
}

#endif // GLOBALS_H
