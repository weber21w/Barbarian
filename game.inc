static void BarbarianRoundClockStep(BarbarianState *g){
    if((s16)((u16)g->temps-(u16)g->chrono)<7)return;
    g->chrono=g->temps;
    if(g->chronoPart)g->chronoPart--;
    else if(g->chronometre>0){g->chronometre--;g->chronoPart=9;}
}
static void BarbarianSetOriginalDefaults(BarbarianState *g){
    BarbarianLoadDefaults(g);
}

static u8 BarbarianPadFromJoy(u8 joy){
        /* SNES directional bits 4..7 map directly to game bits 0..3. */
        u8 p = (joy >> 4) & 15;
        if(joy & (BTN_B | BTN_Y))
                p |= BARB_PAD_ATTACK;
        if(joy & BTN_START)
                p |= BARB_PAD_START;
        if(joy & BTN_SELECT)
                p |= BARB_PAD_SELECT;

        return p;
}

/* Physical input produces only coordinates 6..8. Preserve the original
 * direction numbering, including player A's unusual mirrored diagonals. */
static const u8 barbPadDirections[2][9] PROGMEM={
    {1,8,3,4,5,6,7,2,9}, {3,8,1,4,5,6,9,2,7}
};
static u8 BarbarianPadCoordinates(u16 pad,s8 *x,s8 *y){
    *x=7+!!(pad&BARB_PAD_RIGHT)-!!(pad&BARB_PAD_LEFT);
    *y=7+!!(pad&BARB_PAD_DOWN)-!!(pad&BARB_PAD_UP);
    return (*y-6)*3+*x-6;
}
static BARB_NOINLINE void BarbarianPollPlayerA(BarbarianState *g,u16 pad){
    g->x1=g->y1=7;g->levier1=5;
    if(g->demo==0){
        u8 cell=BarbarianPadCoordinates(pad,&g->x1,&g->y1);
        g->attaqueA=!!(pad&BARB_PAD_ATTACK);
        if(g->sens==6||g->sens==9)
            g->levier1=pgm_read_byte(&barbPadDirections[g->sens==9][cell]);
    }
}
static BARB_NOINLINE void BarbarianPollPlayerB(BarbarianState *g,u16 pad){
    g->x2=g->y2=7;g->levier2=5;
    if(g->partie==2){
        u8 cell=BarbarianPadCoordinates(pad,&g->x2,&g->y2);
        g->attaqueB=!!(pad&BARB_PAD_ATTACK);
        if(g->sens==6||g->sens==9)
            g->levier2=cell+1;
    }
}

static u16 BarbarianRand(BarbarianState *g){
        u16 x = g->randSeed;

        if(x == 0U)
                x = 1U;
        x ^= (u16)(x << 7);
        x ^= (u16)(x >> 9);
        x ^= (u16)(x << 8);
        g->randSeed = x;
        return (u16)(x & 0x7FFFU);
}

#include "barbarian_movement.inc"
#include "barbarian_sound_sequences.inc"
#include "barbarian_scoring.inc"
#include "barbarian_boxes.inc"
#include "barbarian_contacts.inc"
#include "barbarian_jump.inc"
#include "barbarian_timeline.inc"
#include "barbarian_crouch.inc"
#include "barbarian_ai.inc"
#include "barbarian_ending.inc"

static BARB_NOINLINE void BarbarianLogicPreA(BarbarianState *g){
        BarbarianLogicContext ctx = {g->temps, g->reftemps, g->reftempsB, g->GOTOU};
#define GOTOU ctx.jump
#define temps ctx.time
#define reftemps ctx.refA
#define reftempsB ctx.refB
#include "barbarian_logic_macros_begin.inc"
#include "barbarian_logic_pre_a.inc"
#include "barbarian_logic_macros_end.inc"
        g->GOTOU = ctx.jump;
        g->temps = ctx.time;
        g->reftemps = ctx.refA;
        g->reftempsB = ctx.refB;
}

static BARB_NOINLINE void BarbarianLogicPostAPreB(BarbarianState *g){
        BarbarianLogicContext ctx = {g->temps, g->reftemps, g->reftempsB, g->GOTOU};
#define GOTOU ctx.jump
#define temps ctx.time
#define reftemps ctx.refA
#define reftempsB ctx.refB
#include "barbarian_logic_macros_begin.inc"
#include "barbarian_logic_post_a_pre_b.inc"
#include "barbarian_logic_macros_end.inc"
        g->GOTOU = ctx.jump;
        g->temps = ctx.time;
        g->reftemps = ctx.refA;
        g->reftempsB = ctx.refB;
}

static BARB_NOINLINE void BarbarianLogicPostB(BarbarianState *g){
        BarbarianLogicContext ctx = {g->temps, g->reftemps, g->reftempsB, g->GOTOU};
#define GOTOU ctx.jump
#define temps ctx.time
#define reftemps ctx.refA
#define reftempsB ctx.refB
#include "barbarian_logic_macros_begin.inc"
#include "barbarian_logic_post_b.inc"
#include "barbarian_logic_macros_end.inc"
        g->GOTOU = ctx.jump;
        g->temps = ctx.time;
        g->reftemps = ctx.refA;
        g->reftempsB = ctx.refB;
}

static BARB_NOINLINE void BarbarianRunOriginalLogic(BarbarianState *g, const BarbarianInput *in){
        for(;;){
        BarbarianLogicPreA(g);
        if(g->GOTOU==35){g->GOTOU=0;continue;}

        if(g->GOTOU == 22)
                g->GOTOU = 0;
        if(g->GOTOU == 0)
                BarbarianPollPlayerA(g, in->p1);

        BarbarianLogicPostAPreB(g);
        if(g->GOTOU==35){g->GOTOU=0;continue;}

        if(g->GOTOU == 21)
                g->GOTOU = 0;
        if(g->GOTOU == 0)
                BarbarianPollPlayerB(g, in->p2);

        BarbarianLogicPostB(g);
        break;
        }
}


static u8 BarbarianIsDeadOrDecap(s8 j){
        return (u8)(j == 78 || j == 79 || j == 80 || j == 81);
}

static u8 BarbarianIsDecapHead(s8 j){
        return (u8)(j == 80 || j == 81);
}

#include "barbarian_head.inc"

static BARB_NOINLINE void BarbarianPostDisplayLogic(BarbarianState *g){
        /* Original PASSAGE DU GNOME block. It sat immediately before display,
         * but mutates animation frame, xGNOME, reftemps, and GOTOU. */
        if(g->GOTOU == 12)
                g->GOTOU = 0;

        if(g->gnome == 1){
                g->numerognome = 4;
        /* Once this source block takes a GOTO, all later tests are gated
         * off. Keep that ordered behavior without rereading GOTOU each time. */
        if(g->GOTOU == 0){
            if(g->temps > g->reftemps + 6)g->numerognome=1;
            if(g->temps > g->reftemps + 12)g->numerognome=2;
            if(g->temps > g->reftemps + 18)g->numerognome=3;
            if(g->temps > g->reftemps + 24){
                g->reftemps=g->temps;g->GOTOU=31;
            }else if(g->temps > g->reftemps + 19){
                g->GOTOU=31;
            }else if(g->temps > g->reftemps + 18){
                g->xGNOME++;g->GOTOU=31;
            }else if(g->temps > g->reftemps + 7){
                g->GOTOU=31;
            }else if(g->temps > g->reftemps + 6){
                g->xGNOME++;g->GOTOU=31;
            }
        }
        }

        /* affichage label */
        if(g->GOTOU == 31)
                g->GOTOU = 0;

        if(g->GOTOU == 0 && g->transition == 1){
                if(g->temps > g->reftemps + 60){
                        g->transition = 0;
                        g->transitionfinie = 1;
                }
        }

        if(g->GOTOU == 0){
                if(BarbarianIsDeadOrDecap(g->joueurA) || BarbarianIsDeadOrDecap(g->joueurB))
                        g->GOTOU = 13;
        }

        if(g->GOTOU == 0){
                BarbarianRoundClockStep(g);
                if((g->chronometre == 0 && g->chronoPart == 0) || g->chronometre < 0 || g->chronometre > 45){
                        g->chronometre = 0;g->chronoPart = 0;
                        if(g->partie == 2){
                                g->sortieA = 1;
                                g->sortieB = 1;
                                g->tempsfini = 1;
                                g->Aoccupe = 0;
                                g->Boccupe = 0;
                        }
                }
        }

        /* affichetemps label */
        if(g->GOTOU == 13)
                g->GOTOU = 0;

        if(g->GOTOU == 0){
                if(g->scoreA > 99999)
                        g->scoreA = 0;
                if(g->scoreB > 99999)
                        g->scoreB = 0;
        }

        /* taches de sang labels: display-only skips that still affect flow. */
        if(g->GOTOU == 0 && g->sangA == 0)
                g->GOTOU = 15;
        if(g->GOTOU == 15)
                g->GOTOU = 0;
        if(g->GOTOU == 0 && g->sangB == 0)
                g->GOTOU = 11;
        if(g->GOTOU == 11)
                g->GOTOU = 0;

        /* The authoritative source selects serpent art from reaction poses.
         * It does not expire serpent flags on a separate SDL-style timer. */

        /* serpent2 label */
        if(g->GOTOU == 10)
                g->GOTOU = 0;

        /* tete2 label */
        if(g->GOTOU == 17)
                g->GOTOU = 0;

        /* Head/gnome football side effects. */
        if(g->GOTOU == 0 && g->gnome == 1){
                if(g->GOTOU == 0){
                        if(g->joueurA == 78 || g->joueurB == 78 || g->joueurA == 79 || g->joueurB == 79)
                                g->GOTOU = 14;
                }

                if(g->GOTOU == 0 && g->football == 1)
                        g->GOTOU = 18;

                if(g->GOTOU == 0 && g->xGNOME < 35 && ((s16)g->xGNOME * 8) > g->xSPRt - 16){
                        g->football = 1;
                        g->reftempsB = g->temps;
                }

                if(g->GOTOU == 0)
                        g->GOTOU = 19;

                /* gnometapetete label */
                if(g->GOTOU == 18)
                        g->GOTOU = 0;

                BarbarianHeadFootball(g);

                /* afficheteombre label */
                if(g->GOTOU == 19)
                        g->GOTOU = 0;

                /* Choosing/drawing the head is render-only here. The original then forced
                 * the flow into gnome2, so keep that label transition. */
                g->GOTOU = 14;
        }

        if(g->GOTOU == 0){
                if(BarbarianIsDecapHead(g->joueurA) || BarbarianIsDecapHead(g->joueurB)){
                        g->xSPRt = (g->xSPRt - 1) * 8;
                        g->ySPRt += 7;
                }
        }

        /* gnome2 label */
        if(g->GOTOU == 14)
                g->GOTOU = 0;

        if(g->GOTOU == 0 && g->gnome == 1){
                if(g->xGNOME > 39){
                        if(BarbarianIsDecapHead(g->joueurB) || g->joueurB == 78 || g->joueurB == 79){
                                if(g->partie == 2)
                                        g->bonusA = 1;
                                if(g->partie == 1){
                                        if(g->xB >= 39){
                                                g->sortieA = 1;
                                                g->Aoccupe = 0;
                                        }
                                }
                        }

                        if(BarbarianIsDecapHead(g->joueurA) || g->joueurA == 78 || g->joueurA == 79){
                                if(g->partie == 2)
                                        g->bonusB = 1;
                                if(g->partie == 1){
                                        if(g->xA >= 39){
                                                g->sortieB = 1;
                                                g->Boccupe = 0;
                                        }
                                }
                        }
                }

                /* cadavre tire par le gnome */
                if(g->joueurA == 80 || g->joueurA == 78){
                        if(g->xGNOME > g->xA + 5){
                                g->gnometire = 0;
                                g->xA++;
                        }
                }
                if(g->joueurA == 81 || g->joueurA == 79){
                        if(g->xGNOME > g->xA + 4){
                                g->gnometire = 0;
                                g->xA++;
                        }
                }

                if(g->joueurB == 80 || g->joueurB == 78){
                        if(g->xGNOME > g->xB + 4){
                                g->gnometire = 0;
                                g->xB++;
                        }
                }
                if(g->joueurB == 81 || g->joueurB == 79){
                        if(g->xGNOME > g->xB + 4){
                                g->gnometire = 0;
                                g->xB++;
                        }
                }
        }

        /* boulefeu label */
        if(g->GOTOU == 9)
                g->GOTOU = 0;

        if(g->arret == 1){
                if(g->jeu == 1 && g->temps > 30){
                        ((void)0);
                        g->scoreA = 0;
                        g->scoreB = 0;

                        if(g->GOTOU == 0 && g->demo == 1){
                                g->partie = 1;
                                g->decorS = 2;
                                g->IA = 0;
                                g->sorcier = 0;
                                g->marianna = 0;
                                g->demo = 0;
                                g->GOTOU = 8;
                        }
                        if(g->GOTOU == 0 && g->partie == 1){
                                g->partie = 2;
                                g->decorS = 2;
                                g->IA = 0;
                                g->sorcier = 0;
                                g->marianna = 0;
                                g->chronometre = 45;
        g->chronoPart = 0;
                                g->GOTOU = 8;
                        }
                        if(g->GOTOU == 0 && g->partie == 2){
                                g->partie = 1;
                                g->decorS = 2;
                                g->IA = 4;
                                g->sorcier = 0;
                                g->marianna = 0;
                                g->demo = 1;
                                g->GOTOU = 8;
                        }
                }

                if(g->jeu == 2 || g->jeu == 0){
                        g->scoreA = 0;
                        g->scoreB = 0;
                        g->partie = 1;
                        g->decorS = 2;
                        g->IA = 0;
                        g->sorcier = 0;
                        g->marianna = 0;
                        g->jeu = 1;
                        g->GOTOU = 36;
                }
        }
}


BARB_NOINLINE void BarbarianTick(BarbarianState *g, const BarbarianInput *in){
        g->frame++;

        /* Original permanent keyboard section. */
        g->version = 0;
        g->arret = 0;
        if(in->p1 & BARB_PAD_START)
                g->arret = 1;
        if(in->p1 & BARB_PAD_SELECT)
                g->version = 1;

        /* GOTOU 100: 30 seconds idle at the 50 Hz simulation rate. */
        if(g->GOTOU == 100){
                g->temps++;
                if(g->temps >= 1500 || g->arret){
                        g->temps = 0;
                        g->GOTOU = 8;
                }
        }

        /* Scene loading; the shared sprite bank was staged into SPI RAM at boot. */
        if(g->GOTOU == 8){
                LoadBG((u8)g->decorS);
                g->GOTOU = 16;
        }

        /* Source InitVariables: preserve assignment order and retained fields. */
        if(g->GOTOU == 16){
                g->GOTOU = 0;
                BarbarianResetRound(g);
        }

        BarbarianRunOriginalLogic(g, in);
        BarbarianPostDisplayLogic(g);
}
void BarbarianInit(BarbarianState *g){
        BarbarianSetOriginalDefaults(g);
        BarbarianMixHaltChannel(-1);
        nextSprite = 0;
}
void BarbarianReadInput(BarbarianInput *in){
        in->p1 = BarbarianPadFromJoy(ReadJoypad(0));
        in->p2 = BarbarianPadFromJoy(ReadJoypad(1));
}
