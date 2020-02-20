/*
    Copyright (C) 2011 Geodesic Limited, Mumbai, India

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
    OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    Except as contained in this notice, the name of the Geodesic Limited shall not be used
    in advertising or otherwise to promote the sale, use or other dealings in this Software
    without prior written authorization from the Geodesic Limited.

*/

#include "uid_auth.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef XML_SECURITY
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#endif

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>
#endif

#define TRUE 1
#define FALSE 0
#define byte unsigned char
unsigned char *uid_get_skey_data (unsigned char *key);
unsigned char *uid_get_aes_encrypted_data (unsigned char *in, int inlen, 
		 unsigned char *key);

unsigned char *uid_get_aes_encrypted_data_fino (unsigned char *in, int inlen,
         unsigned char *key,int *outlen,int TranType);

unsigned char *uid_get_aes_encrypted_data_fino_FI (unsigned char *in, int inlen,
         unsigned char *key,int *outlen);

unsigned char *uid_get_skey_data_fino(unsigned char *key,int *outlen,int TranType );
unsigned char *uid_get_skey_data_fino_FI(unsigned char *key,int *outlen);
int getPublickey_Exponent(unsigned char* publickey,unsigned char* Exponent,int *pubLen,int *expLen);
extern unsigned char *enc_ses_key_pack;




/************* Added By Praveen **********/
char g_Exponent[16],g_pubkey[1024];
/*****************************************/


static const char alphanum[] =
"0123456789"
"1A2m5Y0E"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";


/*
void convert_byte_string(char *pos,int len,unsigned char *byteString)
{
        int count = 0;

        for(count = 0; count < len/sizeof(byteString[0]); count++) {
                sscanf(pos, "%2hhx", &byteString[count]);
                pos += 2 * sizeof(char);
        }
}
*/
/** 
 * sign_file:
 * @xmlInpData:		the XML data.
 *
 * Signs the @xml_file using private key from @key_file and dynamicaly
 * created enveloped signature template. The certificate from @cert_file
 * is placed in the <dsig:X509Data/> node.
 *
 * Returns 0 on success or a negative value if an error occurs.
 */

#ifdef XML_SECURITY
xmlChar * sign_file(char * xmlInpData)
{
	xmlChar *xmlBuff=NULL;
	xmlDocPtr doc=NULL;
	xmlNodePtr signNode=NULL, refNode=NULL, keyInfoNode=NULL;
	xmlSecDSigCtxPtr dsigCtx=NULL;
	int res=-1;
	X509 *x;
	EVP_PKEY *pkey;
	PKCS12 *p12;
	STACK_OF(X509) *ca=NULL;
	FILE *fp;

//	Storing Auth xml into Temp File so as to be read by xmlParseFile()
	fp = fopen(STORE_TEMP_AUTH_XML, "w");
	fwrite(xmlInpData, 1, strlen(xmlInpData), fp);
	fclose(fp);

	x = X509_new();

	fp = fopen(AUA_PRIVATE_CERTIFICATE, "rb");
	p12 = d2i_PKCS12_fp(fp, NULL);
	fclose(fp);

	if (!PKCS12_parse(p12, KEYPASS, &pkey, &x, &ca)) {
		printf(" Error while parsing\n");
	}
	PKCS12_free(p12);
	
// X509 Certificate
	fp = fopen(TEMP_STORE_CERTIFICATE,"w");
	PEM_write_X509(fp, x);
	fclose(fp);

// RSA Private Certificate
	fp = fopen(TEMP_STORE_KEY_CERTIFICATE,"w");
	PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
	fclose(fp);

/* Load XML doc file */
	doc = xmlParseFile(STORE_TEMP_AUTH_XML);
	if ((doc == NULL) || (xmlDocGetRootElement(doc) == NULL)) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n",
			STORE_TEMP_AUTH_XML);
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}
    
/* create signature template for RSA-SHA1 enveloped signature */
	signNode = xmlSecTmplSignatureCreate(doc, xmlSecTransformInclC14NId,
			xmlSecTransformRsaSha1Id, NULL);
	if (signNode == NULL) {
		fprintf(stderr, "Error: failed to create signature template\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}

/* add <dsig:Signature/> node to the doc */
	xmlAddChild(xmlDocGetRootElement(doc), signNode);
	refNode = xmlSecTmplSignatureAddReference(signNode,
			xmlSecTransformSha1Id, NULL, "", NULL);
	if (refNode == NULL) {
		fprintf(stderr, "Error: failed to add reference to signature template\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}

/* add enveloped transform */
	if (xmlSecTmplReferenceAddTransform(refNode,
		xmlSecTransformEnvelopedId) == NULL) {
		fprintf(stderr, "Error: failed to add enveloped transform to reference\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}
    
/* add <dsig:KeyInfo/> and <dsig:X509Data/> */
	keyInfoNode = xmlSecTmplSignatureEnsureKeyInfo(signNode, NULL);
	if (keyInfoNode == NULL) {
		fprintf(stderr, "Error: failed to add key info\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}
	xmlNodePtr x509Node=xmlSecTmplKeyInfoAddX509Data(keyInfoNode);

	xmlSecTmplX509DataAddSubjectName(x509Node);
	xmlSecTmplX509DataAddCertificate(x509Node);

/* create signature context, we don't need keys manager in this example */
	dsigCtx = xmlSecDSigCtxCreate(NULL);
	if (dsigCtx == NULL) {
		fprintf(stderr,"Error: failed to create signature context\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		return(NULL);
	}
	
/* load private key, assuming that there is not password */
	dsigCtx->signKey = xmlSecCryptoAppKeyLoad(TEMP_STORE_KEY_CERTIFICATE,
		xmlSecKeyDataFormatPem, KEYPASS, NULL, NULL);
	if (dsigCtx->signKey == NULL) {
		fprintf(stderr,"Error: failed to load private pem key from \"%s\"\n", TEMP_STORE_KEY_CERTIFICATE);
		if (doc != NULL)
			xmlFreeDoc(doc); 
		if (dsigCtx != NULL)
			xmlSecDSigCtxDestroy(dsigCtx);
		return(NULL);
	}
    
/* load certificate and add to the key */
	if (xmlSecCryptoAppKeyCertLoad(dsigCtx->signKey, TEMP_STORE_CERTIFICATE,
		xmlSecKeyDataFormatPem) < 0) {
		fprintf(stderr,"Error: failed to load pem certificate \"%s\"\n", TEMP_STORE_CERTIFICATE);
		if (doc != NULL)
			xmlFreeDoc(doc); 
		if (dsigCtx != NULL)
			xmlSecDSigCtxDestroy(dsigCtx);
		return(NULL);
	}
/* sign the template */
	if (xmlSecDSigCtxSign(dsigCtx, signNode) < 0) {
		fprintf(stderr,"Error: signature failed\n");
		if (doc != NULL)
			xmlFreeDoc(doc); 
		if (dsigCtx != NULL)
			xmlSecDSigCtxDestroy(dsigCtx);
		return(NULL);
	}
	unlink(TEMP_STORE_KEY_CERTIFICATE);
	unlink(TEMP_STORE_CERTIFICATE);
	unlink(STORE_TEMP_AUTH_XML);
	{
		int bufferSize=0;

		xmlDocDumpFormatMemory(doc, &xmlBuff, &bufferSize, 1);
	}
#ifdef DEBUG
/* print signed document to stdout */
	xmlDocDump(stdout, doc);
#endif
	if (dsigCtx != NULL)
		xmlSecDSigCtxDestroy(dsigCtx);
	if (doc != NULL)
		xmlFreeDoc(doc); 
	return xmlBuff;
}
#endif

/*  Function: do_digital_signature 
	Parameters:
	1. Auth-Xml data
	2. Private Key File - PEM format
	3. Private Key password
	4. Certificate File - PEM format
*/
#ifdef XML_SECURITY
xmlChar * do_digital_signature(char *xmlInpData, xmlChar **out)
{
	xmlInitParser();
	LIBXML_TEST_VERSION
	xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
	xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
	xmlIndentTreeOutput = 1; 
#endif
        	
/* Init xmlsec library */
	if (xmlSecInit() < 0) {
		fprintf(stderr, "Error: xmlsec initialization failed.\n");
		return NULL;
	}

/* Check loaded library version */
	if (xmlSecCheckVersion() != 1) {
		fprintf(stderr, "Error: loaded xmlsec library version is not compatible.\n");
		return NULL;
	}

/* Load default crypto engine if we are supporting dynamic
 * loading for xmlsec-crypto libraries. Use the crypto library
 * name ("openssl", "nss", etc.) to load corresponding 
 * xmlsec-crypto library.
*/
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
	if (xmlSecCryptoDLLoadLibrary(BAD_CAST XMLSEC_CRYPTO) < 0) {
		fprintf(stderr, "Error: unable to load default xmlsec-crypto library. Make sure\n"
		"that you have it installed and check shared libraries path\n"
		"(LD_LIBRARY_PATH) envornment variable.\n");
		return NULL;	
	}
#endif

/* Init crypto library */
	if (xmlSecCryptoAppInit(NULL) < 0) {
		fprintf(stderr, "Error: crypto initialization failed.\n");
		return NULL;
	}

/* Init xmlsec-crypto library */
	if (xmlSecCryptoInit() < 0) {
		fprintf(stderr, "Error: xmlsec-crypto initialization failed.\n");
		return NULL;
	}
	*out = sign_file(xmlInpData);
   
#ifdef DEBUG
	printf("After Sign_file :%s\n\n",*out);
#endif	

	xmlSecCryptoShutdown();
	xmlSecCryptoAppShutdown();
	xmlSecShutdown();

#ifndef XMLSEC_NO_XSLT
	xsltCleanupGlobals();            
#endif
	xmlCleanupParser();
	return *out;
}
#endif

/*******************************************************
	 Retrieve Expiry Date from Certificate
*********************************************** *******/
char * get_expiry_date( char *expiryStr )
{
	X509 *x;
	unsigned char *not;
	int n=0;
	BIO *out;
	FILE *fp=fopen(UIDAI_PUBLIC_CERTIFICATE, "r");

	x = X509_new();
	x = PEM_read_X509(fp,NULL,NULL,NULL);
	fclose(fp);

	out = BIO_new(BIO_s_mem());
	ASN1_TIME_print(out, X509_get_notAfter(x));
	n = BIO_get_mem_data(out, &not);
	expiryStr = (char *) malloc (n+1);
	expiryStr[n] = '\0';
	memcpy(expiryStr, not, n);
	BIO_free(out);
	
	X509_free(x);
	return(expiryStr);
}

#ifdef DEBUG
int print_data(unsigned char *data, int datalen)
{
	int i;

	for (i=0; i < datalen; i++) {
        printf("%02X  ", data[i]);
		if ((i+1)%16 == 0)
			printf("\n");
	}
	printf("\n");
	return 0;
}
#endif

/*****************************************************
	Generates Sha256 hash
*****************************************************/

int hMacSha256(char *xml,int len, unsigned char *outbuff)
{
	SHA256_CTX ctx;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx,xml, len);
	SHA256_Final(outbuff,&ctx);
	return 0;
}

/*****************************************************
	Assign RSA Key
*****************************************************/
int assign_key_rsa(RSA *rsa, unsigned char *key, int n_len, unsigned char *e_key, int e_len)
{
        rsa->n = BN_bin2bn(key, n_len, rsa->n);
        rsa->e = BN_bin2bn(e_key, e_len, rsa->e);
    	return 0;
}
/*****************************************************
	Generates Random Number
*****************************************************/

unsigned char * uid_random_bytes(unsigned char *rand, int len) 
{ 
	RAND_bytes(rand, len);
	return rand; 
}

/*****************************************************
	RSA Encryption
*****************************************************/
/*****************************************************
    AES Encryption
*****************************************************/
int uid_aes_encrypt(unsigned char *in, int inlen, unsigned char *out,
        int *outlen, unsigned char *key, unsigned char *iv)
{
    int tmplen;
    // Straight encrypt

    EVP_CIPHER_CTX x;
    EVP_CIPHER_CTX_init(&x);
  #if DEBUG
    printf ("AES encrypted data %d len\n", *outlen);
#endif
    EVP_CIPHER_CTX_set_padding(&x,1); // 1- padding, 0 - No Padding

    if (!EVP_EncryptInit_ex(&x, EVP_aes_128_cbc(), NULL, key, iv)) {
        printf("\n ERROR!! \n");
        return -1;
    }
    if (!EVP_EncryptUpdate(&x, out, outlen,
            (const unsigned char*) in, inlen)) {
        printf("\n ERROR!! \n");
        return -2;
    }
    if (!EVP_EncryptFinal_ex(&x,out + *outlen,&tmplen)) {
        printf("\n ERROR!! \n");
        return -3;
    }
    *outlen += tmplen;
#ifdef DEBUG
    printf ("AES encrypted data %d len\n", *outlen);
    print_data (out, *outlen);
#endif
    EVP_CIPHER_CTX_cleanup(&x);
    return 0;
}
/*****************************************************
    Base64 Encoding
*****************************************************/

int uid_encode_b64(unsigned char *in, int inlen, unsigned char *out,
            int *outlen)
{
    BIO *bmem=NULL, *b64=NULL;
    BUF_MEM *bptr=NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, in, inlen);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    memcpy(out, bptr->data, bptr->length);
    out[bptr->length] = 0;
    *outlen = bptr->length;
    BIO_free_all(b64);

    return 0;
}


int uid_rsa_encrypt_with_fino_pubkey(unsigned char *in, int inlen, unsigned char *outbuf, int *outlen)
{
    unsigned char key[400]={0};
    unsigned char *pubKey;
    int i=0;
    unsigned char fino_pubkey_hex[400];
    unsigned char exponent_hex[20];

    int len=0;
    int pub_key_len=256,pub_exponent=3;

    RSA *rsa = RSA_new();
    printf("public key & Exponent in HEX_-----------------------------------------------------------");
    getPublickey_Exponent(fino_pubkey_hex,exponent_hex,&pub_key_len,&pub_exponent);
    printf("\nThe length of finger print and exponent is : %d %d\n",pub_key_len,pub_exponent);


    fprintf(stdout, "FINO PUB KEY in HEX:\n");
    for(i=0; i<pub_key_len; i++)
    {
        fprintf(stdout, "%02X ", fino_pubkey_hex[i]);
    }
    fprintf(stdout, "FINO EXPONENT IN HEX:\n");
    for(i=0; i<pub_exponent; i++)
    {
        fprintf(stdout, "%02X ", exponent_hex[i]);
    }
    rsa->d=BN_bin2bn(exponent_hex,3 ,rsa->d);

    rsa->e=BN_bin2bn(exponent_hex,3,rsa->e);

    rsa->n=BN_bin2bn(fino_pubkey_hex,256,rsa->n);


    ERR_load_crypto_strings();


    /*int bitSize = 255; // Bit Size
    //BIGNUM *publickey = epkey->pkey.rsa->n; // modulus

    BIGNUM *publickey = rsa->n;
    BIGNUM *exp = rsa->e; // exponential key
    pubKey = (unsigned char *)malloc(sizeof(unsigned char) * bitSize);
    unsigned char *eKey = (unsigned char *)malloc(sizeof(unsigned char)*100);
    //printf("\nGet Key Over\n");

    int n_len = BN_bn2bin(publickey,pubKey); // convert it

    //printf("\nIn middle\n");
    BN_bn2bin(exp,eKey); // convert it

    memcpy(key,pubKey,n_len);
   /* assign_key_rsa(rsa, key,n_len,eKey,e_len);
    if (!EVP_PKEY_assign_RSA(epkey,rsa)) {
        printf("key assign error\n");
        return -1;
        }
        */
    printf("\nEncryt inside RSA --------------------------------->\n");
    len = RSA_public_encrypt(inlen, in, outbuf,rsa,RSA_PKCS1_PADDING); //RSA_NO_PADDING
    enc_ses_key_pack=(char*)malloc(len+5);
    //memset(enc_ses_key_pack,0x00,sizeof(enc_ses_key_pack));
   for(i=0;i<len;i++)
   {	printf("%02X ",outbuf[i]);
    //enc_ses_key_pack[i]=outbuf[i];
   }
/*    printf("\nEncryt inside RSA ------------enc_ses_key_pack------->\n");
   for(i=0;i<len;i++)
   {	printf("%02X ",enc_ses_key_pack[i]);
   }*/
    //printf("\nEncryot Over\n");
    *outlen = len;
    free(pubKey);

    return 0;
}

int uid_rsa_encrypt_with_fino_pubkeyFI(unsigned char *in, int inlen, unsigned char *outbuf, int *outlen)
{
    unsigned char key[400]={0};
    unsigned char *pubKey;
    int i=0;
    unsigned char fino_pubkey_hex[400];
    unsigned char exponent_hex[20];

    int len=0;
    int pub_key_len=256,pub_exponent=3;

    RSA *rsa = RSA_new();
    printf("public key & Exponent in HEX_-----------------------------------------------------------");
    getPublickey_Exponent(fino_pubkey_hex,exponent_hex,&pub_key_len,&pub_exponent);
    printf("\nThe length of finger print and exponent is : %d %d\n",pub_key_len,pub_exponent);


    fprintf(stdout, "FINO PUB KEY in HEX:\n");
    for(i=0; i<pub_key_len; i++)
    {
        fprintf(stdout, "%02X ", fino_pubkey_hex[i]);
    }
    fprintf(stdout, "FINO EXPONENT IN HEX:\n");
    for(i=0; i<pub_exponent; i++)
    {
        fprintf(stdout, "%02X ", exponent_hex[i]);
    }
    rsa->d=BN_bin2bn(exponent_hex,3 ,rsa->d);

    rsa->e=BN_bin2bn(exponent_hex,3,rsa->e);

    rsa->n=BN_bin2bn(fino_pubkey_hex,256,rsa->n);


    ERR_load_crypto_strings();

    printf("\nEncryt inside RSA --------------------------------->\n");
    len = RSA_public_encrypt(inlen, in, outbuf,rsa,RSA_PKCS1_PADDING); //RSA_NO_PADDING

   for(i=0;i<len;i++)
   {
       printf("%02X ",outbuf[i]);
   }

    *outlen = len;
    free(pubKey);

    return 0;
}



/*****************************************************
	Generates Session key
*****************************************************/
//unsigned char *uid_get_skey_data(unsigned char *key)
//{
//	unsigned char OutRSA[512], *outData=NULL;
//	int keyLen=0, OutRSALen=0, OutDataLen=0;

//	memset(key, 0, 32);
//	key = uid_random_bytes(key, RSA_KEY_LEN);
//	keyLen = RSA_KEY_LEN;
//	if (uid_rsa_encrypt_with_fino_pubkey(key, keyLen, OutRSA, &OutRSALen) != 0) {
//		printf ("RSA encrypt failed\n");
//		return NULL;
//	}

//	outData = (unsigned char *)malloc(sizeof(unsigned char)*512);

//	uid_encode_b64(OutRSA, OutRSALen, outData, &OutDataLen);
//	if (OutRSALen == 0) {
//		printf ("uid_get_skey_data: base64 encode failed\n");
//		return NULL;
//	}
//	outData[OutDataLen] = 0;
//	return(outData);
//}

unsigned char *uid_get_skey_data_fino(unsigned char *key,int *outlen,int TranType)
{

    //FI 0
    //AEPS 2
    unsigned char OutRSA[512]={0x00};
    unsigned char* outData=NULL;
    int OutRSALen=0;
    int keyLen=0,i=0;

    memset(key, 0, 32);

        printf("\n In AEPS uid_get_skey_data_fino");
        key = uid_random_bytes(key, RSA_KEY_LEN);
        printf("The value Session key is : \n");
        for(i=0;i<RSA_KEY_LEN;i++)
                printf("%02X ",key[i]);

        keyLen = RSA_KEY_LEN;
        if (uid_rsa_encrypt_with_fino_pubkey(key, keyLen, OutRSA, outlen) != 0)
        {
            printf ("RSA encrypt failed\n");
            return NULL;
        }
        printf("The length of outrsa len IN AEPS is : %d\n\n",*outlen);
        printf("The encrypted session key is : \n");
        for(i=0;i<*outlen;i++)
            printf("%02X ",OutRSA[i]);
      return OutRSA;
}

//unsigned char *uid_get_skey_data_fino_FI(unsigned char *key,int *outlen)
//{
//    unsigned char OutRSAFI[512]={0x00},*outData=NULL;
//    int keyLen=0,i=0,OutRSALen=0;

//    memset(key, 0, 32);
//    key = uid_random_bytes(key, RSA_KEY_LEN);
//    printf("The value Session key is : \n");
//    for(i=0;i<RSA_KEY_LEN;i++)
//            printf("%02X ",key[i]);

//    keyLen = RSA_KEY_LEN;
//    if (uid_rsa_encrypt_with_fino_pubkeyFI(key, keyLen, OutRSAFI, &OutRSALen) != 0) {
//        printf ("RSA encrypt failed\n");
//        return NULL;
//    }
//    //printf("The length of outrsa len is : %d\n\n",*outlen);
//    outData = (unsigned char *)malloc(sizeof(unsigned char)*512);

//    uid_encode_b64(OutRSAFI, OutRSALen, outData,outlen);
//    if (OutRSALen == 0) {
//        printf ("uid_get_skey_data: base64 encode failed\n");
//        return NULL;
//    }
//    printf("The length of outrsa len is : %d\n\n",*outlen);
//    outData[*outlen] = 0;
//    return(outData);

//   // printf("The encrypted session key is : \n");
///*    for(i=0;i<*outlen;i++)
//        printf("%02X ",OutRSA[i]);*/
//  return OutRSAFI;
//}
///*****************************************************
//	Aes Encryption with Base64
//*****************************************************/

unsigned char *uid_get_aes_encrypted_data (unsigned char *in, int inlen, unsigned char *key)
{
    unsigned char iv[32]={0}, *temp;
    int templen;

    temp = (char *)malloc(inlen+32);
    uid_aes_encrypt (in, inlen, temp, &templen, key, iv);



    if (templen == 0){
        printf ("aes encrypt failed\n");
        return NULL;
    }

    /*
    out = (char *)malloc(templen*4);
    uid_encode_b64(temp, templen, out, &outlen);
    if (outlen == 0){
        printf ("aes b64 encode failed\n");
        return NULL;
    }
    out[outlen] = 0;
    return out;*/
    return temp;
}


//unsigned char *uid_get_aes_encrypted_data_fino (unsigned char *in, int inlen,
//         unsigned char *key,int *outlen,int TranType)
//{
//	unsigned char iv[32]={0}, *temp, *out=NULL;
//	*outlen=0;
//    int templen,i=0;


//        printf("\n In AEPS uid_get_aes_encrypted_data_fino");
//        temp = (char *)malloc(inlen+32);
//        uid_aes_encrypt (in, inlen, temp, outlen, key, iv);

//        //printf("\nthe length of pload bus is %d\n",*outlen);

//        if (*outlen == 0)
//        {
//            printf ("aes encrypt failed\n");
//            return NULL;
//        }
//        return temp;
//}

//unsigned char *uid_get_aes_encrypted_data_fino_FI(unsigned char *in, int inlen,
//         unsigned char *key,int *outlen)
//{
//    unsigned char iv[32]={0}, *tempFI, *out=NULL;
//    int templen,i=0;

//    //printf("\n In AES Enc\n");
//    *outlen=0;

////    for(i=0;i<32;i++)
////        printf("%02X ",key[i]);
////    printf("\n");

//    tempFI = (char *)malloc(inlen+32);
//    uid_aes_encrypt (in, inlen, tempFI, &templen, key, iv);

//    printf("\nthe length of aes enc is %d\n",templen);

//    if (templen == 0){
//        printf ("aes encrypt failed\n");
//        return NULL;
//    }
//    //for(i =0;i<templen;i++)
//      //      printf("%02X ",temp[i]);

//    out = (char *)malloc(templen*4);
//    uid_encode_b64(tempFI, templen, out, outlen);
//    if (*outlen == 0){
//        printf ("aes b64 encode failed\n");
//        return NULL;
//    }
//    printf("\nthe length of base64 is %d\n",*outlen);
//    out[*outlen] = 0;
//    return out;
//    //return temp;
//}
int getPublickey_Exponent(unsigned char* pubKey,unsigned char* exponent,int *pubLen,int *expLen)
{
    AsciiStr2HexByte(g_pubkey,strlen(g_pubkey),pubKey,pubLen);
    AsciiStr2HexByte(g_Exponent,strlen(g_Exponent),exponent,expLen);

    return 0;
}


int uid_aes_encrypt_256(unsigned char *in, int inlen, unsigned char *out,
        int *outlen, unsigned char *key, unsigned char *iv)
{
    int tmplen;
    // Straight encrypt

    EVP_CIPHER_CTX x;
    EVP_CIPHER_CTX_init(&x);
 #if DEBUG
    printf ("AES encrypted data %d len\n", *outlen);
#endif
    EVP_CIPHER_CTX_set_padding(&x,1); // 1- padding, 0 - No Padding

    if (!EVP_EncryptInit_ex(&x, EVP_aes_256_cbc(), NULL, key, iv)) {
        printf("\n ERROR!! \n");
        return -1;
    }
    if (!EVP_EncryptUpdate(&x, out, outlen,
            (const unsigned char*) in, inlen)) {
        printf("\n ERROR!! \n");
        return -2;
    }
    if (!EVP_EncryptFinal_ex(&x,out + *outlen,&tmplen)) {
        printf("\n ERROR!! \n");
        return -3;
    }
    *outlen += tmplen;
#ifdef DEBUG
    printf ("AES encrypted data %d len\n", *outlen);
    print_data (out, *outlen);
#endif
    EVP_CIPHER_CTX_cleanup(&x);

    return 0;
}


int calcDecodeLength(char* b64input)
{
  size_t len = strlen(b64input), padding = 0;

  if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
    padding = 2;
  else if (b64input[len-1] == '=') //last char is =
    padding = 1;
  return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length)
{
  BIO *bio, *b64;
  int decodeLen = calcDecodeLength(b64message);
  *buffer = (unsigned char*)malloc(decodeLen + 1);
  (*buffer)[decodeLen] = '\0';

  bio = BIO_new_mem_buf(b64message, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
  *length = BIO_read(bio, *buffer, strlen(b64message));
  assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
  BIO_free_all(bio);

  return (0);
}




int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv,unsigned char * plain_text)
{

  EVP_CIPHER_CTX *ctx;
  unsigned char *plaintext;
  int len;
  int plaintext_len;
 // unsigned char* plaintext = new unsigned char[ciphertext_len];
  plaintext = (unsigned char *)malloc(ciphertext_len);
  bzero(plaintext,ciphertext_len);

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new()))
  {
      free(plaintext);
      return -1;
  }

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
  {
      free(plaintext);
      return -1;
  }

  EVP_CIPHER_CTX_set_key_length(ctx, EVP_MAX_KEY_LENGTH);

 /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
  {
      free(plaintext);
      return -1;
  }

  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
  {
      free(plaintext);
      return -1;
  }
  plaintext_len += len;


  /* Add the null terminator */
  plaintext[plaintext_len] = 0;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  //char ret[50];
  strcpy(plain_text,plaintext);
 // strcpy(ret,plaintext);
  // ret = (char*)plaintext;
 // delete [] plaintext;
  free(plaintext);
  return 1 ;
}

void BinToHex(unsigned char*input,char*output,int len)
{
    int i =0,j=0;
    //sprintf(temp,"%X",input[0]);
    for(i=0;i<=len;i++)
    {
        j=i*2;
        sprintf(output+j,"%02x",input[i]);
    }
    return;
}


int stringLength = sizeof(alphanum) - 1;

char genRandom()
{

    return alphanum[rand() % stringLength];
}

int fpb_rsa_encrypt_with_fino_pubkey(unsigned char *in, int inlen, unsigned char *outbuf, int *outlen)
{
#if RSA_KEY_UAT_PROD

    static const char key[] = "-----BEGIN PUBLIC KEY-----\n"
            "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCf2eP31PcIzNfoPOcboDVi/e++B\n"
            "AD8scLxVhWD9JOVCaX1mUZXbFdfyluQkWlrD60uahK4ZT0JgOlUEhn7SiMR8uClMrt\n"
            "XC3OH/oRr/Dlf2JJLxjwYxuMp1zpZpsrn/ftCSfDhFJtKUlRn7uVKSuz0Y3unnBK25\n"
            "h5eIp95qObNxQIDAQAB\n"
            "-----END PUBLIC KEY-----\n";
#else

    static const char key[] = "-----BEGIN PUBLIC KEY-----\n"
            "MIGeMA0GCSqGSIb3DQEBAQUAA4GMADCBiAKBgFfSikUF30ExV/gzLWJfJOOBNNMH\n"
            "zSgii+xYtXZIOCsjm+3nUoPE5RvyZdPy7xCDgYX0U1pZ8ddv1Vz4oI68xm2NFe2g7\n"
            "1fuH1unOovkR++r9RZ/VaVX38ZefBC9sq5UIiBmXzJPxgqEXM4INc3LR1tcAXu5KD\n"
            "UdarnKfj+aZfYNAgMBAAE=\n"
            "-----END PUBLIC KEY-----\n";


#endif
        int len=0;
        int i=0;



    BIO* bio = BIO_new_mem_buf(key, (int)sizeof(key));
    assert(bio != NULL);

    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    assert(pkey != NULL);

    int type = EVP_PKEY_get_type(pkey);
    assert(type == EVP_PKEY_RSA || type == EVP_PKEY_RSA2);

    RSA* rsa = EVP_PKEY_get1_RSA(pkey);
    assert(rsa != NULL);

    len = RSA_public_encrypt(inlen, in, outbuf,rsa,RSA_PKCS1_PADDING); //RSA_NO_PADDING

#if DEBUG
    printf("\nEncryt inside RSA --------------------------------->\n");
    printf("Output buffer %s",outbuf);
#endif
    enc_ses_key_pack=(char*)malloc(len+5);
   for(i=0;i<len;i++)
   {	printf("%02X ",outbuf[i]);

   }

    *outlen = len;
    EVP_PKEY_free(pkey);
    RSA_free(rsa);
    BIO_free(bio);


}
int EVP_PKEY_get_type(EVP_PKEY *pkey)
{
    assert(pkey);
    if (!pkey)
        return EVP_PKEY_NONE;
    printf("Inside the EVP_PKEY_get_type");
    return EVP_PKEY_type(pkey->type);
}
//void handleOpenSSLErrors(void)
//{
//  ERR_print_errors_fp(stderr);
//  abort();
//}
int uid_aes_encryption_128(unsigned char *in, int inlen, unsigned char *out,
        int *outlen, unsigned char *key, unsigned char *iv)
{
    printf("inside uid_aes_encrypt");
    int tmplen;
    // Straight encrypt

    EVP_CIPHER_CTX x;
    EVP_CIPHER_CTX_init(&x);
    printf ("AES encrypted data %d len\n", *outlen);
    EVP_CIPHER_CTX_set_padding(&x,1); // 1- padding, 0 - No Padding

    if (!EVP_EncryptInit_ex(&x, EVP_aes_128_ecb(), NULL, key, iv)) {
        printf("\n ERROR!! \n");
        return -1;
    }
    if (!EVP_EncryptUpdate(&x, out, outlen,
            (const unsigned char*) in, inlen)) {
        printf("\n ERROR!! \n");
        return -2;
    }
    if (!EVP_EncryptFinal_ex(&x,out + *outlen,&tmplen)) {
        printf("\n ERROR!! \n");
        return -3;
    }
    *outlen += tmplen;
#ifdef DEBUG
    printf ("AES encrypted data %d len\n", *outlen);
    print_data (out, *outlen);
#endif
    EVP_CIPHER_CTX_cleanup(&x);
    return 0;
}
