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

#include <QMovie>
#include <QPixmapCache>
#include "mylabelpreviewer.h"
#include "formats.h"

myLabelPreviewer::myLabelPreviewer(QWidget *parent) : QLabel(parent)
{
    thumbGen = new ThumbnailGeneratorThread(this);

    connect(thumbGen, SIGNAL(pixmapGenerated(QImage)), this, SLOT(showPreview(QImage)));

    setText("No preview!");
}

void myLabelPreviewer::loadPreview(QString fileName)
{
    key = fileName;

    if (!QPixmapCache::find(key, m_pixmap)) {
        showLoadingAnimation();
        thumbGen->setFileName(fileName);
        thumbGen->start();
    }
    else
        this->setPixmap(m_pixmap);
}

void myLabelPreviewer::showPreview(QImage image)
{
    m_pixmap = QPixmap::fromImage(image);
    QPixmapCache::insert(key, m_pixmap);

    this->setPixmap(m_pixmap);
}

void myLabelPreviewer::showLoadingAnimation()
{
    QMovie *movie = new QMovie;
    movie->setFileName(":/movies/res/loading.gif");

    setMovie(movie);
    movie->start();
}
