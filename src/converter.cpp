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

#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QDebug>
#include "converter.h"

Converter::Converter(QObject *parent)
    : QThread(parent)
{
    reset();
    connect(this, SIGNAL(finished()), this, SLOT(conversionCompleted()));
}

void Converter::conversionCompleted()
{
    emit nextConversion(m_conv_status);
}

void Converter::reset()
{
    m_resize = false;
    m_density = false;
    m_rotation = false;
    m_flip = false;
}

Converter::~Converter()
{

}

void Converter::run()
{
    m_process_stopped = 0;

    Image my_image;
    QFileInfo fi(m_fileNameIn);
    QString out = m_fileNameOut;
    QString err_read_status;
    QString err_write_status;

    // Skip existing files
    if (m_overwrite == SKIP) {
        if (skipExisting(out)) {
            m_conv_status = 2;  // Skipped
            QString skipMsg = QString(tr("Skipped: %1 already esists."))
                                  .arg(fi.fileName());
            emit errorMessage(skipMsg);

            return;
        }
    }

    // Overwrite existing files
    if (m_overwrite == ASK)
        out = overwriteOldFileName(out);

    if (!m_process_stopped) {
        my_image.quiet(true);
        try {
            my_image.read(m_fileNameIn.toStdString());
        } catch (Error &error) {
            m_conv_status = -1;

            err_read_status = tr("Error: %1").arg(QString::fromStdString(error.what()));
            qWarning() << "Read Error: " << err_read_status;
            emit errorMessage(err_read_status);

            return;
        }
        catch( Magick::WarningCoder &warning )
        {
            m_conv_status = -1;

            err_read_status = tr("Error: %1").arg(QString::fromStdString(warning.what()));
            qWarning() << "Read Warning: " << err_read_status;
        }
        catch( Magick::Warning &warning )
        {
            m_conv_status = -1;

            err_read_status = tr("Error: %1").arg(QString::fromStdString(warning.what()));
            qWarning() << "Read Warning: " << err_read_status;
        }

        if (m_resize)
            resize(my_image);
        if (m_rotation)
            rotate(my_image);
        if (m_flip)
            flip(my_image);

        if (writeImage(my_image, m_format, m_quality, out, err_write_status)) {
            m_conv_status = 1;
        }
        else {
            qWarning() << "Write Error: " << err_write_status;

            m_conv_status = -1;
            emit errorMessage(err_write_status);
        }
    }
}

void Converter::setInputPicture(QString fileName)   //Name
{
    m_fileNameIn = fileName;
}

void Converter::setOutputPictureName(QString fileName)
{
    m_fileNameOut = fileName;
}

void Converter::setFormat(QString format)
{
    m_format = format;
}

void Converter::setQuality(int quality)
{
    m_quality = quality;
}

void Converter::setOutputDir(QString outputDir)
{
    m_outputDir = outputDir;
}

void Converter::setResize(const double &width, const double &height, const bool &percent, const bool &maintainAspectRatio)
{
    m_resize = true;

    m_width = width;
    m_height = height;
    m_percent = percent;
    m_maintainAspectRatio = maintainAspectRatio;
}

void Converter::setRotation(double deg)
{
    m_rotation = true;
    m_rotation_deg = deg;
}

void Converter::rotate(Image &my_image)
{
    my_image.rotate(m_rotation_deg);
}

void Converter::setFlip(FlipOrientation orientation)
{
    m_flip = true;
    m_orientation = orientation;
}

void Converter::flip(Image &my_image)
{
    if (m_orientation == VERTICAL)
        my_image.flip();
    if (m_orientation == HORIZONTAL)
        my_image.flop();
}

void Converter::resize(Image &my_image)
{
    Magick::Geometry size;
    my_image.filterType(m_resamplingFilter);

    if (m_percent) {
        int p_w = qRound(my_image.columns() * (m_width / 100));
        int p_h = qRound(my_image.rows() * (m_height / 100));

        size.width(p_w);
        size.height(p_h);
    }
    else {
        size.width((int)m_width);
        size.height((int)m_height);
    }

    //Resize without preserving aspect ratio (!)
    size.aspect(!m_maintainAspectRatio);

    my_image.resize(size);
}

void Converter::setDensity(QString densityStr)
{
    m_densityString = densityStr;//.replace(".", ",");
    m_density = true;
}

void Converter::setBackgroundColor(QString bg_color, bool changeBg_color)
{
    m_bg_color = bg_color;
    m_changeBg_color = changeBg_color;
}

void Converter::setOverwrite(OverwriteMode overwrite)
{
    m_overwrite = overwrite;
}

QString Converter::overwriteOldFileName(QString out)
{
    QFileInfo fi(out);
    m_newBaseName = fi.baseName();

    if (fi.exists()) {
        if (m_overwrite == ASK) {
            emit requestOverwrite(m_newBaseName);

            mutex.lock();
            imageCondition.wait(&mutex);
            mutex.unlock();
        }
        return QString("%1/%2.%3").arg(fi.path()).arg(m_newBaseName).arg(fi.suffix());
    }
    else
        return out;
}

bool Converter::skipExisting(QString out)
{
    QFileInfo fi(out);
    m_newBaseName = fi.baseName();

    if (fi.exists())
        return true;
    else
        return false;
}

void Converter::setNewBasename(QString newBaseName, bool ok)
{
    if (ok)
        m_newBaseName = newBaseName;

    imageCondition.wakeAll();
}

void Converter::setResamplingFilter(IMFilterType resamplingFilter)
{
    m_resamplingFilter = resamplingFilter;
}

void Converter::setMagickDefines(const QList<MagickDefine> &magickDefines)
{
    m_magickDefines = magickDefines;
}

void Converter::setRemoveMetadata(const bool &value)
{
    m_removeMetadata = value;
}

bool Converter::writeImage(Image &my_image, const QString &format, const int &quality, const QString &out, QString &error_status)
{
    QString inputFormat = QString::fromLocal8Bit(my_image.magick().c_str());

    Magick::CoderInfo formatCoderInfo(format.toUpper().toStdString());
    bool is_writable = formatCoderInfo.isWritable();

    if (!is_writable) {
        error_status = tr("ERROR: Format %1 is not supported for writing, choose another output format. Skipping!")
                           .arg(QString::fromStdString(my_image.magick().c_str()));

        return false;
    }

    my_image.magick(format.toUpper().toStdString());

    if (inputFormat == "PDF") {
        my_image = convertPDFtoImage(my_image);
    }

    QStringList excludedFormats;
    excludedFormats << "jpg" << "jpeg" << "bmp";

    bool hasTransparency = false;

    error_status = "";

#if MagickLibVersion < 0x700
    hasTransparency = my_image.matte();
#else
    hasTransparency = my_image.alpha();
#endif

    if (m_changeBg_color || (excludedFormats.contains(format, Qt::CaseInsensitive) && hasTransparency)) {
        Image bgImg;
        bgImg.size(Magick::Geometry(my_image.columns(), my_image.rows()));

        if (m_density) {
            //bgImg.resolutionUnits(PixelsPerInchResolution);
            bgImg.density(m_densityString.toStdString());
        }

        bgImg.read("xc:" + m_bg_color.toStdString());
        bgImg.label("bgImg");
        bgImg.depth(my_image.depth());

        bgImg.composite(my_image, Magick::Geometry(bgImg.columns(), bgImg.rows()), Magick::DissolveCompositeOp);

        my_image = bgImg;
    }

    bool converted = false;

    if (m_removeMetadata) {
        my_image.strip();
        my_image.autoOrient();
    }

    if (quality != -1)
        my_image.quality(quality);

    if (!m_magickDefines.empty()) {
        for (int i = 0; i < m_magickDefines.count(); i++) {
            MagickDefine mDef = m_magickDefines.at(i);

            my_image.defineValue(mDef.magick().toStdString(), mDef.key().toStdString(), mDef.value().toStdString());
        }
    }

    if (m_density) {
        //my_image.resolutionUnits(PixelsPerInchResolution);
        my_image.density(m_densityString.toStdString());
    }

    try {
        my_image.write(out.toStdString());
        converted = true;
    }
    catch (Error& my_error) {
        converted = false;
        error_status = QString::fromStdString(my_error.what());
    }

    return converted;
}

Image Converter::convertPDFtoImage(const Image &my_image)
{
    // Transform PDF page to image

    Image ximage;
    ximage.magick(my_image.magick());
    
#if MagickLibVersion < 0x700
    ximage.antiAlias(true);
#else
    ximage.textAntiAlias(true);
#endif

    ximage.quiet(false);

    //ximage.resolutionUnits(PixelsPerInchResolution);
    if (m_density) {
        ximage.density(m_densityString.toStdString());
    }
    else {
        ximage.density("150x150");
    }

    ximage.read(m_fileNameIn.toStdString());

    ximage.backgroundColor(Magick::Color("white"));
    ximage.extent(Magick::Geometry(ximage.size().width(), ximage.size().height()), Magick::Color("white"));

    ximage.sharpen(0.0, 1.0);

    return ximage;
}

void Converter::stopProcess()
{
    m_process_stopped = 1;
}
