#ifndef CITYSHOPPINGCARTITEM_H
#define CITYSHOPPINGCARTITEM_H

#include <QObject>

class CityShoppingCartItem : public QObject
{
    Q_OBJECT
public:
    explicit CityShoppingCartItem(QObject *parent, QString cityName);

    /**************************************************
     * Return the City name as a QString
     *************************************************/
    QString getCityName();

    /**************************************************
     * Return the total price of an item as a float
     *************************************************/
    float   getTotalPrice();

    /**************************************************
     * Return the total quantity of an item as an int
     *************************************************/
    int     getQty();

public slots:
    void setQty(int newQty);
    void setPrice(float newPrice);

signals:
    void qtyPerCityChanged();
    void totalPriceChanged();

private:
QString cityName;
float totalPrice;
int qty;
};

#endif // CITYSHOPPINGCARTITEM_H
