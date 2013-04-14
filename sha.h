/* SHA.H - header file for SHA.C
 */
#ifndef _SHA_H_
#define _SHA_H_

#include <sys/types.h>

/*
* Define to 1 for FIPS 180.1 version (with extra rotate in prescheduling),
* 0 for FIPS 180 version (with the mysterious "weakness" that the NSA
* isn't talking about).
*/
#define SHA_VERSION 1


#define SHA_BLOCKBYTES 64
#define SHA_BLOCKWORDS 16

#define SHA_HASHBYTES 20
#define SHA_HASHWORDS 5

/* SHA context. */
typedef struct SHAContext {
 unsigned int key[SHA_BLOCKWORDS];
 u_int32_t iv[SHA_HASHWORDS];
#ifdef __GNUC__
 u_int64_t bytes;
#else
 u_int32_t bytesHi, bytesLo;
#endif
} SHA_CTX;

#include <sys/cdefs.h>

__BEGIN_DECLS
void   SHAInit(SHA_CTX *);
void   SHAUpdate(SHA_CTX *, const unsigned char *, unsigned int);
void   SHAFinal(unsigned char [SHA_HASHBYTES], SHA_CTX *);
char * SHAEnd(SHA_CTX *, char *);
char * SHAFile(const char *, char *);
char * SHAData(const unsigned char *, unsigned int, char *);
__END_DECLS

#endif /* _SHA_H_ */
