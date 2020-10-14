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

    for (auto cityID: route)
    {
        QString cityName = SQLDatabase::GetCityNameById(cityID);
//        ui->routeList->addItem(cityName);

        QList<food> subList = SQLDatabase::GetFoodsForCity(cityID);
        foodList += subList;
    }

    ui->TotalDistance->setText(QString("%1").arg(totalDistance));

    ui->SelectFoodTable->setRowCount(foodList.size());

    for (int i = 0; i < foodList.size(); i++)
    {
        qDebug() << "test";

        FoodShoppingCartItem* shoppingItem = new FoodShoppingCartItem(
                    this, foodList[i].cityName, foodList[i].foodName, foodList[i].price);
        connect(shoppingItem, &FoodShoppingCartItem::qtyChanged, this, &RouteDisplayer::qtyChanged);
        shoppingCart.append(shoppingItem);

        QSpinBox* spinBox = new QSpinBox(ui->SelectFoodTable);
        ui->SelectFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), shoppingItem, &FoodShoppingCartItem::setQty);

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
