/* CPU attack cycles are data, not repeated state-assignment ladders.
 * Each sequence stores count, restart lever, then the selected states.
 * Invalid counters deliberately leave the pass untouched.
 */
static BARB_NOINLINE void BarbarianAiSequence(BarbarianState *g, s8 *jump,
        s16 time, s16 *reference, const u8 *sequence){
    u8 count = pgm_read_byte(sequence);
    u8 index = (u8)g->infocoupB;
    if(*jump != 0) return;
    if(index < count){
        g->joueurB = pgm_read_byte(sequence + 2 + index);
        g->infocoupB++;
        *reference = time;
        g->Boccupe = 1;
        *jump = 24;
    }else if(index == count){
        g->levier2 = pgm_read_byte(sequence + 1);
        g->infocoupB = 0;
        *jump = 20;
    }
}
