/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2024
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

#include "multipageconverter.h"
#include <Magick++.h>
#include <string>
#include <iostream>
#include <list>
#include <QDebug>

using namespace Magick;
using namespace std;

MultipageConverter::MultipageConverter(QObject *parent) :
    QObject(parent)
{

}

void MultipageConverter::readFile(QString fileName)
{
    Image my_image;

    m_pagesList.clear();

    list<Image> pdfPages;

    try {
        Magick::pingImages(&pdfPages, fileName.toStdString());

        for (std::list<Image>::iterator it = pdfPages.begin(); it != pdfPages.end(); ++it){
            MultipageItem mpi;

            mpi.w = it->columns();
            mpi.h = it->rows();

            mpi.xres = it->xResolution();
            mpi.yres = it->yResolution();

            mpi.depth = it->depth();

            m_pagesList << mpi;
        }
    }
    catch (Error& my_error) {
        qWarning() << "Warning: " << QString::fromStdString(my_error.what());
    }
    catch( Magick::WarningCoder &warning )
    {
        qWarning() << "Warning: " << QString::fromStdString(warning.what());
    }
    catch( Magick::Warning &warning )
    {
        qWarning() << "Warning: " << QString::fromStdString(warning.what());
    }
}

QList<MultipageItem> MultipageConverter::pages()
{
    return m_pagesList;
}
