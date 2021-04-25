OUTPUT_ARCH (mips)

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    _##name##SegmentSize = _##name##SegmentEnd - _##name##SegmentStart; \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentNoloadStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentNoloadEnd = ADDR(.name.noload) + SIZEOF(.name.noload);

#define PATCH(segment, symbol, offset) \
    .segment##symbol##offset##_patch symbol + offset : AT(_##segment##SegmentRomStart + symbol - _##segment##SegmentStart + offset)

#define CODE_PATCH(segment, symbol, offset) \
    .segment##symbol##offset##_patch symbol + offset : AT(segment##_ROM_START + symbol - segment##_VRAM + offset)

#define JAL_HOOK(segment, symbol, offset, helper, name) \
    .segment##symbol##offset##_patch symbol + offset : AT(segment##_ROM_START + symbol - segment##_VRAM + offset) \
    { \
        name = .; \
        BYTE(0x0C); \
        BYTE((helper >> 18) & 0xFF); \
        BYTE((helper >> 10) & 0xFF); \
        BYTE((helper >> 2)  & 0xFF); \
    }

#define J_HOOK(segment, symbol, offset, helper, name) \
    .segment##symbol##offset##_patch symbol + offset : AT(segment##_ROM_START + symbol - segment##_VRAM + offset) \
    { \
        name = .; \
        BYTE(0x08); \
        BYTE((helper >> 18) & 0xFF); \
        BYTE((helper >> 10) & 0xFF); \
        BYTE((helper >> 2)  & 0xFF); \
    }

#define ROM_PATCH(address) \
    .rom##address##_patch : AT(address)

SECTIONS
{
    __romPos = 0;

    BEGIN_SEG(baserom, 0)
    {
        BASEROM;
    }
    END_SEG(baserom)
    baseromEnd = __romPos;
    extRamStart = 0xFFFFFFFF80400000;
    . = extRamStart;
    BEGIN_SEG(custom, .)
    {
        BUILD_DIR/src/custom/helowrld.o(.text);
        BUILD_DIR/src/custom/helowrld.o(.*data*);
    }
    END_SEG(custom)

    /* overwrite osLeoDiskInit with our dma function */
    CODE_PATCH(main, osLeoDiskInit, 0x0)
    {
        BUILD_DIR/src/patches/dma_code.o(.text);
        BUILD_DIR/src/patches/dma_code.o(.*data*);
    }

    /* call 'osLeoDiskInit' at read_rom_header() */
    J_HOOK(main, read_rom_header, 0x24, osLeoDiskInit, leo_hook)

    /* call hello world at end of each display list */
    J_HOOK(main, func_8012A490, 0xF0, helo_wrld, helo_wrld_hook)

    /* Discard everything not specifically mentioned above. */
    /DISCARD/ :
    {
        *(*);
    }
}
