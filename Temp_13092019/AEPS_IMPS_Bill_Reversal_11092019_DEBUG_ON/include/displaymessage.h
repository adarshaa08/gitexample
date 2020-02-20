#ifndef DISPLAYMESSAGE_H
#define DISPLAYMESSAGE_H
#include<QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QObject>


#include "finomisc.h"
class uidmessage:public QObject
{
     Q_OBJECT
private:

public:

    void DisplayMessage(QString);
    int displayconfirmation();
    void DisplayBusyMessage_GIF(QString mesg);
    void Display_BusyMessage(QString mesg);
    void Display_BusyMessage_static(QString);
    int  displayyesornoconfirmation(QString message);
    void hide_Message();


    QLineEdit *textbox;
    QDialog *messagehandler;

static uidmessage *instance();
static uidmessage *settingstance;
explicit uidmessage(QObject *parent=0);

};

#endif // DISPLAYMESSAGE_H
