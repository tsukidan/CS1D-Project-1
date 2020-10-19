#ifndef DISPLAYFOODSFORCITY_H
#define DISPLAYFOODSFORCITY_H

#include "Header.h"

namespace Ui {
class DisplayFoodsForCity;
}

class DisplayFoodsForCity : public QDialog
{
    Q_OBJECT

public:
    /// DisplayFoodsForCity constructor
    ///
    /// Initializes the display food ui.
    explicit DisplayFoodsForCity(QWidget *parent, QSqlQuery query);

    /// ~DisplayFoodsForCity destructor
    ///
    /// Deletes the ui when the page is out of scope.
    ~DisplayFoodsForCity();

private:
    Ui::DisplayFoodsForCity *ui;
    QSqlQueryModel *model;
    QString selectedCity;

};

#endif // DISPLAYFOODSFORCITY_H
