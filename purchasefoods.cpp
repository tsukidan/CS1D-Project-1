#include "purchasefoods.h"
#include "ui_purchasefoods.h"
#include "foodshoppingcartitem.h"
#include "sqldatabase.h"
#include "customerpage.h"

purchaseFoods::purchaseFoods(QWidget *parent, QList<int>route, int totalDistance) :
    QDialog(parent),
    purchaseUi(new Ui::purchaseFoods),
    route(route),
    totalDistance(totalDistance),
    model(new QStandardItemModel(this))
{
    purchaseUi->setupUi(this);
    QList<food> foodList;

qDebug() << "test";

for (auto cityID: route)
{
        qDebug() << "test";
        QString cityName = SQLDatabase::GetCityNameById(cityID);

        QList<food> subList = SQLDatabase::GetFoodsForCity(cityID);
        foodList += subList;
}

    purchaseUi->TotalDistance->setText(QString("%1").arg(totalDistance));

    purchaseUi->cartFoodTable->setRowCount(foodList.size());

    for (int i = 0; i < foodList.size(); i++)
    {
       qDebug() << "test";
        FoodShoppingCartItem* shoppingItem = new FoodShoppingCartItem(
                    this, foodList[i].cityName, foodList[i].foodName, foodList[i].price);
        connect(shoppingItem, &FoodShoppingCartItem::qtyChanged, this, &purchaseFoods::qtyChanged);
        shoppingCart.append(shoppingItem);

        QSpinBox* spinBox = new QSpinBox(purchaseUi->cartFoodTable);
        purchaseUi->cartFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), shoppingItem, &FoodShoppingCartItem::setQty);

        QTableWidgetItem* item = new QTableWidgetItem(foodList[i].cityName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 1, item);

        item = new QTableWidgetItem(foodList[i].foodName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 2, item);

        item = new QTableWidgetItem(
                    tr("$%1").arg(foodList[i].price, 0, 'f', 2));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 3, item);
    }

    purchaseUi->cartFoodTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
void purchaseFoods::qtyChanged()
{
    float totalPrice = 0;

    for (auto shoppingItem: shoppingCart)
    {
        totalPrice += shoppingItem->getPrice() * shoppingItem->getQty();
    }
    purchaseUi->totalFood->setText(tr("$%1").arg(totalPrice, 0, 'f', 2));
}
purchaseFoods::~purchaseFoods()
{
    delete purchaseUi;
}


void purchaseFoods::on_pushButton_clicked()
{
    CustomerPage *newCustomerUi;
    newCustomerUi = new CustomerPage(this);
    hide();
    newCustomerUi->show();
}

void purchaseFoods::on_listFoods_clicked()
{
    QList<food> foodList;

qDebug() << "test";

for (auto cityID: route)
{
        qDebug() << "test";
        QString cityName = SQLDatabase::GetCityNameById(cityID);

        QList<food> subList = SQLDatabase::GetFoodsForCity(cityID);
        foodList += subList;
}

    purchaseUi->TotalDistance->setText(QString("%1").arg(totalDistance));

    purchaseUi->cartFoodTable->setRowCount(foodList.size());

    for (int i = 0; i < foodList.size(); i++)
    {
       qDebug() << "test";
        FoodShoppingCartItem* shoppingItem = new FoodShoppingCartItem(
                    this, foodList[i].cityName, foodList[i].foodName, foodList[i].price);
        connect(shoppingItem, &FoodShoppingCartItem::qtyChanged, this, &purchaseFoods::qtyChanged);
        shoppingCart.append(shoppingItem);

        QSpinBox* spinBox = new QSpinBox(purchaseUi->cartFoodTable);
        purchaseUi->cartFoodTable->setCellWidget(i, 0, spinBox);
        QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), shoppingItem, &FoodShoppingCartItem::setQty);

        QTableWidgetItem* item = new QTableWidgetItem(foodList[i].cityName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 1, item);

        item = new QTableWidgetItem(foodList[i].foodName);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 2, item);

        item = new QTableWidgetItem(
                    tr("$%1").arg(foodList[i].price, 0, 'f', 2));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        purchaseUi->cartFoodTable->setItem(i, 3, item);
    }

    purchaseUi->cartFoodTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
