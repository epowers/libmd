
/* ----------------------------- SHA Test code --------------------------- */
#include <stdio.h>

#include "sha.h"

#include <stdio.h>
#include <stdlib.h> /* For exit() */
#include <time.h>

/* Size of buffer for SHA speed test data */

#define TEST_BLOCK_SIZE	( SHA_HASHBYTES * 100 )

/* Number of bytes of test data to process */

#define TEST_BYTES 10000000L
#define TEST_BLOCKS ( TEST_BYTES / TEST_BLOCK_SIZE )

#if SHA_VERSION
static char const *shaTestResults[] = {
		"A9993E364706816ABA3E25717850C26C9CD0D89D",
		"84983E441C3BD26EBAAE4AA1F95129E5E54670F1",
		"34AA973CD4C4DAA4F61EEB2BDBAD27316534016F",
		"34AA973CD4C4DAA4F61EEB2BDBAD27316534016F",
		"34AA973CD4C4DAA4F61EEB2BDBAD27316534016F" };
#else
static char const *shaTestResults[] = {
 "0164B8A914CD2A5E74C4F7FF082C4D97F1EDF880",
 "D2516EE1ACFA5BAF33DFC1C471E438449EF134C8",
 "3232AFFA48628A26653B5AAA44541FD90D690603",
 "3232AFFA48628A26653B5AAA44541FD90D690603",
"3232AFFA48628A26653B5AAA44541FD90D690603" };
#endif

static int
compareSHAresults(u_int8_t const *hash, int level)
	{
		char buf[41];
		int i;

		for (i = 0; i < SHA_HASHBYTES; i++)
		 sprintf(buf+2*i, "%02X", hash[i]);

		if (strcmp(buf, shaTestResults[level-1]) == 0) {
		 printf("Test %d passed, result = %s\n", level, buf);
		 return 0;
		} else {
			printf("Error in SHA implementation: Test %d failed\n", level);
			printf("  Result = %s\n", buf);
			printf("Expected = %s\n", shaTestResults[level-1]);
			return -1;
		}
	}
int
main(void)
	{
		struct SHAContext sha;
		u_int8_t data[TEST_BLOCK_SIZE];
		u_int8_t hash[20];
		clock_t ticks;
		long i;

		/*
		* Test output data (these are the only test data given in the
		* Secure Hash Standard document, but chances are if it works
		* for this it'll work for anything)
		*/
		SHAInit(&sha);
		SHAUpdate(&sha, "abc", 3);
		SHAFinal(hash,&sha);
		if (compareSHAresults(hash, 1) < 0)
		 exit (-1);

		SHAInit(&sha);
		SHAUpdate(&sha, "abcdbcdecdefdefgefghfghighijhijki\
jkljklmklmnlmnomnopnopq", 56);
		SHAFinal(hash,&sha);
		if (compareSHAresults(hash, 2) < 0)
		 exit (-1);

		/* 1,000,000 bytes of ASCII 'a' (0x61), by 64's */
		SHAInit(&sha);
		for (i = 0; i < 15625; i++)
		 SHAUpdate(&sha, "aaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 64);
 SHAFinal(hash,&sha);
 if (compareSHAresults(hash, 3) < 0)
		 exit (-1);

		/* 1,000,000 bytes of ASCII 'a' (0x61), by 25's */
		SHAInit(&sha);
		for (i = 0; i < 40000; i++)
		 SHAUpdate(&sha, "aaaaaaaaaaaaaaaaaaaaaaaaa", 25);
		SHAFinal(hash,&sha);
		if (compareSHAresults(hash, 4) < 0)
		 exit (-1);

		/* 1,000,000 bytes of ASCII 'a' (0x61), by 125's */
		SHAInit(&sha);
		for (i = 0; i < 8000; i++)
		 SHAUpdate(&sha, "aaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 125);
		SHAFinal(hash,&sha);
		if (compareSHAresults(hash, 5) < 0)
		 exit (-1);

		/* Now perform time trial, generating MD for 10MB of data. First,
		initialize the test data */
		memset(data, 0, TEST_BLOCK_SIZE);

		/* Get start time */
		printf("SHA time trial.  Processing %ld characters...\n", TEST_BYTES);
		ticks = clock();

		/* Calculate SHA message digest in TEST_BLOCK_SIZE byte blocks */
		SHAInit(&sha);
		for (i = TEST_BLOCKS; i > 0; i--)
		 SHAUpdate(&sha, data, TEST_BLOCK_SIZE);
		SHAFinal(hash,&sha);

		/* Get finish time and print difference */
		ticks = clock() - ticks;
		printf("Ticks to process test input: %lu\n", (unsigned long)ticks);

		return 0;
	}
