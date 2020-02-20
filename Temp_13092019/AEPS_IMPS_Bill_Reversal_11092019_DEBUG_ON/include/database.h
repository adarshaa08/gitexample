#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>

class Database
{
public:
    bool openDB();
    //bool openDB1();
    bool createTable();
    bool closeDB();
  //  bool closeDB1();

        QSqlDatabase db,db1;
private:
    //QSqlDatabase db,db1;
};


#endif // DATABASE_H
