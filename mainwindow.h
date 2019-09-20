#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <SDL.h>
#include "QTimer"
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#define SDL_MAIN_HANDLED


#define FORWARD 1
#define BACKWARD 4
#define LEFT 8
#define RIGHT 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort *arduino;
    QList<QString> comList;
    QList<QString> namesList;
    SDL_Joystick *joystick;
    QString com="";
    QString lastCom="";
    QTimer *joystickTimer;
    QStandardItemModel *model;
    QGraphicsScene * scene = new QGraphicsScene();
     QGraphicsPixmapItem  * map  = new QGraphicsPixmapItem();
    QGraphicsPixmapItem * robot  = new QGraphicsPixmapItem();
    QString position;
    int x_pos = 0 ;
    int y_pos = 0 ;
    QString joystickConnected="Joystick Disconnected";
    QString lastEvent = "";
    QString currentEvent = "";
    QString SM = "";
    QString BM = "";
    QString PS_State = "";
private slots:


    void read_buttons();


    void readSerial();

    void on_pushButton_9_clicked();

    void on_pushButton_released();

    void on_pushButton_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_4_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_5_pressed();

    void on_pushButton_8_pressed();

    void on_pushButton_7_pressed();

    void on_pushButton_3_released();

    void on_pushButton_2_released();

    void on_pushButton_4_released();

    void on_pushButton_5_released();

    void on_pushButton_6_released();

    void on_pushButton_7_released();



    void on_pushButton_11_pressed();


    void on_pushButton_12_clicked();

    void on_pushButton_13_pressed();



    void on_PS2Mode_pressed();

    void on_PS4Mode_pressed();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
