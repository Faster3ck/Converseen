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

#include <QMovie>
#include "mylabelpreviewer.h"
#include "formats.h"

myLabelPreviewer::myLabelPreviewer(QWidget *parent) : QLabel(parent)
{
    thumbGen = new ThumbnailGeneratorThread(this);

    connect(thumbGen, SIGNAL(pixmapGenerated(QImage, int, int, double, double)), this, SLOT(showPreview(QImage, int, int, double, double)));
}

void myLabelPreviewer::loadPreview(QString fileName, bool generateThumbnail)
{
    m_generateThumbnail = generateThumbnail;

    if (generateThumbnail)
        showLoadingAnimation();

    thumbGen->setFileName(fileName);
    thumbGen->setThumbnailGeneration(generateThumbnail);
    thumbGen->start();
}

void myLabelPreviewer::showPreview(QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y)
{
    if (thumbnail.isNull() || !m_generateThumbnail) {
        setText(tr("Preview"));
    }
    else {
        QPixmap pixmap = QPixmap::fromImage(thumbnail);
        this->setPixmap(pixmap);
    }

    emit previewReady(orig_w, orig_h, orig_dens_x, orig_dens_y);
}

void myLabelPreviewer::showLoadingAnimation()
{
    QMovie *movie = new QMovie;
    movie->setFileName(":/movies/res/loading.gif");

    setMovie(movie);
    movie->start();
}
