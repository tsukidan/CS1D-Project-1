#ifndef ROUTEDISPLAYER_H
#define ROUTEDISPLAYER_H

#include "cityshoppingcartitem.h"
#include "foodshoppingcartitem.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class RouteDisplayer;
}

class RouteDisplayer : public QDialog
{
    Q_OBJECT

public:
    /// RouteDisplayer Constructor
    ///
    /// Populates the page with A food list, cities list, quantity and totals.
    explicit RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance);

    /// ~RouteDisplayer Deconstructor
    ///
    /// Deletes the ui when it goes out of scope.
    ~RouteDisplayer();

public slots:
    /// qtyChanged()
    ///
    /// Updates the qty of food items dynamically when the QSpinBox is updated.
    /// @return void
    void qtyChanged();

private:
    Ui::RouteDisplayer *ui;
    QList<int> route;
    int totalDistance;
    QVector<FoodShoppingCartItem*> shoppingCart;
    QVector<CityShoppingCartItem*> cityShoppingCart;
    QMap<QString, CityShoppingCartItem*> cityTable;
};

#endif // ROUTEDISPLAYER_H
