#include "FPB.h"

struct Date1
{
    int d, m, y;
};

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int FPB::countLeapYears(QDate d)
{
    int years = d.year();

    if (d.month() <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}

int FPB::getDifference(QDate dt1, QDate dt2)
{
    long int n1 = dt1.year()*365 + dt1.day();

    for (int i=0; i<dt1.month() - 1; i++)
        n1 += monthDays[i];

    n1 += countLeapYears(dt1);

    long int n2 = dt2.year()*365 + dt2.day();
    for (int i=0; i<dt2.month() - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);


    return (n2 - n1);
}

int FPB::findAge(QDate dt1, QDate dt2)
{
    int age1 = 0, age2 = 0;

    if(dt2.month() >12 || dt2.month()<1)
        return 1;

    if(dt2.month() > dt1.month()){
         age1=dt1.year()-dt2.year()-1;
         age2=(12-dt2.month()) + dt1.month();
    }else{
         age1=dt1.year()-dt2.year();
         age2=12-dt2.month();
    }

    int totalAge = -((age1*12 + age2)/12);

#if DEBUG
    printf("%d year and %d month ....Total %d\n",age1, age2, totalAge);
#endif
    return totalAge;

}

// Driver program
//int main()
//{
//    Date1 dt1 = {1, 2, 2000};
//    Date1 dt2 = {1, 2, 2004};

//    cout << "Difference between two Date1s is " << getDifference(dt1, dt2);

//    return 0;
//}
