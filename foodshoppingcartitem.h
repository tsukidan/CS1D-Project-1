#ifndef FOODSHOPPINGCARTITEM_H
#define FOODSHOPPINGCARTITEM_H

#include <QObject>

class FoodShoppingCartItem : public QObject
{
    Q_OBJECT
public:
    FoodShoppingCartItem(QObject *parent, QString cityName,
                         QString foodName, float price);

    QString getCityName();
    QString getFoodName();
    float getPrice();
    int getQty();

public slots:
    void setQty(int newQty);

signals:
    void qtyChanged();

private:
    QString cityName;
    QString foodName;
    float price;
    int qty;

};

#endif // FOODSHOPPINGCARTITEM_H
