#include "foodshoppingcartitem.h"

FoodShoppingCartItem::FoodShoppingCartItem(QObject *parent, QString cityName,
                                           QString foodName, float price) :
    QObject(parent),
    cityName(cityName),
    foodName(foodName),
    price(price),
    qty(0)
{

}

QString FoodShoppingCartItem::getCityName()
{
    return cityName;
}

QString FoodShoppingCartItem::getFoodName()
{
    return foodName;
}

float FoodShoppingCartItem::getPrice()
{
    return price;
}

int FoodShoppingCartItem::getQty()
{
    return qty;
}

void FoodShoppingCartItem::setQty(int newQty)
{
    if (newQty != qty)
    {
        qty = newQty;
        emit qtyChanged();
    }
}
