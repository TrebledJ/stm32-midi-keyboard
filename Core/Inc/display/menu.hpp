#pragma once

#include "buttons.hpp"
#include "display/songwidget.hpp"


class SongMenu
{
public:
    SongMenu(std::array<SongData, NUM_SONGS>& data) : song_data{data} { widget.load(data[0]); }

    bool on_w() { return widget.on_w(); }
    bool on_s() { return widget.on_s(); }
    bool on_a() { return widget.on_a() || truef(load_prev_song()); }
    bool on_d() { return widget.on_d() || truef(load_next_song()); }
    bool on_f1() { return widget.on_f1(); }
    bool on_f2() { return widget.on_f2(); }

    void draw(const urect& bounds)
    {
        extern LCD_ lcd;
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        widget.draw(urect(bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT));
    }

    void update(const urect& bounds)
    {
        extern LCD_ lcd;
        lcd.draw_stringf(0, 12, "song index: %d", song_index);
        if (prev_song_index != song_index) {
            prev_song_index = song_index;
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }
        widget.update(urect(bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT));
    }

private:
    uint8_t prev_song_index = 0;
    uint8_t song_index      = 0;
    std::array<SongData, NUM_SONGS> song_data;

    SongWidget widget;

    void load_prev_song()
    {
        if (song_index == 0) {
            return;
        }
        song_index--;
        widget.load(song_data[song_index]);
    }
    void load_next_song()
    {
        if (song_index + 1 == NUM_SONGS) {
            return;
        }
        song_index++;
        widget.load(song_data[song_index]);
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
