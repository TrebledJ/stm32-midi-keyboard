#pragma once

#include "buttons.hpp"
#include "display/songwidget.hpp"
#include "keyboard.hpp"
#include "metronome.hpp"


struct ButtonEvent {
    bool double_pressed;
};

inline ButtonEvent make_event(ButtonName button) { return ButtonEvent{buttons::is_btn_double_pressed(button)}; }


class HomePage
{
public:
    HomePage() {}

    bool on_w(ButtonEvent) { return truef(--index); }
    bool on_s(ButtonEvent) { return truef(++index); }
    bool on_a(ButtonEvent)
    {
        extern Metronome metronome;
        return truef(metronome.toggle());
    }
    bool on_d(ButtonEvent) { return index != 0; }
    bool on_f1(ButtonEvent) { return truef(kb::toggle_playback()); }
    bool on_f2(ButtonEvent) { return truef(kb::toggle_record()); }

    void reset_selection() { index = 0; }
    PageName selected_page() const { return static_cast<PageName>(index.data); }

    void draw(const urect& bounds, bool force = false);

private:
    clamped_int<3> index             = 0;
    decltype(index)::type prev_index = 0;
};


class SongPage
{
public:
    SongPage(std::array<SongData, NUM_SONGS>& data) : song_data{data} { load(data[0]); }

    bool on_w(ButtonEvent) { return (is_ch(v_index) && channel[to_ch(v_index)].on_w()) || select_prev_v_index(); }
    bool on_s(ButtonEvent)
    {
        return (is_ch(v_index) && channel[to_ch(v_index)].on_s()) || truef(select_next_v_index());
    }
    bool on_a(ButtonEvent) { return (is_ch(v_index) && channel[to_ch(v_index)].on_a()) || truef(load_prev_song()); }
    bool on_d(ButtonEvent) { return (is_ch(v_index) && channel[to_ch(v_index)].on_d()) || truef(load_next_song()); }
    bool on_f1(ButtonEvent) { return (is_ch(v_index) && channel[to_ch(v_index)].on_f1()) || truef(playpause()); }
    bool on_f2(ButtonEvent) { return (is_ch(v_index) && channel[to_ch(v_index)].on_f2()) || truef(record()); }

    void load(SongData& data);
    void draw(const urect& bounds, bool force = false);

private:
    uint8_t song_index = 0;
    uint8_t v_index    = 0; // 0: Default. 1-4: Channel.

    ChannelWidget channel[NUM_CHANNELS];
    uint8_t max_v_index = NUM_CHANNELS + 1;

    // Internal update state.
    uint8_t prev_song_index = 0;
    uint8_t prev_v_index    = 0;

    std::array<SongData, NUM_SONGS> song_data;

    void load_prev_song();
    void load_next_song();

    static bool is_ch(size_t index) { return 1 <= index && index <= NUM_CHANNELS; }
    static uint16_t to_ch(size_t index) { return index - 1; }

    bool select_prev_v_index()
    {
        if (v_index == 0)
            return false;
        v_index--;
        return true;
    }
    bool select_next_v_index()
    {
        if (v_index + 1 == max_v_index)
            return false;
        v_index++;
        return true;
    }

    void playpause();
    void record();
};


class SettingsPage
{
public:
    enum Selection { DEFAULT, VOLUME, TRANSPOSE, AUTO_CHORD, KEY, METRONOME_DIV, NUM_SELECTION };

    SettingsPage();

    void reset_selection() { select_index = 0; }

    bool on_w(ButtonEvent);
    bool on_s(ButtonEvent);
    bool on_a(ButtonEvent);
    bool on_d(ButtonEvent);
    bool on_f1(ButtonEvent) { return false; }
    bool on_f2(ButtonEvent) { return false; }

    void draw(const urect& bounds, bool force = false);

private:
    uint8_t select_index = DEFAULT;
    uint8_t max_index    = NUM_SELECTION;

    SliderWidget volume;
    SliderWidget transpose;
    ComboSelectWidget autochord;
    ComboSelectWidget key;
    SliderWidget metronome_div;

    uint8_t prev_select_index = 0;
};


class MenuController
{
public:
    MenuController(std::array<SongData, NUM_SONGS>& data) : song_page{data} {}

    void draw() { update(true); }
    void update(bool force = false);
    void loop();
    void go_to_page(PageName page);

private:
    PageName page;

    HomePage home_page;
    SongPage song_page;
    SettingsPage settings_page;

    template <typename T>
    void draw_delegate(T& page, bool force);

    template <typename T>
    void callback_delegate(T& page);
};


template <typename T>
void MenuController::draw_delegate(T& page, bool force)
{
    page.draw({0, 0, LCD_WIDTH, LCD_HEIGHT}, force);
}

template <typename T>
void MenuController::callback_delegate(T& page)
{
    if (buttons::is_btn_just_pressed(BTN_Bottom)) {
        page.on_f2(make_event(BTN_Bottom));
    }
    if (buttons::is_btn_just_pressed(BTN_Top)) {
        page.on_f1(make_event(BTN_Top));
    }
    if (buttons::is_btn_just_pressed(BTN_W)) {
        page.on_w(make_event(BTN_W)) || truef(go_to_page(PageName::HOME));
    }
    if (buttons::is_btn_just_pressed(BTN_A)) {
        page.on_a(make_event(BTN_A));
    }
    if (buttons::is_btn_just_pressed(BTN_S)) {
        page.on_s(make_event(BTN_S));
    }
    if (buttons::is_btn_just_pressed(BTN_D)) {
        page.on_d(make_event(BTN_D)) && (this->page == PageName::HOME) && truef(go_to_page(home_page.selected_page()));
    }
}
