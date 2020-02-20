#include<QDialog>
#include<QLabel>
#include<QDebug>
#include<QFormLayout>
#include<QGroupBox>
#include<QLineEdit>
#include<QTimer>
#include<QTime>
#include<QProgressBar>
#include<QMovie>
#include<QDebug>
#include<QCoreApplication>


#include "displaymessage.h"


QDialog  *busymesgindicator;

uidmessage* uidmessage::settingstance = 0;


uidmessage::uidmessage(QObject *parent):QObject(parent)
{

}
uidmessage* uidmessage::instance()
{
    if(!settingstance)
    {
        settingstance = new uidmessage();
    }
    return settingstance;
}

void uidmessage::DisplayBusyMessage_GIF(QString message)
{
    qDebug() << "Displaying"<<message<<"Message";



    QFont newFont("Crimson Text", 12, QFont::Bold, true);
    busymesgindicator = new QDialog();
    busymesgindicator->setWindowFlags(Qt::FramelessWindowHint);
    busymesgindicator->setFont(newFont);
    //busymesgindicator->setParent(this);



    busymesgindicator->setAutoFillBackground(true);
    //busymesgindicator->setStyleSheet("color: rgb(0, 0, 0); background-color: rgb(255, 252, 142);"); // File Download
    busymesgindicator->setStyleSheet("color: rgb(85, 0, 127); background-color: rgb(255,255,255);"); // File Download
   // busymesgindicator->setGeometry(30, 30, 260, 170);
   // busymesgindicator->setGeometry(5, 80, 310, 60);
    //busymesgindicator->setGeometry(5, 80, 310, 60);
    busymesgindicator->setGeometry(0, 0, 320, 240);
    busymesgindicator->setWindowModality(Qt::ApplicationModal);
    busymesgindicator->setMouseTracking(false);

    QLabel *label = new QLabel(busymesgindicator);
    QMovie *movie=new QMovie(label);
    //movie->setFileName("/usr/FINO/FPB/fino.gif");
    if (!movie->isValid())
    {

    }

    // Play GIF
    //label=new QLabel(busymesgindicator);
    label->setGeometry(115,60,128,128);
    label->setMovie(movie);
    movie->start();
//    QLabel *msg = new QLabel(busymesgindicator);
//    msg->setGeometry(30, 20, 248, 100);
//     //msg->setGeometry(10,20, 250, 20);
//    msg->setText(message);
//    msg->setWordWrap(true);
//    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//    msg->setStyleSheet("border: 0");

    busymesgindicator->show();

    QCoreApplication::processEvents(QEventLoop::AllEvents, 1800);

    QCoreApplication::processEvents();

//    QCoreApplication::processEvents();



}
///////////////////////////////////////////////////////////////////
//
// This below function will display the busy message untill calling
// The hidemessage function
//
//  Input : Display Message
//  Type  : Qstring
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

void uidmessage::Display_BusyMessage(QString message)
{
  //  qDebug() << "Displaying"<<message<<"Message";



    QFont newFont("Crimson Text", 12, QFont::Bold, true);
    busymesgindicator = new QDialog();
    busymesgindicator->setWindowFlags(Qt::FramelessWindowHint);
    busymesgindicator->setFont(newFont);
    //busymesgindicator->setParent(this);


    busymesgindicator->setAutoFillBackground(true);
    //busymesgindicator->setStyleSheet("color: rgb(0, 0, 0); background-color: rgb(255, 252, 142);"); // File Download
    busymesgindicator->setStyleSheet("color: rgb(85, 0, 127); background-color: rgb(255,255,255);"); // File Download
   // busymesgindicator->setGeometry(30, 30, 260, 170);
   // busymesgindicator->setGeometry(5, 80, 310, 60);
    //busymesgindicator->setGeometry(5, 80, 310, 60);
    busymesgindicator->setGeometry(0, 0, 320, 240);
    busymesgindicator->setWindowModality(Qt::WindowModal);
    busymesgindicator->setMouseTracking(false);


    QLabel *msg = new QLabel(busymesgindicator);
    msg->setGeometry(30, 70, 248, 100);
     //msg->setGeometry(10,20, 250, 20);
    msg->setText(message);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    msg->setStyleSheet("border: 0");



    busymesgindicator->show();

    QCoreApplication::processEvents(QEventLoop::AllEvents, 1800);

    QCoreApplication::processEvents();

//    QCoreApplication::processEvents();



}

void uidmessage::Display_BusyMessage_static(QString message)
{

    qDebug() << "Displaying"<<message<<"Message";


    QFont newFont("Crimson Text", 16, QFont::Bold, true);
    busymesgindicator = new QDialog();
    busymesgindicator->setWindowFlags(Qt::FramelessWindowHint);
    busymesgindicator->setFont(newFont);


    busymesgindicator->setAutoFillBackground(true);

    busymesgindicator->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(0, 85, 127);");
   // busymesgindicator->setGeometry(30, 30, 260, 170);
   // busymesgindicator->setGeometry(5, 80, 310, 60);
    busymesgindicator->setGeometry(5, 5, 310, 240);
    busymesgindicator->setWindowModality(Qt::NonModal);
    busymesgindicator->setMouseTracking(false);


    QLabel *msg = new QLabel(busymesgindicator);
    msg->setGeometry(30, 50, 248, 100);
     //msg->setGeometry(10,20, 250, 20);
    msg->setText(message);
    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    msg->setStyleSheet("border: 0");



    busymesgindicator->show();

    QCoreApplication::processEvents(QEventLoop::AllEvents, 1800);

    QCoreApplication::processEvents();

    QCoreApplication::processEvents();

}

///////////////////////////////////////////////////////////////////
//
// This below function will display message upto 3 seconds
//
//  Input : Displaying Message
//  Type  : Qstring
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////


void uidmessage::DisplayMessage(QString mesg)
{
    qDebug() << "Displaying"<<mesg<<"Message";

    QFont newFont("Crimson Text", 16, QFont::Bold, true);
    QDialog  *mesgindicator = new QDialog();
    mesgindicator->setWindowFlags(Qt::FramelessWindowHint);
    mesgindicator->setAutoFillBackground(true);
    mesgindicator->setStyleSheet("color: rgb(85, 0, 127); background-color: rgb(255, 255, 255);");
    mesgindicator->setGeometry(0, 0, 320, 240);
    mesgindicator->setFont(newFont);

    QLabel *msg = new QLabel(mesgindicator);
    msg->setGeometry(30, 50, 248, 100);
    msg->setText(mesg);

    msg->setWordWrap(true);
    msg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    msg->setStyleSheet("border:0");



    QTimer::singleShot(1000, mesgindicator, SLOT(close()));
    // mesgindicator->show();

   mesgindicator->exec();
}



///////////////////////////////////////////////////////////////////
//
// This below function will close the displaying busy message
//
//
//
// Date :                           Author:
///////////////////////////////////////////////////////////////////



void uidmessage::hide_Message()
{
    if(busymesgindicator)
    {
        if(busymesgindicator->isVisible())
            busymesgindicator->hide();
        busymesgindicator->close();

    }

}

