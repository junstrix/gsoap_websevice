#include "motro.h"
#include "ui_motro.h"
#include "globalvar.h"
#include <QDesktopWidget>



Motro::Motro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Motro)
{
    ui->setupUi(this);
    setWindowTitle(tr("MOTOR"));

    connect( ui->forward_radioButton ,SIGNAL(clicked()) ,this , SLOT(on_motor_forward_radioButton_clicked())) ;
    connect( ui->reverse_radioButton ,SIGNAL(clicked()) ,this , SLOT(on_motor_revert_radioButton_clicked())) ;
    connect( ui->startButton , SIGNAL(clicked()) ,this , SLOT(on_motor_start_Button_clicked())) ;
    connect( ui->stopButton , SIGNAL(clicked()) ,this , SLOT(on_motor_stop_Button_clicked())) ;

    int x = (qApp->desktop()->width()- this->width()) / 2;
    int y = (qApp->desktop()->height() - this->height()) / 2;
    this->move(x, y);
}

Motro::~Motro()
{
    delete ui;
}

void Motro::on_motor_forward_radioButton_clicked ()
{
    dir = 1 ;
}

void Motro::on_motor_revert_radioButton_clicked ()
{
    dir = 2 ;
}

void Motro::on_motor_start_Button_clicked ()
{
    client.soap_endpoint = myserveripaddr;
    client.motorControl( 1 , dir , &result );
}

void Motro::on_motor_stop_Button_clicked ()
{
    client.soap_endpoint = myserveripaddr;
    client.motorControl( 1 , 3 , &result );
}


