#include "defines.hpp"
#include "lcd/lcd.hpp"

template <typename T>
struct rect {
    T x;
    T y;
    T w;
    T h;
};

using urect = rect<uint16_t>;

typedef enum {
    HOME,
    SONG,
    CHANNEL,
    EXPORT,
    SETTING,
    EFFECTS,
} PAGE_NAME;

typedef enum {
    DEFAULT,       // Play normally.
    AUTO_CHORD,    // Complete notes based on the pressed notes.
    AUTO_ARPEGGIO, // Like autochord, but alternates between notes.
} Effect;

enum Instrument {
    SINE,
    PIANO,
};

typedef enum {
    NO_FILTER,
    LOW_PASS,
    HIGH_PASS,
    LOW_BAND,
    HIGH_BAND,
} Equaliser;

struct Settings {
    int16_t transpose;
    uint16_t bpm;
    uint16_t time_sig_n;
    uint16_t time_sig_d;
    uint32_t bitrate;
    bool power_saving;
};

struct Instance {
    uint16_t selected_song;
    uint16_t selected_channel;
    uint8_t is_music_stop;
    Settings settings;
};

extern SPI_HandleTypeDef hspi2;

#include "lcd/lcd.hpp"
extern LCD<> lcd;

// class Menu
// {
// public:
//     void init();

//     Instance instance;
//     void tick();
//     void move_up()
//     {
//         lcd.clear();
//         if (selected <= 0) {
//             return;
//         }
//         selected -= 1;
//     }
//     void move_down()
//     {
//         lcd.clear();
//         if (selected >= 19) {
//             return;
//         }
//         selected += 1;
//     }
//     void plus();
//     void minus();
//     void enter();
//     void exit();

//     void record(bool playback = false, bool countin = false);
//     // Play selected song.
//     void toggle_play_stop();
//     void clear_all_song();
//     void set_song(int song);
//     void clear(); // Delete all music data in the selected song.
//     void set_channel();
//     void set_volume(uint16_t volume); // Volume, 0-100.
//     void toggle_mute();
//     void toggle_solo();
//     void change_instrument();
//     void clear_channel_data(); // Delete music data in the selected channel.
//     void Export();
//     void Import();
//     void set_transpose(int16_t semitones);
//     void set_tempo(uint16_t bpm);
//     void set_time_signature(uint16_t n, uint16_t d);
//     void set_bitrate(uint32_t br);
//     void set_power_saving(bool on);
//     void set_fx(Effect);
//     void set_reverb(uint16_t wet); // 0-100.
//     void set_delay(uint16_t wet);  // 0-100.
//     void set_eq(Equaliser);
//     LCD<LCD_ORIENTATION> lcd{&hspi2};

// private:
//     PAGE_NAME page = HOME;
//     void plus_value(auto& inp, auto range) { inp += range; };
//     void minus_value(auto& inp, auto range) { inp -= range; };
//     void draw_circle(uint16_t usC, uint16_t usP, uint16_t SR, color_t usColor);

//     int depth;
//     int selected = 0;
// };

using string_t                                 = const char[16];
inline constexpr string_t INSTRUMENT_OPTIONS[] = {"Sine", "Piano"};
inline constexpr int NUM_SONGS                 = 4;
inline constexpr int NUM_CHANNELS              = 4;


// Call and return true;
#define truef(...)   \
    ({               \
        __VA_ARGS__; \
        true;        \
    })

// template <typename M>
// concept Menu = requires(M menu)
// {
//     menu.on_function1();
//     menu.on_function2();
//     menu.on_w();
//     menu.on_a();
//     menu.on_s();
//     menu.on_d();
//     // menu.set_area();
// };

class ButtonEvent
{
public:
    enum Event { PRESSED, RELEASED, HELD };

    ButtonEvent(Event event) : m_event{event} {}

    Event event() const { return m_event; }

    // Test whether the event is still active (unhandled).
    // bool operator()() const { return !m_handled; }

    // void finish() { m_handled = true; }

private:
    Event m_event;
    // bool m_handled = false;
};

// template <typename W>
// concept Widget = requires(W widget, Menu& menu, uint32_t u32)
// {
//     W(menu);
//     // widget.clear();
//     widget.draw(u32, u32);
//     // {
//     //     widget.btn_ctrl()
//     //     } -> btn_ctrl_t;
//     {
//         widget.on_function1()
//         } -> std::same_as<bool>;
//     {
//         widget.on_function2()
//         } -> std::same_as<bool>;
//     {
//         widget.on_w()
//         } -> std::same_as<bool>;
//     {
//         widget.on_a()
//         } -> std::same_as<bool>;
//     {
//         widget.on_s()
//         } -> std::same_as<bool>;
//     {
//         widget.on_d()
//         } -> std::same_as<bool>;
// };

class ComboSelectWidget
{
public:
    ComboSelectWidget(string_t* options, uint8_t max) : options{options}, max{max} {}

    void set_index(uint8_t index) { this->curr = (index >= max ? max - 1 : index); }
    void select_prev() { curr = (curr == 0 ? max - 1 : curr - 1); }
    void select_next() { curr = (curr + 1 == max ? 0 : curr + 1); }

    void draw(const urect& bounds)
    {
        // TODO: display curr selected option
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "%-10s", options[curr]);
        lcd.draw_stringf(0, 15, "%d %d", curr, max);
    }

    void update(const urect& bounds)
    {
        if (prev != curr) {
            draw(bounds);
            prev = curr;
        }
    }

private:
    string_t* options;
    uint8_t prev;
    uint8_t curr;
    uint8_t max;
};

class ToggleWidget
{
public:
    bool state;
    bool prev_state;

    ToggleWidget(bool state = false) : state{state} {}

    void toggle() { state = !state; }
    void set_state(bool state) { this->state = state; }

    void draw(const urect& bounds)
    {
        // TODO
        state ? lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, GREEN)
              : lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, RED);
    }

    void update(const urect& bounds)
    {
        if (prev_state != state) {
            draw(bounds);
            prev_state = state;
        }
    }
};

class SliderWidget
{
public:
    SliderWidget(int32_t value = 0, int32_t inc = 1) : value{value}, inc{inc} {}

    void set_range(int32_t min, int32_t max)
    {
        this->min = min;
        this->max = max;
    }
    void set_value(int32_t value) { this->value = value; }
    void set_inc(int32_t inc) { this->inc = inc; }
    void up()
    {
        value += inc;
        if (value > max)
            value = max;
    }
    void down()
    {
        value -= inc;
        if (value < min)
            value = min;
    }

    void draw(const urect& bounds)
    {
        float prog = 1.0 * value / (max - min);
        lcd.draw_rect(bounds.x, bounds.y, bounds.w * prog, bounds.h, lcd.palette.foreground());
        lcd.draw_rect(bounds.x + bounds.w * prog, bounds.y, bounds.w * (1 - prog), bounds.h, WHITE);
    }

    void update(const urect& bounds)
    {
        if (prev_value != value) {
            // TODO: update.
            draw(bounds);
            prev_value = value;
        }
    }

private:
    int32_t value;
    int32_t prev_value;
    int32_t inc;
    int32_t min;
    int32_t max;
};

struct ChannelData {
    static constexpr uint8_t MUTE = 0x01; // TODO: aggregate this in all song data?
    static constexpr uint8_t SOLO = 0x02;

    Instrument instrument = SINE;
    uint8_t flags         = 0;
    uint8_t volume        = 50;

    bool is_mute() const { return flags & MUTE; }
    bool is_solo() const { return flags & SOLO; }
};

struct SongData {
    ChannelData channel[NUM_CHANNELS];
};


class ChannelWidget
{
public:
    enum Selection {
        DEFAULT,   // Nothing selected
        VOLUME,    // Volume control selected
        INSTRUMENT // Instrument combo selected.
    };

    ChannelWidget()
    {
        volume.set_inc(10);
        volume.set_range(0, 100);
    }

    Selection selection() const { return select_index; }

    bool on_a()
    {
        switch (select_index) {
            case VOLUME: select_index = DEFAULT; return true;
            case INSTRUMENT: select_index = VOLUME; return true;
            default: return true;
        }
    }

    bool on_d()
    {
        switch (select_index) {
            case DEFAULT: select_index = VOLUME; return true;
            case VOLUME: select_index = INSTRUMENT; return true;
            default: return true;
        }
    }

    bool on_w()
    {
        switch (select_index) {
            case VOLUME:
                // TODO: handle volume control
                return truef(volume.up());
            case INSTRUMENT: return truef(instrument.select_prev());
            default: return false;
        }
    }

    bool on_s()
    {
        switch (select_index) {
            case VOLUME:
                // TODO: handle volume control
                return truef(volume.down());
            case INSTRUMENT: return truef(instrument.select_next());
            default: return false;
        }
    }

    bool on_f1()
    {
        mute.toggle();
        return true;
    }

    bool on_f2()
    {
        solo.toggle();
        return true;
    }

    void load(ChannelData& data)
    {
        select_index = DEFAULT;
        volume.set_value(data.volume);
        instrument.set_index(data.instrument);
        mute.set_state(data.is_mute());
        solo.set_state(data.is_solo());
    }

    void draw(const urect& bounds)
    {
        uint16_t togglesz = bounds.h;
        uint16_t restw    = (bounds.w - 2 * togglesz) / 2;

        lcd.palette.foreground(select_index == VOLUME ? BLUE : DARKGREY);
        volume.draw({bounds.x, bounds.y + 4, restw - 20, bounds.h - 10});
        lcd.palette.foreground(WHITE);

        instrument.draw({bounds.x + restw, bounds.y, restw - 10, bounds.h});
        mute.draw({bounds.x + restw * 2, bounds.y, togglesz, togglesz});
        solo.draw({bounds.x + restw * 2 + togglesz, bounds.y, togglesz, togglesz});
    }

    void update(const urect& bounds)
    {
        uint16_t togglesz = bounds.h;
        uint16_t restw    = (bounds.w - 2 * togglesz) / 2;
        if (prev_index != select_index) {
            // draw(bounds);
            // prev_index = index;

            lcd.palette.foreground(select_index == VOLUME ? BLUE : DARKGREY);
            volume.draw({bounds.x, bounds.y + 4, restw - 20, bounds.h - 10});
            lcd.palette.foreground(WHITE);

            lcd.palette.background(select_index == INSTRUMENT ? BLUE : BLACK);
            instrument.draw({bounds.x + restw, bounds.y, restw - 10, bounds.h});
            lcd.palette.background(BLACK);

            lcd.draw_stringf(0, 14, "ch index: %d", select_index);

            prev_index = select_index;
        } else {
            lcd.palette.foreground(select_index == VOLUME ? BLUE : DARKGREY);
            volume.update({bounds.x, bounds.y + 4, restw - 20, bounds.h - 10});
            lcd.palette.foreground(WHITE);

            lcd.palette.background(select_index == INSTRUMENT ? BLUE : BLACK);
            instrument.update({bounds.x + restw, bounds.y, restw - 10, bounds.h});
            lcd.palette.background(BLACK);
        }
        mute.update({bounds.x + restw * 2, bounds.y, togglesz, togglesz});
        solo.update({bounds.x + restw * 2 + togglesz, bounds.y, togglesz, togglesz});
    }

private:
    Selection select_index = DEFAULT;
    Selection prev_index   = DEFAULT;

    SliderWidget volume;
    ComboSelectWidget instrument{INSTRUMENT_OPTIONS, sizeof(INSTRUMENT_OPTIONS) / sizeof(string_t)};
    ToggleWidget mute;
    ToggleWidget solo;
};

class SongWidget
{
public:
    bool on_w() { return (is_ch(index) && channel[to_ch(index)].on_w()) || truef(select_prev_index()); }
    bool on_s() { return (is_ch(index) && channel[to_ch(index)].on_s()) || truef(select_next_index()); }
    bool on_a() { return is_ch(index) && channel[to_ch(index)].on_a(); }
    bool on_d() { return is_ch(index) && channel[to_ch(index)].on_d(); }
    bool on_f1() { return (is_ch(index) && channel[to_ch(index)].on_f1()); }
    bool on_f2() { return (is_ch(index) && channel[to_ch(index)].on_f2()) || truef(playpause()); }

    void load(SongData& data)
    {
        for (int i = 0; i < NUM_CHANNELS; i++) {
            channel[i].load(data.channel[i]);
        }
    }

    void draw(const urect& bounds)
    {
        for (int i = 0; i < NUM_CHANNELS; i++) {
            if (is_ch(index) && i == to_ch(index)) {
                lcd.palette.background(BLUE);
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + i, "Channel %d:", i + 1);
                lcd.palette.background(BLACK);
            } else {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + i, "Channel %d:", i + 1);
            }
            channel[i].draw(
                {bounds.x + CHAR_WIDTH * 12, bounds.y + CHAR_HEIGHT * i, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT});
        }
    }

    void update(const urect& bounds)
    {
        lcd.draw_stringf(0, 13, "select index: %d", index);
        if (index != prev_index) {
            // Draw and update.
            // draw(x, y);

            if (is_ch(prev_index)) {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(prev_index),
                                 "Channel %d:", to_ch(prev_index) + 1);
            }
            if (is_ch(index)) {
                lcd.palette.background(BLUE);
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(index),
                                 "Channel %d:", to_ch(index) + 1);
                lcd.palette.background(BLACK);
            }

            // ch[prev_index].draw(x, y + 20 * prev_index);
            prev_index = index;
            // lcd.palette.background(BLUE);
            // ch[index].draw(x, y + 20 * index);
            // lcd.palette.background(BLACK);
        }
        if (is_ch(index)) {
            lcd.palette.background(channel[to_ch(index)].selection() == ChannelWidget::DEFAULT ? BLUE : BLACK);
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(index),
                             "Channel %d:", to_ch(index) + 1);
            lcd.palette.background(BLACK);

            channel[to_ch(index)].update({bounds.x + CHAR_WIDTH * 12, bounds.y + CHAR_HEIGHT * to_ch(index),
                                          bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT});
        }
    }

private:
    uint16_t index     = 0; // 0: Default. 1-4: Channel.
    uint16_t max_index = NUM_CHANNELS + 1;
    ChannelWidget channel[NUM_CHANNELS];

    // Internal update state.
    uint16_t prev_index = 0;

    bool is_ch(size_t index) const { return 1 <= index && index <= NUM_CHANNELS; }
    uint16_t to_ch(size_t index) const { return index - 1; }
    // ChannelWidget& curr_channel() { return ch[channel_index()]; }

    void select_prev_index() { index = (index == 0 ? 0 : index - 1); }
    void select_next_index() { index = (index + 1 == max_index ? index : index + 1); }

    void playpause()
    {
        // TODO
    }

    void clear()
    {
        // TODO
    }
};

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
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        widget.draw({bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT});
    }

    void update(const urect& bounds)
    {
        lcd.draw_stringf(0, 12, "song index: %d", song_index);
        // if (song_index != prev_song_index) {
        //     prev_song_index = song_index;
        // }
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        widget.update({bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT});
    }

private:
    uint8_t prev_song_index = 0;
    uint8_t song_index      = 0;
    std::array<SongData, NUM_SONGS> song_data;

    SongWidget widget;

    void load_prev_song()
    {
        // TODO
        if (song_index == 0) {
            return;
        }
        song_index--;
        widget.load(song_data[song_index]);
    }
    void load_next_song()
    {
        // TODO
        if (song_index + 1 == NUM_SONGS) {
            return;
        }
        song_index++;
        widget.load(song_data[song_index]);
    }
};

#include "buttons.hpp"

class MenuController
{
public:
    MenuController(std::array<SongData, NUM_SONGS>& data) : song_menu{data} {}

    void draw(const urect& bounds) { song_menu.draw({bounds.x, bounds.y, bounds.w, bounds.h}); }

    void update(const urect& bounds) { song_menu.update({bounds.x, bounds.y, bounds.w, bounds.h}); }

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
