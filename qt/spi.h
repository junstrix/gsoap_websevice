#ifndef SPI_H
#define SPI_H

#include <QWidget>

namespace Ui
{
    class Spi;
}

class Spi : public QWidget
{
    Q_OBJECT

public:
    explicit Spi(QWidget *parent = 0);
    ~Spi();
private slots:
    void on_read_spi_pushButton_clicked();

private:
    Ui::Spi *ui;

};

#endif // SPI_H
