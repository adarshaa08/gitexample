#ifndef MWALLET_H
#define MWALLET_H

#include <QDebug>
#include <time.h>


#include "iso8583.h"
#include "isomessage.h"
#include "unifiedapplication.h"


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
QT_END_INCLUDE_NAMESPACE
class mWallet
{
public:
    mWallet(unifiedapplication *);
    unifiedapplication *wid_obj;
    int iCheckPrecondition();
    int mWalletTransaction(merged_transaction_structure *IsoMsg_structure_fun);
    int iParseResponse(merged_transaction_response_structure *ResponseISOStruct);
    int iPrintReciept(merged_transaction_structure *,merged_transaction_response_structure *);



};

#endif
