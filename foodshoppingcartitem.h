#ifndef FOODSHOPPINGCARTITEM_H
#define FOODSHOPPINGCARTITEM_H

#include <QObject>

class FoodShoppingCartItem : public QObject
{
    Q_OBJECT
public:
    /// FoodShoppingCartItem constructor
    ///
    /// Initilizes the food shopping cart item.
    FoodShoppingCartItem(QObject *parent, QString cityName,
                         QString foodName, float price);

    /// getCityName
    ///
    /// Returns the city name of FoodShoppingCartItem
    /// @return QString
    QString getCityName();

    /// getFoodName
    ///
    /// Returns the food name of FoodShoppingCartItem
    /// @return QString
    QString getFoodName();

    /// getPrice
    ///
    /// Returns the price of FoodShoppingCartItem
    /// @return float
    float getPrice();

    ///getQty
    ///
    /// Returns the quantity of FoodShoppingCartItems
    /// @return int
    int getQty();

public slots:
    void setQty(int newQty);

signals:
    /// @see routedisplayer::qtyChanged()
    void qtyChanged();

private:
    QString cityName;
    QString foodName;
    float price;
    int qty;

};

#endif // FOODSHOPPINGCARTITEM_H
