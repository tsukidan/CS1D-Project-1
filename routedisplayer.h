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
    explicit RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance);
    ~RouteDisplayer();

public slots:
    void qtyChanged();

private:
    Ui::RouteDisplayer *ui;
    QList<int> route;
    int totalDistance;
//    QStandardItemModel* model;
    QVector<FoodShoppingCartItem*> shoppingCart;
    QVector<CityShoppingCartItem*> cityShoppingCart;
    QMap<QString, CityShoppingCartItem*> cityTable;
};

#endif // ROUTEDISPLAYER_H
