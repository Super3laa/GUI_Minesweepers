#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QByteArray>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //2D Graph simulation
    robot->setPixmap(QPixmap (":/pic/robot.jpg"));
    robot->setPos((0*30)+5,(-0*30)-5);
    map->setPixmap(QPixmap(":/pic/map.png"));
    map->setPos(0,-550);
    scene->addItem(map);
    scene->addItem(robot);
     QGraphicsView * view= new QGraphicsView(scene);
     view->resize(600,600);
     view->setWindowTitle("2D Simulation");
     view->setWindowIcon(QIcon(":/pic/icon.ico"));
   //  view->show();



     ui->setupUi(this);
     model = new QStandardItemModel(19,19,this);


    this->ui->map->setModel(model);

    arduino = new QSerialPort(this);
    if(QSerialPortInfo::availablePorts().length()){
        qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            comList.append(serialPortInfo.portName());
            namesList.append(serialPortInfo.manufacturer());
        }

        qDebug() << comList;
        qDebug() << namesList.last();

        arduino = new QSerialPort(comList.last());
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud57600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
       connect(arduino,SIGNAL(readyRead()),this,SLOT(readSerial()));
       this->ui->NRF->setText("Connected");
       this->ui->NRF->setStyleSheet(QString("color:#00ff00;"));

 }else{
        this->ui->NRF->setText("Disconnected");
        this->ui->NRF->setStyleSheet(QString("color:#ff5252;"));
    }



    joystickTimer = new QTimer(this);
    connect(joystickTimer, SIGNAL(timeout()), this, SLOT(read_buttons()));
   // SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    joystick = SDL_JoystickOpen(0);

    joystickTimer->start(10);



}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_W){
        currentEvent = "F";
    }else if (event->key() == Qt::Key_A){
        currentEvent = "L";
    }else if (event->key() == Qt::Key_C){
        currentEvent = "C";
    }else if (event->key() == Qt::Key_S){
        currentEvent = "B";
    }else if (event->key() == Qt::Key_D){
        currentEvent = "R";
    }else if (event->key() == Qt::Key_Space){
        currentEvent = "S";
    }else{

    }

    if (currentEvent != lastEvent){
        QByteArray data = currentEvent.toUtf8().left(1000);
        arduino->write(data);
        lastEvent =currentEvent;
        qDebug()<<lastEvent;
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_W){
        currentEvent = "S";

    }else if (event->key() == Qt::Key_A){
        currentEvent = "S";

    }else if (event->key() == Qt::Key_C){
        currentEvent = "S";

    }else if (event->key() == Qt::Key_S){
        currentEvent = "S";

    }else if (event->key() == Qt::Key_D){
        currentEvent = "S";

    }else if (event->key() == Qt::Key_Space){
        currentEvent = "S";

   }
    if (currentEvent != lastEvent){
        QByteArray data = currentEvent.toUtf8().left(1000);
        arduino->write(data);
        lastEvent =currentEvent;
        qDebug()<<lastEvent;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    if(arduino->isOpen()){
           arduino->close();
       }
}


void MainWindow::on_pushButton_9_clicked()
{
    QString t = ui->textEdit->toPlainText();
    QByteArray data = t.toUtf8().left(1000) ;


    arduino->write(data);
   // this->ui->pushButton->setText(data);
    this->ui->textEdit->setText("");
}

void MainWindow::readSerial(){
    QByteArray data;
    data.resize(4);
    data = arduino->readAll();
    QString temp = this->ui->label_8->text();

    QString x = QString(data) ;
    temp =x+ temp+"\n" ;
   this->ui->label_8->setText(temp);
   // this->ui->label->addItem(QString(data));
    
     if (x[0]=='S'){
         try{
         //Vector representation
             position = x.mid(1);
             double xx  = position.split(",").at(0).toDouble()/100;
             double yy = position.split(",").at(1).toDouble()/100;
             x_pos = long(xx);
             y_pos = long(yy);

         char xpos =  static_cast<char>('A' - 1 + x_pos);
         QString vec_pos = "( "+QString(xpos)+" , "+QString::number(y_pos)+" )";
           if (SM != vec_pos){
             this->ui->listWidget->addItem(vec_pos);
             SM = vec_pos;
             this->ui->surfaceCounter->display(this->ui->listWidget->count());
          }

         //Graphical representation
         QStandardItem *item1  = new QStandardItem();
         item1->setIcon(QIcon(":/pic/surface.png"));
         model->setItem(y_pos-1,x_pos-1,item1);

         //Ultrasonic reading are sent with the Metal Detector status
         this->ui->Metal_Detection->setText("SurfaceMine");
         this->ui->Metal_Detection->setStyleSheet(QString("color:#ffb142;"));
        } catch(...){


         }

     }else if (x[0]=='B'){
         try{
         //Vector representation
             position = x.mid(1);
             double xx  = position.split(",").at(0).toDouble()/100;
             double yy = position.split(",").at(1).toDouble()/100;
             x_pos = long(xx);
             y_pos = long(yy);

         char xpos =  static_cast<char>('A' - 1 + x_pos);
         QString vec_pos = "( "+QString(xpos)+" , "+QString::number(y_pos)+" )";
         if (BM  != vec_pos){
            this->ui->listWidget_2->addItem(vec_pos);
             this->ui->buriedCounter->display(this->ui->listWidget_2->count());
            BM = vec_pos;
         }
         //Graphical representation

         QStandardItem *item1  = new QStandardItem();
         item1->setIcon(QIcon(":/pic/Buried.png"));
         model->setItem(y_pos-1,x_pos-1,item1);

         //Ultrasonic reading are sent with the Metal Detector status

         this->ui->Metal_Detection->setText("BuriedMine");
         this->ui->Metal_Detection->setStyleSheet(QString("color:#ffb142;"));
        }catch(...){


         }
     }else if (x[0]=='Z'){
         //Ultrasonic reading are sent with the Metal Detector status
//         this->ui->ultraDistance->setText(x.mid(1));
//         this->ui->Metal_Detection->setText("NoMine");
//         this->ui->ultraDistance->setStyleSheet(QString("color:#00ff00;"));
//         this->ui->Metal_Detection->setStyleSheet(QString("color:#00ff00;"));

     }else if (x[0]=='s'){

         if (x[1] == 'F'){this->ui->Robot_Status->setText("Forward");}
         else if (x[1] == 'S'){this->ui->Robot_Status->setText("idle");}
         else if (x[1] == 'B'){this->ui->Robot_Status->setText("Backward");}
         else if (x[1] == 'R'){this->ui->Robot_Status->setText("Right");}
         else if (x[1] == 'L'){this->ui->Robot_Status->setText("Left");}

         this->ui->Robot_Status->setStyleSheet(QString("color:#ffb142;"));
         this->ui->mined->setStyleSheet(QString("color:#ffb142;"));



     }else if (x[0]=='p'){
         try{
           position = x.mid(1);
           double xx  = position.split(",").at(0).toDouble()/100;
           double yy = position.split(",").at(1).toDouble()/100;

          robot->setPos(((xx-1)*30)+5,-((yy-1)*30)-5);
         this->ui->Position->setText(QString::number(xx)+","+QString::number(yy));
         this->ui->Position->setStyleSheet(QString("color:#ffb142;"));
         }catch(...){


         }
     }

}

void MainWindow::read_buttons(){
    if(!SDL_NumJoysticks())
    {   joystickConnected = "Joystick Disconnected";
        this->ui->js->setText("DisConnected");
        this->ui->js->setStyleSheet(QString("color:#ff5252;"));
    }
    else if(SDL_NumJoysticks()){
        joystickConnected= "Joystick Connected";
        this->ui->js->setText("Connected");
        this->ui->js->setStyleSheet(QString("color:#00ff00;"));

    }

    SDL_Event event;
    SDL_PollEvent(&event);
//PS4 commands

   if (PS_State == "PS4"){
    // qDebug()<<"PS4";
     quint8 hat= SDL_JoystickGetHat(joystick,0);
    if(hat == FORWARD){com="F";}//Up Arrow
    else if(hat == RIGHT){com="R";}//Right Arrow
    else if(hat == LEFT){com="L";}//Left Arrow
    else if(hat == BACKWARD){com="B";}//Back Arrow
    else{com="S";}//Sending Stop signal when relasing
    if(SDL_JoystickGetButton(joystick,0)){com = "x";}//cross
    else if(SDL_JoystickGetButton(joystick,1)){com = "o";} // Circle
    else if(SDL_JoystickGetButton(joystick,2)){com = "T";} //triangle
    else if(SDL_JoystickGetButton(joystick,3)){com = "q";} //Square
    else if(SDL_JoystickGetButton(joystick,4)){com = "BM";}//L1
    else if(SDL_JoystickGetButton(joystick,5)){com = "SM";}//R1
    else if(SDL_JoystickGetButton(joystick,6)){com = "SD";}//L2
    else if(SDL_JoystickGetButton(joystick,7)){com = "SU";}//R2
    else if(SDL_JoystickGetButton(joystick,8)){com = "Z";}//share
    else if(SDL_JoystickGetButton(joystick,9)){com = "options";}//options
    else if(SDL_JoystickGetButton(joystick,10)){com = "Ps";}//Ps
   }
//PS2 commands
   if (PS_State  == "PS2"){
    // qDebug()<<"PS2";
    if(SDL_JoystickGetAxis(joystick, 1)== -32768){com="F";}//Up Arrow
    else if(SDL_JoystickGetAxis(joystick, 0)== -32768){com="L";}//Left Arrow
    else if(SDL_JoystickGetAxis(joystick, 0)== 32767){com="R";}//Righ Arrow
    else if(SDL_JoystickGetAxis(joystick, 1)== 32767){com="B";}//Back Arrow
    else if(SDL_JoystickGetButton(joystick,0)){com = "S1900";}//triangle
    else if(SDL_JoystickGetButton(joystick,1)){com = "S00";} // Circle
    else if(SDL_JoystickGetButton(joystick,2)){com = "s0019";} //cross
    else if(SDL_JoystickGetButton(joystick,3)){com = "cal";} //Square
    else if(SDL_JoystickGetButton(joystick,4)){com = "BM";}//L1
    else if(SDL_JoystickGetButton(joystick,5)){com = "SM";}//R1
    else if(SDL_JoystickGetButton(joystick,6)){com = "SD";}//L2
    else if(SDL_JoystickGetButton(joystick,7)){com = "SU";}//R2
    else if(SDL_JoystickGetButton(joystick,8)){com = "Z";}//share
    else if(SDL_JoystickGetButton(joystick,9)){com = "T";}//Start
    else if(SDL_JoystickGetButton(joystick,10)){com = "Ps";}//Ps  */
    else if(SDL_JoystickGetButton(joystick,11)){com = "11";}//Ps  */
    else {com = "S";}
   }
    if(com != lastCom )
    {
        QByteArray data = com.toUtf8().left(1000);
        arduino->write(data);
        qDebug()<< com;
        lastCom = com;
    }
}


void MainWindow::on_pushButton_released()
{
    arduino->write("S");
}

void MainWindow::on_pushButton_pressed()
{
    arduino->write("F");
}

void MainWindow::on_pushButton_3_pressed()
{
    arduino->write("L");

}

void MainWindow::on_pushButton_2_pressed()
{
    arduino->write("R");

}

void MainWindow::on_pushButton_4_pressed()
{
    arduino->write("B");

}

void MainWindow::on_pushButton_6_pressed()
{
    arduino->write("RL");

}

void MainWindow::on_pushButton_5_pressed()
{
    arduino->write("RR");

}

void MainWindow::on_pushButton_8_pressed()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_7_pressed()
{
    arduino->write("siren");

}

void MainWindow::on_pushButton_3_released()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_2_released()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_4_released()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_5_released()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_6_released()
{
    arduino->write("S");

}

void MainWindow::on_pushButton_7_released()
{
    arduino->write("S");

}


void MainWindow::on_pushButton_11_pressed()
{
    QString t = ui->speed->toPlainText();
    t = "sf"+t;
    QByteArray data = t.toUtf8().left(100);
    arduino->write(data);
}


void MainWindow::on_pushButton_12_clicked()
{
    this->ui->label_8->clear();
}

void MainWindow::on_pushButton_13_pressed()
{
    arduino = new QSerialPort(this);

    if(QSerialPortInfo::availablePorts().length()){
        qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            comList.append(serialPortInfo.portName());
            namesList.append(serialPortInfo.manufacturer());
        }

        qDebug() << comList;
        qDebug() << namesList.last();

        arduino = new QSerialPort(comList.last());
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud57600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
       connect(arduino,SIGNAL(readyRead()),this,SLOT(readSerial()));
       this->ui->NRF->setText("Connected");
       this->ui->NRF->setStyleSheet(QString("color:#00ff00;"));

 }else{
        this->ui->NRF->setText("Disconnected");
        this->ui->NRF->setStyleSheet(QString("color:#ff5252;"));
    }
}



void MainWindow::on_PS2Mode_pressed()
{
    PS_State = "PS2";
}

void MainWindow::on_PS4Mode_pressed()
{
    PS_State = "PS4";
}
