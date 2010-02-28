#ifndef _L1_TDMA_SCHED_H
#define _L1_TDMA_SCHED_H

#include <stdint.h>

/* TDMA scheduler */

/* The idea of this scheduler is that we have a circular buffer of buckets,
 * where each bucket corresponds to one future TDMA frame [interrupt]. Each
 * bucket contains of a list of callbacks which are executed when the bucket
 * index reaches that particular bucket. */

#define TDMASCHED_NUM_FRAMES	25
#define TDMASCHED_NUM_CB	5

typedef int tdma_sched_cb(uint16_t p1, uint16_t p2);

/* A single item in a TDMA scheduler bucket */
struct tdma_sched_item {
	tdma_sched_cb *cb;
	uint16_t p1;
	uint16_t p2;
};

/* A bucket inside the TDMA scheduler */
struct tdma_sched_bucket {
	struct tdma_sched_item item[TDMASCHED_NUM_CB];
	uint8_t num_items;
};

/* The scheduler itself, consisting of buckets and a current index */
struct tdma_scheduler {
	struct tdma_sched_bucket bucket[TDMASCHED_NUM_FRAMES];
	uint8_t cur_bucket;
};

/* Schedule an item at 'frame_offset' TDMA frames in the future */
int tdma_schedule(uint8_t frame_offset, tdma_sched_cb *cb, uint16_t p1, uint16_t p2);

/* Schedule a set of items starting from 'frame_offset' TDMA frames in the future */
int tdma_schedule_set(uint8_t frame_offset, const struct tdma_sched_item *item_set);

/* Execute pre-scheduled events for current frame */
int tdma_sched_execute(void);

/* Advance TDMA scheduler to the next bucket */
void tdma_sched_advance(void);

/* reset the scheduler; erase all scheduled items */
void tdma_sched_reset(void);

/* debug function: print number of entries of all TDMA buckets */
void tdma_sched_dump(void);


extern void tdma_end_set(uint16_t p1, uint16_t p2);
#define SCHED_ITEM(x, y, z)		{ .cb = x, .p1 = y, .p2 = z }
#define SCHED_END_FRAME()		{ .cb = NULL, .p1 = 0, .p2 = 0 }
#define SCHED_END_SET()			{ .cb = &tdma_end_set, .p1 = 0, .p2 = 0 }

#endif /* _L1_TDMA_SCHED_H */
