// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __CRYPTO_H
#define __CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* transport verification */
#define	LEN_RAND_TRANSP				8

/* SHA lengh */
#define SHS_DIGEST_SIZE  20
#define SHS_DIGESTSIZE	SHS_DIGEST_SIZE

/* DSA key length */
#define LEN_SIG_DSA							40


#define DES_SIMPLE_KEY_LEN					8
#define DES_DOUBLE_KEY_LEN					16
#define DES_TRIPLE_KEY_LEN					24
#define DES_BLOCKSIZE						8

#define LEN_CMPT_ANTI_REPLAY				4

#define	LEN_PRIV_DSA						20


typedef enum {ALGO_NONE, ALGO_RSA, ALGO_DSA} T_ALGO_TYP, *PT_ALGO_TYP;

typedef struct 
{
	T_ALGO_TYP	m_x_typAlgo;
	UC			m_auc_keyPriv[LEN_PRIV_DSA];
} T_privKeyDSA, *PT_privKeyDSA;

typedef struct 
{
	T_ALGO_TYP	m_x_typAlgo;
	UC			m_uc_pad[3];	// to align the structure and avoid warnings
	int			lenprime;		// Longueur des Parametre p et q
	mot			*dmodp;			// Precalcul (d mod p-1)
	mot			*dmodq;			// Precalcul (d mod q-1)
  	mot			*p_prime;		// Parametre p
	mot			*q_prime;		// Parametre q
	mot			*C2;			// Precalcul (1/p mod q)
	mot			motp;			// Precalcul Montgomery  sur p
	mot			motq;			// Precalcul Montgomery  sur q
} T_privKeyRSA, *PT_privKeyRSA;

typedef union
{
	T_privKeyRSA	m_x_privRSA;
	T_privKeyDSA	m_x_privDSA;
} T_privKey, *PT_privKey;

typedef struct
{
	T_ALGO_TYP		m_x_typAlgo;
	UC				m_uc_pad[3];// to align the structure and avoid warnings
	T_SECU_BUF		m_x_modulus;
	T_SECU_BUF		m_x_exp;
} T_pubKeyRSA, *PT_pubKeyRSA;

typedef struct	
{
	T_ALGO_TYP		m_x_typAlgo;
	UC				m_uc_pad[3];// to align the structure and avoid warnings
	T_SECU_BUF		m_x_p;
	T_SECU_BUF		m_x_q;
	T_SECU_BUF		m_x_g;
	T_SECU_BUF		m_x_y;
} T_pubKeyDSA, *PT_pubKeyDSA;

typedef union
{
	T_pubKeyRSA		m_x_pubRSA;
	T_pubKeyDSA		m_x_pubDSA;
} T_pubKey, *PT_pubKey;

			
typedef struct
{
	UC				m_uc_flag;
	UC				m_uc_pad[3];// to align the structure and avoid warnings
	T_SECU_BUF		m_x_DistinguishName;
	T_pubKey		m_x_pubKey;
	T_SECU_BUF		m_x_certifASN1;
	UC				m_auc_Shs[SHS_DIGEST_SIZE];  //shs du certificat X509 complet
} T_X509Certificate, *PT_X509Certificate;

// Ces defines sont definis maintenant dans MorphoTypes.h SECU_TUNNELING, SECU_OFFERED_SECURITY, SECU_PK_ONLY_SIGNED, SECU_SIGNED_DOWNLOAD, SECU_NEVER_EXPORT_SCORE, SECU_FFD, SECU_LOCKED


typedef struct
{
	T_SECU_BUF		m_x_certifMSO;					// certificat du MSO
	T_SECU_BUF		m_x_token;
} T_TokenInitAuthOut1, *PT_TokenInitAuthOut1;

typedef struct
{
	T_SECU_BUF		m_x_token;
} T_TokenInitAuthIn2, *PT_TokenInitAuthIn2;

typedef struct
{
	T_SECU_BUF		m_x_token;
} T_TokenInitAuthOut2,*PT_TokenInitAuthOut2;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__CRYPTO_H*/


