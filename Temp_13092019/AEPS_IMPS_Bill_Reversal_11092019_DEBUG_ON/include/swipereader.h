#ifndef SWIPEREADER_H
#define SWIPEREADER_H

#include <QString>
#include <QHash>

class swipereader
{
public:    
    swipereader();
    int trackDataReading();
    QHash<QString,QString> parsingTrackData() ;
    QString track1Str;
    QString track2Str;
    QString track3Str;

};

#endif // SWIPEREADER_H
