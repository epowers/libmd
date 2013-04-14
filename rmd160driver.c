
/* --------------------------- RMD160 Test code --------------------------- */
#include <stdio.h>
#include <stdlib.h> /* For exit() */
#include <time.h>

#include "rmd160.h"

/* Size of buffer for RMD160 speed test data */

#define TEST_BLOCK_SIZE	( RIPEMD160_HASHBYTES * 100 )

/* Number of bytes of test data to process */

#define TEST_BYTES 10000000L
#define TEST_BLOCKS	( TEST_BYTES / TEST_BLOCK_SIZE )

static char const *rmd160TestResults[] = {
		"9C1185A5C5E9FC54612808977EE8F548B2258D31",	/* "" */
		"0BDC9D2D256B3EE9DAAE347BE6F4DC835A467FFE",	/* "a" */
		"8EB208F7E05D987A9B044A8E98C6B087F15A0BFC",	/* "abc" */
		"5D0689EF49D2FAE572B881B123A85FFA21595F36",	/* "message digest" */
		"F71C27109C692C1B56BBDCEB5B9D2865B3708DBC",	/* "a..z" */
		"12A053384A9C0C88E405A06C27DCF49ADA62EB2B",	/* "abcdbcde...nopq" */
		"B0E20B6E3116640286ED3A87A5713079B21F5189",	/* "A..Za..z0..9" */
		"9B752E45573D4B39F4DBD3323CAB82BF63326BFB",	/* 8 * "1234567890" */
		"52783243C1697BDBE16D37F97F68F08325DC1528",	/* 1,000,000 "a" */
		"52783243C1697BDBE16D37F97F68F08325DC1528",	/* 1,000,000 "a" */
		"52783243C1697BDBE16D37F97F68F08325DC1528" };	/* 1,000,000 "a" */

static int
compareRMD160results(u_int8_t const *hash, int level)
{
 char buf[41];
 int i;

 for (i = 0; i < RIPEMD160_HASHBYTES; i++)
  sprintf(buf+2*i, "%02X", hash[i]);

 if (strcmp(buf, rmd160TestResults[level-1]) == 0) {
  printf("Test %d passed, result = %s\n", level, buf);
  return 0;
 } else {
  printf("Error in RMD160 implementation: Test %d failed\n", level);
  printf("  Result = %s\n", buf);
  printf("Expected = %s\n", rmd160TestResults[level-1]);
  return -1;
 }
}


int
main(void)
{
 struct RIPEMD160Context rmd160;
 u_int8_t data[TEST_BLOCK_SIZE];
 u_int8_t hash[20];
 clock_t ticks;
 long i;

 /*
 * Test output data, based on URL above.
 */
 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"", 0);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 1) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"a", 1);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 2) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"abc", 3);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 3) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"message digest", 14);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 4) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"abcdefghijklmnopqrstuvwxyz", 26);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 5) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"abcdbcdecdefdefgefghfghighijhijkijkl\
jklmklmnlmnomnopnopq", 56);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 6) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ\
abcdefghijklmnopqrstuvwxyz0123456789", 62);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 7) < 0)
  exit (-1);

 RMD160Init(&rmd160);
 RMD160Update(&rmd160, (u_int8_t *)"1234567890123456789012345678901234567890\
1234567890123456789012345678901234567890", 80);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 8) < 0)
  exit (-1);

 /* 1,000,000 bytes of ASCII 'a' (0x61), by 64's */
 RMD160Init(&rmd160);
 for (i = 0; i < 15625; i++)
  RMD160Update(&rmd160, (u_int8_t *)"aaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 64);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 9) < 0)
  exit (-1);

 /* 1,000,000 bytes of ASCII 'a' (0x61), by 25's */
 RMD160Init(&rmd160);
 for (i = 0; i < 40000; i++)
  RMD160Update(&rmd160, (u_int8_t *)"aaaaaaaaaaaaaaaaaaaaaaaaa", 25);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 10) < 0)
  exit (-1);

 /* 1,000,000 bytes of ASCII 'a' (0x61), by 125's */
 RMD160Init(&rmd160);
 for (i = 0; i < 8000; i++)
  RMD160Update(&rmd160, (u_int8_t *)"aaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 125);
 RMD160Final(hash,&rmd160);
 if (compareRMD160results(hash, 11) < 0)
  exit (-1);

 /* Now perform time trial, generating MD for 10MB of data. First,
 initialize the test data */
 memset(data, 0, TEST_BLOCK_SIZE);

 /* Get start time */
 printf("RMD160 time trial.  Processing %ld characters...\n", TEST_BYTES);
 ticks = clock();

 /* Calculate RMD160 message digest in TEST_BLOCK_SIZE byte blocks */
 RMD160Init(&rmd160);
 for (i = TEST_BLOCKS; i > 0; i--)
  RMD160Update(&rmd160, data, TEST_BLOCK_SIZE);
 RMD160Final(hash,&rmd160);

 /* Get finish time and print difference */
 ticks = clock() - ticks;
 printf("Ticks to process test input: %lu\n", (unsigned long)ticks);

 return 0;
}
