/* Row-ordered blade pixels use fixed steel (7) or their blood threshold.
 * Row/point combinations keep both decoding time and the pose bank small. */
typedef struct{const u8 *data;u8 count,row,x;} BladeCursor;
#ifdef __AVR__
/* Pose 0 occupies at most 502 bytes. The final ten bytes of its 512-byte
 * cache are free during composition; disk loading invalidates composition. */
typedef char BladeCursorFits[(BARB_WORKBUF_BYTES-BARB_SPRITE_CACHE_BYTES>=2*sizeof(BladeCursor))?1:-1];
#define bladeCursor ((BladeCursor *)(workBuf+BARB_SPRITE_CACHE_BYTES))
#else
static BladeCursor bladeCursor[2];
#endif
#ifdef __AVR__
extern void BladeBegin(void);
#else
static void BladeBegin(void){
    for(u8 i=0;i<nextSprite&&i<2;i++){
        u8 *p=PoseData(i)+(pgm_read_word(&barbSpriteBytes[spr[i].f])&511)-1;
        bladeCursor[i]=(BladeCursor){p-*p-2,*p,p[-2],0};
    }
}
#endif
#ifndef __AVR__
static void BladeRow(u8 i,u8 y,u8 start,u8 left,u8 right){
    if(i>1)return;
    BarbSprite *a=&spr[i];u8 level=(a->p>>2)&3;
    BladeCursor *c=&bladeCursor[i];
    const u8 *p=c->data;u8 count=c->count,row=c->row,sourceX=c->x;
    u8 sy=y-a->y;
    while(count--){
        u8 code=*p++,x=code&63;
        if(x==63)row++;
        else if(x==62){row=*p++;count--;}
        else{
            if(x>=48){row++;sourceX+=x-49;}else sourceX=x;
        }
        if(row>sy)break;
        if(row<sy){*c=(BladeCursor){p,count,row,sourceX};continue;}
        if(x>=62)continue;
        u8 grade=code>>6,color=grade&&level>=grade?3:7;
        /* Use signed world coordinates, including partially clipped blades. */
        s16 screenX=sourceX;
        if(a->p&BARB_SPRITE_FLIP_X)screenX=pgm_read_byte(&sprite_widths[a->f])-1-screenX;
        screenX+=a->x;
        if(screenX<left||screenX>=right)continue;
        u8 *dest=barbLineBuf+(screenX>>1)-start;
        if(screenX&1)*dest=(*dest&240)|color;else *dest=(*dest&15)|(color<<4);
    }
}

#endif
