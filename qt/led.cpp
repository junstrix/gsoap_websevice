#include "led.h"
#include "ui_led.h"
#include "globalvar.h"
#include <QDesktopWidget>



Led::Led(QWidget *parent) :QWidget(parent),ui(new Ui::Led)
{
    ui->setupUi(this);
    setWindowTitle(tr("LED"));

    connect(ui->onButton  , SIGNAL(clicked()) , this , SLOT(on_ledOn_pushButton_clicked() ) ) ;
    connect(ui->offButton , SIGNAL(clicked()) , this , SLOT(on_ledOff_pushButton_clicked())) ;
    connect(ui->led1 , SIGNAL(clicked()) , this , SLOT(on_led1_radioButton_clicked())) ;
    connect(ui->led2 , SIGNAL(clicked()) , this , SLOT(on_led2_radioButton_clicked())) ;
    connect(ui->led3 , SIGNAL(clicked()) , this , SLOT(on_led3_radioButton_clicked())) ;
    connect(ui->led4 , SIGNAL(clicked()) , this , SLOT(on_led4_radioButton_clicked())) ;
    connect(ui->led5 , SIGNAL(clicked()) , this , SLOT(on_led5_radioButton_clicked())) ;
    connect(ui->led6 , SIGNAL(clicked()) , this , SLOT(on_led6_radioButton_clicked())) ;
    connect(ui->led7 , SIGNAL(clicked()) , this , SLOT(on_led7_radioButton_clicked())) ;
    connect(ui->led8 , SIGNAL(clicked()) , this , SLOT(on_led8_radioButton_clicked())) ;
    connect( ui->all_on , SIGNAL(clicked()) , this , SLOT(on_ledall_On_radioButton_clicked()) ) ;
    connect( ui->all_off , SIGNAL(clicked()) , this , SLOT(on_ledall_off_radioButton_clicked()) ) ;

    int x = (qApp->desktop()->width()- this->width()) / 2;
    int y = (qApp->desktop()->height() - this->height()) / 2;
    this->move(x, y);

}

Led::~Led()
{
    delete ui;
}

void Led::on_ledOn_pushButton_clicked()

{
    led_statu = 1 ;
    ui->onButton->setEnabled  ( false );
    ui->all_off->setEnabled   ( false );
    ui->offButton->setEnabled ( true );
    ui->all_on->setEnabled    ( true );
}

void Led::on_ledOff_pushButton_clicked()
{
    led_statu = 0 ;
    ui->offButton->setEnabled ( false );
    ui->all_on->setEnabled    ( false );
    ui->onButton->setEnabled  ( true );
    ui->all_off->setEnabled   ( true );
}

void Led::on_ledall_On_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(9, led_statu ,&result);
}

void Led::on_ledall_off_radioButton_clicked ()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(9, led_statu ,&result);
}

void Led::on_led1_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(1, led_statu ,&result);
}

void Led::on_led2_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(2, led_statu ,&result);
}

void Led::on_led3_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(3, led_statu ,&result);
}

void Led::on_led4_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(4, led_statu ,&result);
}

void Led::on_led5_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(5, led_statu ,&result);
}

void Led::on_led6_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(6, led_statu ,&result);
}

void Led::on_led7_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(7, led_statu ,&result);
}

void Led::on_led8_radioButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.ledControl(8, led_statu ,&result);
}

