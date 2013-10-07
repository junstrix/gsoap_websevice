#ifndef ADC_H
#define ADC_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>


namespace Ui
{
    class ADC;
}

class ADC : public QWidget
{
    Q_OBJECT

public:
    explicit ADC(QWidget *parent = 0);
    ~ADC();
private slots:
    void on_adc_startButton_clicked();
    void on_adc_stopButton_clicked();
    void get_adc() ;

private:
    Ui::ADC *ui;
    QTimer *timer ;
};

#endif // ADC_H
