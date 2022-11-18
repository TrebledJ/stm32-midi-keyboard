#include "speaker.hpp"

#include "buttons.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "main.h"
#include "waveform.hpp"

#define Note2MIDI(n)   n + 21
#define MIDI2Note(n)   n - 21
#define note2button(n) n - C4
#define button2note(n) n + C4
#define MEM_SIZE       40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 29;
leaf::osc::cycle sine[NUM_SINES];
leaf::midi::poly midi_poly;
inline constexpr int NUM_SINES_CH = 2;
leaf::osc::cycle sine_ch[NUM_SINES_CH];
extern LCD<> lcd;
float freq[88];
// float chord[] = {440.0, 554.37, 659.25};
float chord[] = {440, 660};
uint32_t midi_time_stamp[100];
MIDI_Pkg midi_file[1000];
uint8_t active[NUM_SINES]      = {0};
uint8_t prev_active[NUM_SINES] = {0};
void note_on(MIDI_Pkg* pkg, uint32_t t, uint8_t channel, uint8_t note, uint8_t v = 90)
{
    pkg->message.time_stamp  = t;
    pkg->message.status_byte = (NOTE_ON << 4) + channel;
    pkg->message.data_byte   = note;
    pkg->message.velocity    = v;
}
void note_off(MIDI_Pkg* pkg, uint32_t t, uint8_t channel, uint8_t note, uint8_t v = 90)
{
    pkg->message.time_stamp  = t;
    pkg->message.status_byte = (NOTE_OFF << 4) + channel;
    pkg->message.data_byte   = note;
    pkg->message.velocity    = v;
}
void speaker::init()
{
    NOTE_FREQ_TABLE(init_element);
    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);
    midi_poly.init(4);

    for (int i = 0; i < NUM_SINES; i++) {
        sine[i].init();
        sine[i].setFreq(freq[button2note(i)]);
        sine[i].setPhase(0);
    }
    for (int i = 0; i < NUM_SINES_CH; i++) {
        sine_ch[i].init();
        sine_ch[i].setFreq(chord[i]);
    }

    speaker::play();
}

void speaker::loop()
{
    // static int sound_mode = 0;
    static int active_count           = 0;
    static uint8_t toggle_record      = 0;
    static uint8_t is_playback        = 0;
    static uint32_t start_record_time = 0;
    static uint32_t start_play_time   = 0;
    static uint16_t midi_action_cnt   = 0;
    // speaker::load(sine[sound_mode]);
    // if (sound_mode == 0)
    //     speaker::load(sine_ch[0]);
    // else if (sound_mode == 1)
    //     speaker::load(sine_ch[1]);
    // else
    //     speaker::load(sine_ch[0], sine_ch[1]);
    if (buttons::is_btn_just_pressed(BTN_Top)) {
        if (toggle_record % 2) {
            is_playback     = 0;
            midi_action_cnt = 0;
            memset(midi_file, 0, sizeof(midi_file));
            start_record_time = get_ticks();
        }
        toggle_record++;
    }
    if (buttons::is_btn_just_pressed(BTN_Bottom)) {
        start_play_time = get_ticks();
        memset(active, 0, sizeof(active));
        is_playback = 1;
    }

    // for (int i = 0; i < 29; i++)
    //     lcd.draw_string(i, 0, "%d", active[i]);


    // on key released
    if (!is_playback) {
        active_count = 0;
        for (int i = 0; i < NUM_SINES; i += 2) {
            active[i / 2] = buttons::is_btn_pressed((ButtonName)(i + BTN_1_U))
                            || buttons::is_btn_pressed((ButtonName)(i + BTN_1_D));
            if (active[i / 2]) { // press the button
                active_count++;
                if (!prev_active[i / 2]) { // just press the button
                    midi_poly.noteOn(Note2MIDI(button2note(i / 2)), 90);
                    if ((toggle_record % 2)) {
                        note_on(&midi_file[midi_action_cnt], get_ticks() - start_record_time, 1,
                                Note2MIDI(button2note(i / 2)));
                        midi_action_cnt++;
                    }
                }
            } else {
                if (prev_active[i / 2]) { // just relase the button
                    midi_poly.noteOff(Note2MIDI(button2note(i / 2)));

                    if ((toggle_record % 2)) {
                        note_off(&midi_file[midi_action_cnt], get_ticks() - start_record_time, 1,
                                 Note2MIDI(button2note(i / 2)));
                        midi_action_cnt++;
                    }
                }
            }
            prev_active[i / 2] = active[i / 2];
        }
        speaker::load(sine, active, active_count);
        speaker::send();
    } else {
        for (int i = 0; i < midi_action_cnt;) {
            speaker::load(sine, active, active_count);
            speaker::send();
            if (get_ticks() - start_play_time > midi_file[i].message.time_stamp) {
                active[MIDI2Note(note2button(midi_file[i].message.data_byte))] =
                    ((midi_file[i].message.status_byte >> 4) & 1);
                if (active[i]) { // press the button
                    active_count++;
                } else {
                    active_count--;
                }
                i++;
            }

            lcd.draw_string(0, 1, "%d %d %d %d", active[0], active[1], active[2], active_count);
        }
        is_playback = 0;
    }


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
