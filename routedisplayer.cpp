#include "routedisplayer.h"
#include "sqldatabase.h"
#include "ui_routedisplayer.h"

// constructor
RouteDisplayer::RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance) :
    QDialog(parent),
    ui(new Ui::RouteDisplayer),
    route(route),
    totalDistance(totalDistance)
{
    ui->setupUi(this);

    for (auto cityID: route)
    {
        QString cityName = SQLDatabase::GetCityNameById(cityID);
            ui->routeList->addItem(cityName);

    }


    // create & display cities, qtys, price totals
    for (int i = 0; i < cityShoppingCart.size(); i++)
    {
        CityShoppingCartItem* cityShoppingItem = cityShoppingCart[i];

        ui->routeList->insertRow (ui->routeList->rowCount() );
        ui->routeList->setItem   (ui->routeList->rowCount()-1, 0,
                                  new QTableWidgetItem
                                  (cityShoppingItem->getCityName()));
        ui->routeList->setItem   (ui->routeList->rowCount()-1, 1,
                                  new QTableWidgetItem(
                                      tr("%1").arg(cityShoppingItem->getQty())));
        ui->routeList->setItem   (ui->routeList->rowCount()-1, 2,
                                  new QTableWidgetItem(
                                      tr("$%1").arg
                                      (cityShoppingItem->getTotalPrice(),
                                       0, 'f', 2)));
    }

    // display total distance
    ui->TotalDistance->setText(QString("%1").arg(totalDistance));

    // set row count for table
    ui->SelectFoodTable->setRowCount(foodList.size());

    // Food per city display and calculation
    for (int i = 0; i < foodList.size(); i++)
    {
        // stores food related items for each food row
        FoodShoppingCartItem* shoppingItem = new FoodShoppingCartItem(
                    this, foodList[i].cityName, foodList[i].foodName,
                    foodList[i].price);
        connect(shoppingItem, &FoodShoppingCartItem::qtyChanged, this,
                &RouteDisplayer::qtyChanged);
        shoppingCart.append(shoppingItem);

        // stores food related items for each city row

        QSpinBox* spinBox = new QSpinBox(ui->SelectFoodTable);
        ui->SelectFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                         shoppingItem, &FoodShoppingCartItem::setQty);

        QTableWidgetItem* item = new QTableWidgetItem(foodList[i].cityName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->SelectFoodTable->setItem(i, 1, item);

        item = new QTableWidgetItem(foodList[i].foodName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->SelectFoodTable->setItem(i, 2, item);

        item = new QTableWidgetItem(
                    tr("$%1").arg(foodList[i].price, 0, 'f', 2));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->SelectFoodTable->setItem(i, 3, item);
    }

    // reset tables to size to contents
    ui->SelectFoodTable->horizontalHeader()->setSectionResizeMode
            (QHeaderView::ResizeToContents);
    ui->routeList->horizontalHeader()->setSectionResizeMode
            (QHeaderView::ResizeToContents);

}

// destructor
RouteDisplayer::~RouteDisplayer()
{
    delete ui;
}

// Qty Changed - update tables in route displayer
void RouteDisplayer::qtyChanged()
{
    float totalPrice = 0;

    // resets tables
    for (auto cityItem: cityTable)
    {
        cityItem->setQty(0);
        cityItem->setPrice(0);
    }

    // updates the food list for all cities
    for (auto shoppingItem: shoppingCart)
    {
        float subtotal = shoppingItem->getPrice() * shoppingItem->getQty();
        totalPrice += subtotal;

        CityShoppingCartItem* city = cityTable[shoppingItem->getCityName()];
        city->setQty(city->getQty() + shoppingItem->getQty());
        city->setPrice(city->getTotalPrice() + subtotal);
    }

    // updates the food list PER city
    for (int i = 0; i < cityShoppingCart.size(); i++)
    {
        CityShoppingCartItem* cityItem = cityShoppingCart[i];

        ui->routeList->setItem(i, 1, new QTableWidgetItem(
                                   tr("%1").arg(cityItem->getQty())));
        ui->routeList->setItem(i, 2, new QTableWidgetItem(
                                   tr("$%1").arg(cityItem->getTotalPrice(), 0, 'f', 2)));
    }

    // displays total prices
    ui->Subtotal->setText(tr("$%1").arg(totalPrice, 0, 'f', 2));
    ui->SalesTax->setText(tr("$%1").arg(totalPrice * 0.0875, 0, 'f', 2));
    ui->TotalFoodPrice->setText(tr("$%1").arg(totalPrice * 1.0875, 0, 'f', 2));
}
