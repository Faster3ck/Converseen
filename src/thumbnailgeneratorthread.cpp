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

#include "formats.h"
#include "thumbnailgeneratorthread.h"

ThumbnailGeneratorThread::ThumbnailGeneratorThread(QObject *parent) :
    QThread(parent)
{
}

void ThumbnailGeneratorThread::run()
{
    loadPreview();
}

void ThumbnailGeneratorThread::setFileName(QString fileName)
{
    m_fileName = fileName;
}

QImage* ThumbnailGeneratorThread::toQImage(const Image &image)
{
    QImage *newQImage = new QImage(image.columns(), image.rows(), QImage::Format_RGB32);

    const Magick::PixelPacket *pixels;
    Magick::ColorRGB rgb;
    for (int y = 0; y < newQImage->height(); y++) {
        pixels = image.getConstPixels(0, y, newQImage->width(), 1);
        for (int x = 0; x < newQImage->width(); x++) {
            rgb = (*(pixels + x));
            newQImage->setPixel(x, y, QColor((int) (255 * rgb.red())
                                             , (int) (255 * rgb.green())
                                             , (int) (255 * rgb.blue())).rgb());
        }
    }
    return newQImage;
}

void ThumbnailGeneratorThread::loadPreview()
{  
    QImage tmpImage;
    Image my_image;

    try
    {
        my_image.read(m_fileName.toStdString());

        if (Formats::isNativeReadable(m_fileName))
            tmpImage.load(m_fileName);
        else {
            QImage *aQImage = toQImage(my_image);
            tmpImage = *aQImage;
            delete aQImage;
        }

        int img_width = my_image.columns();
        int img_height = my_image.rows();

        int n_heigth = (320 * img_height) / img_width;

        if (!((img_width <= 320) && (img_height <= 240))) {
            if (img_width > img_height) {
                tmpImage = tmpImage.scaled(QSize(320, n_heigth),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);
            }
            else
                if ((img_width <= img_height) || (n_heigth > 240)) {
                    int n_width = (240 * img_width) / img_height;
                    tmpImage = tmpImage.scaled(QSize(n_width, 240),
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation);
                }
        }
    }
    catch (Error& my_error) {
        //setText(tr("Sorry! Selected image is damaged!"));
    }
    catch( Magick::WarningCoder &warning )
    {
        //setText(tr("Sorry! Selected image is damaged!"));
    }
    catch( Magick::Warning &warning )
    {
        //setText(tr("Sorry! Selected image is damaged!"));
    }

    emit pixmapGenerated(tmpImage);
}
