#ifndef RING_H_
#define RING_H_

// Define circular buffer struct
struct ring_buffer_control {
	unsigned char * buffer;
	unsigned int bufferbytesize;
	unsigned int inwaiting;
	unsigned int head;
	unsigned int tail;
	unsigned char overflowflag;
};

void RingInitBuffer(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned int bufferbytesize);
int RingPut(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *data, unsigned int length);
void RingPutIncrement(struct ring_buffer_control *rbufcntl);
int RingGet(struct ring_buffer_control *rbufcntl, unsigned char *rbuf, unsigned char *getbuf, unsigned int length);
void RingGetIncrement(struct ring_buffer_control *rbufcntl);
int RingInWaiting(struct ring_buffer_control *rbufcntl);
int RingResetOverflow(struct ring_buffer_control *rbufcntl);

#endif
