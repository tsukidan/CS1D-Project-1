#include "purchasefoods.h"
#include "ui_purchasefoods.h"
#include "foodshoppingcartitem.h"
#include "sqldatabase.h"
/*

RouteDisplayer::RouteDisplayer(QWidget *parent, QList<int> route, int totalDistance) :
    QDialog(parent),
    ui(new Ui::RouteDisplayer),
    route(route),
    totalDistance(totalDistance),
    model(new QStandardItemModel(this))

*/

purchaseFoods::purchaseFoods(QWidget *parent) :
    QDialog(parent),
    purchaseUi(new Ui::purchaseFoods),
    model(new QStandardItemModel(this))
{
    purchaseUi->setupUi(this);
    QList<food> foodList;

    purchaseUi->cartFoodTable->setRowCount(foodList.size());
}

purchaseFoods::~purchaseFoods()
{
    delete purchaseUi;
}
