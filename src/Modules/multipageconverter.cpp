/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2017
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

#include "multipageconverter.h"

MultipageConverter::MultipageConverter(QObject *parent) :
    QObject(parent)
{

}

void MultipageConverter::readFile(QString fileName)
{
    int page_counter = 0;
    QString tmpFileName;
    Image my_image;

    m_pagesList.clear();

    for (;;) {
        try {
            MultipageItem mpi;

            tmpFileName = QString("%1[%2]")
                    .arg(fileName)
                    .arg(QString::number(page_counter));

            my_image.read(tmpFileName.toStdString());

            mpi.w = my_image.columns();
            mpi.h = my_image.rows();

            mpi.xres = my_image.xResolution();
            mpi.yres = my_image.yResolution();

            mpi.depth = my_image.depth();

            m_pagesList << mpi;

            page_counter++;
        }
        catch (Error& my_error) {

            break;
        }
        catch( Magick::WarningCoder &warning )
        {

            break;
        }
        catch( Magick::Warning &warning )
        {

            break;
        }
    }
}

QList<MultipageItem> MultipageConverter::pages()
{
    return m_pagesList;
}
