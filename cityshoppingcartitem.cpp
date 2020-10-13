#include "cityshoppingcartitem.h"

CityShoppingCartItem::CityShoppingCartItem(QObject *parent, QString cityName, float price) :
    QObject(parent),
    cityName(cityName),
     price(price),
    qty(0)
{

}

QString CityShoppingCartItem::getCityName()
{
    return cityName;
}

float CityShoppingCartItem::getPrice()
{
    return price;
}

int CityShoppingCartItem::getQty()
{
    return qty;
}

void CityShoppingCartItem::setQtyPerCity(int newQty)
{
    if (newQty != qty)
    {
        qty = newQty;
        emit qtyPerCityChanged();
    }
}
