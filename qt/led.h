#ifndef LED_H
#define LED_H

#include <QWidget>


namespace Ui
{
    class Led;
}

class Led : public QWidget
{
    Q_OBJECT

public:
    explicit Led(QWidget *parent = 0);
    ~Led();

private:
    Ui::Led *ui;


private slots :
        void on_ledOn_pushButton_clicked();

        void on_ledOff_pushButton_clicked();

        void on_led1_radioButton_clicked();

        void on_led2_radioButton_clicked();

        void on_led3_radioButton_clicked();

        void on_led4_radioButton_clicked();

        void on_led5_radioButton_clicked();

        void on_led6_radioButton_clicked();

        void on_led7_radioButton_clicked();

        void on_led8_radioButton_clicked();

        void on_ledall_On_radioButton_clicked();

        void on_ledall_off_radioButton_clicked();

};

#endif // LED_H
