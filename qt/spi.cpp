#include "spi.h"
#include "ui_spi.h"
#include "globalvar.h"
#include <QDesktopWidget>


Spi::Spi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Spi)
{
    ui->setupUi(this);
    setWindowTitle(tr("SPI"));

    connect( ui->spi_getButton , SIGNAL(clicked()) , this ,SLOT(on_read_spi_pushButton_clicked()));
    connect( ui->spi_exitButton , SIGNAL(clicked()) ,this ,SLOT(close()));

    int x = (qApp->desktop()->width()- this->width()) / 2;
    int y = (qApp->desktop()->height() - this->height()) / 2;
    this->move(x, y);
}

Spi::~Spi()
{
    delete ui;
}

void  Spi::on_read_spi_pushButton_clicked()
{
    client.soap_endpoint = myserveripaddr;
    client.spiReadId(&result);
    if (result == -1) {
        ;
    }
    else {
        ui->spi_lcdNumber->display ( result );
    }
}
