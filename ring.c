/*
* @file ring.c
* @author Brenton Salmi, KB1LQD
* @date 15 June, 2020
* @brief A simple circular byte buffer implementation.
*
* This is a simple implementation of a byte circular buffer that stores and
* retrieves data as a First-in-first-out (FIFO).
*/

/* -- Includes -- */

/* libc includes. */
#include <stdio.h>

/* specific includes. */
#include "ring.h"

void RingInitBuffer(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned int bufferbytesize)
{
	//buffer;
	rbufcntl->bufferbytesize = bufferbytesize;
	rbufcntl->inwaiting = 0;
	rbufcntl->head = 0;
	rbufcntl->tail = 0;
	rbufcntl->overflowflag = 0;
}

int RingPut(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *data, unsigned int length)
{
	unsigned int i; // byte count

	// Put each element into ring buffer location
	for(i=0; i<length; i++)
	{

		//Check if overflow
		if(rbufcntl->inwaiting >= rbufcntl->bufferbytesize) //overflow
		{
			rbufcntl->overflowflag=1;

			//return failure
			return 1;
		}

		else //not overflow
		{
			//Insert element into buffer byte by byte
			rbuf[rbufcntl->head] = data[i];
			//Increment head counter
			RingPutIncrement(rbufcntl);
		}
	}

	//return success
	return 0;
}

void RingPutIncrement(struct ring_buffer_control *rbufcntl)
{
	//Increament head counter
	if((rbufcntl->head+1)>=rbufcntl->bufferbytesize)
	{
		//Wrap
		rbufcntl->head=0;
	}
	else
	{
		//Increment
		rbufcntl->head++;
	}

	//increment inwaiting counter
	rbufcntl->inwaiting++;
}

int RingGet(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *getbuf, unsigned int length)
{
	if(rbufcntl->inwaiting<=0) //no byte(s) to get
	{
		return 1;
	}
	else if(length > rbufcntl->inwaiting) // length of bytes to get is larger than bytes in waiting
	{
		return 1;
	}
	else // bytes to get
	{
		unsigned int i;
		for(i=0; i<length; i++)
		{
			getbuf[i]=rbuf[rbufcntl->tail];
			//increment tail location
			RingGetIncrement(rbufcntl);
		}
	}
}

void RingGetIncrement(struct ring_buffer_control *rbufcntl)
{
	//Increament head counter
	if((rbufcntl->tail+1)>=rbufcntl->bufferbytesize)
	{
		//Wrap
		rbufcntl->tail=0;
	}
	else
	{
		//Increment
		rbufcntl->tail++;
	}

	//decrement inwaiting counter
	rbufcntl->inwaiting--;
}

int RingInWaiting(struct ring_buffer_control *rbufcntl)
{
	return rbufcntl->inwaiting;
}

int RingResetOverflow(struct ring_buffer_control *rbufcntl)
{
	rbufcntl->overflowflag = 0;
	return 0;
}
