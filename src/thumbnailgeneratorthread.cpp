/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2026
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

#include <QImageReader>
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

    int orientation = 0;

    my_image.quiet(true);

    if (Formats::isNativeReadable(m_fileName)) {
        QImageReader imgReader(m_fileName);
        tmpImage = imgReader.read();

        img_width = tmpImage.width();
        img_height = tmpImage.height();

        try {
            my_image.ping(m_fileName.toStdString());

            orientation = my_image.orientation();
            my_image.autoOrient();

            img_dens_x = my_image.xResolution();
            img_dens_y = my_image.yResolution();
        }
        catch (Error &error) {
            QString err_read_status = QString("Error in reading metadata (Generating preview): %1").arg(QString::fromStdString(error.what()));
            qWarning() << "Read Error: " << err_read_status;

            int dpiX = tmpImage.dotsPerMeterX();
            int dpiY = tmpImage.dotsPerMeterY();

            img_dens_x = qRound(dpiX * 0.0254);
            img_dens_y = qRound(dpiY * 0.0254);
        }
        catch( Magick::WarningCoder &warning )
        {
            qWarning() << "Read Warning: " << QString("Warning : %1").arg(QString::fromStdString(warning.what()));;
        }
        catch( Magick::Warning &warning )
        {
            qWarning() << "Read Warning: " << QString("Warning : %1").arg(QString::fromStdString(warning.what()));;
        }

        if (m_generateThumbnail) {
            qreal scaleFactor = globals::Globals::scaleFactor();

            tmpImage = autoOrientImage(tmpImage, orientation);

            thumbnail = tmpImage.scaled(QSize(MAX_THUMB_W, MAX_THUMB_H) * scaleFactor,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
        }
    }
    else {
        try
        {
            my_image.read(m_fileName.toStdString());
            my_image.resolutionUnits(PixelsPerInchResolution);
            my_image.autoOrient();

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

QImage ThumbnailGeneratorThread::autoOrientImage(const QImage &image, const int &orientation)
{
    QTransform transform;

    switch (orientation) {
    case 1:
        // No transformation
        break;
    case 2:
        // Flip horizontally
        transform.scale(-1, 1);
        break;
    case 3:
        // Rotate 180 degrees
        transform.rotate(180);
        break;
    case 4:
        // Flip vertically
        transform.scale(1, -1);
        break;
    case 5:
        // Rotate 90 CW and flip horizontally
        transform.rotate(90);
        transform.scale(1, -1);
        break;
    case 6:
        // Rotate 90 CW
        transform.rotate(90);
        break;
    case 7:
        // Rotate 90 CW and flip vertically
        transform.rotate(90);
        transform.scale(-1, 1);
        break;
    case 8:
        // Rotate 270 CW (== rotate -90)
        transform.rotate(270);
        break;
    default:
        // Orientation not recognized; return original image
        return image;
    }

    return image.transformed(transform);
}
