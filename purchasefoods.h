#ifndef PURCHASEFOODS_H
#define PURCHASEFOODS_H

#include "foodshoppingcartitem.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class purchaseFoods;
}

class purchaseFoods : public QDialog
{
    Q_OBJECT

public:
    explicit purchaseFoods(QWidget *parent, QList<int> route, int totalDistance);
    ~purchaseFoods();

public slots:
    void qtyChanged();

private slots:
    void on_pushButton_clicked();

    void on_listFoods_clicked();

private:
    Ui::purchaseFoods *purchaseUi;
    QList<int> route;
    int totalDistance;
    QStandardItemModel* model;
    QVector<FoodShoppingCartItem*> shoppingCart;
};

#endif // PURCHASEFOODS_H
