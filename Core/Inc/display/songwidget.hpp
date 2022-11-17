#pragma once

#include "display/data.hpp"
#include "display/widgets.hpp"


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
            case VOLUME: return truef(volume.up());
            case INSTRUMENT: return truef(instrument.select_prev());
            default: return false;
        }
    }

    bool on_s()
    {
        switch (select_index) {
            case VOLUME: return truef(volume.down());
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

        with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
        {
            volume.draw(urect(bounds.x, bounds.y + 4, restw - 20, bounds.h - 10));
        }

        instrument.draw(urect(bounds.x + restw, bounds.y, restw - 10, bounds.h));
        mute.draw(urect(bounds.x + restw * 2, bounds.y, togglesz, togglesz));
        solo.draw(urect(bounds.x + restw * 2 + togglesz, bounds.y, togglesz, togglesz));
    }

    void update(const urect& bounds)
    {
        extern LCD_ lcd;
        uint16_t togglesz = bounds.h;
        uint16_t restw    = (bounds.w - 2 * togglesz) / 2;
        if (prev_index != select_index) {
            prev_index = select_index;

            with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
            {
                volume.draw(urect(bounds.x, bounds.y + 4, restw - 20, bounds.h - 10));
            }

            with_bg_if(select_index == INSTRUMENT, BLUE)
            {
                instrument.draw(urect(bounds.x + restw, bounds.y, restw - 10, bounds.h));
            }

            lcd.draw_stringf(0, 14, "ch index: %d", select_index);
        } else {
            with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
            {
                volume.update(urect(bounds.x, bounds.y + 4, restw - 20, bounds.h - 10));
            }

            with_bg_if(select_index == INSTRUMENT, BLUE)
            {
                instrument.update(urect(bounds.x + restw, bounds.y, restw - 10, bounds.h));
            }
        }
        mute.update(urect(bounds.x + restw * 2, bounds.y, togglesz, togglesz));
        solo.update(urect(bounds.x + restw * 2 + togglesz, bounds.y, togglesz, togglesz));
    }

private:
    Selection select_index = DEFAULT;
    Selection prev_index   = DEFAULT;

    SliderWidget volume;
    ComboSelectWidget instrument{INSTRUMENT_NAMES.data(), INSTRUMENT_NAMES.size()};
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
        extern LCD_ lcd;
        for (int i = 0; i < NUM_CHANNELS; i++) {
            with_bg_if(is_ch(index) && i == to_ch(index), BLUE)
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
        lcd.draw_stringf(0, 13, "select index: %d", index);
        if (prev_index != index) {
            prev_index = index;

            if (is_ch(prev_index)) {
                // Deselect previous index.
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(prev_index),
                                 "Channel %d:", to_ch(prev_index) + 1);
            }
            if (is_ch(index)) {
                // Select current index.
                with_bg(BLUE)
                {
                    lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(index),
                                     "Channel %d:", to_ch(index) + 1);
                }
            }
        }
        if (is_ch(index)) {
            with_bg_if(channel[to_ch(index)].selection() == ChannelWidget::DEFAULT, BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + to_ch(index),
                                 "Channel %d:", to_ch(index) + 1);
            }

            channel[to_ch(index)].update(urect(bounds.x + CHAR_WIDTH * 12, bounds.y + CHAR_HEIGHT * to_ch(index),
                                               bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
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
