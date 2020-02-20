#ifndef BFD_H
#define BFD_H


#include <QDebug>
#include <time.h>

#include "unifiedapplication.h"
#include "iso8583.h"

#define LEFT_LITTLE   "LL"
#define LEFT_RING     "LR"
#define LEFT_MIDDLE   "LM"
#define LEFT_INDEX    "LI"
#define LEFT_THUMB    "LT"

#define RIGHT_THUMB   "RT"
#define RIGHT_INDEX   "RI"
#define RIGHT_MIDDLE  "RM"
#define RIGHT_RING    "RR"
#define RIGHT_LITTLE  "RL"


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
QT_END_INCLUDE_NAMESPACE




class BFD
{
public:
    BFD(unifiedapplication *);
    ~BFD();
    unifiedapplication *wid_Obj;

    int makeBFDpacket(int number_of_finger);
    int BFD_Block(unsigned char *p,int mxl,int rml, char *temp);

};

#endif // PRINT_H
