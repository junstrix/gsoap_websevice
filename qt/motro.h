#ifndef MOTRO_H
#define MOTRO_H

#include <QWidget>

namespace Ui
{
    class Motro;
}

class Motro : public QWidget
{
    Q_OBJECT

public:
    explicit Motro(QWidget *parent = 0);
    ~Motro();
public slots:

    void on_motor_forward_radioButton_clicked();
    void on_motor_revert_radioButton_clicked();
    void on_motor_stop_Button_clicked();
    void on_motor_start_Button_clicked () ;

private:
    Ui::Motro *ui;
    int dir  ;

};

#endif // MOTRO_H
