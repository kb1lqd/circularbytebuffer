/*
* @file ring.h
* @author Brenton Salmi, KB1LQD
* @date 15 June, 2020
* @brief Function prototypes for the circular byte buffer.
*
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
* @brief Puts byte(s) into the FIFO circular buffer
*
* This function is used to place new byte(s) into the FIFO for storage. It will
* accept a single or multiple bytes (byte array) respective to the (int)
* length of data (in bytes) to put into the FIFO.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @param rbuf A pointer to the byte array that the FIFO will use to store bytes
* @param data A pointer to the byte or byte array containing the data to put into the FIFO
* @param length integer of the size in bytes of data to put into the FIFO
* @return 0 for success
*
*/
int RingPut(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *data, unsigned int length);

/**
* @brief A sub function used to increment the FIFO state logic when placing new data into the fifo
*
* This function is used to increment the inwaiting byte counter and the HEAD
* location variable appropriately. Specifically this handles incrementing past
* the end of the buffer (rollover) safely.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @return Void
*
*/
void RingPutIncrement(struct ring_buffer_control *rbufcntl);

/**
* @brief Gets byte(s) from the FIFO circular buffer
*
* This function is used to retrieve byte(s) from the FIFO. It will
* retrieve a single or multiple bytes (byte array) respective to the (int)
* length of data (in bytes) to get from the FIFO.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @param rbuf A pointer to the byte array that the FIFO will use to store bytes
* @param getbuf A pointer to the byte or byte array to place the retrieved bytes into
* @param length integer of the size in bytes of data to put into the FIFO
* @return 0 for success
*
*/
int RingGet(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *getbuf, unsigned int length);

/**
* @brief A sub function used to increment the FIFO state logic when retrieving new data from the fifo
*
* This function is used to decrement the inwaiting byte counter and increment the TAIL
* location variable appropriately. Specifically this handles decrementing past
* the end of the buffer (rollover) safely.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @return Void
*
*/
void RingGetIncrement(struct ring_buffer_control *rbufcntl);

/**
* @brief This function returns the number of bytes in the fifo awaiting retrieval
*
* This function simply returns the number of bytes actively stored inside of the FIFO.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @return The number of bytes (int) of active data in the FIFO waiting to be retrieved
*
*/
int RingInWaiting(struct ring_buffer_control *rbufcntl);

/**
* @brief This function resets the flag indicating overflog has previously occured.
*
* This function resets to 0 the FIFO control logic flag indicating overflog has previously occured.
*
* @param rbufcntl A pointer to the ring_buffer_control struct that the FIFO will use to store control logic state
* @return 0 for success
*
*/
int RingResetOverflow(struct ring_buffer_control *rbufcntl);

#endif
