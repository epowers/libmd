/*
 * Test driver for libmd library (MD2,4,5 and SHA-1,RIPEMD160 message digests).
 * Copyright (c) 1999 Martin Hinner, <mhi@penguin.cz>.
 * This code is in the public domain; do with it what you wish.
 */
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <md2.h>
#include <md4.h>
#include <md5.h>
#include <sha.h>
#include <rmd160.h>

static char MDResult[0x40];
int MDFailed = 0;

extern char *libmd_version;

#define TEST_BLOCK_SIZE		8000
#define TEST_BYTES		10000000
#define TEST_BLOCKS		(TEST_BYTES/TEST_BLOCK_SIZE)

#define MAKE_STRING(HASH,NAME) \
static void HASH##String (char *string, char *print,char *result) \
{ \
  printf(NAME "(%s)%*s= %s, ", \
         print, 27 - strlen(print) - strlen(NAME) , "", \
         HASH##Data(string,strlen(string),MDResult)); \
  if (!strcmp(MDResult,result)) \
       printf("ok\n"); \
         else { \
          printf("*BAD*\n"); \
          MDFailed = 1; \
         } \
}

MAKE_STRING(MD2,"MD2")
MAKE_STRING(MD4,"MD4")
MAKE_STRING(MD5,"MD5")
MAKE_STRING(SHA,"SHA-1")
MAKE_STRING(RMD160,"RMD160")

#define MAKE_TRIAL(HASH,NAME) \
static void HASH##TimeTrial (void) \
{ \
 void *data; \
 time_t ticks; \
 int i; \
 struct HASH##Context ctx; \
\
 printf(NAME " time trial%*s= ", 18-strlen(NAME), ""); \
 fflush(stdout); \
\
 data = malloc(TEST_BLOCK_SIZE); \
 memset(data, 0, TEST_BLOCK_SIZE); \
 ticks = clock(); \
\
 HASH##Init(&ctx); \
 for (i = TEST_BLOCKS; i > 0; i--) \
   HASH##Update(&ctx, data, TEST_BLOCK_SIZE); \
 HASH##Final(MDResult,&ctx); \
\
 ticks = clock() - ticks; \
 printf("%u ticks, %g bytes/sec.\n", \
        ticks,(double)CLOCKS_PER_SEC*TEST_BYTES/((double)ticks)); \
}

MAKE_TRIAL(MD2,"MD2")
MAKE_TRIAL(MD4,"MD4")
MAKE_TRIAL(MD5,"MD5")
MAKE_TRIAL(SHA,"SHA-1")
MAKE_TRIAL(RMD160,"RMD160")

int main()
{
  printf("libmd version %s, built at " __TIME__ ", " __DATE__ ".\n",
       libmd_version);

  printf ("---------- MD2 TEST SUITE ----------\n");
  MD2String ("", "\"\"", "8350e5a3e24c153df2275c9f80692773");
  MD2String ("a", "\"a\"", "32ec01ec4a6dac72c0ab96fb34c0b5d1");
  MD2String ("abc", "\"abc\"", "da853b0d3f88d99b30283a69e6ded6bb");
  MD2String ("message digest","\"message digest\"",
             "ab4f496bfb2a530b219ff33031fe06b0");
  MD2String ("abcdefghijklmnopqrstuvwxyz","a..z",
             "4e8ddff3650292ab5a4108c3aa47940b");
  MD2String ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "A..Za..z0..9","da33def2a42df13975352846c30338cd");
  MD2String ("123456789012345678901234567890123456789"
             "01234567890123456789012345678901234567890",
             "8 times \"1234567890\"","d5976f79d83d3a0dc9806c3c66f3efd8");
  MD2TimeTrial();

  printf ("---------- MD4 TEST SUITE ----------\n");
  MD4String ("", "\"\"", "31d6cfe0d16ae931b73c59d7e0c089c0");
  MD4String ("a", "\"a\"", "bde52cb31de33e46245e05fbdbd6fb24");
  MD4String ("abc", "\"abc\"", "a448017aaf21d8525fc10ae87aa6729d");
  MD4String ("message digest","\"message digest\"",
             "d9130a8164549fe818874806e1c7014b");
  MD4String ("abcdefghijklmnopqrstuvwxyz","a..z",
             "d79e1c308aa5bbcdeea8ed63df412da9");
  MD4String ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "A..Za..z0..9","043f8582f241db351ce627e153e7f0e4");
  MD4String ("123456789012345678901234567890123456789"
             "01234567890123456789012345678901234567890",
             "8 times \"1234567890\"","e33b4ddc9c38f2199c3e7b164fcc0536");
  MD4TimeTrial();

  printf ("---------- MD5 TEST SUITE ----------\n");
  MD5String ("", "\"\"", "d41d8cd98f00b204e9800998ecf8427e");
  MD5String ("a", "\"a\"", "0cc175b9c0f1b6a831c399e269772661");
  MD5String ("abc", "\"abc\"", "900150983cd24fb0d6963f7d28e17f72");
  MD5String ("message digest","\"message digest\"",
             "f96b697d7cb7938d525a2f31aaf161d0");
  MD5String ("abcdefghijklmnopqrstuvwxyz","a..z",
             "c3fcd3d76192e4007dfb496cca67e13b");
  MD5String ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "A..Za..z0..9","d174ab98d277d9f5a5611c2c9f419d9f");
  MD5String ("123456789012345678901234567890123456789"
             "01234567890123456789012345678901234567890",
             "8 times \"1234567890\"","57edf4a22be3c955ac49da2e2107b67a");
  MD5TimeTrial();
 
  printf ("---------- RIPEMD-160 TEST SUITE ----------\n");
  RMD160String ("", "\"\"", "9c1185a5c5e9fc54612808977ee8f548b2258d31");
  RMD160String ("a", "\"a\"", "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
  RMD160String ("abc", "\"abc\"", "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
  RMD160String ("message digest","\"message digest\"",
                "5d0689ef49d2fae572b881b123a85ffa21595f36");
  RMD160String ("abcdefghijklmnopqrstuvwxyz","a..z",
                "f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
  RMD160String ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz0123456789",
                "A..Za..z0..9","b0e20b6e3116640286ed3a87a5713079b21f5189");
  RMD160String ("123456789012345678901234567890123456789"
                "01234567890123456789012345678901234567890",
                "8 times \"1234567890\"",
                "9b752e45573d4b39f4dbd3323cab82bf63326bfb");
  RMD160TimeTrial();

  printf ("---------- SHA-1 TEST SUITE ----------\n");
  SHAString ("", "\"\"", "da39a3ee5e6b4b0d3255bfef95601890afd80709");
  SHAString ("a", "\"a\"", "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8");
  SHAString ("abc", "\"abc\"", "a9993e364706816aba3e25717850c26c9cd0d89d");
  SHAString ("message digest","\"message digest\"",
             "c12252ceda8be8994d5fa0290a47231c1d16aae3");
  SHAString ("abcdefghijklmnopqrstuvwxyz","a..z",
             "32d10c7b8cf96570ca04ce37f2a19d84240d3a89");
  SHAString ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "A..Za..z0..9","761c457bf73b14d27e9e9265c46f4b4dda11f940");
  SHAString ("123456789012345678901234567890123456789"
             "01234567890123456789012345678901234567890",
             "8 times \"1234567890\"",
             "50abf5706a150990a08b2c5ea40fa0e585554732");
  SHATimeTrial();

  exit(MDFailed);               /* Solaris bug... */
  return MDFailed;
}
