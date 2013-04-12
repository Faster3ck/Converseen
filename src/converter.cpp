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

#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
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
    m_zoom = false;
    m_density = false;
}

Converter::~Converter()
{

}

void Converter::run()
{
    m_process_stopped = 0;
    m_conv_status = 2;  // Jumped

    Image my_image;
    QFileInfo fi(m_fileName);

    QString out = QString("%1/%2.%3").arg(m_outputDir).arg(fi.completeBaseName()).arg(m_format);

    if (m_allow_rename)   // rinomina in base alla stringa #_copy
        out = renameFileName(out);

    if (!m_overwrite)   // modalità sovrascittura
        out = overwriteOldFileName(out);

    if (!m_process_stopped) {
        try {

            my_image.read(m_fileName.toStdString());

            if (m_zoom)
                resize(my_image);
            if (m_density)
                changeDensity(my_image);

            if (writeImage(my_image, m_format, m_quality, out))
                m_conv_status = 1;
        }
        catch (Error& my_error) {
            m_conv_status = -1;

            QString err_status = tr("Error: %1").arg(QString::fromStdString(my_error.what()));
            emit errorMessage(err_status);
        }
        catch( Magick::WarningCoder &warning )
        {
            m_conv_status = -1;

            QString err_status = tr("Error: %1").arg(QString::fromStdString(warning.what()));
            emit errorMessage(err_status);
        }
        catch( Magick::Warning &warning )
        {
            m_conv_status = -1;

            QString err_status = tr("Error: %1").arg(QString::fromStdString(warning.what()));
            emit errorMessage(err_status);
        }
    }
}

void Converter::setInputPicture(QString fileName)
{
    m_fileName = fileName;
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

void Converter::setResize(QString resizingStr)
{
    resizingString = resizingStr;
    m_zoom = true;
}

void Converter::resize(Image &my_image)
{
    my_image.resize(resizingString.toStdString());
}

void Converter::setDensity(QString densityStr)
{
    m_densityString = densityStr;
    m_density = true;
}

void Converter::setBackgroundColor(QString bg_color, bool changeBg_color)
{
    m_bg_color = bg_color;
    m_changeBg_color = changeBg_color;
}

void Converter::changeDensity(Image &my_image)
{
    QString n_den = QString(m_densityString);
    my_image.resolutionUnits(PixelsPerInchResolution);
    my_image.density(n_den.toStdString());
}

void Converter::setOverwrite(bool overwrite)
{
    m_overwrite = overwrite;
}

QString Converter::overwriteOldFileName(QString out)
{
    QFileInfo fi(out);
    m_newBaseName = fi.baseName();

    if (fi.exists()) {
        if (!m_overwrite) {
            emit requestOverwrite(m_newBaseName);

            imageCondition.wait(&mutex);
        }
        return QString("%1/%2.%3").arg(fi.path()).arg(m_newBaseName).arg(fi.suffix());
    }
    else
        return out;
}

void Converter::setNewBasename(QString newBaseName, bool ok)
{
    if (ok)
        m_newBaseName = newBaseName;

    imageCondition.wakeAll();
}

void Converter::enableRenamingString(bool rename)
{
    m_allow_rename = rename;
}

void Converter::setRenamingString(QString renamingString)   // Rinomina il file secondo #_copy
{
    m_renamingString = renamingString;
}

QString Converter::renameFileName(QString oldFileName)
{
    QString t_renamingString = m_renamingString;

    QFileInfo fi(oldFileName);
    QString newFileName = QString("%1.%2")
            .arg(t_renamingString.replace("#", fi.baseName()))
            .arg(fi.suffix());

    return QString("%1/%2").arg(fi.path()).arg(newFileName);
}

bool Converter::writeImage(Image &my_image, QString format, int quality, QString out)
{
    my_image.magick(format.toUpper().toStdString());

    QStringList excludedFormats;
    excludedFormats << "jpg" << "jpeg" << "bmp";

    if (m_changeBg_color || (excludedFormats.contains(format, Qt::CaseInsensitive) && my_image.matte())) {
        Image bgImg;
        bgImg.size(Magick::Geometry(my_image.columns(), my_image.rows()));

        bgImg.read("xc:" + m_bg_color.toStdString());
        bgImg.label("bgImg");
        bgImg.depth(my_image.depth());

        bgImg.composite(my_image, Magick::Geometry(bgImg.columns(),bgImg.rows()), Magick::DissolveCompositeOp );

        my_image = bgImg;
    }

    bool converted = false;

    if (quality != -1)
        my_image.quality(quality);

    try {
        my_image.write(out.toStdString());

        #ifndef Q_OS_WIN32
        // Delete images with upper extension (not on Windows)
        /*
            L'operazione seguente ovviamente avviene solo se
            la conversione è andata a buon fine. Quindi in caso di errore,
            l'immagine con l'estensione maiuscola, non viene eliminata.
        */
        if (m_overwrite) {
            QFileInfo fi1(m_fileName);
            QFileInfo fi2(out);

            if (fi1.suffix() != format)
                if ((fi1.suffix().toLower() == format) && (fi1.path() == fi2.path()))
                    QFile::remove(m_fileName);
        }
        #endif

        converted = true;
    }
    catch (Error& my_error) {
        converted = false;
    }

    return converted;
}

void Converter::stopProcess()
{
    m_process_stopped = 1;
}
