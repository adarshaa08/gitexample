#ifndef USB_H
#define USB_H

#include <QDialog>
#include <QDebug>


extern "C"{
        #include "gl11.h"
      }


namespace Ui {
class Dialog;
}

class USB : public QDialog
{
    Q_OBJECT

public:
    explicit USB(QWidget *parent = 0);
    ~USB();

    int USBMount();
    void USBUnMount();
};



#endif // USB_H
