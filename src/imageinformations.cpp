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

#include "imageinformations.h"
#include <QPixmap>
#include "formats.h"

ImageInformations::ImageInformations(QString fileName)
{
    m_width = 0;
    m_height = 0;

    m_resX = 0.0;
    m_resY = 0.0;

    bool native = false;

    if (Formats::isNativeReadable(fileName)) {
        native = true;

        QPixmap pix(fileName);
        m_width = pix.width();
        m_height = pix.height();
    }

    Image my_image;

    try {
        my_image.read(fileName.toStdString());

        m_resX = my_image.xResolution();
        m_resY = my_image.yResolution();

        if (native) {
            m_width = my_image.columns();
            m_height = my_image.rows();
        }
    }
    catch(Exception &error_)
    {

    }

}

int ImageInformations::imageWidth()
{
    return m_width;
}

int ImageInformations::imageHeight()
{
    return m_height;
}

double ImageInformations::x_Resolution()
{
    return m_resX;
}

double ImageInformations::y_Resolution()
{
    return m_resX;
}
