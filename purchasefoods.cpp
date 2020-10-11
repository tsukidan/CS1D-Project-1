#include "purchasefoods.h"
#include "ui_purchasefoods.h"

purchaseFoods::purchaseFoods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::purchaseFoods)
{
    ui->setupUi(this);
}

purchaseFoods::~purchaseFoods()
{
    delete ui;
}
