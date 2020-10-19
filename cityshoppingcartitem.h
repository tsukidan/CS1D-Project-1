#ifndef CITYSHOPPINGCARTITEM_H
#define CITYSHOPPINGCARTITEM_H

#include <QObject>

class CityShoppingCartItem : public QObject
{
    Q_OBJECT
public:
    /// CityShoppingCartItem Constructor
    ///
    /// Initializes the data for a shopping cart item.
    explicit CityShoppingCartItem(QObject *parent, QString cityName);

    /// getCityName returns city.
    ///
    /// This function returns the city name to the corresponding object.
    /// @return QString
    QString getCityName();

    /// getTotalPrice returns price for trip.
    ///
    /// This function returns the total price of food items for the trip.
    /// @return float
    float getTotalPrice();

    /// getQty returns food quantity
    ///
    /// This function returns the amount of food items selected.
    /// @return int
    int getQty();

public slots:
    /// setQty sets the number of items.
    ///
    /// This function sets the amount of items being purchased from the cart.
    /// @param int
    /// @see getQty()
    void setQty(int newQty);

    /// setPrice
    ///
    /// This function sets the price for the whole shopping cart.
    /// @param float
    /// @return void
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
