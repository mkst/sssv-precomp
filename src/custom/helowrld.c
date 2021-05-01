#include <ultra64.h>
#include "common.h"

void walrus_fire_missile(Animal *);
void func_802E89AC_6FA05C(void);
void func_803769E0_788090(void);

static u8 selector = 0;
static u8 debounce = 0;

#define DEBOUNCE 4

void check_controller(void)
{
    if (debounce > 0) {
        debounce--;
        return;
    }

    if (gControllerInput->button & CONT_UP) {
        selector++;
        debounce = DEBOUNCE;
    } else if (gControllerInput->button & CONT_DOWN) {
        selector--;
        debounce = DEBOUNCE;
    }
    if (gControllerInput->button & L_TRIG) {

        switch (selector) {
        case 0:
            // spawn mine under animal 0, change '33' to spawn a different item
            spawn_temporary_object(D_801DDD8C[gCurrentAnimalIndex].unk0->xPos, D_801DDD8C[gCurrentAnimalIndex].unk0->zPos, D_801DDD8C[gCurrentAnimalIndex].unk0->yPos, 0, 0xB4, D_801DDD8C[gCurrentAnimalIndex].unk0, 50, 33);
            break;
        case 1:
            // is the argument the target?
            walrus_fire_missile(D_803D5530);
            break;
        case 2:
            rat_drop_mine();
            break;
        case 3:
            biker_hyena_fire_missile(D_803D5530);
            break;
        case 4:
            // spawn exit teleporter
            spawn_temporary_object(D_801DDD8C[gCurrentAnimalIndex].unk0->xPos, D_801DDD8C[gCurrentAnimalIndex].unk0->zPos, D_801DDD8C[gCurrentAnimalIndex].unk0->yPos, 0, 255, D_801DDD8C[gCurrentAnimalIndex].unk0, 50, 56);
            break;
        default:
            // bugel call!
            play_sound_effect(184, 0, 0x5000, 1.0f, 64);
            break;
        }
    }
}

void helo_wrld()
{
    char text[128];
    s16  textw[64];

    check_controller();

    // rewind gSPEndDisplayList
    D_801D9E7C--;
    // rewind gDPFullSync
    D_801D9E7C--;

    sprintf(text, "selector: %00d", selector);
    prepare_text((u8*)text, textw);

    load_default_display_list(&D_801D9E7C);
    set_menu_text_color(255, 0, 183, 255);
    select_comic_sans_font();
    display_text(&D_801D9E7C, textw, gScreenWidth/2, gScreenHeight-40, 12.0f, 12.0f);

    // add overwritten display list commands back in
    gDPFullSync(D_801D9E7C++);
    gSPEndDisplayList(D_801D9E7C++);
}
