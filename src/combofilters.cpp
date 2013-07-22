#include "combofilters.h"

Combofilters::Combofilters(QWidget *parent) : QComboBox(parent)
{
    populateList();
}

FilterTypes Combofilters::currentFilter()
{
    return filtersList.at(currentIndex());
}

int Combofilters::searchFilterIndex(FilterTypes filter)
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
