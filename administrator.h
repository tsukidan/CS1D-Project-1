#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Header.h"

namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = nullptr);
    ~Administrator();

private:
    Ui::Administrator *ui;
};

#endif // ADMINISTRATOR_H
