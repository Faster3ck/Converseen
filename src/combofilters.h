#ifndef COMBOFILTERS_H
#define COMBOFILTERS_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <Magick++.h>

using namespace Magick;
using namespace std;

class Combofilters : public QComboBox
{
    Q_OBJECT
public:
    explicit Combofilters(QWidget *parent = 0);

    FilterTypes currentFilter();
    int searchFilterIndex(FilterTypes filter);
    
signals:

public slots:
    
private:
    void populateList();

    QList<FilterTypes> filtersList;
};

#endif // COMBOFILTERS_H
