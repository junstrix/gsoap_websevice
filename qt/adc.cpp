#include "adc.h"
#include "ui_adc.h"
#include "globalvar.h"
#include <QDesktopWidget>


ADC::ADC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ADC)
{
    ui->setupUi(this);
    setWindowTitle(tr("ADC"));
    timer = new QTimer(this);


    connect(ui->adc_startButton , SIGNAL(clicked()) , this , SLOT(on_adc_startButton_clicked())) ;
    connect(ui->adc_stopButton, SIGNAL(clicked()) , this ,   SLOT(on_adc_stopButton_clicked())) ;
    connect( timer , SIGNAL(timeout()) , this , SLOT(get_adc()) );

    int x = (qApp->desktop()->width()- this->width()) / 2;
    int y = (qApp->desktop()->height() - this->height()) / 2;
    this->move(x, y);

}

ADC::~ADC()
{
    delete ui;
}

void ADC::on_adc_startButton_clicked ()
{
    timer->start(1000) ;
    ui->adc_startButton->setEnabled ( false );
    ui->adc_stopButton->setEnabled ( true );
}

void ADC::on_adc_stopButton_clicked ()
{
    timer->stop() ;
    ui->adc_startButton->setEnabled ( true );
    ui->adc_stopButton->setEnabled ( false );
    ui->adc_lcdNumber->display ( 0 );
}

void ADC::get_adc ()
{
    client.soap_endpoint = myserveripaddr;
    client.adcReadValue(&result);
    if (result != 0 )
    {
        ui->adc_lcdNumber->display(((long)result*3.3)/1024);
    }

}
