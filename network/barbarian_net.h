#ifndef BARBARIAN_NET_H
#define BARBARIAN_NET_H
#ifndef BARB_NET
#define BARB_NET 0
#endif
#if BARB_NET
#include "un_uart_avr.h"
#include "un_pad.h"
#include "un_cancel.h"
enum {BN_CHECK=7,BN_CHECK_DONE,BN_NEXT,BN_SETTLE,BN_SETTLED,BN_REPAIR,
      BN_REPLAY,BN_REPLAY_DONE,BN_REPLAY_RUN,BN_FENCE,BN_FENCE_DONE};
typedef struct {
    volatile u8 phase,sample;
    u8 count,error,seat,frame,resume,scene,loadedScene,chord,checkDue;
    u16 clock,last;
#if !BARB_SFA_VIDEO
    u8 setup[25];
#endif
} BarbNetwork;
static BarbNetwork barbNetwork;
static u8 barbNetSimulating,barbNetReplaying;
static void BarbarianNetworkCapture(void);
static void BarbarianNetworkPoll(void);
static u8 BarbarianNetworkFrame(BarbarianInput *input,BarbarianClock *clock);
static void BarbarianNetworkScene(u8 scene);
#endif
#endif
