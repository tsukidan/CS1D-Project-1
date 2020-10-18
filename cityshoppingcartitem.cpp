#include "cityshoppingcartitem.h"

// constructor
CityShoppingCartItem::CityShoppingCartItem(QObject *parent, QString cityName) :
    QObject(parent),
    cityName(cityName),
     totalPrice(0),
    qty(0)
{

}

// get city name
QString CityShoppingCartItem::getCityName()
{
    return cityName;
}

// get total price
float CityShoppingCartItem::getTotalPrice()
{
    return totalPrice;
}

// get qty
int CityShoppingCartItem::getQty()
{
    return qty;
}

// set qty
void CityShoppingCartItem::setQty(int newQty)
{
    if (newQty != qty)
    {
        qty = newQty;
        emit qtyPerCityChanged();
    }
}

// set price
void CityShoppingCartItem::setPrice(float newPrice)
{
    if (newPrice != totalPrice)
    {
        totalPrice = newPrice;
        emit totalPriceChanged();
    }
}
