#ifndef CITYSHOPPINGCARTITEM_H
#define CITYSHOPPINGCARTITEM_H

#include <QObject>

class CityShoppingCartItem : public QObject
{
    Q_OBJECT
public:
    explicit CityShoppingCartItem(QObject *parent, QString cityName, float price);

    QString getCityName();
    float getPrice();
    int getQty();

public slots:
    void setQtyPerCity(int newQty);

signals:
    void qtyPerCityChanged();

private:
QString cityName;
float price;
int qty;
};

#endif // CITYSHOPPINGCARTITEM_H
