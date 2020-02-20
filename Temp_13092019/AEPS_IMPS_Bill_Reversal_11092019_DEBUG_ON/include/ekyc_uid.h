#ifndef EKYC_UID_H
#define EKYC_UID_H 
	
#include <locale.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "ipd_base.h"
#include "error_codes.h"

#ifdef DEBUG
#ifndef PRINT
#define PRINT(...) fprintf(stderr,__VA_ARGS__);
#endif
#else
#define PRINT(...) 
#endif


#define MICROATM_CHANNEL        (unsigned char) 0xc4

#define UIDLEN 12
#define PATH_LEN 256
#define FILE_URL_FAILED 1
#define IP_STRLEN  16
#define MAX_FP_COUNT	10
#define RESP_300		300
#define CAN_EKYC_RESP_MAX_SIZE 4096
#define EKYC_RESP_MAX_SIZE 30000

//#define CURL_TIME_OUT_ERR	-2
//#define CURL_CONNECT_ERR	-3
#define IC_ERR_CONNECT_TIME_OUT -2
#define IC_ERR_COMM_FAILURE  -3
#define IC_ERR_RECEIVE_TIME_OUT -4
#define IC_ERR_RECEIVE_DATA_FAILURE -5
#define IC_ERR_INVALID_DATA -6
#define CONNECTION_TIMEDOUT      -7
#define CONNECTION_FAILED        -8

#if defined (IMTS)

#define CONNECTION_TYPE_FILE "/mnt/userfs/conf/iEnrollment/conn_type"
#define EKYC_IP_PORT_FILE 	"/mnt/userfs/conf/iEnrollment/ekyc_ip_port"
#define AUTH_TYPE_FILE 		"/mnt/userfs/conf/iEnrollment/ekyc_auth_type"
#define CUST_IMAGE_PATH		 "/mnt/tmp/cust.jpeg"
#define ASERVER_CRT			"/mnt/userfs/conf/client_crt/CAfile.pem" 
#define ACLIENT_CRT			"/mnt/userfs/conf/client_crt/client.crt"
#define ACLIENT_PEM			"/mnt/userfs/conf/client_crt/client.pem"

#elif defined (IMGS) 

#define CONNECTION_TYPE_FILE "/mnt/userfs/conf/conn_type"
#define EKYC_IP_PORT_FILE	 "/mnt/userfs/conf/ekyc_ip_port"
#define AUTH_TYPE_FILE		 "/mnt/userfs/conf/ekyc_auth_type"
#define CUST_IMAGE_PATH 	"/mnt/tmp/cust.jpeg"
	#define ASERVER_CRT			"/mnt/userfs/conf/client_crt/CAfile.pem"
#define ACLIENT_CRT			"/mnt/userfs/conf/client_crt/client.crt"
#define ACLIENT_PEM			"/mnt/userfs/conf/client_crt/client.pem"

#else
#define ASERVER_CRT			"/etc/client_crt/CAfile.pem"
#define ACLIENT_CRT			"/etc/client_crt/client.crt"
#define ACLIENT_PEM			"/etc/client_crt/client.pem"

#define CONNECTION_TYPE_FILE "/etc/conn_type"
#define EKYC_IP_PORT_FILE "/etc/ekyc_ip_port"
#define AUTH_TYPE_FILE "/etc/ekyc_auth_type"
#define CUST_IMAGE_PATH "/etc/cust.jpeg"

#endif

typedef struct Time_Progress_ekyc
{
	time_t start_tm;
	time_t progs_tm;
	IC_INT32 TmOut_steps;
	IC_INT32 step_size;
}ST_TIME_PROGS_EKYC;


typedef enum e_auth_type
{
	E_ONLY_BIO,
	E_ONLY_OTP,
	E_BIO_OTP,
	E_NONE

}E_AUTH_TYPE;

typedef enum e_channel_type
{
	E_IPOS,
	E_IASK,
	E_GDEP

}E_CHANNEL_TYPE;

typedef enum e_service_name 
{
	E_AUTH,
	E_BFD,
	E_KYC

}E_SERVICE_NAME;



typedef struct {
	int verbose_mode;
	int verify_depth;
	int always_continue;
	char host[256] ;
} st_verify_data;



typedef struct FpData
{
	IC_UINT8   *pc_fp;
	IC_UINT32  fp_size;
	int fp_type;
}ST_FP_DATA;

typedef struct StCustDet
{
	IC_INT8 auc_cust_uid[UIDLEN + 1];
	IC_INT8 ac_otp[6 + 1];
	ST_FP_DATA st_cust_fp[2];
	int FPCount;
	IC_INT8 terminalID[12 + 1];
	IC_INT8 bcID[10 + 1];
	IC_INT8 bccID[18 + 1];
	IC_INT8 bankID[2 + 1];
	IC_INT8 app_version[30 + 1];
	IC_INT8 serial_Num[6 + 1];
	IC_INT8 ac_key[1024 + 1];
	IC_INT8 certID[8 + 1];
	IC_INT8 Hmac[2048 + 1];
	IC_INT8 encrypt_pid[2048 + 1];
	IC_INT8 pid_TS[20 + 1];
	IC_INT8 auc_acc_num[24 + 1];
	int authType;
	int authChannel;
	int service_name;
	int aadhaar_seeding;
    E_AUTH_TYPE eauth_type;

	/* Request frame - added by Kalidass */
	IC_INT8 mcc[10 + 1];
	IC_INT8 posEntryMode[10 + 1];
	IC_INT8 posCode[10 + 1];
	IC_INT8 caTid[25 + 1];
	IC_INT8 caId[25 + 1];
	IC_INT8 caTa[255 + 1];
	IC_INT8 bio[25 + 1];
	IC_INT8 bt[25 + 1];
	IC_INT8 udc[25 + 1];
	IC_INT8 pip[25 + 1];
	IC_INT8 fdc[25 + 1];
	IC_INT8 idc[25 + 1];
	IC_INT8 lot[25 + 1];
	IC_INT8 lov[10 + 1];
	IC_INT8 type[10 + 1];

	/* path , username and password */    //Nandini P
	IC_INT32 conctntimeout ;
    IC_INT32 recvtimeout ;
	IC_INT8 seedpath[100 + 1] ;
    IC_INT8 usernamepwd[250 + 1] ;	

}ST_CUST_DET;

typedef struct st_kyc_resp
{
	char ipos_ret[2 + 1];
	char ipos_err_msg[40 + 1];
	char ret[2 + 1];
	char respdesc[60 + 1];
	char respCode[5 + 1];
	char Msg[40 + 1];
	char name[50 + 1];
	char dob[20+1];
	char gender[2 + 1];
	char phone[10 + 1];
	char email[60 + 1];
	char co[60 + 1];
	char street[60 + 1];
	char house[60 + 1];
	char lm[60 + 1];
	char lc[60 + 1];
	char subdist[60 + 1];
	char dist[60 + 1];
	char vtc[60 + 1];
	char state[60 + 1];
	char pc[60 + 1];
	char po[60 + 1];
	char iaskRefId[60 + 1];
	char uidaiAuthCode[60 + 1];
	char resp_buf[2048 + 1];
}ST_KYC_RESP;

IC_RETCODE do_ekyc_auth (ST_CUST_DET *pst_cust_det,ST_KYC_RESP *pst_ekyc_resp);
IC_RETCODE do_ekyc_auth_Canara(ST_CUST_DET *pst_cust_det, ST_KYC_RESP *pst_ekyc_resp);
IC_RETCODE parse_kyc_resp(ST_KYC_RESP *pst_kyc_resp, char *resp, int ServiceName);
IC_RETCODE ReadFromFile(char *infilepath, char *outParam);
IC_RETCODE Get_PrimaryIp_And_Port(char *puc_prim_ip,unsigned int *ui_port);  
IC_RETCODE Talk2Server(char *ServerIp,int ServerPort,char *InBuff,int InSize,char *OutBuff,int *OutSize);
IC_RETCODE Init_EKYC_Client();
IC_RETCODE Deinit_EKYC_Client();
IC_RETCODE get_skey_ci_hmac_encryptPID(ST_CUST_DET *pst_cust_det);
 int sendEKYCData(char *idepURL, char *buff, char *resp);
IC_RETCODE get_biometric_protobuff_pid(void **buff, int *len, ST_CUST_DET *pst_cust_det);
IC_RETCODE set_ip_port(IC_INT8 *puid_ip, IC_UINT32 uid_port);
IC_RETCODE get_encrypt_encode_data_key_fusn(IC_INT8 *pid_data, IC_INT32 pid_len, IC_INT8 *pc_data, IC_INT8 *pc_key, IC_INT8 *ci, char *hmac);
IC_RETCODE GetTermSerialNum(char *gaucTermSerialNo_UID);
IC_RETCODE GeneratetxnID(IC_UINT8 *puc_reqid,IC_INT8 *pc_serial);
IC_RETCODE GetLicenseKeyFusn(char *licKey);
IC_RETCODE GetLicenseKeyforURL(char *licKey);
IC_RETCODE Read_ac_code(IC_INT8 *pac_code);
static IC_RETCODE  get_xml_time(time_t t, IC_INT8 *ac_time);
int uid_random_bytes(unsigned char *rand, int len); 
int uid_rsa_encrypt(unsigned char *in, int inlen, unsigned char *out, int *outlen, char *ci);
int uid_aes_encrypt(unsigned char *in, int inlen, unsigned char *out, 
						int *outlen, unsigned char *key, unsigned char *iv);
int uid_aes_decrypt(unsigned char *in, int inlen, unsigned char *out, 
			int *outlen, unsigned char *key, unsigned char *iv);
int uid_encode_b64(unsigned char *in, int inlen, unsigned char *out, 
			int *outlen);
int uid_decode_b64(unsigned char *in, int inlen, unsigned char *out, 
		int *outlen);
unsigned char *uid_get_skey_data (unsigned char *key);
unsigned char *uid_get_aes_encrypted_data (unsigned char *in, int inlen, 
		 unsigned char *key);
int InitAES(unsigned char *SKey);
int getSha2(char *xml,int len, unsigned char *outbuff);
int get_protobuff_otp(void **buff, int *len, char *param_otp);
IC_RETCODE get_biometric_otp_protobuff_pid(void **buff, int *len, ST_CUST_DET *pst_cust_det);

#endif
