#include "foodshoppingcartitem.h"
#include "routedisplayer.h"
#include "sqldatabase.h"
#include "ui_routedisplayer.h"

RouteDisplayer::RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance) :
    QDialog(parent),
    ui(new Ui::RouteDisplayer),
    route(route),
    totalDistance(totalDistance),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    QList<food> foodList;

    int i = 1;
    for (auto cityID: route)
    {
        QString cityName = SQLDatabase::GetCityNameById(cityID);
        ui->routeList->insertRow (ui->routeList->rowCount() );
        ui->routeList->setItem   (ui->routeList->rowCount()-1, 0,
                                 new QTableWidgetItem(cityName));

        QList<food> subList = SQLDatabase::GetFoodsForCity(cityID);
        foodList += subList;
        i++;
    }

    ui->TotalDistance->setText(QString("%1").arg(totalDistance));

    ui->SelectFoodTable->setRowCount(foodList.size());

    // Food per city display and calculation (?)
    for (int i = 0; i < foodList.size(); i++)
    {
        qDebug() << "test";

        FoodShoppingCartItem* shoppingItem = new FoodShoppingCartItem(
                    this, foodList[i].cityName, foodList[i].foodName, foodList[i].price);
        connect(shoppingItem, &FoodShoppingCartItem::qtyChanged, this, &RouteDisplayer::qtyChanged);
        shoppingCart.append(shoppingItem);

<<<<<<< HEAD
        QSpinBox* spinBox = new QSpinBox(ui->SelectFoodTable);
        ui->SelectFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), shoppingItem, &FoodShoppingCartItem::setQty);

=======
//        CityShoppingCartItem* cityShoppingItem = new CityShoppingCartItem(
//                    this, foodList[i].cityName, foodList[i].price);
//        connect(cityShoppingItem, &CityShoppingCartItem::qtyPerCityChanged,
//                this, &RouteDisplayer::qtyPerCityChanged);
//        cityShoppingCart.append(cityShoppingItem);

//        QStandardItem *item = new QStandardItem;
//        item->setText(tr("%1").arg(0));
//        model->setItem(i, 0, item);
        QSpinBox* spinBox = new QSpinBox(ui->SelectFoodTable);
        ui->SelectFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                         shoppingItem, &FoodShoppingCartItem::setQty);
//        item = new QStandardItem;
//        item->setText(foodList[i].cityName);
//        model->setItem(i, 1, item);
>>>>>>> a28f9f2c01dbae2334f91a5b432e4b70d94c970a
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

    ui->SelectFoodTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

RouteDisplayer::~RouteDisplayer()
{
    delete ui;
}

void RouteDisplayer::qtyChanged()
{
    float totalPrice = 0;

    for (auto shoppingItem: shoppingCart)
    {
        totalPrice += shoppingItem->getPrice() * shoppingItem->getQty();
    }
    ui->TotalFoodPrice->setText(tr("$%1").arg(totalPrice, 0, 'f', 2));
}
