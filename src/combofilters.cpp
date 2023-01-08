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

#include "combofilters.h"

Combofilters::Combofilters(QWidget *parent) : QComboBox(parent)
{
    populateList();
}

IMFilterType Combofilters::currentFilter()
{
    return filtersList.at(currentIndex());
}

int Combofilters::searchFilterIndex(IMFilterType filter)
{
    return filtersList.indexOf(filter);
}

void Combofilters::populateList()
{
    QStringList filterNamesList;
    filterNamesList << "Automatic"
                    << "Point Filter"
                    << "Box Filter"
                    << "Triangle Filter"
                    << "Hermite Filter"
                    << "Hanning Filter"
                    << "Hamming Filter"
                    << "Blackman Filter"
                    << "Gaussian Filter"
                    << "Quadratic Filter"
                    << "Cubic Filter"
                    << "Catrom Filter"
                    << "Mitchell Filter"
                    << "Lanczos Filter"
                    << "Bessel Filter"
                    << "Sinc Filter";

    filtersList << UndefinedFilter
                << PointFilter
                << BoxFilter
                << TriangleFilter
                << HermiteFilter
                << HanningFilter
                << HammingFilter
                << BlackmanFilter
                << GaussianFilter
                << QuadraticFilter
                << CubicFilter
                << CatromFilter
                << MitchellFilter
                << LanczosFilter
                << BesselFilter
                << SincFilter;

    for (int i = 0; i < filterNamesList.count(); i++) {
        insertItem(i, filterNamesList.at(i));
    }
}
