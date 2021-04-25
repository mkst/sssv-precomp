#include <ultra64.h>

void load_default_display_list(Gfx**);
void set_menu_text_color(u8 r, u8 g, u8 b, u8 a);
void select_comic_sans_font(void);
void func_801308B4(u8 *src, s16 *dst);
void func_8012C978(Gfx **, s16*, u16, u16, f32, f32);

extern Gfx *D_801D9E7C;
extern s16 gScreenWidth;
extern s16 gScreenHeight;

void helo_wrld()
{
    char text[128];
    s16  textw[64];

    // rewind gSPEndDisplayList
    D_801D9E7C--;
    // rewind gDPFullSync
    D_801D9E7C--;

    sprintf(text, "helo wrld");
    func_801308B4((u8*)text, textw);

    load_default_display_list(&D_801D9E7C);
    set_menu_text_color(255, 0, 183, 255);
    select_comic_sans_font();
    func_8012C978(&D_801D9E7C, textw, gScreenWidth/2, gScreenHeight-60, 16.0f, 16.0f);

    // add overwritten display list commands back in
    gDPFullSync(D_801D9E7C++);
    gSPEndDisplayList(D_801D9E7C++);
}
