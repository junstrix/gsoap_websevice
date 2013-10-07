#ifndef TEMP_H
#define TEMP_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>


namespace Ui {
    class Temp;
}

class Temp : public QWidget
{
    Q_OBJECT

public:
    explicit Temp(QWidget *parent = 0);
    ~Temp();

public slots:
    void on_lm75_startButton_clicked();
    void on_lm75_stopButton_clicked();
    void temp_get();

private:
    Ui::Temp *ui;
    QTimer   *timer ;
};

#endif // TEMP_H
