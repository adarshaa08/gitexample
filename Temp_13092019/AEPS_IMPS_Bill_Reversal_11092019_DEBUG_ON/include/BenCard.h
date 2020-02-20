
#include "iso8583.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QVariant>

class clBen
{
public:
    int ReadBenCard(int);
    int close_card_reader();
    int ReadAccNo();
    int ReadCardHolderDynamicData();
    int Process_AccInfo(unsigned char *);
    int ReadBenPersonalDetails();
    int ReadBenCardInfo();
    int GetCardExpiryDate(char *);
    int txn_insert_into_customer_card(int,int);

    int BenCard(int, int);


    int update_customer_card(int );

    int GetRecNo();

    int WriteRecNo(int);
private:

};
