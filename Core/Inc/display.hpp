#include "defines.hpp"
#include "lcd/lcd.hpp"
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
class Menu
{
public:
    void init();

    Instance instance;
    void tick();
    void move_up()
    {
        lcd.clear();
        if (selected <= 0) {
            return;
        }
        selected -= 1;
    }
    void move_down()
    {
        lcd.clear();
        if (selected >= 19) {
            return;
        }
        selected += 1;
    }
    void plus();
    void minus();
    void enter();
    void exit();

    void record(bool playback = false, bool countin = false);
    // Play selected song.
    void toggle_play_stop();
    void clear_all_song();
    void set_song(int song);
    void clear(); // Delete all music data in the selected song.
    void set_channel();
    void set_volume(uint16_t volume); // Volume, 0-100.
    void toggle_mute();
    void toggle_solo();
    void change_instrument();
    void clear_channel_data(); // Delete music data in the selected channel.
    void Export();
    void Import();
    void set_transpose(int16_t semitones);
    void set_tempo(uint16_t bpm);
    void set_time_signature(uint16_t n, uint16_t d);
    void set_bitrate(uint32_t br);
    void set_power_saving(bool on);
    void set_fx(Effect);
    void set_reverb(uint16_t wet); // 0-100.
    void set_delay(uint16_t wet);  // 0-100.
    void set_eq(Equaliser);
    LCD<LCD_ORIENTATION> lcd{&hspi2};

private:
    PAGE_NAME page = HOME;
    void plus_value(auto& inp, auto range) { inp += range; };
    void minus_value(auto& inp, auto range) { inp -= range; };
    void draw_circle(uint16_t usC, uint16_t usP, uint16_t SR, color_t usColor);

    int depth;
    int selected = 0;
};
