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
    /// BerilnCityDisplay
    ///
    /// Constructor specific for BerlinCityDisplay.
    /// Displays the closets cities from Berlin trip.
    explicit BerlinCityDisplay(QWidget *parent = nullptr);
    /// BerlinCityDisplay destructor.
    ///
    /// Deletes the ui when it's scope is left.
    ~BerlinCityDisplay();

private:
    Ui::BerlinCityDisplay *ui;
};

#endif // BERLINCITYDISPLAY_H
