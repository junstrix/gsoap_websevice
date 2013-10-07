#include "widget.h"
#include "ui_widget.h"
#include "globalvar.h"
#include <QDesktopWidget>
#include <ns.nsmap>
 int click_countor = 0 ;

Widget::Widget(QWidget *parent) : QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("WED SERVICE"));

   connect(ui->main_ledButton , SIGNAL(clicked()) , this , SLOT(led_widget()));
   connect(ui->main_adcButton , SIGNAL(clicked()) , this , SLOT(adc_widget()) );
   connect(ui->main_spiButton , SIGNAL(clicked()) , this , SLOT(spi_widget()) );
   connect(ui->main_tempButton , SIGNAL(clicked()) , this , SLOT(temp_widget()) );
   connect(ui->main_motroButton , SIGNAL(clicked()) , this , SLOT(motro_widget()) );
   connect(ui->main_quitButton , SIGNAL(clicked()) , this ,SLOT(close())) ;
   connect(ui->main_startButton , SIGNAL(clicked()) , this , SLOT(start_server())) ;

   int x = (qApp->desktop()->width()- this->width()) / 2;
   int y = (qApp->desktop()->height() - this->height()) / 2;
   this->move(x, y);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::start_server()
{

   if( click_countor % 2 == 0 )
   {
       const char *tmp_str = ui->main_addrEdit->text().toStdString().c_str();
       strcpy(myserveripaddr,tmp_str);
       ui->main_addrEdit->setEnabled(false);
       ui->main_startButton->setText("Stop");
   }
   if( click_countor % 2 == 1 )
   {
       led_id = 9 ;            // led_id
       led_statu = 0 ;         //
       result = 0 ;           // return back value
       memset( myserveripaddr , 0 , 128 ) ; //clear server ip address
       ui->main_startButton->setText ( " start " );
       ui->main_addrEdit->setEnabled(true);
   }

   click_countor ++ ;
}


void Widget::led_widget()
{
    led = new Led() ;
    led->show() ;
}

void Widget::adc_widget()
{
    adc = new ADC();
    adc->show() ;
}

void Widget::spi_widget()
{
    spi = new Spi();
    spi->show() ;
}


void Widget::motro_widget()
{
    motro = new Motro();
    motro->show() ;
}

void Widget::temp_widget()
{
    temp = new Temp();
    temp->show() ;
}



