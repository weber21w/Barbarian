/* Compact straight-line animation timelines. Complex combat stays in C.
 * Each row: byte deadline (255 escapes to a little-endian word), action count
 * (bit 7: >=), then
 * action IDs into a shared flash pool (255 introduces an inline three-byte
 * action: destination, source, signed addend). Zero count ends the timeline.
 * Assignments and sounds execute in source order, even after GOTOU is set.
 * Only the next condition is gated by GOTOU, matching the original C.
 */
enum {
    BT_BODY_X = 245, BT_BODY_Y, BT_CLASH, BT_GRUNT, BT_REF_B, BT_REF_A,
    BT_TIME, BT_ZERO, BT_SOUND, BT_GOTO
};
#include "barbarian_logic_context.h"
typedef char BarbTimelineOffsetsFit[(sizeof(BarbarianState) < BT_BODY_X) ? 1 : -1];

#include "data/animation-timelines.inc"

#if BARB_SPI_TIMELINES
static u8 barbTimelineCacheA[BARB_TIMELINE_CACHE_A],barbTimelineCacheB[BARB_TIMELINE_CACHE_B];
static u16 barbTimelineCachedOffset[2]={65535,65535};
static u8 barbTimelineAction[3];
static u8 barbTimelineActionCode=255;
#define TimelineByte(p) (*(const u8 *)(p))
#define TimelineWord(p) ((u16)(p)[0]|((u16)(p)[1]<<8))
#else
#define TimelineByte(p) pgm_read_byte(p)
#define TimelineWord(p) pgm_read_word(p)
#endif

static s16 BarbarianTimelineRead(BarbarianState *g, BarbarianLogicContext *ctx, u8 source){
    switch(source){
        case BT_GOTO: return ctx->jump;
        case BT_TIME: return ctx->time;
        case BT_REF_A: return ctx->refA;
        case BT_REF_B: return ctx->refB;
        case BT_ZERO: return 0;
        default: return *(s8 *)((u8 *)g + source);
    }
}

static BARB_NOINLINE void BarbarianTimeline(BarbarianState *g, BarbarianLogicContext *ctx,
        const u8 *row, u8 useRefB){
    if(ctx->jump)return;
#if BARB_SPI_TIMELINES
    u16 key=(uintptr_t)row,offset=key&4095;
    u8 player=offset>=BARB_TIMELINE_B_START;
    u8 *cache=player?barbTimelineCacheB:barbTimelineCacheA;
    if(key!=barbTimelineCachedOffset[player]){
        u8 n=((key>>12)+1)*8,limit=player?BARB_TIMELINE_CACHE_B:BARB_TIMELINE_CACHE_A;
        if(n>limit)n=limit;
        BarbarianSpiReadBytes(BARB_RUNTIME_BASE+offset,cache,n);
        barbTimelineCachedOffset[player]=key;
    }
    row=cache;
#endif
    while(ctx->jump == 0){
        s16 deadline = TimelineByte(row++);
        if(deadline == 255){deadline = (s16)TimelineWord(row);row += 2;}
        u8 count = TimelineByte(row++);
        s16 reference = useRefB ? ctx->refB : ctx->refA;
        u8 inclusive = count & 128U;
        count &= 127U;
        if(!count) return;
        {
            u8 take = inclusive ? ctx->time >= reference + deadline : ctx->time > reference + deadline;
            while(count--){
                u8 code = TimelineByte(row++);
                const u8 *action;
                if(code == 255U){action = row; row += 3;}
#if !BARB_SPI_TIMELINES
                else action = barbTimelineActions[code];
#endif
                if(!take) continue;
#if BARB_SPI_TIMELINES
                /* The two most frequent actions are unconditional source
                 * GOTOs. Keep this tiny control-flow fast path in flash. */
                if(code<2){ctx->jump=2-code;continue;}
                if(code!=255){
                    if(code!=barbTimelineActionCode){
                        BarbarianSpiReadBytes(BARB_RUNTIME_BASE+
                            BARB_ACTIONS_OFFSET+(u16)code*3,barbTimelineAction,3);
                        barbTimelineActionCode=code;
                    }
                    action=barbTimelineAction;
                }
                u8 dest=TimelineByte(action),source=TimelineByte(action+1);
                s8 add=(s8)TimelineByte(action+2);
#else
                u8 dest = code==255?TimelineByte(action):pgm_read_byte(action);
                u8 source = code==255?TimelineByte(action+1):pgm_read_byte(action+1);
                s8 add = (s8)(code==255?TimelineByte(action+2):pgm_read_byte(action+2));
#endif
                if(dest == BT_BODY_X) BarbarianBodyX(g, source, add);
                else if(dest == BT_BODY_Y) BarbarianBodyY(g, source);
                else if(dest == BT_CLASH) BarbarianClashSound(g);
                else if(dest == BT_GRUNT) BarbarianGruntSound(g);
                else if(dest == BT_SOUND){
                    BarbarianMixPlayChannel(add, source, 0);
                }else{
                    s16 value = BarbarianTimelineRead(g, ctx, source) + add;
                    if(dest == BT_GOTO) ctx->jump = value;
                    else if(dest == BT_REF_A) ctx->refA = value;
                    else if(dest == BT_REF_B) ctx->refB = value;
                    else *(s8 *)((u8 *)g + dest) = value;
                }
            }
        }
    }
}
