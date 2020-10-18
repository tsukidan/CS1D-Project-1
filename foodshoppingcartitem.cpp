#include "foodshoppingcartitem.h"

// Constructor
FoodShoppingCartItem::FoodShoppingCartItem(QObject *parent, QString cityName,
                                           QString foodName, float price) :
    QObject(parent),
    cityName(cityName),
    foodName(foodName),
    price(price),
    qty(0)
{

}

// get city name
QString FoodShoppingCartItem::getCityName()
{
    return cityName;
}

// get food name
QString FoodShoppingCartItem::getFoodName()
{
    return foodName;
}

// get price
float FoodShoppingCartItem::getPrice()
{
    return price;
}

// get qty
int FoodShoppingCartItem::getQty()
{
    return qty;
}

// set qty
void FoodShoppingCartItem::setQty(int newQty)
{
    if (newQty != qty)
    {
        qty = newQty;
        emit qtyChanged();
    }
}
