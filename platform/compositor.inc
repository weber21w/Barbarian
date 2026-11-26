/* SFA2 fifo_display's back-buffer history and fused damaged-row composition,
 * adapted to Barbarian's transparent 2bpp spans at native display resolution.
 * Old/new damage is merged before reading BG; all intersecting actors are
 * blended in original draw order, then the span is written just once. */
#define COMPOSE_ACTORS 4
#include "platform/hud.h"
static BarbSprite composeHistory[2][COMPOSE_ACTORS];
static u8 composeCount[2];
static u8 poseCacheB[BARB_SPRITE_CACHE_BYTES];
static u8 poseCacheLarge[BARB_EXTRA_SPRITE_CACHE_BYTES],poseCacheSmall[BARB_SMALL_SPRITE_CACHE_BYTES];
static u8 smallSwap;
static u8 endingWanted,composeEnding[2];
/* Shared with the assembly HUD scheduler and painter. */
u8 barbHudShown=255,barbHudPhase,barbHudDeferred;
static u8 cachedPose[COMPOSE_ACTORS]={255,255,255,255};
static u8 cachedFlags[COMPOSE_ACTORS];
#ifdef __AVR__
/* Resolve old fighter bound banks once per composition, in existing padding. */
static const u8 *composeOldBounds[2];
#endif
/* The AVR blitter ORs its five-bit entry offset into this aligned base. */
static u16 composePairs[COMPOSE_ACTORS][16] __attribute__((aligned(32)));
#ifdef __AVR__
/* Two-pixel outward margins, 0xff empty; one byte per row/fighter/buffer. */
static u8 composeRowBounds[2][2][80],composeBoundsValid[2];
/* BarbBlendPairsRaw has a private assembly ABI and is called by ComposeSpan. */
extern void BarbBuildPairs(u16 *table,const u8 *palette,u8 flags);
#else
static void BarbBuildPairs(u16 *table,const u8 *palette,u8 flags){
    for(u8 c=0;c<16;c++){
        u8 a=c>>2,b=c&3;
        if(flags&BARB_SPRITE_FLIP_X){u8 t=a;a=b;b=t;}
        table[c]=((u16)((a?0:240)|(b?0:15))<<8)|(palette[a]<<4)|palette[b];
    }
}
static void BarbBlendPairs(u8 *dest,const u8 *source,const u16 *table,u16 config){
    u8 first=config,count=(config>>8)&127;s8 step=config&32768?-2:2;
    while(count--){
        u8 phase=first&3,codes=source[first>>2];
        if(phase==3)codes=((codes&3)<<2)|(source[(first>>2)+1]>>6);
        else codes=(codes>>(4-phase*2))&15;
        u16 blend=table[codes];*dest=(*dest&(blend>>8))|(u8)blend;
        dest++;first+=step;
    }
}
#endif
static u8 *PoseData(u8 i){return i<2?(i?poseCacheB:workBuf):((i==3)^smallSwap?poseCacheSmall:poseCacheLarge);}
void InvalidateComposition(void){
    walkValid=0;
    composeCount[0]=composeCount[1]=0;memset(cachedPose,255,sizeof cachedPose);
#ifdef __AVR__
    composeBoundsValid[0]=composeBoundsValid[1]=0;
#endif
    composeEnding[0]=composeEnding[1]=0;
    barbHudShown=255;
    bloodVictim=bloodNext=0;bloodFrame=255;
    bloodVersion=bloodDrawn[0]=bloodDrawn[1]=0;
    bloodDrawnFrame[0]=bloodDrawnFrame[1]=255;bloodTrailColumn=255;
    bloodEffect=0;
    memset(bloodBounds,0,sizeof bloodBounds);
}
#ifndef __AVR__
static void CachePose(u8 i,u8 f){
    if(cachedPose[i]==f)return;
    u16 off=0;
    for(u8 j=0;j<f;j++)off+=pgm_read_word(&barbSpriteBytes[j])&511;
    u16 n=pgm_read_word(&barbSpriteBytes[f])&511;
    u8 *p=PoseData(i);
    u32 address=BARB_SPIVRAM_SPRITES+off;
#if BARB_SPR_BANK0_BYTES < BARB_SPR_DATA_SIZE
    if(off>=BARB_SPR_BANK0_BYTES)address=BARB_SPR_BANK1_BASE+off-BARB_SPR_BANK0_BYTES;
#endif
    while(n){u8 part=n>80?80:n;BarbarianSpiReadBytes(address,p,part);p+=part;address+=part;n-=part;}
    cachedPose[i]=f;
}
#endif
/* AVR byte 4 holds source offset at left plus facing; the C oracle keeps
 * the original sourceLeft and performs the coordinate transform itself. */
typedef struct{u8 left,right;const u8 *data;u8 sourceLeft;} ComposeRow;
#include "platform/blade_row.inc"
#ifdef __AVR__
typedef char ComposeRowLayout[sizeof(ComposeRow)==5?1:-1];
extern void ComposeSpan(u8 y,u8 start,u8 end,ComposeRow *rows);
extern void BarbComposeSpans(u8 y,u8 count,u8 *left,u8 *right,ComposeRow *rows);
extern void BarbPrepareRows(u8 y,ComposeRow *rows,u8 *left,u8 *right,u16 *cursor,const u8 *dirty,u8 count,u8 buffer);
extern u16 BarbRowRange(u8 buffer);
extern u8 BarbComposeActors(u8 buffer,u8 *dirty,u8 count);
extern void BarbRememberBounds(u8 buffer);
extern u8 BarbRetainFront(void);
asm(".global barbAsmScene\n.set barbAsmScene,barbScene\n"
    ".global barbAsmLineBuf\n.set barbAsmLineBuf,barbLineBuf\n"
    ".global barbAsmBack\n.set barbAsmBack,back\n"
    ".global barbAsmComposePairs\n.set barbAsmComposePairs,composePairs\n"
    ".global barbAsmSpriteWidths\n.set barbAsmSpriteWidths,sprite_widths\n"
    ".global barbAsmSpriteHeights\n.set barbAsmSpriteHeights,sprite_heights\n"
    ".global barbAsmSpriteYBounds\n.set barbAsmSpriteYBounds,barbSpriteYBounds\n"
    ".global barbAsmPoseB\n.set barbAsmPoseB,poseCacheB\n"
    ".global barbAsmPoseLarge\n.set barbAsmPoseLarge,poseCacheLarge\n"
    ".global barbAsmPoseSmall\n.set barbAsmPoseSmall,poseCacheSmall\n"
    ".global barbAsmSmallSwap\n.set barbAsmSmallSwap,smallSwap\n"
    ".global barbAsmCachedPose\n.set barbAsmCachedPose,cachedPose\n"
    ".global barbAsmCachedFlags\n.set barbAsmCachedFlags,cachedFlags\n"
    ".global barbAsmSpriteBytes\n.set barbAsmSpriteBytes,barbSpriteBytes\n"
    ".global barbAsmSpritePalettes\n.set barbAsmSpritePalettes,barbSpritePalettes\n"
    ".global barbAsmOldBounds\n.set barbAsmOldBounds,composeOldBounds\n"
    ".global barbAsmRowBounds\n.set barbAsmRowBounds,composeRowBounds\n"
    ".global barbAsmBoundsValid\n.set barbAsmBoundsValid,composeBoundsValid\n"
    ".global barbAsmComposeCamera\n.set barbAsmComposeCamera,composeCamera\n"
    ".global barbAsmComposeEnding\n.set barbAsmComposeEnding,composeEnding\n"
    ".global barbAsmEndingWanted\n.set barbAsmEndingWanted,endingWanted\n"
    ".global barbAsmBloodDrawn\n.set barbAsmBloodDrawn,bloodDrawn\n"
    ".global barbAsmBloodVersion\n.set barbAsmBloodVersion,bloodVersion\n"
    ".global barbAsmHistory\n.set barbAsmHistory,composeHistory\n"
    ".global barbAsmCount\n.set barbAsmCount,composeCount\n");
asm(".global barbAsmBloodVictim\n.set barbAsmBloodVictim,bloodVictim\n"
    ".global barbAsmBloodDrawnFrame\n.set barbAsmBloodDrawnFrame,bloodDrawnFrame\n");
#else
static void ComposeSpan(u8 y,u8 start,u8 end,ComposeRow *rows){
    u8 n=end-start;
    u8 clipLeft=start*2,clipRight=end*2;
    u8 scene=barbScene-1;
    if((clipLeft<39||clipRight>176)&&scene<5&&(u8)(y-32)<80){
        u8 edge;
        BarbGraphicsRead((u16)(BARB_RUNTIME_BASE+BARB_FOREGROUND_OFFSET+
            (u16)scene*80+y-32),&edge,1);
        u8 left=(edge>>4)+24;
        u8 right=(edge&15)+176;
        if(left>clipLeft)clipLeft=left;
        if(right<clipRight)clipRight=right;
    }
    BackgroundRead(y,start,n);
    for(u8 i=0;i<nextSprite;i++){
        ComposeRow *r=&rows[i];
        u8 lo=r->left>clipLeft?r->left:clipLeft;
        u8 hi=r->right<clipRight?r->right:clipRight;
        if(lo>=hi)continue;
        BarbSprite *a=&spr[i];
        u8 sx=lo-a->x;
        u8 flip=a->p&BARB_SPRITE_FLIP_X;
        if(flip)sx=pgm_read_byte(&sprite_widths[a->f])-1-sx;
        sx-=r->sourceLeft;
        while(lo<hi){
            u8 *dest=&barbLineBuf[(lo>>1)-start];
            if((lo&1)||lo+1==hi){
                u8 code=(r->data[(u8)sx>>2]>>(6-(((u8)sx&3)*2)))&3;
                if(code){
                    u8 color=composePairs[i][code*5]&15;
                    if(lo&1)*dest=(*dest&240)|color;
                    else *dest=(*dest&15)|(color<<4);
                }
                sx+=flip?-1:1;lo++;
            }else{
                u8 count=(hi-lo)>>1;
                u16 config=((u16)(count|(flip?128:0))<<8)|(flip?sx-1:sx);
                BarbBlendPairs(dest,r->data,composePairs[i],config);
                u8 pixels=count*2;sx+=flip?-pixels:pixels;lo+=pixels;
            }
        }
        BladeRow(i,y,start,clipLeft,clipRight);
    }
    BarbGraphicsWrite(back+(u16)y*BARB_FB_ROW_BYTES+start,barbLineBuf,n);
#ifdef BARB_DEBUG_COMPOSE_ROWS
    barbComposeRows++;
#endif
}
#endif
#ifdef __AVR__
extern u8 Compose(void);
#else
static __attribute__((noinline)) u8 Compose(void){
    u8 buffer=back?1:0,dirty[COMPOSE_ACTORS],changed=0;
    /* AVR cursors retain header addresses; the C oracle retains offsets. */
    u16 cursor[COMPOSE_ACTORS]={0};
    if(nextSprite>COMPOSE_ACTORS)nextSprite=COMPOSE_ACTORS;
    /* The current front already represents this scene. Leave the other
     * buffer stale until a visible change needs it; its history remains valid. */
    u8 scroll=endingWanted!=composeEnding[buffer];
#ifdef __AVR__
    if(BarbRetainFront())return 0;
#else
    u8 shown=front?1:0;
    if(composeCamera[shown]==camera&&bloodDrawn[shown]==bloodVersion&&endingWanted==composeEnding[shown]&&nextSprite==composeCount[shown]&&
       !memcmp(spr,composeHistory[shown],nextSprite*sizeof(BarbSprite))){
        barbRetained++;return 0;
    }
#endif
    u8 swap=nextSprite>3&&(pgm_read_word(&barbSpriteBytes[spr[3].f])&511)>BARB_SMALL_SPRITE_CACHE_BYTES;
    if(swap!=smallSwap){smallSwap=swap;cachedPose[2]=cachedPose[3]=255;}
    u8 count=nextSprite>composeCount[buffer]?nextSprite:composeCount[buffer];
    u8 bloodDirty=bloodDrawn[buffer]!=bloodVersion;
    /* Blood has its own old/current bounds; changing masks does not dirty
     * the fighter's entire silhouette. */
#ifdef __AVR__
    changed=BarbComposeActors(buffer,dirty,count);
#else
    u8 palette[4];
    for(u8 i=0;i<count;i++){
        /* Bit 1 means that old rows need conservative canvas repair. Compute
         * pose/y equality once per frame, rather than for every source row. */
        dirty[i]=i>=nextSprite||i>=composeCount[buffer]||
            spr[i].f!=composeHistory[buffer][i].f||spr[i].y!=composeHistory[buffer][i].y?3:
            spr[i].x!=composeHistory[buffer][i].x||spr[i].p!=composeHistory[buffer][i].p;
        changed|=dirty[i];
        if(i>=nextSprite)continue;
        if(cachedPose[i]==spr[i].f&&cachedFlags[i]==(spr[i].p&129))continue;
        cachedFlags[i]=spr[i].p&129;
        CachePose(i,spr[i].f);palette[0]=0;
        u8 pal=pgm_read_word(&barbSpriteBytes[spr[i].f])>>9;
        for(u8 c=1;c<4;c++){
            u8 color=pgm_read_byte(&barbSpritePalettes[pal][c-1]);
            if(spr[i].p&BARB_SPRITE_PLAYER_B){if(color==1)color=6;if(color==2)color=9;}
            palette[c]=color;
        }
        BarbBuildPairs(composePairs[i],palette,spr[i].p);
    }
#endif
    composeCamera[buffer]=camera;
    if(!changed&&!bloodDirty&&!scroll)return 1;
    BladeBegin();
    u16 range=128U<<8;
    u8 fullBlood=bloodDirty&&(bloodVictim||bloodDrawnFrame[buffer]!=255);
#ifdef __AVR__
    if(!scroll)range=BarbRowRange(buffer);
#else
    if(!scroll){
        u8 first=128,last=0;
        for(u8 set=0;set<2;set++)for(u8 i=0;i<(set?composeCount[buffer]:nextSprite);i++){
            BarbSprite *a=set?&composeHistory[buffer][i]:&spr[i];
            u8 topRow=pgm_read_byte(&barbSpriteYBounds[a->f][0]);
            u8 bottomRow=pgm_read_byte(&barbSpriteYBounds[a->f][1]);
            if(a->p&2){if(topRow>32)topRow=32;if(bottomRow<60)bottomRow=60;}
            int32_t top=(int32_t)a->y+topRow,bottom=(int32_t)a->y+bottomRow;
            if(top<0)top=0;if(bottom>128)bottom=128;
            if(top<first)first=top;if(bottom>last)last=bottom;
        }
        range=((u16)last<<8)|first;
    }
#endif
    if(!scroll&&fullBlood){
        u8 first=range,last=range>>8;
        if(first>24)first=24;if(last<118)last=118;
        range=first|((u16)last<<8);
    }
    u8 last=range>>8;
    for(u8 y=range;y<last;y++){
        ComposeRow rows[COMPOSE_ACTORS];
        u8 left[COMPOSE_ACTORS+1],right[COMPOSE_ACTORS+1];
#ifdef __AVR__
        BarbPrepareRows(y,rows,left,right,cursor,dirty,count,buffer);
#else
        for(u8 i=0;i<count;i++){
            rows[i].left=rows[i].right=0;left[i]=BARB_FB_ROW_BYTES;right[i]=0;
            if(i<nextSprite){
                BarbSprite *a=&spr[i];
                s16 sy=(s16)y-a->y;
                if(sy>=0&&sy<pgm_read_byte(&sprite_heights[a->f])){
                    /* First visit may skip clipped or transparent top rows. */
                    u8 *p=PoseData(i);
                    if(!cursor[i])for(s16 j=0;j<sy;j++){
                        u8 l=p[cursor[i]++],r=p[cursor[i]++];
                        if(l!=255)cursor[i]+=(r-l+3)>>2;
                    }
                    u8 l=p[cursor[i]++],r=p[cursor[i]++];
                    rows[i].sourceLeft=l;rows[i].data=p+cursor[i];
                    if(l!=255){
                        cursor[i]+=(r-l+3)>>2;
                        s16 lo,hi;u8 w=pgm_read_byte(&sprite_widths[a->f]);
                        if(a->p&BARB_SPRITE_FLIP_X){lo=a->x+w-r;hi=a->x+w-l;}
                        else{lo=a->x+l;hi=a->x+r;}
                        lo=ScaleX(lo);hi=ScaleX(hi);
                        if(lo<0)lo=0;if(hi>214)hi=214;
                        if(lo<hi){rows[i].left=lo;rows[i].right=hi;
                            if(dirty[i]){left[i]=lo>>1;right[i]=(hi+1)>>1;}}
                    }
                }
            }
            if(dirty[i]&&i<composeCount[buffer]){
                BarbSprite *a=&composeHistory[buffer][i];
                s16 sy=(s16)y-a->y;
                u8 top=pgm_read_byte(&barbSpriteYBounds[a->f][0]);
                u8 bottom=pgm_read_byte(&barbSpriteYBounds[a->f][1]);
                if(sy>=top&&sy<bottom){
                    s16 lo,hi;u8 w=pgm_read_byte(&sprite_widths[a->f]);
                    if(i<nextSprite&&a->f==spr[i].f&&a->y==spr[i].y){
                        const u8 *header=rows[i].data-2;u8 l=header[0],r=header[1];
                        if(l==255)continue;
                        if(a->p&BARB_SPRITE_FLIP_X){lo=a->x+w-r;hi=a->x+w-l;}
                        else{lo=a->x+l;hi=a->x+r;}
                        lo=ScaleX(lo);hi=ScaleX(hi);
                    }else{
                    lo=ScaleX(a->x);hi=ScaleX(a->x+w);
                    }
                    if(lo<0)lo=0;if(hi>214)hi=214;
                    if(lo<hi){lo>>=1;hi=(hi+1)>>1;
                        if(lo<left[i])left[i]=lo;if(hi>right[i])right[i]=hi;}
                }
            }
        }
#endif
        /* The opaque HUD replaces these rows, including clipped actors. */
        if(!endingWanted&&y<BARB_HUD_ROWS)continue;
        /* Disjoint spans total at most one world row. Batch four rows between
         * queue checks; every transfer still retains its SPI raster guard. */
        if(!(y&3))AudioRefill();
        /* As in SFA2 Row(): merge nearby spans to save an SPI read/write pair. */
        if(scroll||(fullBlood&&y>=24&&y<118)){ComposeSpan(y,0,BARB_FB_ROW_BYTES,rows);continue;}
        u8 spans=count;
        if(bloodDirty&&!fullBlood){u16 b=BarbBloodRow(y,buffer);left[count]=b;right[count]=b>>8;spans++;}
        if(!spans)continue;
#ifdef __AVR__
        BarbComposeSpans(y,spans,left,right,rows);
#else
        for(u8 i=0;i<spans;i++)for(u8 j=i+1;j<spans;j++)if(left[j]<left[i]){
            u8 t=left[i];left[i]=left[j];left[j]=t;t=right[i];right[i]=right[j];right[j]=t;
        }
        u8 lo=left[0],hi=right[0];
        for(u8 i=1;i<spans;i++){
            if(left[i]>=right[i])continue;
            /* hi <= 107: adding this six-byte gap fits in one byte. */
            if(left[i]<=(u8)(hi+6)){if(right[i]>hi)hi=right[i];}
            else{if(lo<hi)ComposeSpan(y,lo,hi,rows);lo=left[i];hi=right[i];}
        }
        if(lo<hi)ComposeSpan(y,lo,hi,rows);
#endif
    }
#ifdef __AVR__
    /* Source bounds survive translation and palette/facing changes. */
    if(changed&2)BarbRememberBounds(buffer);
#endif
    memcpy(composeHistory[buffer],spr,nextSprite*sizeof(BarbSprite));
    composeCount[buffer]=nextSprite;
    memcpy(bloodBounds[buffer],bloodCommand,4);
    return 1;
}
#endif
