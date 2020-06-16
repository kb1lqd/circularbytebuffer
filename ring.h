/*
* @file ring.h
* @author Brenton Salmi, KB1LQD
* @date 15 June, 2020
* @brief A simple circular byte buffer implementation.
*
* This is a simple implementation of a byte circular buffer that stores and
* retrieves data as a First-in-first-out (FIFO).
*/

#ifndef RING_H_
#define RING_H_

/**
* @brief This is the main control logic struct to maintain FIFO paramters and state.
*
* A pointer to a fixed byte array and the buffer size (int) in bytes is
* Initialized at program boot. During operations the logic states and values are
* updated to reflect proper operation. In an overflow condition (no more room
* left in buffer for new data) then the data is ignored, dropped, and overflowflag
* is toggled to a value of 1.
*/
struct ring_buffer_control {
	unsigned char * buffer;
	unsigned int bufferbytesize;
	unsigned int inwaiting;
	unsigned int head;
	unsigned int tail;
	unsigned char overflowflag;
};

/**
* @brief Initializes a circular buffer control logic struct.
*
* This function initlizes a circular buffer instance to a known initial state
* and the desired size in bytes.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @param rbuf A pointer to the byte array that the FIFO will use to store bytes
* @param bufferbytesize Size in bytes (int) of the rbuf byte array
* @return Void
*/
void RingInitBuffer(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned int bufferbytesize);

/**
*
*
*
*/
int RingPut(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *data, unsigned int length);

/**
*
*
*
*/
void RingPutIncrement(struct ring_buffer_control *rbufcntl);

/**
*
*
*
*/
int RingGet(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *getbuf, unsigned int length);

/**
*
*
*
*/
void RingGetIncrement(struct ring_buffer_control *rbufcntl);

/**
*
*
*
*/
int RingInWaiting(struct ring_buffer_control *rbufcntl);

/**
*
*
*
*/
int RingResetOverflow(struct ring_buffer_control *rbufcntl);

#endif
