#ifndef ROUTEDISPLAYER_H
#define ROUTEDISPLAYER_H

#include <QDialog>

namespace Ui {
class RouteDisplayer;
}

class RouteDisplayer : public QDialog
{
    Q_OBJECT

public:
    explicit RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance);
    ~RouteDisplayer();

private:
    Ui::RouteDisplayer *ui;
    QList<int> route;
    int totalDistance;

    QVector<FoodShoppingCartItem*> shoppingCart;
    QVector<CityShoppingCartItem*> cityShoppingCart;
    QMap<QString, CityShoppingCartItem*> cityTable;
};

#endif // ROUTEDISPLAYER_H
