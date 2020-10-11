#ifndef PURCHASEFOODS_H
#define PURCHASEFOODS_H

#include <QDialog>

namespace Ui {
class purchaseFoods;
}

class purchaseFoods : public QDialog
{
    Q_OBJECT

public:
    explicit purchaseFoods(QWidget *parent = nullptr);
    ~purchaseFoods();

private:
    Ui::purchaseFoods *ui;
};

#endif // PURCHASEFOODS_H
