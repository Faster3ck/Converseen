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

#include "formats.h"
#include "thumbnailgeneratorthread.h"
#include "globals.h"

#define MAX_THUMB_W 420
#define MAX_THUMB_H 315

ThumbnailGeneratorThread::ThumbnailGeneratorThread(QObject *parent) :
    QThread(parent)
{
    m_generateThumbnail = true;
}

void ThumbnailGeneratorThread::run()
{
    if (CachingSystem::find(m_fileName)) {  // image informations already in cache
        QImage thumbnail = CachingSystem::thumbnail();
        if (thumbnail.isNull() && m_generateThumbnail) {
            /*
                If previously the "show preview" checkbox was unchecked end then checked
                the old cached informations will be deleted and reloaded with the thumbnail
            */

            CachingSystem::remove(m_fileName);
            createThumbnail();
        }
        else {
            int orig_w = CachingSystem::originalWidth();
            int orig_h = CachingSystem::originalHeight();
            double orig_dens_x = CachingSystem::originalDensityX();
            double orig_dens_y = CachingSystem::originalDensityY();

            emit pixmapGenerated(thumbnail, orig_w, orig_h, orig_dens_x, orig_dens_y);
        }
    }
    else {                                  // image informations not in cache
        createThumbnail();
    }
}

void ThumbnailGeneratorThread::setFileName(QString fileName)
{
    m_fileName = fileName;
}

QImage* ThumbnailGeneratorThread::toQImage(const Image &image)
{
    QImage *qmg = new QImage();
    Image tmpImage = image;

    Geometry geometry;

    geometry.width(MAX_THUMB_W);
    geometry.height(MAX_THUMB_H);
    geometry.aspect(false);

    tmpImage.resize(geometry);

    Blob blob;
    tmpImage.magick("PNG"); // or PNG
    tmpImage.write(&blob);
    const QByteArray imageData((char*)(blob.data()),blob.length());

    // Convert the data to a QPixmap in the vector
    qmg->loadFromData(imageData);

    return qmg;
}

void ThumbnailGeneratorThread::setThumbnailGeneration(bool generate)
{
    m_generateThumbnail = generate;
}

void ThumbnailGeneratorThread::createThumbnail()
{      
    QImage tmpImage, thumbnail;
    Image my_image;

    int img_width = 0;
    int img_height = 0;

    double img_dens_x = 0.0;
    double img_dens_y = 0.0;

    if (Formats::isNativeReadable(m_fileName)) {
        tmpImage.load(m_fileName);

        img_width = tmpImage.width();
        img_height = tmpImage.height();

        img_dens_x = tmpImage.logicalDpiX();
        img_dens_y = tmpImage.logicalDpiY();

        qreal scaleFactor = globals::Globals::scaleFactor();

        if (m_generateThumbnail) {
            thumbnail = tmpImage.scaled(QSize(MAX_THUMB_W, MAX_THUMB_H) * scaleFactor,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);

        }
    }
    else {
        try
        {
            my_image.quiet(true);
            my_image.read(m_fileName.toStdString());

            img_width = my_image.columns();
            img_height = my_image.rows();

            img_dens_x = my_image.xResolution();
            img_dens_y = my_image.yResolution();

            if (m_generateThumbnail) {
                QImage *aQImage = toQImage(my_image);

                thumbnail = *aQImage;
                delete aQImage;
            }
        }
        catch (Error& my_error) {
            //setText(tr("Sorry! Selected image is damaged!"));
        }
        catch ( Magick::WarningCoder &warning )
        {
            //setText(tr("Sorry! Selected image is damaged!"));
        }
        catch ( Magick::Warning &warning )
        {
            //setText(tr("Sorry! Selected image is damaged!"));
        }
    }

    CachingSystem::insert(m_fileName, thumbnail, img_width, img_height, img_dens_x, img_dens_y);

    emit pixmapGenerated(thumbnail, img_width, img_height, img_dens_x, img_dens_y);
}
