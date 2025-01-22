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

#ifndef PIXTREEWIDGET_H
#define PIXTREEWIDGET_H

#include <QTreeWidget>
#include <QList>

class QKeyEvent;
class QDropEvent;
class QDragEnterEvent;
class QDragMoveEvent;

class ImageAttributes
{
public:
    QString completeFileName, fileName, suffix, path, format;
    qint64 size;
};

class PixTreeWidget : public QTreeWidget {

    Q_OBJECT

    public:
        PixTreeWidget(QWidget *parent = 0);
        void addItems(QList<ImageAttributes> *iAList);
        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void setListItems(QList<ImageAttributes> *iAList);
        void removeItems(QList<ImageAttributes> *iAList);
        bool thereAreItemsChecked();
        int countChecked();

    signals:
        void dropped(QStringList, QStringList);

    public slots:
        void checkItems();
        void checkAllItems();
        void uncheckItems();
        void uncheckAllItems();
};

#endif // PIXTREEWIDGET_H
