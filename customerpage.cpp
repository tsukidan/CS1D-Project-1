#include "customerpage.h"
#include "ui_customerpage.h"

CustomerPage::CustomerPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerPage)
{
    ui->setupUi(this);
}

CustomerPage::~CustomerPage()
{
    delete ui;
}
