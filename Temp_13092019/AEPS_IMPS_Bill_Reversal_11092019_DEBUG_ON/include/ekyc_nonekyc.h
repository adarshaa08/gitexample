#ifndef EKYC_NONEKYC_H
#define EKYC_NONEKYC_H
#include <stdio.h>

#include "unifiedapplication.h"
#include "popupmsgdlg.h"
#include <QtGui/QWidget>
#include <QLabel>
#include <QLine>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QFile>
#include <QTimer>


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
QT_END_INCLUDE_NAMESPACE


typedef struct {
    QString QTitle;
    QString QFirstName;
    QString QMiddleName;
    QString QLastName;
    QString QDOB;
    QString QGender;
    QString QProfession;
    QString QReligion;
    QString QMaritalStatus;
    QString QMobileNumber;
    QString QAlternateNumber;
    QString QEmailID;
    QString QAddressLine1;
    QString QAddressLine2;
    QString QAddressLine3;
    QString QCity;
    QString QState;
    QString QPinCode;
    QString QBranchCode;
    QString QproductType;
    QString QAadharNumber;
    QString QPAN_Number;
    QString QVoterID;
    QString QPassport;
    QString QDriving_Licence;
    QString QOtherIDType;
    QString QOtherIDNumber;
    QString QIntroducerID;
    QString QSameASIDProof;
    QString QTelephoneBill;
    QString QBankStatement;
    QString QElectricityBill;
    QString QRationCard;
    QString QEmployeeLetter;
    QString QLetterFromPubAuth;
    QString QOtherAddressProof;
    QString QATM;
    QString QTeleBanking;
    QString QPOS;
    QString QInternetBanking;
    QString QMobileBanking;
    QString Qleg1RRN;
}kyc_nonkycFieldPreparation;

typedef struct {
    QString ResponseCode;
    QString TxnStatus;
    QString custID;
    QString AccountNumber;
    QString BankTid;
}Accparsing;

class EKYC_NONEKYC
{
public:
     EKYC_NONEKYC(unifiedapplication *);
    ~EKYC_NONEKYC();
    unifiedapplication *wid_Obj;



    int Stringtrim(QString);
    int iCapturePersonalDetailsFirstPage(QString,QString,QString,QString,int);
    int iCapturePersonalDetailsSecondPage(QDate,QString,QString,QString,QString,int);
    int iCaptureContactDetailPage(QString,QString,QString,int);
    int iCaptureAddressDetailPage1(QString,QString,QString,int);
    int iCaptureAddressDetailPage2(QString,QString,QString,int);
    int iCaptureIdentityProofPage1(QString,QString,QString,QString,int);
    int iCaptureIdentityProofPage2(QString,QString,int);
    int iCaptureAddressProofPage1(QString,QString,QString,QString,int);
    int iCaptureAddressProofPage2(QString,QString,QString,QString,int);
    int iOtherFacilityPage1(QString,QString,QString,QString,QString,int);
    int AgeCalculator(QDate);
    int AccountOpeningRequest(kyc_nonkycFieldPreparation *);
    int GetLocaldateTime(char *, char *, char *);
    int AccountOpeningXMLParsing();
    int Account_Opening_Print(Accparsing *ResponseStruct);
//    int EKYC_UID_VID(void); // ekyc vid

//public slots:
//    int UID_FUN(void); // ekyc vid
//    int VID_FUN(void); // ekyc vid

};


#endif // EKYC_NONEKYC_H
