#include "FPB.h"
#include "ui_FPB.h"

int Flag_eAgreement_Acceptance = 0;

//void FPB::on_eAgreement_radioButton_clicked(bool checked)
//{
//    if(checked == true)
//    {
//        Flag_eAgreement_Acceptance = 1;
//        ui->eAgreement_radioButton->setCheckable(false);
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(true);
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(true);
//        ui->Eagreement_IAccept_Button_pg27->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)"); // pink and white
//    }
//    else
//    {
//        Flag_eAgreement_Acceptance = 0;
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(false);
//        ui->Eagreement_IAccept_Button_pg27->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(107, 107, 107)");
//    }
//    return;
//}

//void FPB::on_eAgreement_checkBox_clicked(bool checked)
//{
//    if(checked == true)
//    {
//        Flag_eAgreement_Acceptance = 1;
//        ui->eAgreement_checkBox->setCheckable(false);
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(true);
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(true);
//        ui->Eagreement_IAccept_Button_pg27->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 0, 127)"); // pink and white
//    }
//    else
//    {
//        Flag_eAgreement_Acceptance = 0;
//        ui->Eagreement_IAccept_Button_pg27->setEnabled(false);
//        ui->Eagreement_IAccept_Button_pg27->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(107, 107, 107)");
//    }
//    return;
//}


//void FPB::on_Eagreement_Back_Button_pg27_clicked()
//{
//    Flag_eAgreement_Acceptance=0;
//    ui->Eagreement_Merchant_Name_pg27->clear();
//    ui->stackedWidget->setCurrentIndex(0);
//    return;
//}

void FPB::on_Eagreement_IAccept_Button_pg27_clicked()
{

//    if(Flag_eAgreement_Acceptance == 1)
//    {
        //***********************************************  eAgreement Request Response Code write here  *********************************

        qDebug()<<"eAgreement accepted by merchant";
        ui->stackedWidget->setCurrentIndex(0);

        //********************************************************************************************************************************


        //        int i_ldf = login_download_func();
        //        if(i_ldf == FUNCTION_SUCCESS)
        //        {
        //            qDebug()<<"Chnage Passwd ldf success";
        //            ui->stackedWidget->setCurrentIndex(12);
        //            return;
        //        }
        //        else
        //        {
        //            qDebug()<<"Chnage Passwd ldf failed";
        //            ui->stackedWidget->setCurrentIndex(0);
        //            return;
        //        }
//    }
    return;
}
