/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2018
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

#ifndef DIALOGCONVERSIONSTATUS_H
#define DIALOGCONVERSIONSTATUS_H

#include "ui_dialogconversionstatus.h"

class DialogConversionStatus : public QDialog, private Ui::DialogConversionStatus {
    Q_OBJECT
public:
    DialogConversionStatus(QWidget *parent = 0);
    void setup(int n_images);
    void conversionStatus(int conv_status, QString fileName);
    void step(int conv_status, QString fileName);
    void setErrorMsg(QString err_status);
    void addOutputDirectory(QString path);

private:
    int c_tot;
    int c_ok;
    int c_no;

    int m_totimages;

    QString m_msg;

    QStringList outDirs;
    QMenu *menu;
    QAction *actionOpenAllDirs;

    void counter(int conv_status);
    void resetButtons();
    void setupOpenButton();

signals:
    void stopProcess();

private slots:
    void abort();
    void openDir(QAction *action);
    void openAllDirs();
};

#endif // DIALOGCONVERSIONSTATUS_H
