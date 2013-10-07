#include "temp.h"
#include "ui_temp.h"
#include "globalvar.h"
#include <QDesktopWidget>


Temp::Temp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Temp)
{
    ui->setupUi(this);
    setWindowTitle(tr("TEMP"));
    timer = new QTimer(this);

    connect(ui->startButton , SIGNAL(clicked()) , this , SLOT(on_lm75_startButton_clicked())) ;
    connect(ui->stopButton , SIGNAL(clicked()) , this , SLOT(on_lm75_stopButton_clicked())) ;
    connect( timer , SIGNAL( timeout() ) , this , SLOT(temp_get())) ;

    int x = (qApp->desktop()->width()- this->width()) / 2;
    int y = (qApp->desktop()->height() - this->height()) / 2;
    this->move(x, y);
}

Temp::~Temp()
{
    delete ui;
}

void Temp::on_lm75_startButton_clicked ()
{
    timer->start(1000);
    ui->startButton->setEnabled (false);
    ui->stopButton->setEnabled ( true);
}

void Temp::on_lm75_stopButton_clicked ()
{
    timer->stop() ;
    ui->startButton->setEnabled (true);
    ui->stopButton->setEnabled (false);
    ui->lm75_lcdNumber->display ( 0 );
}

void Temp::temp_get()
{
    client.soap_endpoint = myserveripaddr;
    client.lm75ReadTemp(&result);
    if (result == 0)
    {
        ;
    }
    else
    {
        ui->lm75_lcdNumber->display(result);
    }
}
