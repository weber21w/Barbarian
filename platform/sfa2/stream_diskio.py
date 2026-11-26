"""Project-local Petit FAT disk driver with raster-safe sector transactions."""
def generate(kernel,out):
 s=(kernel/'petitfatfs/diskio.c').read_text()
 s=s.replace('#include "pff.h"','#include <petitfatfs/pff.h>').replace('#include "diskio.h"','#include <petitfatfs/diskio.h>')
 s=s.replace('DRESULT disk_readp (','static DRESULT ReadRaw (')
 s=s.replace('BYTE rcv_spi (void)','static inline __attribute__((always_inline)) BYTE rcv_spi (void)')
 s='''#include <uzebox.h>
extern volatile u8 walk_ready,walk_busy,sync_phase,sync_pulse;
extern void AudioBusWait(u16 n),AudioRefill(void);
static u8 DiskClosing(void){return walk_ready&&sync_phase&&sync_pulse>=61&&sync_pulse<=197;}
'''+s
 s=s.replace('rc = rcv_spi();','rc = rcv_spi();\n if(DiskClosing()){rc=0xff;break;}')
 s+='''
DRESULT disk_readp(BYTE *buffer,DWORD lba,WORD offset,WORD count){
 for(u8 attempt=0;attempt<8;attempt++){
  /* The token cutoff stays at 197. Reserve through pulse 198 so a late
   * command falls into the same cutoff/retry path; data acceptance is unchanged. */
  AudioBusWait(buffer?768:2048);walk_busy=1;
  DRESULT result=ReadRaw(buffer,lba,offset,count);walk_busy=0;
  if(result==RES_OK){AudioRefill();return result;}
  AudioRefill();
 }
 return RES_ERROR;
}
'''
 (out/'diskio.c').write_text(s)
 # The game opens one fixed 8.3 file in the root directory. Retain Petit
 # FAT's directory and cluster traversal (including fragmented FAT32 files),
 # but omit its unused general path parser from this project-local build.
 s=(kernel/'petitfatfs/pff.c').read_text()
 s=s.replace('#include "pff.h"','#include <petitfatfs/pff.h>').replace('#include "diskio.h"','#include <petitfatfs/diskio.h>')
 s=s.replace('BYTE sp[12], dir[32];','BYTE sp[12]="SFA2    UZE", dir[32];',1)
 old='res = follow_path(&dj, dir, path);'
 assert old in s
 s=s.replace(old,'dj.sclust=0; res=dir_rewind(&dj); if(res==FR_OK)res=dir_find(&dj,dir);',1)
 (out/'pff.c').write_text(s)
