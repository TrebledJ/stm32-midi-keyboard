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

    void draw(const urect& bounds)
    {
        extern LCD_ lcd;
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 0, "Home");
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 1, "Song Menu");
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 2, "Settings Menu");
    }
    void update(const urect& bounds)
    {
        extern LCD_ lcd;
        if (prev_index != index.data) {
            prev_index = index.data;
            with_bg_if(index == 1, BLUE)
            {
                lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 1, "Song Menu");
            }
            with_bg_if(index == 2, BLUE)
            {
                lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 2, "Settings Menu");
            }
        }
    }

private:
    clamped_int<3> index             = 0;
    decltype(index)::type prev_index = 0;
};

class SettingsPage
{
public:
    enum Selection { DEFAULT, VOLUME, TRANSPOSE, NUM_SELECTION };

    SettingsPage()
    {
        volume.set_value(50);
        volume.set_inc(10);
        volume.set_range(0, 100);
        transpose.set_value(0);
        transpose.set_range(-12, 12);
    }

    void reset_selection() { select_index = 0; }

    bool on_w()
    {
        if (select_index > 0) {
            select_index -= 1;
            return true;
        }
        return false;
    }

    bool on_s()
    {
        if (select_index + 1 < max_index) {
            select_index += 1;
            return true;
        }
        return false;
    }

    bool on_a()
    {
        switch (select_index) {
            case VOLUME: return truef(volume.down());
            case TRANSPOSE: return truef(transpose.down());
            default: return false;
        }
    }

    bool on_d()
    {
        switch (select_index) {
            case VOLUME: return truef(volume.up());
            case TRANSPOSE: return truef(transpose.up());
            default: return false;
        }
    }

    bool on_f1() { return false; }
    bool on_f2() { return false; }

    void draw(const urect& bounds)
    {
        extern LCD_ lcd;

        size_t col2 = bounds.x + CHAR_WIDTH * 12;
        size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
        size_t y    = bounds.y;

        with_bg_if(select_index == DEFAULT, BLUE)
        {
            lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Settings");
        }

        y += CHAR_HEIGHT;
        with_bg_if(select_index == VOLUME, BLUE) { lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Volume:"); }
        with_fg_if(select_index == VOLUME, BLUE, DARKGREY) { volume.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8)); }

        y += CHAR_HEIGHT;
        with_bg_if(select_index == TRANSPOSE, BLUE)
        {
            lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Transpose:");
        }
        with_fg_if(select_index == TRANSPOSE, BLUE, DARKGREY)
        {
            transpose.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8));
        }
    }

    void update(const urect& bounds)
    {
        if (prev_select_index != select_index) {
            prev_select_index = select_index;
            draw(bounds);
        } else {
            extern LCD_ lcd;
            lcd.draw_stringf(15, 12, "%d %d %d %d   ", volume.prev_value, volume.value, volume.min, volume.max);
            size_t col2 = bounds.x + CHAR_WIDTH * 12;
            size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
            size_t y    = bounds.y;
            y += CHAR_HEIGHT;
            with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
            {
                volume.update(urect(col2, y + 4, w2, CHAR_HEIGHT - 8));
            }
            y += CHAR_HEIGHT;
            with_fg_if(select_index == TRANSPOSE, BLUE, DARKGREY)
            {
                transpose.update(urect(col2, y + 4, w2, CHAR_HEIGHT - 8));
            }
        }
    }

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

    void draw()
    {
        switch (page) {
            case PageName::HOME: draw_delegate(home_page); break;
            case PageName::SONG: draw_delegate(song_page); break;
            case PageName::SETTING: draw_delegate(settings_page); break;
            default: break;
        }
    }

    // void update() { song_page.update(urect(bounds.x, bounds.y, bounds.w, bounds.h)); }

    void loop()
    {
        switch (page) {
            case PageName::HOME:
                callback_delegate(home_page);
                update_delegate(home_page);
                break;
            case PageName::SONG:
                callback_delegate(song_page);
                update_delegate(song_page);
                break;
            case PageName::SETTING:
                callback_delegate(settings_page);
                update_delegate(settings_page);
                break;
            default: break;
        }
    }

    void go_to_page(PageName page)
    {
        extern LCD_ lcd;
        if (this->page != page) {
            this->page = page;
            lcd.clear();
            draw();
            switch (page) {
                case PageName::HOME: home_page.reset_selection(); break;
                case PageName::SETTING: settings_page.reset_selection(); break;
                default: break;
            }
        }
    }

    template <typename T>
    void draw_delegate(T& page)
    {
        page.draw({0, 0, LCD_WIDTH, LCD_HEIGHT});
    }

    template <typename T>
    void update_delegate(T& page)
    {
        page.update({0, 0, LCD_WIDTH, LCD_HEIGHT});
    }

    template <typename T>
    void callback_delegate(T& page)
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

private:
    PageName page;

    HomePage home_page;
    SongPage song_page;
    SettingsPage settings_page;
};
