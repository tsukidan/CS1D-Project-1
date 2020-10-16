#include "cityshoppingcartitem.h"

CityShoppingCartItem::CityShoppingCartItem(QObject *parent, QString cityName) :
    QObject(parent),
    cityName(cityName),
     totalPrice(0),
    qty(0)
{

}

QString CityShoppingCartItem::getCityName()
{
    return cityName;
}

float CityShoppingCartItem::getTotalPrice()
{
    return totalPrice;
}

int CityShoppingCartItem::getQty()
{
    return qty;
}

void CityShoppingCartItem::setQty(int newQty)
{
    if (newQty != qty)
    {
        qty = newQty;
        emit qtyPerCityChanged();
    }
}

void CityShoppingCartItem::setPrice(float newPrice)
{
    if (newPrice != totalPrice)
    {
        totalPrice = newPrice;
        emit totalPriceChanged();
    }
}
