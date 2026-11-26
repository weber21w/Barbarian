static u8 BarbarianMapOriginalSprite(s8 f){
        switch(f){
                case 1: return SPR_IDLE1;
                case 2: return SPR_IDLE2;
                case 3: return SPR_IDLE3;
                case 4: return SPR_WALK1;
                case 5: return SPR_WALK2;
                case 6: return SPR_WALK3;
                case 7: return SPR_JUMP1;
                case 8: return SPR_JUMP2;
                case 9: return SPR_CROUCH1;
                case 10: return SPR_CROUCH2;
                case 11: return SPR_ROLL1;
                case 12: return SPR_ROLL2;
                case 13: return SPR_ROLL3;
                case 14: return SPR_ROLL2;
                case 15: return SPR_ROLL5;
                case 16: return SPR_ROLL1;
                case 17: return SPR_GUARD_HI;
                case 18: return SPR_GUARD_MID;
                case 19: return SPR_NECK2;
                case 20: return SPR_NECK3;
                case 21: return SPR_MIDDLE1;
                case 22: return SPR_MIDDLE2;
                case 23: return SPR_MIDDLE3;
                case 24: return SPR_KNEE1;
                case 25: return SPR_KNEE3;
                case 26: return SPR_WEB_OD1;
                case 27: return SPR_WEB_OD2;
                case 28: return SPR_WEB_OD3;
                case 29: return SPR_WEB_OD4;
                case 30: return SPR_KICK1;
                case 31: return SPR_KICK2;
                case 32: return SPR_HEADBUTT1;
                case 33: return SPR_HEADBUTT2;
                case 34: return SPR_FRONT1;
                case 35: return SPR_FRONT2;
                case 36: return SPR_FRONT3;
                case 37: return SPR_SPIN1;
                case 38: return SPR_SPIN2;
                case 39: return SPR_SPIN3;
                case 40: return SPR_VICTORY1;
                case 41: return SPR_VICTORY2;
                case 42: return SPR_VICTORY3;
                case 43: return SPR_TOUCHE2;
                case 44: return SPR_TOUCHE1;
                case 45: return SPR_FALL1;
                case 46: return SPR_FALL2;
                case 47: return SPR_FALL3;
                case 48: return SPR_DEATH1;
                case 49: return SPR_DEATH2;
                case 50: return SPR_DEATH3;
                case 51: return SPR_DECAP1;
                case 52: return SPR_DECAP2;
                case 53: return SPR_DECAP3;
                case 54: return SPR_DECAP4;
                case 55: return SPR_DRAX1;
                case 56: return SPR_DRAX2;
                case 57: return SPR_MARIANNA1;
                default: return SPR_STAND;
        }
}
/* Original 8-pixel game cells projected through the cropped stage view.
 * Full source canvases preserve foot placement; wide mirrored attacks extend
 * left from the normal 40-pixel canvas, as in the original PUTIMAGE endpoints. */
#ifndef __AVR__
static __attribute__((noinline)) s16 GridScreenX(s16 x){return (x-6)*16/3;}
#endif
#include "data/enemy-palettes.inc"
#include "blood.h"
#ifdef __AVR__
extern void DrawHealth(u8 life,u8 heavy);
#else
static void BarbPaintHud(u8 column,u8 n){
    for(u8 y=0;y<BARB_HUD_ROWS;y++){
        u16 a=(u16)y*BARB_FB_ROW_BYTES+column;
        BarbGraphicsWrite(a,barbLineBuf,n);
        BarbGraphicsWrite(a+BARB_FB_BYTES,barbLineBuf,n);
    }
}
/* Three-row steel band; update P1, rest, P2, rest, only when life changes.
 * Both fixed buffers receive each update, so swaps cannot resurrect old life. */
static void DrawHealth(u8 life,u8 heavy){
    u8 phase=barbHudPhase++&3,column=13,n=80;
    if(barbHudShown==255){
        barbHudShown=life;
        memset(barbLineBuf,0,80);
        memset(barbLineBuf+2,BARB_HUD_STEEL,24);
        memset(barbLineBuf+54,BARB_HUD_STEEL,24);
        memset(barbLineBuf+2,BARB_HUD_BLOOD,(life&15)*2);
        memset(barbLineBuf+78-(life>>4)*2,BARB_HUD_BLOOD,(life>>4)*2);
    }else{
        if(phase&1)return;
        life=phase?(life&240)|(barbHudShown&15):(life&15)|(barbHudShown&240);
        if(barbHudShown==life)return;
        /* Retry the same eligible gauge; at most two heavy deferrals. */
        u8 red=(phase?life>>4:life&15)*2;
        if(heavy&&red&&barbHudDeferred<2){
            barbHudDeferred++;barbHudPhase--;return;
        }
        u8 old=(phase?barbHudShown>>4:barbHudShown&15)*2;
        barbHudShown=life;
        u8 lo=red<old?red:old,hi=red>old?red:old;
        n=hi-lo;column=phase?91-hi:15+lo;
        memset(barbLineBuf,red>old?BARB_HUD_BLOOD:BARB_HUD_STEEL,n);
    }
    barbHudDeferred=0;
    AudioRefill();BarbPaintHud(column,n);
}
#endif
static BARB_NOINLINE void DrawStateHealth(const BarbarianState *g){
    u8 ending=BarbarianEndingMessage(g);
    /* Spend HUD work after fighter presentation; pending fields mark a busy
     * tick. DrawHealth retains unpainted life and bounds heavy retries. */
    if(ending)barbHudShown=255;
    else{
        u8 a=g->vieA,b=g->vieB;
        if((s8)a<0)a=0;if(a>12)a=12;
        if((s8)b<0)b=0;if(b>12)b=12;
        DrawHealth(a|(b<<4),GetVsyncCounter()!=0);
    }
}
void BarbarianRenderFrame(const BarbarianState *g){
    u16 colors=pgm_read_word(&barbEnemyPalettes[g->IA&7]);
    walk_palette[6]=colors;walk_palette[9]=colors>>8;
    u16 ticks=g->frame-walkFrame;walkFrame=g->frame;
    if(ticks>6||g->sens!=walkFacing){walkValid=0;walkFacing=g->sens;}
    /* Follow the pair, with a dead zone. Freeze after a death so the head,
     * pools and body cleanup do not cause a camera chase. Canonical positions,
     * collision bounds and network snapshots are never changed. */
    if(ticks&&ticks<=6&&!BarbarianEndingMessage(g)&&g->joueurA<78&&g->joueurB<78&&g->xA<39&&g->xB<39){
        s16 midpoint=(GridScreenX(g->xA)+GridScreenX(g->xB))/2+39;
        s16 delta=midpoint-(camera*2+80);
        if(delta>16&&camera<27)camera++;
        if(delta< -16&&camera>0)camera--;
    }
    ResetSprites();
    u8 poses[2]={g->spriteA,g->spriteB};
    for(u8 i=0;i<2;i++){
        u8 *held=i?&barbHeldPoseB:&barbHeldPoseA;
        if(poses[i]==58||poses[i]==59)poses[i]=*held;
        else *held=poses[i];
#if BARB_PUBLIC
        /* Defensive rendering for externally supplied/old debug snapshots. */
        if(poses[i]>=51&&poses[i]<=54)poses[i]=50;
#endif
        u8 f=BarbarianMapOriginalSprite(poses[i]);
        u8 flip=(g->sens==6)?i:!i;
        if(poses[i]==14||poses[i]==16)flip=!flip;
        s16 x=(s16)(i?g->xB:g->xA);
        /* Re-entry must not reuse a position last seen on the other side. */
        if(x>=39){walkPosition[i]=-16384;continue;}
        x=GridScreenX(x);
        s8 state=i?g->joueurB:g->joueurA;
        u8 dragging=g->gnome&&!g->gnometire&&(u8)(state-78)<4;
        if(dragging)walkGroup[i]=0;
        x=SmoothWalkX(i,x,dragging?4:state,ticks);
#if !BARB_PUBLIC
        if(dragging)BloodTrail(BARB_DRAG_NECK_X(x,flip));
#endif
        /* Mariana's PNG faces as supplied. Keep her centre at grid x - 7,
         * matching the former mirrored placement (local centre 33). */
        if(poses[i]==57){x-=40;flip=0;}
        else if(flip)x+=27-pgm_read_byte(&sprite_widths[f]);
        s16 y=((s16)(i?g->yFB:g->yFA)-11)*8;
        if(state>=78)motionY[i]=y*16;
        else y=SmoothAxis(&motionY[i],y,ticks,64);
        BlitSprite(x,y,f,(i&&poses[i]<55?BARB_SPRITE_PLAYER_B:0)|(flip?BARB_SPRITE_FLIP_X:0)|(
#if BARB_PUBLIC
        0
#else
        (i?((bloodEffect>>1)&2):(bloodEffect&2))|(bladeLevel[i]<<2)
#endif
        ));
    }
    u8 ending=BarbarianEndingMessage(g);
    if(!ending){
#if !BARB_PUBLIC
    u8 victimB=BarbarianIsDecapHead(g->joueurB);
    if((BarbarianIsDecapHead(g->joueurA)||victimB)&&g->xSPRt<320){
        u8 n=g->numerotete-1;if(n>=6)n=0;
        BlitSprite((g->xSPRt-40)*2/3,g->ySPRt-72,SPR_HEAD1+n,victimB?BARB_SPRITE_PLAYER_B:0);
    }
#endif
    if(g->gnome&&g->xGNOME<40){
        u8 n=g->numerognome-1;if(n>=4)n=3;
        BlitSprite(SmoothAxis(&gnomePosition,GridScreenX(g->xGNOME),ticks,7),80,SPR_GNOME1+n,0);
    }else gnomePosition=-16384;
    if(g->jeu==1&&g->sorcier&&g->xAttB>g->xB+5){
        u8 phase=255;
        if(g->xAttB==14||g->xAttB==23||g->xAttB==32)phase=0;
        if(g->xAttB==17||(g->xAttB==26&&g->xAttB<g->xB+20))phase=1;
        if(g->xAttB==20||g->xAttB==29)phase=2;
        if(phase<3)BlitSprite((g->xAttB-7)*16/3,g->yAttB*8-72,SPR_FIRE1+phase,0);
    }
    }
    walkValid=1;
    endingWanted=ending;
    bloodCommand[3]=0;
#if !BARB_PUBLIC
    if(bloodEffect&6)BloodEffects(g,2);
#endif
    if(Compose()){
#if !BARB_PUBLIC
        if(bloodVictim)BloodStamp(bloodFrame,0);
        BloodEffects(g,1);
#endif
        bloodDrawn[back?1:0]=bloodVersion;
        bloodDrawnFrame[back?1:0]=bloodFrame;
        if(ending==1){
            /* The custom scaled balcony figure exceeds the old Amiga mask. */
            memset(barbLineBuf,0,8);
            for(u8 y=22;y<50;y++)BarbarianSpiWriteBytes(back+(u16)y*BARB_FB_ROW_BYTES+61,barbLineBuf,8);
        }
        if(ending)DrawEndingText(ending);
        composeEnding[back?1:0]=ending;
        Present();
    }
    DrawStateHealth(g);
}
/* A no-simulation field retains the completed picture. Online repair can
 * change state without advancing its frame, so keep that path conservative.
 * Scene/pose invalidation, initial HUD and pause/ending repair force a draw. */
void BarbarianDisplayFrame(const BarbarianState *g){
    if(g->frame==walkFrame&&walkValid&&barbHudShown!=255
#if BARB_NET
       &&!barbNetwork.phase
#endif
    ){
        barbRetained++;
        DrawStateHealth(g);
    }else{
        BloodUpdate(g);
        BarbarianRenderFrame(g);
    }
    AudioRefill();
}
