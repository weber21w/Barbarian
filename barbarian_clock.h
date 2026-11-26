#ifndef BARBARIAN_CLOCK_H
#define BARBARIAN_CLOCK_H
#include <stdint.h>

/* The QB64 Amiga remake ends its gameplay loop with _LIMIT 50.
 * Advance five simulation steps per six 60 Hz video fields. The SFA platform
 * calls Poll once for each elapsed field, including bounded render catch-up.
 * Retain short pad presses across the iteration that does not simulate.
 * Scene-load catch-up is bounded by the platform scheduler.
 */
typedef struct {
    uint8_t phase;
    uint16_t pending1, pending2;
} BarbarianClock;

static inline uint8_t BarbarianClockPoll(BarbarianClock *clock,
        uint16_t *pad1, uint16_t *pad2){
    clock->pending1 |= *pad1;
    clock->pending2 |= *pad2;
    clock->phase += 5;
    if(clock->phase < 6) return 0;
    clock->phase -= 6;
    *pad1 = clock->pending1;
    *pad2 = clock->pending2;
    clock->pending1 = clock->pending2 = 0;
    return 1;
}
#endif
