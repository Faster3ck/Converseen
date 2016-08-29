/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2016
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

#include "dialogquality.h"
#include <Magick++.h>

using namespace Magick;
using namespace std;

DialogQuality::DialogQuality(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(sliderJpegQ, SIGNAL(valueChanged(int)), this, SLOT(setJpegSpin(int)));
    connect(spinJpegQ, SIGNAL(valueChanged(int)), this, SLOT(setJpegSlider(int)));
    connect(sliderPngQ, SIGNAL(valueChanged(int)), this, SLOT(setPngSpin(int)));
    connect(spinPngQ, SIGNAL(valueChanged(int)), this, SLOT(setPngSlider(int)));
    connect(pushOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void DialogQuality::setInitValues(int jpegVal, int pngVal, FilterTypes filter)
{
    spinJpegQ->setValue(jpegVal);
    spinPngQ->setValue(pngVal);

    int filterIndex = comboFilters->searchFilterIndex(filter);
    comboFilters->setCurrentIndex(filterIndex);
}

int DialogQuality::getJpegQuality()
{
    return spinJpegQ->value();
}

int DialogQuality::getPngQuality()
{
    return spinPngQ->value();
}

FilterTypes DialogQuality::getResamplingFilter()
{
    return comboFilters->currentFilter();
}

void DialogQuality::setJpegSpin(int value)
{
    spinJpegQ->setValue(value);
}

void DialogQuality::setJpegSlider(int value)
{
    sliderJpegQ->setValue(value);
}
//
void DialogQuality::setPngSpin(int value)
{
    spinPngQ->setValue(value);
}

void DialogQuality::setPngSlider(int value)
{
    sliderPngQ->setValue(value);
}
