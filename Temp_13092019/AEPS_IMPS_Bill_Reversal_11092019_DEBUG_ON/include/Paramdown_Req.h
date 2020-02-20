#ifndef PARAMDOWNLOADREQUEST_H
#define PARAMDOWNLOADREQUEST_H
#include<QObject>
#include<QIODevice>
#include<QtNetwork/QNetworkReply>
#include<QBuffer>
#include<QXmlStreamReader>

#include "unifiedapplication.h"
#include "print.h"


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
class CRijndael;
QT_BEGIN_INCLUDE_NAMESPACE


class paramdownrequest : public QObject
{
    Q_OBJECT
private:
    QIODevice * sendingData;
    QNetworkReply *responsedata;
    CRijndael *Rijndeal_encrypt_object;

    void startNetworkTimer();
    void vMobileParam();
    void vParseAgentMobileParam();

public slots:
   // void NetworkRequestFinished();
 void ParseParamResponse();  // aksnhay j changes
    void reponseError(QNetworkReply::NetworkError);



//signals:

public:



    paramdownrequest(unifiedapplication *);
    char gCparamreq_flag;
    unifiedapplication *wid_Obj;
    QVariantMap extractElementValues(QXmlStreamReader &xml, QVariantMap &chkBalResponse);

    QVariantMap parseXML(QString response);

    ~paramdownrequest();


    bool process();
    bool sendRequest();
};


#endif // PARAMDOWNLOADREQUEST_H
