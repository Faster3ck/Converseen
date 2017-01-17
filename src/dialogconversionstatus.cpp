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

#include <QMenu>
#include <QAction>
#include <QList>
#include <QDesktopServices>
#include <QUrl>
#include "dialogconversionstatus.h"

DialogConversionStatus::DialogConversionStatus(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);

    connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pushAbort, SIGNAL(clicked()), this, SLOT(abort()));

    menu = new QMenu(this);
    actionOpenAllDirs = new QAction(tr("Open all the destination folders"), this);

    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(openDir(QAction*)));
    connect(actionOpenAllDirs, SIGNAL(triggered()), this, SLOT(openAllDirs()));
}

void DialogConversionStatus::setup(int n_images)
{
    treeWidget->clear();

    c_tot = 0;
    c_ok = 0;
    c_no = 0;

    progressBar->reset();
    progressBar->setMinimum(0);
    progressBar->setMaximum(n_images);

    pushClose->setEnabled(false);
    pushAbort->setEnabled(true);

    m_totimages = n_images;

    m_msg.clear();
    outDirs.clear();

    pushOpen->setEnabled(false);
}

void DialogConversionStatus::conversionStatus(int conv_status, QString fileName)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);

    if (conv_status == 1)
        m_msg = tr("Converted");

    item->setText(0, fileName);
    item->setText(1, m_msg);
}

void DialogConversionStatus::counter(int conv_status)
{
    c_tot++;    // Count total images and compare with total global to reset the window

    if (conv_status == 1)
        c_ok++;
    if (conv_status == -1)
        c_no++;

    labelOk->setText(QString::number(c_ok));
    labelKo->setText(QString::number(c_no));
    labelTotal->setText(QString::number(c_tot));
}

void DialogConversionStatus::step(int conv_status, QString fileName) // avviene quando un'immagine è stata processata (o saltata)
{
    counter(conv_status);
    conversionStatus(conv_status, fileName);
    progressBar->setValue(c_tot);

    if (c_tot == m_totimages)
        resetButtons();
}

void DialogConversionStatus::abort()
{
    resetButtons();
    emit stopProcess();
}

void DialogConversionStatus::resetButtons()
{
    pushClose->setEnabled(true);
    pushAbort->setEnabled(false);
}

void DialogConversionStatus::setErrorMsg(QString err_status)
{
    m_msg = err_status;
}

void DialogConversionStatus::addOutputDirectory(QString path)
{
    if (!outDirs.contains(path))
        outDirs.append(path);

    if (c_tot == m_totimages) {
        if (outDirs.count() > 0) {
            setupOpenButton();
            pushOpen->setEnabled(true);
        }
    }
}

void DialogConversionStatus::setupOpenButton()
{
    QList<QAction *> aL;

	menu->clear();

    for (int i = 0; i < outDirs.count(); i++) {
        QString s = outDirs.at(i);
        QAction *a = new QAction(s, this);
        a->setData(s);
        aL << a;
    }

    menu->addActions(aL);
    menu->addSeparator();
    menu->addAction(actionOpenAllDirs);

    pushOpen->setMenu(menu);
}

void DialogConversionStatus::openDir(QAction *action)
{
    QString value = action->data().toString();
    QDesktopServices::openUrl(QUrl("file:///" + value, QUrl::TolerantMode));
}

void DialogConversionStatus::openAllDirs()
{
    for (int i = 0; i < outDirs.count(); i++) {
        QDesktopServices::openUrl(QUrl("file:///" + outDirs.at(i), QUrl::TolerantMode));
    }
}
