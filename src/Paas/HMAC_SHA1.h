/*
	100% free public domain implementation of the HMAC-SHA1 algorithm
	by Chien-Chung, Chung (Jim Chung) <jimchung1221@gmail.com>
*/


#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__

#include "SHA1.h"

enum {
    SHA1_DIGEST_LENGTH	= 20,
    SHA1_BLOCK_SIZE		= 64,
    HMAC_BUF_LEN		= 1024*1024
} ;

class CHMAC_SHA1 : public CSHA1
{
    private:
		UINT_8 m_ipad[64];
		UINT_8 m_opad[64];

		char * szReport ;
		char * SHA1_Key ;
		char * AppendBuf1 ;
		char * AppendBuf2 ;


	public:
		CHMAC_SHA1(size_t len)
			:szReport(new char[len+sizeof(m_ipad)+1024]),
             AppendBuf1(new char[len+sizeof(m_ipad)+1024]),
             AppendBuf2(new char[len+sizeof(m_ipad)+1024]),
             SHA1_Key(new char[len+sizeof(m_ipad)+1024])
    {}

        ~CHMAC_SHA1()
        {
            delete[] szReport ;
            delete[] AppendBuf1 ;
            delete[] AppendBuf2 ;
            delete[] SHA1_Key ;
        }

        void HMAC_SHA1(UINT_8 *text, int text_len, UINT_8 *key, int key_len, UINT_8 *digest);
};


#endif /* __HMAC_SHA1_H__ */
