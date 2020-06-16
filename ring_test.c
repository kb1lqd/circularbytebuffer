
#include <stdio.h>
#include "ring.h"

#define RINGBUFSIZEBYTES 16
#define TESTBUFSIZEBYTES 64

int PrintBufferRaw(struct ring_buffer_control *rbufcntl, unsigned char *rbuf);
int PrintBufferStats(struct ring_buffer_control rbufcntl);

int main(int argc, char *argv[])
{

	unsigned char ringbuf[RINGBUFSIZEBYTES];
	struct ring_buffer_control testbufcntl;
	unsigned char msgbuf[TESTBUFSIZEBYTES];
	unsigned char datamsg0[5] = {0,1,2,3,4};
  unsigned char datamsg1[5] = {5,6,7,8,9};
	unsigned char datamsg2[15] = {0xD,0xE,0xA,0xD,0xB,0xE,0xE,0xF,0,1,2,3,4,5,6};
	unsigned char datamsg3[15] = {'D','E','A','D','B','E','E','F',0,1,2,3,4,5,6};
	unsigned int rxbufsize;
	unsigned char rxbuf[TESTBUFSIZEBYTES];


	int i;

	for(i=0;i<RINGBUFSIZEBYTES;i++)
	{
		ringbuf[i]=0;
	}

	for(i=0;i<TESTBUFSIZEBYTES;i++)
	{
		msgbuf[i]=i;
	}




	/////////////////////////////
	// TEST 1 -Test a simple put and get smaller than buffer size
	/////////////////////////////

	//Initialize buffer
	RingInitBuffer(&testbufcntl, ringbuf, RINGBUFSIZEBYTES);

	printf("--- TEST 1 ---\n");
	PrintBufferStats(testbufcntl);
	RingPut(&testbufcntl, ringbuf, datamsg2, 10);
	PrintBufferStats(testbufcntl);

	//print data in buffer
	PrintBufferRaw(&testbufcntl, ringbuf);

	//get bytes from buffer
	printf("Getting bytes.\n");
	rxbufsize = RingInWaiting(&testbufcntl);
	RingGet(&testbufcntl, ringbuf, rxbuf, RingInWaiting(&testbufcntl));

	//Print GOT bytes
	printf("GOT BYTES: ");
	for(i=0; i<rxbufsize; i++)
		{
			printf("%d%c ", i, rxbuf[i]);
		}
	printf("\n");
	PrintBufferStats(testbufcntl);

	//END
	printf("--- END TEST 1 ---\n\n");

	/////////////////////////////
	// Test 2 - Test a simple put and get smaller than buffer size but that wraps around buffer end
	/////////////////////////////

	//Initialize buffer
	RingInitBuffer(&testbufcntl, ringbuf, RINGBUFSIZEBYTES);

	printf("--- TEST 2 ---\n");

	//setup conditions
	RingPut(&testbufcntl, ringbuf, datamsg2, 10);
	RingGet(&testbufcntl, ringbuf, rxbuf, RingInWaiting(&testbufcntl));

	//Put data in
	RingPut(&testbufcntl, ringbuf, datamsg2, 10);
	PrintBufferStats(testbufcntl);
	//print data in buffer
	PrintBufferRaw(&testbufcntl, ringbuf);
	printf("Getting bytes.\n");
	rxbufsize = RingInWaiting(&testbufcntl);
	RingGet(&testbufcntl, ringbuf, rxbuf, RingInWaiting(&testbufcntl));
	//Print GOT bytes
	printf("GOT BYTES: ");
	for(i=0; i<rxbufsize; i++)
		{
			printf("%X", rxbuf[i]);
			if((i+1)%4==0)
				printf(" "); //place a space every 4 bytes
		}
	printf("\n");
	PrintBufferStats(testbufcntl);

	//END
	printf("--- END TEST 2 ---\n\n");

	/////////////////////////////
	// Test 3 - Test Overflow
	/////////////////////////////

	//Initialize buffer
	RingInitBuffer(&testbufcntl, ringbuf, RINGBUFSIZEBYTES);

	printf("--- TEST 3 ---\n");

	//setup conditions
	RingPut(&testbufcntl, ringbuf, datamsg2, 10);

	//Put data in (this will overflow with size 16 buffer at second E leaving [DEADBEED01DEADBE])
	RingPut(&testbufcntl, ringbuf, datamsg2, 10);
	PrintBufferStats(testbufcntl);
	//print data in buffer
	PrintBufferRaw(&testbufcntl, ringbuf);
	printf("Getting bytes.\n");
	rxbufsize = RingInWaiting(&testbufcntl);
	RingGet(&testbufcntl, ringbuf, rxbuf, RingInWaiting(&testbufcntl));
	//Print GOT bytes
	printf("GOT BYTES: ");
	for(i=0; i<rxbufsize; i++)
		{
			printf("%X", rxbuf[i]);
			if((i+1)%4==0)
				printf(" "); //place a space every 4 bytes
		}
	printf("\n");
	PrintBufferStats(testbufcntl);

	//Put data in to check for working buffer ()
	RingPut(&testbufcntl, ringbuf, datamsg0, 5);

	//Get bytes and check for working condition (should leave [01234EED01DEADBE])
	RingGet(&testbufcntl, ringbuf, rxbuf, RingInWaiting(&testbufcntl));
	//Print GOT bytes
	printf("GOT BYTES: ");
	for(i=0; i<rxbufsize; i++)
		{
			printf("%X", rxbuf[i]);
			if((i+1)%4==0)
				printf(" "); //place a space every 4 bytes
		}
	printf("\n");
	PrintBufferStats(testbufcntl);

	//Reset overflow flag
	printf("Reseting Overflow Flag!\n");
	RingResetOverflow(&testbufcntl);
	PrintBufferStats(testbufcntl);
	//END
	printf("--- END TEST 3 ---\n\n");


	return 0;
}


int PrintBufferRaw(struct ring_buffer_control *rbufcntl, unsigned char *rbuf)
{
	unsigned int i;
	printf("RAW BUFFER(%d): ", rbufcntl->bufferbytesize);
	for(i=0; i<rbufcntl->bufferbytesize; i++)
	{
		printf("%X", rbuf[i]);
		if((i+1)%4==0)
			printf(" "); //place a space every 4 bytes
	}
	printf("\n");
}

int PrintBufferStats(struct ring_buffer_control rbufcntl)
{
	printf("---Buffer Stats---\n");
	printf("Buffer Size: %d\n", rbufcntl.bufferbytesize);
	printf("Bytes In Waiting: %d\n", rbufcntl.inwaiting);
	printf("HEAD LOC: %d\n", rbufcntl.head);
	printf("TAIL LOC: %d\n", rbufcntl.tail);
	printf("OVERFLOW FLAG: %d\n", rbufcntl.overflowflag);
	printf("------------------\n");
}
