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

#include <QColorDialog>
#include <QTimer>
#include <string>
#include <iostream>
#include "mainwindowimpl.h"
#include "dialogoptions.h"
#include "dialoginfo.h"
#include "formats.h"
#include "inisettings.h"
#include "sizeutil.h"
#include "updatechecker.h"
#include "dialogmultipageeditor.h"
#include "globals.h"

using namespace Magick;
using namespace std;

// TODO: Inserire funzione per sostituire tutte le immagini

MainWindowImpl::MainWindowImpl(QWidget * parent, Qt::WindowFlags f)
    : QMainWindow(parent, f)
{
    setupUi(this);

    iAList = new QList<ImageAttributes>;
    convertThread = new Converter(this);
    dlgCStatus = new DialogConversionStatus();

    CachingSystem::init();

    connect(treeWidget, SIGNAL(dropped(QStringList)), this, SLOT(loadFiles(QStringList)));

    connect(bntSelDir, SIGNAL(clicked()), this, SLOT(openOutDirectory()));
    connect(checkSameDir, SIGNAL(stateChanged(int)), this, SLOT(setCurrentDirectory()));
    connect(btnFormatOptions, SIGNAL(clicked()), this, SLOT(setQuality()));
    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelection()));

    connect(spin_geoWidth, SIGNAL(editingFinished()), this, SLOT(relativeSizeW()));
    connect(spin_geoHeight, SIGNAL(editingFinished()), this, SLOT(relativeSizeH()));

    connect(comboResizeValues, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectGeometryUnit(QString)));

    connect(checkRename, SIGNAL(stateChanged(int)), this, SLOT(enableRenameLine()));
    connect(radioPrefixSuffix, SIGNAL(clicked()), this, SLOT(enableProgressiveSpin()));
    connect(radioProgressive, SIGNAL(clicked()), this, SLOT(enableProgressiveSpin()));
    connect(checkShowPreview, SIGNAL(stateChanged(int)), this, SLOT(enableShowPreview()));

    connect(pushReset, SIGNAL(clicked()), this, SLOT(onPushResetClick()));

    connect(convertThread, SIGNAL(nextConversion(int)), this, SLOT(nextConversion(int)));
    connect(convertThread, SIGNAL(requestOverwrite(QString)), this, SLOT(overwriteDialog(QString)));
    connect(convertThread, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));

    connect(dlgCStatus, SIGNAL(stopProcess()), this, SLOT(stopProcess()));

    connect(pushColor, SIGNAL(clicked()), this, SLOT(choseBackgroundColor()));

    connect(labelPreview, SIGNAL(previewReady(int, int, double, double)), this, SLOT(showImageInformations(int, int, double, double)));

    connect(checkRelative, SIGNAL(stateChanged(int)), this, SLOT(setRelativeSizeCheckboxes(int)));
    connect(checkOverwrite, SIGNAL(stateChanged(int)), this, SLOT(setOverwriteStatus(int)));

    createActions();
    setupMenu();
    createContextMenu();

    loadFormats();

    comboResizeValues->addItems((QStringList() << "%" << "px"));

    adjustSize();
    dockWidget->adjustSize();

    treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    loadOptions();

    resetDisplays();

    if (IniSettings::isAutoChechUpdates()) {
        checkForUpdates();
    }

    checkVersion();
}

MainWindowImpl::~MainWindowImpl()
{
    saveSettings();
}

void MainWindowImpl::importListFromArgv(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString text = file.readAll();
    QStringList rawlist = text.split("\n");

    QStringList list;

    for (int i = 0; i < rawlist.count(); i++) {
        QFileInfo fi(rawlist.at(i));
        if (Formats::readableFilters().contains(fi.suffix(), Qt::CaseInsensitive))
            list << rawlist.at(i);
    }

    loadFiles(list);
}

void MainWindowImpl::printSupportedFormats()
{
    Formats::printSupportedFormats();
}

void MainWindowImpl::createActions()
{
    // "File" actions
    connect(actionOpenFiles, SIGNAL(triggered()), this, SLOT(openFiles()));
    connect(actionAddFiles, SIGNAL(triggered()), this, SLOT(addFiles()));
    connect(actionImportWindowsIconIco, SIGNAL(triggered()), this, SLOT(importIcoFile()));
    connect(actionImportPDFFile, SIGNAL(triggered()), this, SLOT(importPdfFile()));

    connect(actionShowOptions, SIGNAL(triggered()), this, SLOT(editSettings()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()));

    // "Edit" actions
    connect(actionCheckItems, SIGNAL(triggered()), treeWidget, SLOT(checkItems()));
    connect(actionCheckAllItems, SIGNAL(triggered()), treeWidget, SLOT(checkAllItems()));

    connect(actionUncheckItems, SIGNAL(triggered()), treeWidget, SLOT(uncheckItems()));
    connect(actionUncheckAllItems, SIGNAL(triggered()), treeWidget, SLOT(uncheckAllItems()));

    connect(actionRemoveItems, SIGNAL(triggered()), this, SLOT(removeItems()));
    connect(actionRemoveAllItems, SIGNAL(triggered()), this, SLOT(removeAllItems()));

    // "Actions" actions
    connect(actionConvert, SIGNAL(triggered()), this, SLOT(elabora()));

    connect(actionInfo, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionDonatePaypal, SIGNAL(triggered()), this, SLOT(openPaypalLink()));
    connect(actionReportBug, SIGNAL(triggered()), this, SLOT(bugReport()));
    connect(actionCheckForUpdates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
}

void MainWindowImpl::setupMenu()
{
    menu_File->addAction(actionOpenFiles);
    menu_File->addAction(actionAddFiles);
    menu_File->addSeparator();
    menu_File->addAction(actionImportWindowsIconIco);
    menu_File->addAction(actionImportPDFFile);
    menu_File->addSeparator();
    menu_File->addAction(actionShowOptions);
    menu_File->addSeparator();
    menu_File->addAction(actionClose);

    menu_Edit->addAction(actionRemoveItems);
    menu_Edit->addAction(actionRemoveAllItems);
    menu_Edit->addSeparator();
    menu_Edit->addAction(actionCheckItems);
    menu_Edit->addAction(actionCheckAllItems);
    menu_Edit->addAction(actionUncheckItems);
    menu_Edit->addAction(actionUncheckAllItems);

    menu_Actions->addAction(actionConvert);

    menu_About->addAction(actionInfo);
}

void MainWindowImpl::createContextMenu()
{
    QAction *actionSeparator = new QAction(this);
    actionSeparator->setSeparator(true);

    treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    treeWidget->addAction(actionCheckItems);
    treeWidget->addAction(actionCheckAllItems);
    treeWidget->addAction(actionUncheckItems);
    treeWidget->addAction(actionUncheckAllItems);
    treeWidget->addAction(actionSeparator);
    treeWidget->addAction(actionRemoveItems);
    treeWidget->addAction(actionRemoveAllItems);
}

void MainWindowImpl::openFiles()
{
    iAList->clear();
    addFiles();
}

void MainWindowImpl::addFiles()
{
    CachingSystem::clear();

    QString path = IniSettings::latestOpenedDir();

    QString readableFiltersString = Formats::readableFiltersString();
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Open Image"), path,
                                                          readableFiltersString);

    if (!fileNames.isEmpty())
        loadFiles(fileNames);
}

void MainWindowImpl::importIcoFile()
{
    QString path = IniSettings::latestOpenedDir();

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Icon file"), path, tr("Microsoft icon (*.ico *.icon)"));

    if (!fileName.isEmpty())
        openMultipageFile(fileName);
}

void MainWindowImpl::importPdfFile()
{
    QString path = IniSettings::latestOpenedDir();

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Pdf file"), path, tr("Portable Document Format (*.pdf)"));

    if (!fileName.isEmpty())
        openMultipageFile(fileName);
}

void MainWindowImpl::openMultipageFile(QString fileName)
{
    DialogMultipageEditor *dlg = new DialogMultipageEditor();

    statusBar()->showMessage(tr("Analyzing the file. It may take a while, please wait..."));
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    dlg->readFile(fileName);
    QApplication::restoreOverrideCursor();
    statusBar()->clearMessage();

    dlg->exec();

    if (dlg->result() == 1) {
        QStringList fileNames = dlg->fileNames();
        loadFiles(fileNames);
    }
}

void MainWindowImpl::loadFiles(QStringList fileNames)
{
    treeWidget->clear();

    QFileInfo fi(fileNames.at(0));
    IniSettings::setLatestOpenedDir(fi.path());

    for (int i = 0; i < fileNames.count(); i++) {
        QFileInfo fi(fileNames.at(i));
        ImageAttributes iA;

        /* Controlla i doppioni! */
        if (!checkDuplicates(fileNames, i)) {
            iA.completeFileName = fileNames.at(i).toLocal8Bit();
            iA.fileName = fi.fileName();
            iA.suffix = fi.suffix();
            iA.size = fi.size();
            iA.path = fi.path();
            iAList->append(iA);   // aggiunge
        }
    }
    fillTreeView();
}

bool MainWindowImpl::checkDuplicates(QStringList fileNames, int e)
{
    int cnt = iAList->count();

    for (int i = 0; i < cnt; i++)
        if (iAList->at(i).completeFileName == fileNames.at(e).toLocal8Bit())
            return true;

    return false;
}

void MainWindowImpl::fillTreeView()
{
    treeWidget->addItems(iAList);
}

void MainWindowImpl::elabora()
{
    if (!iAList->isEmpty()) {
        if (treeWidget->thereAreItemsChecked()) {
            if (!lineDirectory->text().isEmpty()) {
                QDir dir(lineDirectory->text());
                if((dir.exists()) || (checkSameDir->isChecked())) {
                    startConversion();
                }
                else {
                    int ret = QMessageBox::warning(0, tr("Warning"),
                                                   tr("The output directory doesn't exists! Would you create it?"),
                                                   QMessageBox::Ok | QMessageBox::Cancel);

                    if (ret == QMessageBox::Ok) {
                        if (!dir.mkpath(lineDirectory->text()))
                            QMessageBox::warning(0, tr("Warning"),
                                                 tr("The directory cannot be created!"));
                        else {
                            startConversion();
                        }
                    }
                }
            }
            else
                QMessageBox::warning(0, tr("Warning"),
                                     tr("Set an output directory!"));
        }
        else
            QMessageBox::warning(0, tr("Warning"),
                                 tr("No image checked!"));
    }
    else
        QMessageBox::warning(0, tr("Warning"),
                             tr("No images to convert!"));
}

void MainWindowImpl::startConversion()
{
    IniSettings::setOutputDir(lineDirectory->text());

    abort_all = false;

    statusBar()->showMessage(tr("Processing..."));
    curr_index = 0;

    m_progressiveNren = spinProgressiveRen->value();    // Used when the progressive number renaming is checked

    convertThread->reset();

    // Colore
    bool noTransparency = this->checkNoTransp->isChecked();
    convertThread->setBackgroundColor(m_bgColor, noTransparency);

    /* Change image size */
    if (groupDimensions->isChecked()) {
        QString aspectRatio = "!";

        if (checkRelative->isChecked())
            aspectRatio = "";

        // "%1 x %2 % !"

        QString resizingString;
        if (comboResizeValues->currentText() == "px")
            resizingString = QString("%1x%2%3")
                    .arg(new_img_width)
                    .arg(new_img_height)
                    .arg(aspectRatio);
        if (comboResizeValues->currentText() == "%")
            resizingString = QString("%1x%2%")
                    .arg(spin_geoWidth->value())
                    .arg(spin_geoHeight->value());

        convertThread->setResize(resizingString);
    }

    /* Change image density */
    if ((groupResolution->isChecked()) && ((m_xResolution != spin_resX->value()) || (m_yResolution != spin_resY->value()))) {
        QString densStr = QString("%1x%2")
                .arg(spin_resX->value())
                .arg(spin_resY->value());

        convertThread->setDensity(densStr);
    }

    if (!radioRotNo->isChecked()) {
        double rotation = 0;

        if (radioRotPos90->isChecked())
            rotation = 90.0;
        if (radioRotNeg90->isChecked())
            rotation = -90.0;
        if (radioRot180->isChecked())
            rotation = 180.0;

        convertThread->setRotation(rotation);
    }

    if (!radioFlipNo->isChecked()) {
        FlipOrientation fo;

        if (radioFlipV->isChecked())
            fo = VERTICAL;
        if (radioFlipH->isChecked())
            fo = HORIZONTAL;

        convertThread->setFlip(fo);
    }

    convertThread->setResamplingFilter(m_resamplingFilter);

    convert();

    dlgCStatus->setup(treeWidget->countChecked());
    dlgCStatus->exec();
}

void MainWindowImpl::convert()
{
    if ((treeWidget->topLevelItem(curr_index))->checkState(0)) {
        IniSettings::setLatestWrittenFormatIndex(comboWFormats->currentIndex());
        statusBar()->showMessage(iAList->at(curr_index).fileName, 5000);

        QString out_format;

        QString inputFilename = iAList->at(curr_index).completeFileName;

        QFileInfo fi(inputFilename);

        QList<MagickDefine> magickDefines;

        if (comboWFormats->currentIndex() != 0) {
            out_format = comboWFormats->currentText().split(" - ").at(0);
        }
        else {
            QString suffix = fi.suffix();

            out_format = suffix;
        }

        out_format = out_format.toLower();

        QString outFileName = QString("%1/%2.%3").arg(destinationPath()).arg(fi.completeBaseName()).arg(out_format);

        int quality = -1;

        /*
            For further details:
            http://www.imagemagick.org/script/command-line-options.php#quality
        **/

        // Compression level 1-100
        if ((out_format == "jpg") || (out_format == "jpeg") || (out_format == "mpeg") || (out_format == "mpg")) {
            quality = m_jpgQuality;
        }

        // Compression level 0-9. For compression level 0 (quality value less than 10), the Huffman-only strategy is used
        if ((out_format == "png") || (out_format == "mng")) {
            quality = m_pngQuality * 10;
        }

        if ((out_format == "webp")) {
            quality = m_webPQuality;

            // magick, key, value
            magickDefines << MagickDefine("webp", "lossless", (m_isWebPLosslessCompression) ? "true" : "false");
            magickDefines << MagickDefine("webp", "method", QString::number(m_webPCompression));
            magickDefines << MagickDefine("webp", "preprocessing", (m_iskWebPDithering) ? "1" : "0");
        }

        convertThread->setInputPicture(inputFilename);
        convertThread->setFormat(out_format);
        convertThread->setQuality(quality);
        convertThread->setMagickDefines(magickDefines);
        convertThread->setRemoveMetadata(m_removeMetadata);
        convertThread->setOutputDir(destinationPath());

        if (checkRename->isChecked()) {
            // Special renaming settings for the output file enabled
            QString renamedOutFileName;

            if (radioPrefixSuffix->isChecked())
                renamedOutFileName = renameFileNameOnPrefixSuffix(outFileName);
            else
                renamedOutFileName = renameFileNameOnProgressiveN(outFileName);

            convertThread->setOutputPictureName(renamedOutFileName);
        }
        else {
            // No special renaming settings for the output file. Set it with tho original name + new extension
            convertThread->setOutputPictureName(outFileName);
        }

        convertThread->setOverwrite(checkOverwrite->isChecked());

        convertThread->start();
    }
    else {
        nextConversion(2);
    }
}

void MainWindowImpl::nextConversion(int conv_status)
{
    if (!abort_all) {
        if ((treeWidget->topLevelItem(curr_index))->checkState(0)) {
            dlgCStatus->step(conv_status, iAList->at(curr_index).fileName);
        }

        QString msg = "";
        if (conv_status == 1) {
            msg = "Converted";
            // Add the destination dirs to DialogConversionStatus so them can be opened at the end of the image processing
            dlgCStatus->addOutputDirectory(QDir::toNativeSeparators(destinationPath()));
        }
        else if (conv_status == -1)
            msg = "Error";

        (treeWidget->topLevelItem(curr_index))->setText(1, msg);

        curr_index++;
        if (curr_index < iAList->count()) {
            convert();
        }
        else {
            statusBar()->showMessage(tr("Processing finished!"), 5000);
        }
    }
}

QString MainWindowImpl::renameFileNameOnPrefixSuffix(QString oldFileName)
{
    QString t_renamingString = lineRename->text();

    QFileInfo fi(oldFileName);
    QString newFileName = QString("%1.%2")
            .arg(t_renamingString.replace("#", fi.completeBaseName()))
            .arg(fi.suffix());

    return QString("%1/%2").arg(fi.path()).arg(newFileName);
}

QString MainWindowImpl::renameFileNameOnProgressiveN(QString oldFileName)
{
    QString t_renamingString = lineRename->text();

    QFileInfo fi(oldFileName);  // For the output directory
    QString newFileName = QString("%1.%2")
            .arg(t_renamingString.replace("#", QString::number(m_progressiveNren)))
            .arg(fi.suffix());

    m_progressiveNren++;    // the key of progressive number renaming (set to start when the conversion in starded)

    return QString("%1/%2").arg(fi.path()).arg(newFileName);
}

void MainWindowImpl::loadFormats()
{
    Formats::loadFormats();

    comboWFormats->addItems(Formats::writableFilters());
    comboWFormats->setCurrentIndex(0);
}

void MainWindowImpl::setCurrentDirectory()
{
    bool enabled;

    if (checkSameDir->isChecked())
        enabled = false;
    else
        enabled = true;

    lineDirectory->setEnabled(enabled);
    bntSelDir->setEnabled(enabled);
}

void MainWindowImpl::openOutDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!dir.isNull())
        lineDirectory->setText(QDir::toNativeSeparators(dir));
    // La cartella viene salvata quando viene avviata la procedura di conversione

}

void MainWindowImpl::loadOptions()
{
    IniSettings::init();

    this->restoreGeometry(IniSettings::windowGeometry());

    lineDirectory->setText(QDir::toNativeSeparators(IniSettings::outputDir()));
    comboWFormats->setCurrentIndex(IniSettings::latestWrittenFormatIndex());
    checkOverwrite->setChecked(IniSettings::isOverwriteMode());

    loadQuality();

    m_bgColor = IniSettings::backgroundColor();

    QColor c;
    c.setNamedColor(m_bgColor);
    pushColor->setColor(c);

    checkShowPreview->setChecked(IniSettings::previewChecked());
    groupDimensions->setChecked(IniSettings::scaleChecked());
    comboResizeValues->setCurrentIndex(IniSettings::measure());
    checkRelative->setChecked(IniSettings::aspectRatioChecked());
    groupResolution->setChecked(IniSettings::resolutionChecked());
    checkSameDir->setChecked(IniSettings::imageDirChecked());
    checkRename->setChecked(IniSettings::renameChecked());
    checkNoTransp->setChecked(IniSettings::bgColorChecked());

    enableRenameLine();
    enableProgressiveSpin();
}

void MainWindowImpl::setQuality()
{
    loadQuality();

    DialogQuality *dlg = new DialogQuality();
    dlg->setInitValues(m_jpgQuality, m_pngQuality, m_resamplingFilter);

    dlg->setIsWebPLosslessCompression(m_isWebPLosslessCompression);
    dlg->setWebPCompression(m_webPCompression);
    dlg->setWebPQuality(m_webPQuality);
    dlg->setIskWebPDithering(m_iskWebPDithering);

    dlg->setIsRemoveMetadata(m_removeMetadata);

    if (dlg->exec()) {
        m_jpgQuality = dlg->getJpegQuality();
        m_pngQuality = dlg->getPngQuality();

        m_isWebPLosslessCompression = dlg->isWebPLosslessCompression();
        m_webPCompression = dlg->getWebPCompression();
        m_webPQuality = dlg->getWebPQuality();
        m_iskWebPDithering = dlg->iskWebPDithering();

        m_resamplingFilter = dlg->getResamplingFilter();

        m_removeMetadata = dlg->isRemoveMetadata();

        IniSettings::setJpgQuality(m_jpgQuality);
        IniSettings::setPngQuality(m_pngQuality);
        IniSettings::setLatestInterpFiltIdx((int)m_resamplingFilter);

        IniSettings::setIsWebPLosslessCompression(m_isWebPLosslessCompression);
        IniSettings::setWebPCompression(m_webPCompression);
        IniSettings::setWebPQuality(m_webPQuality);
        IniSettings::setIskWebPDithering(m_iskWebPDithering);

        IniSettings::setIsRemoveMetadata(m_removeMetadata);
    }

    delete dlg;
}

void MainWindowImpl::loadQuality()
{
    m_jpgQuality = IniSettings::jpgQuality();
    m_pngQuality = IniSettings::pngQuality();
    m_resamplingFilter = (IMFilterType)IniSettings::latestInterpFiltIdx();

    m_isWebPLosslessCompression = IniSettings::isWebPLosslessCompression();
    m_webPCompression = IniSettings::webPCompression();
    m_webPQuality = IniSettings::webPQuality();
    m_iskWebPDithering = IniSettings::iskWebPDithering();

    m_removeMetadata = IniSettings::isRemoveMetadata();
}

void MainWindowImpl::removeItems()
{
    if (!iAList->isEmpty()) {
        treeWidget->removeItems(iAList);
        resetDisplays();
    }
}

void MainWindowImpl::removeAllItems()
{
    if (!iAList->isEmpty()) {
        iAList->clear();
        CachingSystem::clear();
        treeWidget->clear();

        resetDisplays();
    }
}

void MainWindowImpl::relativeSizeW()
{
    double value = spin_geoWidth->value();

    if (checkLinkAspect->isChecked()) {
        if (checkRelative->isChecked()) {
            if (comboResizeValues->currentText() == "px") {
                double relative_H = ( ((double)img_height * value) / (double)img_width);
                spin_geoHeight->setValue((double)qRound(relative_H));
            }
        }
    }

    if (comboResizeValues->currentText() == "%") {
        if (checkRelative->isChecked())
            spin_geoHeight->setValue(spin_geoWidth->value());
    }

    showNewSizePreview();
}

void MainWindowImpl::relativeSizeH()
{
    double value = spin_geoHeight->value();

    if (checkLinkAspect->isChecked()) {
        if (checkRelative->isChecked()) {
            if (comboResizeValues->currentText() == "px") {
                double relative_V = ( ((double)img_width * value) / (double)img_height);
                spin_geoWidth->setValue((double)qRound(relative_V));
            }
        }
    }

    if (comboResizeValues->currentText() == "%") {
        if (checkRelative->isChecked())
            spin_geoWidth->setValue(spin_geoHeight->value());
    }

    showNewSizePreview();
}

void MainWindowImpl::loadPreview(QString fileName)
{
    labelPreview->loadPreview(fileName, checkShowPreview->isChecked());
}

void MainWindowImpl::showImageInformations(int orig_w, int orig_h, double orig_dens_x, double orig_dens_y)
{
    /*
        Method executed by the signal myLabelPreviewer::previewReady(orig_w, orig_h, orig_dens_x, orig_dens_y);
    */

    img_width = orig_w;
    img_height = orig_h;

    m_xResolution = orig_dens_x;
    m_yResolution = orig_dens_y;

    int index = treeWidget->currentIndex().row();

    labelType->setText(QString("<span style=\" font-size:8pt;\">%1</span>")
                           .arg(iAList->at(index).suffix));

        QString sSize = SizeUtil::simplifyFileSize(iAList->at(index).size);
        labelFileSize->setText(QString("<span style=\" font-size:8pt;\">%1</span>")
                               .arg(sSize));

        label_ImageSize->setText(QString("<span style=\" font-size:8pt;\">%1 x %2 px</span>")
                                 .arg(img_width)
                                 .arg(img_height));

        label_ImageResolution->setText(QString("<span style=\" font-size:8pt;\">X = %1 Y = %2</span>")
                                       .arg(m_xResolution)
                                       .arg(m_yResolution));
}

void MainWindowImpl::onItemSelection()
{
    if (!iAList->isEmpty()) {
        showPreviewAndInfos();
        showNewSizePreview();
    }
}

void MainWindowImpl::onPushResetClick()
{
    selectGeometryUnit(comboResizeValues->currentText());
    spin_resX->setValue(m_xResolution);
    spin_resY->setValue(m_yResolution);
}

void MainWindowImpl::showPreviewAndInfos()
{
    int index = treeWidget->currentIndex().row();
    loadPreview(iAList->at(index).completeFileName);
}

void MainWindowImpl::editSettings()
{
    DialogOptions dlg;
    dlg.exec();

    if (dlg.result() == 1) {
        checkOverwrite->setChecked(IniSettings::isOverwriteMode());
    }
}

void MainWindowImpl::about()
{
    DialogInfo dlg;
    dlg.exec();
}

void MainWindowImpl::enableRenameLine()
{
    if (checkRename->isChecked()) {
        lineRename->setEnabled(true);
        radioPrefixSuffix->setEnabled(true);
        radioProgressive->setEnabled(true);
        spinProgressiveRen->setEnabled(true);
        labeStartWith->setEnabled(true);
    }
    else
    {
        lineRename->setEnabled(false);
        radioPrefixSuffix->setEnabled(false);
        radioProgressive->setEnabled(false);
        spinProgressiveRen->setEnabled(false);
        labeStartWith->setEnabled(false);
    }

    enableProgressiveSpin();
}

void MainWindowImpl::enableProgressiveSpin()
{
    if (radioProgressive->isChecked()) {
        spinProgressiveRen->setEnabled(true);
        labeStartWith->setEnabled(true);
    }
    else {
        spinProgressiveRen->setEnabled(false);
        labeStartWith->setEnabled(false);
    }
}

void MainWindowImpl::enableShowPreview()
{
    if(treeWidget->selectedItems().count() > 0)
        onItemSelection();
    else
        labelPreview->setText(tr("Preview"));
}

void MainWindowImpl::selectGeometryUnit(QString unit)
{
    if (unit == "px") {
        spin_geoWidth->setDecimals(0);
        spin_geoWidth->setSuffix(" px");
        spin_geoWidth->setValue((double)img_width);

        spin_geoHeight->setDecimals(0);
        spin_geoHeight->setSuffix(" px");
        spin_geoHeight->setValue((double)img_height);
    }
    if (unit == "%") {
        //spin_geoWidth->setDecimals(2);
        spin_geoWidth->setSuffix(" %");
        spin_geoWidth->setValue(100.00);

        //spin_geoHeight->setDecimals(2);
        spin_geoHeight->setSuffix(" %");
        spin_geoHeight->setValue(100.00);
    }
}

/* FIXME: Forse è meglio cambiare nome */
void MainWindowImpl::showNewSizePreview()
{
    if (comboResizeValues->currentText() == "%") {
        double new_width, new_height, round_new_width, round_new_height;

        new_width = (((double)img_width / 100) * (spin_geoWidth->value()));
        new_height = (((double)img_height / 100) * (spin_geoHeight->value()));

        round_new_width = qRound(new_width);
        round_new_height = qRound(new_height);

        if (treeWidget->selectedItems().count() > 0)
            labelPixelGeometry->setText(QString("<span style=\" font-size:8pt;\"><center>%1 x %2 pixels</center></span>")
                                        .arg(round_new_width)
                                        .arg(round_new_height));

        new_img_width = round_new_width;
        new_img_height = round_new_height;
    }
    if (comboResizeValues->currentText() == "px") {
        if (treeWidget->selectedItems().count() > 0)
            labelPixelGeometry->setText(QString("<span style=\" font-size:8pt;\"><center>%1 x %2 pixels</center></span>")
                                        .arg(spin_geoWidth->value())
                                        .arg(spin_geoHeight->value()));

        new_img_width = spin_geoWidth->value();
        new_img_height = spin_geoHeight->value();
    }
}

void MainWindowImpl::resetDisplays()
{
    labelPreview->setText(tr("Preview"));

    img_width = 0;
    img_height = 0;

    new_img_width = 0;
    new_img_height = 0;

    m_xResolution = 0;
    m_yResolution = 0;

    spin_resX->setValue(m_xResolution);
    spin_resY->setValue(m_yResolution);

    if (comboResizeValues->currentText() == "%") {
        spin_geoWidth->setValue(100);
        spin_geoHeight->setValue(100);
    }
    if (comboResizeValues->currentText() == "px") {
        spin_geoWidth->setValue(1);
        spin_geoHeight->setValue(1);
    }

    labelType->setText(" - ");
    label_ImageSize->setText(" - ");
    labelFileSize->setText(" - ");
    label_ImageResolution->setText(" - ");

    labelPixelGeometry->setText("");
}

void MainWindowImpl::checkVersion()
{
    int savedVersion = IniSettings::currentVersion();
    int currentVersion = globals::CURRENT_INTERNAL_VERSION;

    if (savedVersion < currentVersion) {
#if defined(Q_OS_WIN)
	// Open thank you page on Windows
		QString welcomePage = QString("http://converseen.fasterland.net/thank/");

		QDesktopServices::openUrl(QUrl(welcomePage, QUrl::TolerantMode));
#endif

		IniSettings::setCurrentVersion(currentVersion);
    }
}

void MainWindowImpl::saveSettings()
{
    IniSettings::setWindowGeometry(saveGeometry());
    IniSettings::setPreviewChecked(checkShowPreview->isChecked());
    IniSettings::setScaleChecked(groupDimensions->isChecked());
    IniSettings::setMeasure(comboResizeValues->currentIndex());
    IniSettings::setAspectRatioChecked(checkRelative->isChecked());
    IniSettings::setResolutionChecked(groupResolution->isChecked());
    IniSettings::setImageDirChecked(checkSameDir->isChecked());
    IniSettings::setRenameChecked(checkRename->isChecked());
    IniSettings::setBgColorChecked(checkNoTransp->isChecked());
    IniSettings::setOverwriteMode(checkOverwrite->isChecked());

    IniSettings::settings->sync();
}

void MainWindowImpl::overwriteDialog(QString baseName)
{
    /*
        TODO: Sostituisci tutte (vedi sotto)
    */
    bool ok;
    QString newBaseName;

    while (newBaseName.isEmpty()) {
        newBaseName = QInputDialog::getText(0, tr("Suggest a new name!"),
                                            tr("New name:"), QLineEdit::Normal,
                                            baseName, &ok);

        if (!ok) {
            QMessageBox::StandardButton msg;
            msg = QMessageBox::question(this, tr("Please, specify a name!"),
                                        /* FIXME: tradurre meglio! */ tr("You should specify a name.\nClick Retry to specify a new name.\nClick Ignore to cancel this operation.\nClick Abort to cancel all operations."),
                                        QMessageBox::Retry | QMessageBox::Ignore | QMessageBox::Abort);

            if (msg == QMessageBox::Ignore) {
                convertThread->stopProcess();
                break;
            }

            if (msg == QMessageBox::Abort) {
                stopProcess();
                break;
            }
        }
        else {
            if (newBaseName.isEmpty())
                QMessageBox::warning(0, tr("Warning!"),
                                     tr("The filename should not be empty!"));

            if (newBaseName == baseName) {
                QMessageBox::StandardButton msg;
                msg = QMessageBox::question(this, tr("Warning!"),
                                            tr("The name chosen is identical to the initial one. Do you want to overwrite the original file?"),
                                            QMessageBox::Yes | QMessageBox::No); // TODO: mettere si per tutte!

                if (msg == QMessageBox::No)
                    newBaseName.clear();
            }
        }
    }
    //convertThread->mutex.unlock();
    convertThread->setNewBasename(newBaseName, ok);
}

void MainWindowImpl::stopProcess()
{
    convertThread->stopProcess();
    abort_all = true;
    statusBar()->showMessage(tr("Aborted!"), 5000);
}

void MainWindowImpl::errorMessage(QString err_status)   // Manda stringa di errore al dialog di conversione
{
    dlgCStatus->setErrorMsg(err_status);
}

void MainWindowImpl::choseBackgroundColor()
{
    QColor color;
    color = QColorDialog::getColor(Qt::green, this);

    m_bgColor = color.name();

    pushColor->setColor(color);

    IniSettings::setBackgroundColor(m_bgColor);
}

QString MainWindowImpl::destinationPath()
{
    QString dest = lineDirectory->text();

    if (checkSameDir->isChecked())
        dest = iAList->at(curr_index).path;

    return dest;
}

void MainWindowImpl::openPaypalLink()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HQA6TBT5354FC", QUrl::TolerantMode));
}

void MainWindowImpl::checkForUpdates()
{
    UpdateChecker *up = new UpdateChecker();
    up->checkForUpdates();
}

void MainWindowImpl::bugReport()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Faster3ck/Converseen/issues", QUrl::TolerantMode));
}

void MainWindowImpl::setRelativeSizeCheckboxes(int state)
{
    if (state == 0)
        checkLinkAspect->setEnabled(false);
    else
        checkLinkAspect->setEnabled(true);
}

void MainWindowImpl::setOverwriteStatus(int state)
{
    bool isChecked = state == 0 ? false : true;

    IniSettings::setOverwriteMode(isChecked);
    IniSettings::settings->sync();
}
