#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <adc.h>
#include <led.h>
#include <spi.h>
#include <temp.h>
#include <motro.h>
namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private slots:

    void led_widget() ;
    void motro_widget();
    void spi_widget() ;
    void adc_widget() ;
    void temp_widget();
    void start_server() ;

private:

    Ui::Widget *ui;
    Led    *led ;
    Motro  *motro;
    Temp   *temp ;
    Spi    *spi ;
    ADC    *adc ;

};

#endif // WIDGET_H
