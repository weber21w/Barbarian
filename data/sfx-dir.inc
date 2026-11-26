
#if BARB_SFA_VIDEO && defined(__AVR__)
/* Keep this immutable table after the mixer's aligned program-memory planes. */
const uint16_t sfx_dir[] __attribute__((aligned(2),section(".text.barbSfxDir"))) = {
#elif BARB_SFA_VIDEO
const uint16_t sfx_dir[] PROGMEM = {
#else
const uint32_t sfx_dir[] PROGMEM = {
#endif
4243,4243,		//../resources/sfx/bell.raw
6119,1876,		//../resources/sfx/block1.raw
7509,1390,		//../resources/sfx/block2.raw
9150,1641,		//../resources/sfx/block3.raw
13684,4534,		//../resources/sfx/deathfall.raw
14178,494,		//../resources/sfx/decapitated.raw
15268,1090,		//../resources/sfx/fall.raw
16221,953,		//../resources/sfx/fire.raw
17116,895,		//../resources/sfx/footcut.raw
17636,520,		//../resources/sfx/guard.raw
18644,1008,		//../resources/sfx/head1.raw
18851,207,		//../resources/sfx/head2.raw
20003,1152,		//../resources/sfx/headhit.raw
22685,2682,		//../resources/sfx/hit1.raw
24195,1510,		//../resources/sfx/hit2.raw
26450,2255,		//../resources/sfx/hit3.raw
37749,11299,		//../resources/sfx/laugh.raw
38360,611,		//../resources/sfx/miss.raw
39856,1496,		//../resources/sfx/neckswing.raw
40616,760,		//../resources/sfx/pain1.raw
41319,703,		//../resources/sfx/pain2.raw
47888,6569,		//../resources/sfx/prepare.raw
49139,1251,		//../resources/sfx/roll.raw
50609,1470,		//../resources/sfx/wait.raw
};
