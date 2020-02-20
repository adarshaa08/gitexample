#ifndef CLSCASHINHAND_H
#define CLSCASHINHAND_H

#include <QMessageBox>
#include <QDebug>
#include "unifiedapplication.h"


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
QT_END_INCLUDE_NAMESPACE

class clsCashInHand
{
public:
    clsCashInHand(unifiedapplication *);

    unifiedapplication *wid_Obj;
    int prepare_Request_CIH();
};

#endif // CLSCASHINHAND_H
