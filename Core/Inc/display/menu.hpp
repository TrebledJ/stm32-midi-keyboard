#pragma once

#include "buttons.hpp"
#include "display/songwidget.hpp"


class SongPage
{
public:
    SongPage(std::array<SongData, NUM_SONGS>& data) : song_data{data} { load(data[0]); }

    bool on_w() { return (is_ch(v_index) && channel[to_ch(v_index)].on_w()) || select_prev_v_index(); }
    bool on_s() { return (is_ch(v_index) && channel[to_ch(v_index)].on_s()) || truef(select_next_v_index()); }
    bool on_a() { return (is_ch(v_index) && channel[to_ch(v_index)].on_a()) || truef(load_prev_song()); }
    bool on_d() { return (is_ch(v_index) && channel[to_ch(v_index)].on_d()) || truef(load_next_song()); }
    bool on_f1() { return (is_ch(v_index) && channel[to_ch(v_index)].on_f1()) || truef(playpause()); }
    bool on_f2() { return (is_ch(v_index) && channel[to_ch(v_index)].on_f2()) || truef(record()); }

    void load(SongData& data);
    void draw(const urect& bounds);
    void update(const urect& bounds);

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

template <size_t MAX, typename T = uint16_t>
struct clamped_int {
    using type = T;
    T data;

    template <typename U>
    clamped_int(U x = 0) : data{static_cast<T>(x)}
    {
    }

    constexpr T& operator++() { return data = (data + 1 == MAX ? data : data + 1); }
    constexpr T operator++(int)
    {
        T tmp = data;
        ++(*this);
        return tmp;
    }
    constexpr T& operator--() { return data = (data == 0 ? data : data - 1); }
    constexpr T operator--(int)
    {
        T tmp = data;
        --(*this);
        return tmp;
    }
    constexpr T& operator()() { return data; }
};

#define clamped_int_bin_op(OP)                           \
    template <size_t N, typename T, typename U>          \
    constexpr bool operator OP(clamped_int<N, T> a, U b) \
    {                                                    \
        return a.data OP b;                              \
    }

clamped_int_bin_op(==);
clamped_int_bin_op(!=);
clamped_int_bin_op(<);
clamped_int_bin_op(<=);
clamped_int_bin_op(>);
clamped_int_bin_op(>=);

class HomePage
{
public:
    HomePage() {}

    bool on_w() { return truef(--index); }
    bool on_s() { return truef(++index); }
    bool on_a() { return false; }
    bool on_d() { return index != 0; }
    bool on_f1() { return false; }
    bool on_f2() { return false; }

    void reset_selection() { index.data = 0; }
    PageName selected_page() const { return static_cast<PageName>(index.data); }

    void draw(const urect& bounds);
    void update(const urect& bounds);

private:
    clamped_int<3> index             = 0;
    decltype(index)::type prev_index = 0;
};

class SettingsPage
{
public:
    enum Selection { DEFAULT, VOLUME, TRANSPOSE, NUM_SELECTION };

    SettingsPage();

    void reset_selection() { select_index = 0; }

    bool on_w();
    bool on_s();
    bool on_a();
    bool on_d();
    bool on_f1() { return false; }
    bool on_f2() { return false; }

    void draw(const urect& bounds);

    void update(const urect& bounds);

private:
    uint8_t select_index = DEFAULT;
    uint8_t max_index    = NUM_SELECTION;

    SliderWidget volume;
    SliderWidget transpose;

    uint8_t prev_select_index = 0;
};


class MenuController
{
public:
    MenuController(std::array<SongData, NUM_SONGS>& data) : song_page{data} {}

    void draw();
    void loop();
    void go_to_page(PageName page);

    template <typename T>
    void draw_delegate(T& page);

    template <typename T>
    void update_delegate(T& page);

    template <typename T>
    void callback_delegate(T& page);

private:
    PageName page;

    HomePage home_page;
    SongPage song_page;
    SettingsPage settings_page;
};


template <typename T>
void MenuController::draw_delegate(T& page)
{
    page.draw({0, 0, LCD_WIDTH, LCD_HEIGHT});
}

template <typename T>
void MenuController::update_delegate(T& page)
{
    page.update({0, 0, LCD_WIDTH, LCD_HEIGHT});
}

template <typename T>
void MenuController::callback_delegate(T& page)
{
    if (buttons::is_btn_just_pressed(BTN_Bottom)) {
        page.on_f2();
    }
    if (buttons::is_btn_just_pressed(BTN_Up)) {
        page.on_f1();
    }
    if (buttons::is_btn_just_pressed(BTN_W)) {
        page.on_w() || truef(go_to_page(PageName::HOME));
    }
    if (buttons::is_btn_just_pressed(BTN_A)) {
        page.on_a();
    }
    if (buttons::is_btn_just_pressed(BTN_S)) {
        page.on_s();
    }
    if (buttons::is_btn_just_pressed(BTN_D)) {
        page.on_d() && this->page == PageName::HOME&& truef(go_to_page(home_page.selected_page()));
    }
}