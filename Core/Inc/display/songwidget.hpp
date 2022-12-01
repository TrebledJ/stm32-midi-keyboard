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

    void draw(const urect& bounds, bool force = false)
    {
        uint16_t togglesz = bounds.h;
        uint16_t restw    = (bounds.w - 2 * togglesz) / 2;

        bool force_subdraw = force;

        if (prev_index != select_index) {
            prev_index = select_index;

            // If index has been updated, force the child widgets to redraw.
            force_subdraw = true;
        }

        with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
        {
            volume.draw(urect(bounds.x, bounds.y + 4, restw - 20, bounds.h - 10), force_subdraw);
        }

        with_bg_if(select_index == INSTRUMENT, BLUE)
        {
            instrument.draw(urect(bounds.x + restw, bounds.y, restw - 10, bounds.h), force_subdraw);
        }

        mute.draw(urect(bounds.x + restw * 2, bounds.y, togglesz, togglesz), force);
        solo.draw(urect(bounds.x + restw * 2 + togglesz, bounds.y, togglesz, togglesz), force);
    }

private:
    Selection select_index = DEFAULT;
    Selection prev_index   = DEFAULT;

    SliderWidget volume;
    ComboSelectWidget instrument{INSTRUMENT_NAMES.data(), INSTRUMENT_NAMES.size()};
    ToggleWidget mute;
    ToggleWidget solo;
};
