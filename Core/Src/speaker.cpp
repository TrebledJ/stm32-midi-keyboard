#include "speaker.hpp"

#include "buttons.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "main.h"
#include "utils/notes.hpp"

#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 29;
// leaf::osc::cycle sine[NUM_SINES];
leaf::midi::poly midi_poly;
inline constexpr int NUM_SINES_CH = 2;
leaf::osc::cycle sine_ch[NUM_SINES_CH];

extern LCD<> lcd;
// float chord[] = {440.0, 554.37, 659.25};
float chord[] = {440, 660};


void speaker::init()
{
    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);
    midi_poly.init(4);

    instance().sines.init();

    for (int i = 0; i < NUM_SINES_CH; i++) {
        sine_ch[i].init();
        sine_ch[i].setFreq(chord[i]);
    }

    speaker::play();
}

void speaker::note_on(Note note, uint8_t vel) { instance().sines.note_on(note, vel); }

void speaker::note_off(Note note) { instance().sines.note_off(note); }


void speaker::default_load(bool (&active)[NUM_KEYBOARD_KEYS])
{
    // TODO: introduce other instruments/oscillators
    // speaker::load(sine, active);
    // for (size_t i = 0; i < buffer_size; i++) {
    //     instance().curr_buffer[i] = instance().sines.tick();
    // }
}


void speaker::loop()
{
    for (size_t i = 0; i < buffer_size; i++) {
        instance().curr_buffer[i] = scale(instance().sines.tick());
    }
    speaker::send();

    instance().adjusted_amplitude = amplitude * volume::get();

    // // static int sound_mode = 0;

    // // speaker::load(sine[sound_mode]);
    // // if (sound_mode == 0)
    // //     speaker::load(sine_ch[0]);
    // // else if (sound_mode == 1)
    // //     speaker::load(sine_ch[1]);
    // // else
    // //     speaker::load(sine_ch[0], sine_ch[1]);
    // if (buttons::is_btn_just_pressed(BTN_Top)) {
    //     if (toggle_record % 2) {
    //         is_playback     = 0;
    //         midi_action_cnt = 0;
    //         memset(midi_file, 0, sizeof(midi_file));
    //         start_record_time = get_ticks();
    //     }
    //     toggle_record++;
    // }
    // if (buttons::is_btn_just_pressed(BTN_Bottom)) {
    //     start_play_time = get_ticks();
    //     memset(active, 0, sizeof(active));
    //     is_playback = 1;
    // }

    // for (int i = 0; i < 29; i++)
    //     lcd.draw_string(i, 0, "%d", active[i]);


    // Testing Purposes: Check state for changing sound.
    // static bool prev_pressed = false;

    // bool pressed = buttons::is_btn_pressed(BTN_A);
    // if (!prev_pressed && pressed) {
    //     // sound_mode = (sound_mode + 1) % NUM_SINES;
    //     sound_mode = (sound_mode + 1) % 3;
    //     if (sound_mode == 2) {
    //         sine_ch[0].setFreq(880);
    //         sine_ch[1].setFreq(1320);
    //     } else {
    //         sine_ch[0].setFreq(440);
    //         sine_ch[1].setFreq(660);
    //     }
    //     sine_ch[0].setPhase(0);
    //     sine_ch[1].setPhase(0);
    // }
    // prev_pressed = pressed;

    // timeit(elapsed, speaker::tick(sine[sound_mode]););
    // timeit(elapsed_send, speaker::send(););

    // timeit(elapsed_loop, {
    //     static bool prev_pressed = false;
    //     bool pressed             = button_matrix.is_btn_pressed(BTN_A);
    //     if (!prev_pressed && pressed) {
    //         sound_mode = (sound_mode + 1) % NUM_SINES;
    //     }
    //     prev_pressed = pressed;


    //     metronome.tick();
    //     button_matrix.tick();
    //     LED0::toggle();
    // });
    // lcd.draw_string(0, 0, "elapsed: %d %d %d", elapsed, elapsed_send, elapsed_loop);
}
