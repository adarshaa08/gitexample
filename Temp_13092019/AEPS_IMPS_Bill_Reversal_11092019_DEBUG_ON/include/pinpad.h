#ifndef PINPAD_H
#define PINPAD_H

#include <QString>
class pinpad
{
	public:
		pinpad();

		int initPinPad();
		int getEncryptedPinBlock(unsigned char[],QString);
        int getEncryptedPinBlock1(unsigned char[],QString, char *);
		int getEncryptedTrackData(unsigned char[]);
		// int sessionKeyInject(QString,QString);
        int sessionKeyInject(QString);
		int pinpadClose();


	private:

		int deviceOpen(char *port);
		char* Convert(QString str);
		int doHandShake(char *port);
		int Encrypt_Track_Data_KBPK(unsigned char *kbpk,unsigned char *track_data,int track_data_len,unsigned char *enc_track_data);

};

#endif // PINPAD_H
