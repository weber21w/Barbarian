/* Place the empty hooks in the existing pre-mixer alignment gap. */
#define VIDEO_EMPTY __attribute__((aligned(2),section(".progmem.data.VideoEmpty")))
void VIDEO_EMPTY InitializeVideoMode(void) {}
void DisplayLogo(void) __attribute__((alias("InitializeVideoMode")));
void ClearVram(void) {}
void VideoModeVsync(void) __attribute__((alias("InitializeVideoMode")));
