#ifndef BERLINCITYDISPLAY_H
#define BERLINCITYDISPLAY_H

#include <QDialog>

namespace Ui {
class BerlinCityDisplay;
}

class BerlinCityDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit BerlinCityDisplay(QWidget *parent = nullptr);
    ~BerlinCityDisplay();

private:
    Ui::BerlinCityDisplay *ui;
};

#endif // BERLINCITYDISPLAY_H
