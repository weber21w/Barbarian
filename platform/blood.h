#pragma once
/* Final corpse neck in the 27-pixel anchor used by wide mirrored poses.
 * Sprite reflection maps local x to 27-x, independent of canvas width. */
#define BARB_DRAG_NECK_X(x,flip) ((x)+((flip)?-9:36))
