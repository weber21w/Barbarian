#ifndef BARBARIAN_LOGIC_CONTEXT_H
#define BARBARIAN_LOGIC_CONTEXT_H
/* One ordered pass owns these locals. Passing one address keeps timeline
 * calls small without changing when values are committed to game state. */
typedef struct {
    s16 time, refA, refB;
    s8 jump;
} BarbarianLogicContext;
#endif
