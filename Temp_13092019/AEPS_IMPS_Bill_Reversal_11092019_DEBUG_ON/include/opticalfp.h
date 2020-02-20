#ifndef OPTICALFP_H
#define OPTICALFP_H

#include <QDebug>
#include <stdio.h>
#include <mcheck.h>
#include <QTimer>
#include <QObject>
#include <QCoreApplication>

#include "popupmsgdlg.h"
#include "misc.h"
#include "auth_1.6.pb-c.h"

extern "C"
{
#include "gl11.h"
#include "bio_api.h"
}

class COpticalFP
{
public:
    COpticalFP();
    int on_OpticalEnroll_clicked();
    int captureFinger (void);
    int verifyfinger(void);
    int iCapacitive_scannerProcess(void);
    int iOptical_scannerProcess(void);
    int verify_localMatch_BFD(unsigned char *isoTemplate,int len);
    void Verify_FP_BFD(int selected_finger,unsigned char *isoTemplate1);

    int BFD_enrollment(int finger_number, int *nfiq, int *imageQuality);
};

#endif // OPTICALFP_H
