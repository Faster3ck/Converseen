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

#ifndef IMAGEINFORMATIONS_H
#define IMAGEINFORMATIONS_H

#include <string>
#include <iostream>
#include <Magick++.h>
#include <QString>

using namespace Magick;
using namespace std;


class ImageInformations
{
public:
    ImageInformations(QString fileName);

    int imageWidth();
    int imageHeight();
    double x_Resolution();
    double y_Resolution();

private:
    int m_width;
    int m_height;

    double m_resX;
    double m_resY;
};

#endif // IMAGEINFORMATIONS_H
