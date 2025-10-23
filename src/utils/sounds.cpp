#include "sounds.h"
#include <miniaudio.h>
#include <stdexcept>
#include <math.h>

using namespace std;


static ma_engine engine;

static ma_sound clickSound;
static ma_sound hoverSound;


void init_sound(){
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        throw runtime_error("Failed to init engine\n");
    }
    ma_sound_init_from_file(&engine, "assets/sound/click.wav", 0, NULL, NULL, &clickSound);
    ma_sound_init_from_file(&engine, "assets/sound/hover.wav", 0, NULL, NULL, &hoverSound);
}

void play_click_sound(){
    ma_sound_start(&clickSound);
}

void play_hover_sound(){
    ma_sound_start(&hoverSound);
}

void set_click_sound(float master_volume, float ui_volume){
    ma_sound_set_volume(&clickSound, powf(master_volume, 2.5f)*powf(ui_volume, 2.5f));
}

void set_hover_sound(float master_volume, float ui_volume){
    ma_sound_set_volume(&hoverSound, powf(master_volume, 2.5f)*powf(ui_volume, 2.5f));
}