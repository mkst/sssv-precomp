#include <ultra64.h>

void dma_read(u8 *devAddr, void *vAddr, s32 nbytes);

extern u8 _customSegmentRomStart[];
extern u8 _customSegmentRomEnd[];
extern u8 _customSegmentStart[];

void dma_code()
{
    dma_read(_customSegmentRomStart, _customSegmentStart, _customSegmentRomEnd - _customSegmentRomStart);
}
