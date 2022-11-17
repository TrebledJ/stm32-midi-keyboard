#pragma once

#include "buttons.hpp"
#include "display/songwidget.hpp"


class SongMenu
{
public:
    SongMenu(std::array<SongData, NUM_SONGS>& data) : song_data{data} { load(data[0]); }

    bool on_w() { return (is_ch(v_index) && channel[to_ch(v_index)].on_w()) || truef(select_prev_v_index()); }
    bool on_s() { return (is_ch(v_index) && channel[to_ch(v_index)].on_s()) || truef(select_next_v_index()); }
    bool on_a() { return (is_ch(v_index) && channel[to_ch(v_index)].on_a()) || truef(load_prev_song()); }
    bool on_d() { return (is_ch(v_index) && channel[to_ch(v_index)].on_d()) || truef(load_next_song()); }
    bool on_f1() { return (is_ch(v_index) && channel[to_ch(v_index)].on_f1()); }
    bool on_f2() { return (is_ch(v_index) && channel[to_ch(v_index)].on_f2()) || truef(playpause()); }

    void load(SongData& data)
    {
        for (int i = 0; i < NUM_CHANNELS; i++) {
            channel[i].load(data.channel[i]);
        }
    }

    void draw(const urect& bounds)
    {
        extern LCD_ lcd;
        with_bg_if(!is_ch(v_index), BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }
        draw(urect(bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT));

        for (int i = 0; i < NUM_CHANNELS; i++) {
            with_bg_if(is_ch(v_index) && i == to_ch(v_index), BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + i, "Channel %d:", i + 1);
            }
            channel[i].draw(
                urect(bounds.x + CHAR_WIDTH * 12, bounds.y + CHAR_HEIGHT * i, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
        }
    }

    void update(const urect& bounds)
    {
        extern LCD_ lcd;
        lcd.draw_stringf(0, 12, "song index: %d", song_index);
        lcd.draw_stringf(0, 13, "select index: %d", v_index);

        if (prev_song_index != song_index) {
            prev_song_index = song_index;
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }

        size_t ch_y = bounds.y + CHAR_HEIGHT;

        if (prev_v_index != v_index) {
            prev_v_index = v_index;

            if (is_ch(prev_v_index)) {
                // Deselect previous index.
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(prev_v_index),
                                 "Channel %d:", to_ch(prev_v_index) + 1);
            }
            if (is_ch(v_index)) {
                // Select current index.
                with_bg(BLUE)
                {
                    lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(v_index),
                                     "Channel %d:", to_ch(v_index) + 1);
                }
            }
        }
        if (is_ch(v_index)) {
            with_bg_if(channel[to_ch(v_index)].selection() == ChannelWidget::DEFAULT, BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(v_index),
                                 "Channel %d:", to_ch(v_index) + 1);
            }

            channel[to_ch(v_index)].update(urect(bounds.x + CHAR_WIDTH * 12, ch_y + CHAR_HEIGHT * to_ch(v_index),
                                                 bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
        }
    }

private:
    uint8_t song_index = 0;
    uint8_t v_index    = 0; // 0: Default. 1-4: Channel.

    ChannelWidget channel[NUM_CHANNELS];
    uint8_t max_v_index = 0;

    // Internal update state.
    uint8_t prev_song_index = 0;
    uint8_t prev_v_index    = 0;

    std::array<SongData, NUM_SONGS> song_data;

    void load_prev_song()
    {
        if (song_index == 0) {
            return;
        }
        song_index--;
        load(song_data[song_index]);
    }
    void load_next_song()
    {
        if (song_index + 1 == NUM_SONGS) {
            return;
        }
        song_index++;
        load(song_data[song_index]);
    }

    static bool is_ch(size_t index) { return 1 <= index && index <= NUM_CHANNELS; }
    static uint16_t to_ch(size_t index) { return index - 1; }

    void select_prev_v_index() { v_index = (v_index == 0 ? 0 : v_index - 1); }
    void select_next_v_index() { v_index = (v_index + 1 == max_v_index ? v_index : v_index + 1); }

    void playpause()
    {
        // TODO: toggle playing/pausing of current song
    }

    void clear()
    {
        // TODO
    }
};


class MenuController
{
public:
    MenuController(std::array<SongData, NUM_SONGS>& data) : song_menu{data} {}

    void draw(const urect& bounds) { song_menu.draw(urect(bounds.x, bounds.y, bounds.w, bounds.h)); }

    void update(const urect& bounds) { song_menu.update(urect(bounds.x, bounds.y, bounds.w, bounds.h)); }

    void loop()
    {
        if (buttons::is_btn_just_pressed(BTN_Bottom)) {
            song_menu.on_f2();
        }
        if (buttons::is_btn_just_pressed(BTN_Up)) {
            song_menu.on_f1();
        }
        if (buttons::is_btn_just_pressed(BTN_W)) {
            song_menu.on_w();
        }
        if (buttons::is_btn_just_pressed(BTN_A)) {
            song_menu.on_a();
        }
        if (buttons::is_btn_just_pressed(BTN_S)) {
            song_menu.on_s();
        }
        if (buttons::is_btn_just_pressed(BTN_D)) {
            song_menu.on_d();
        }

        update({0, 0, 300, 200});
    }

private:
    SongMenu song_menu;
    // SettingsMenu settings_menu;
};
