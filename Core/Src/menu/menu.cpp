#include "autochord.hpp"
#include "display/menu.hpp"
#include "settings.hpp"

//////////////////////////
// ----- HomePage ----- //
//////////////////////////

void HomePage::draw(const urect& bounds, bool force)
{
    extern LCD_ lcd;

    if (force) {
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 0, "Home");
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 1,
                        "------------------------------------------------------------");
    }

    if (force || prev_index != index.data) {
        prev_index                            = index.data;
        static constexpr const char* LABELS[] = {"Song Menu", "Settings Menu", "Export Menu"};
        for (size_t i = 1; i < NUM_PAGES; i++) {
            with_bg_if(index == i, BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + i + 1, "%s", LABELS[i - 1]);
            }
        }
    }

    size_t row = 19;
    if (kb::is_recording()) {
        lcd.draw_string(0, row, "recording...    ");
    } else if (kb::is_playback()) {
        lcd.draw_string(0, row, "playing...    ");
    } else {
        lcd.draw_string(0, row, "idle            ");
    }
}

//////////////////////////
// ----- SongPage ----- //
//////////////////////////

void SongPage::load(SongData& data)
{
    v_index = 0;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        channel[i].load(data.channel[i]);
    }
}

void SongPage::draw(const urect& bounds, bool force)
{
    extern LCD_ lcd;

    bool redraw_header         = force;
    bool redraw_all            = force;
    bool redraw_selected_index = false;

    if (prev_song_index != song_index) {
        prev_song_index = song_index;
        redraw_header   = true;
    }

    if (prev_v_index != v_index) {
        // prev_v_index          = v_index;
        redraw_header         = true;
        redraw_selected_index = true;
    }


    if (redraw_header) {
        with_bg_if(!is_ch(v_index), BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }
    }
    if (force) {
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 1,
                        "------------------------------------------------------------");
    }

    size_t y = bounds.y + CHAR_HEIGHT;

    if (redraw_all) {
        for (int i = 0; i < NUM_CHANNELS; i++) {
            y += CHAR_HEIGHT;
            with_bg_if(is_ch(v_index) && i == to_ch(v_index), BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Channel %d:", i + 1);
            }
            channel[i].draw(urect(bounds.x + CHAR_WIDTH * 12, y, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT), true);
        }
    } else if (redraw_selected_index) {
        // Redraw only selected index.

        y += CHAR_HEIGHT; // Skip header.

        if (is_ch(prev_v_index)) {
            // Deselect previous index.
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + to_ch(prev_v_index),
                             "Channel %d:", to_ch(prev_v_index) + 1);
        }

        if (is_ch(v_index)) {
            with_bg_if(channel[to_ch(v_index)].selection() == ChannelWidget::DEFAULT, BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + to_ch(v_index),
                                 "Channel %d:", to_ch(v_index) + 1);
            }

            channel[to_ch(v_index)].draw(urect(bounds.x + CHAR_WIDTH * 12, y + CHAR_HEIGHT * to_ch(v_index),
                                               bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT),
                                         force);
        }
    } else {
        y += CHAR_HEIGHT; // Skip header.
        if (is_ch(v_index)) {
            with_bg_if(channel[to_ch(v_index)].selection() == ChannelWidget::DEFAULT, BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + to_ch(v_index),
                                 "Channel %d:", to_ch(v_index) + 1);
            }

            channel[to_ch(v_index)].draw(urect(bounds.x + CHAR_WIDTH * 12, y + CHAR_HEIGHT * to_ch(v_index),
                                               bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT),
                                         force);
        }
    }

    prev_v_index = v_index;
}

void SongPage::load_prev_song()
{
    if (song_index == 0) {
        return;
    }
    song_index--;
    load(song_data[song_index]);
}

void SongPage::load_next_song()
{
    if (song_index + 1 == NUM_SONGS) {
        return;
    }
    song_index++;
    load(song_data[song_index]);
}

void SongPage::playpause()
{
    // TODO
}
void SongPage::record()
{
    // TODO
}

////////////////////////////////
// ----- SettingsPage ----- //
////////////////////////////////

SettingsPage::SettingsPage()
    : autochord{AUTOCHORD_OPTIONS.data(), AUTOCHORD_OPTIONS.size()}
    , instrument{INSTRUMENT_NAMES.data(), INSTRUMENT_NAMES.size()}
    , key{notes::DIATONIC_NAMES.data(), notes::DIATONIC_NAMES.size()}
{
    // volume.set_value(50);
    // volume.set_inc(10);
    // volume.set_range(0, 100);

    transpose.set_value(0);
    transpose.set_range(-12, 12);
    transpose.on_value_changed(settings::set_transpose);

    autochord.on_value_changed(settings::set_autochord);

    instrument.on_value_changed(settings::set_instrument);

    key.set_index(3); // Init to C.
    key.on_value_changed(settings::set_key);

    metronome_div.set_value(4);
    metronome_div.set_range(2, 8);

    metronome_div.on_value_changed([](int32_t div) {
        extern Metronome metronome;
        metronome.set_division(div);
    });
}

bool SettingsPage::on_w(ButtonEvent)
{
    if (select_index > 0) {
        select_index -= 1;
        return true;
    }
    return false;
}

bool SettingsPage::on_s(ButtonEvent)
{
    if (select_index + 1 < max_index) {
        select_index += 1;
        return true;
    }
    return false;
}

bool SettingsPage::on_a(ButtonEvent)
{
    switch (select_index) {
        // case VOLUME: return truef(volume.down());
        case TRANSPOSE: return truef(transpose.down());
        case AUTO_CHORD: return truef(autochord.select_prev());
        case INSTRUMENT: return truef(instrument.select_prev());
        case KEY: return truef(key.select_prev());
        case METRONOME_DIV: return truef(metronome_div.down());
        default: return false;
    }
}

bool SettingsPage::on_d(ButtonEvent)
{
    switch (select_index) {
        // case VOLUME: return truef(volume.up());
        case TRANSPOSE: return truef(transpose.up());
        case AUTO_CHORD: return truef(autochord.select_next());
        case INSTRUMENT: return truef(instrument.select_next());
        case KEY: return truef(key.select_next());
        case METRONOME_DIV: return truef(metronome_div.up());
        default: return false;
    }
}

void SettingsPage::draw(const urect& bounds, bool force)
{
    extern LCD_ lcd;

    bool force_subdraw = force;

    if (prev_select_index != select_index) {
        prev_select_index = select_index;
        force_subdraw     = true;
    }

    size_t col2 = bounds.x + CHAR_WIDTH * 12;
    size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
    size_t y    = bounds.y;

    // Header row.
    if (force) {
        lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + 0, "Settings");
        lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + 1,
                        "------------------------------------------------------------");
    }

    y += CHAR_HEIGHT;

    auto subdraw = [=, this](size_t& y, auto& ctrl, uint8_t select, const char* label) {
        y += CHAR_HEIGHT;
        if (force_subdraw) {
            with_bg_if(select_index == select, BLUE) { lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, label); }
        }
        with_fg_if(select_index == select, BLUE, DARKGREY)
        {
            ctrl.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8), force_subdraw);
        }
    };

    // subdraw(y, volume, VOLUME, "Volume:");
    subdraw(y, transpose, TRANSPOSE, "Transpose:");
    subdraw(y, autochord, AUTO_CHORD, "Auto-Chord:");
    subdraw(y, instrument, INSTRUMENT, "Instrument:");
    subdraw(y, key, KEY, "Key:");
    subdraw(y, metronome_div, METRONOME_DIV, "Met. Div.:");
}


////////////////////////////////
// ----- ExportPage ----- //
////////////////////////////////

ExportPage::ExportPage() {}

bool ExportPage::on_w(ButtonEvent)
{
    if (select_index > 0) {
        select_index -= 1;
        return true;
    }
    return false;
}

bool ExportPage::on_s(ButtonEvent)
{
    if (select_index + 1 < max_index) {
        select_index += 1;
        return true;
    }
    return false;
}

bool ExportPage::on_a(ButtonEvent) { return false; }

bool ExportPage::on_d(ButtonEvent)
{
    if (select_index == 0) {
        kb::export_midi();
        return true;
    }
    return false;
}

void ExportPage::draw(const urect& bounds, bool force)
{
    extern LCD_ lcd;

    // bool force_subdraw = force;

    if (prev_select_index != select_index) {
        prev_select_index = select_index;
        // force_subdraw     = true;
    }

    // size_t col2 = bounds.x + CHAR_WIDTH * 12;
    // size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
    size_t y = bounds.y;

    // Header row.
    if (force) {
        lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + 0, "Export");
        lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + 1,
                        "------------------------------------------------------------");
    }

    y += CHAR_HEIGHT;
    if (force) {
        with_bg_if(select_index == EXPORT, BLUE)
        {
            lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT + 1, "Export (TTL)");
        }
    }
}


////////////////////////////////
// ----- MenuController ----- //
////////////////////////////////

void MenuController::update(bool force)
{
    draw_header({0, 0, LCD_WIDTH, 2 * CHAR_HEIGHT}, force);
    urect bounds = {0, 2 * CHAR_HEIGHT, LCD_WIDTH, LCD_HEIGHT};
    switch (page) {
        case PageName::HOME: draw_delegate(home_page, bounds, force); break;
        case PageName::SONG: draw_delegate(song_page, bounds, force); break;
        case PageName::SETTING: draw_delegate(settings_page, bounds, force); break;
        case PageName::EXPORT: draw_delegate(export_page, bounds, force); break;
        default: break;
    }
}

void MenuController::loop()
{
    switch (page) {
        case PageName::HOME: callback_delegate(home_page); break;
        case PageName::SONG: callback_delegate(song_page); break;
        case PageName::SETTING: callback_delegate(settings_page); break;
        case PageName::EXPORT: callback_delegate(export_page); break;
        default: break;
    }
    update(false);
}

void MenuController::go_to_page(PageName page)
{
    extern LCD_ lcd;
    if (this->page != page) {
        this->page = page;
        lcd.clear();
        draw(); // Force refresh.
        switch (page) {
            // case PageName::HOME: home_page.reset_selection(); break;
            case PageName::SETTING: settings_page.reset_selection(); break;
            case PageName::EXPORT: export_page.reset_selection(); break;
            default: break;
        }
    }
}

void MenuController::draw_header(const urect& bounds, bool force)
{
    extern LCD_ lcd;
    if (force) {
        lcd.draw_string(0, 1, "------------------------------------------------------------");
    }

    extern Metronome metronome;
    // if (metronome.count()) {
    lcd.draw_stringf(54, 0, "M: %d/%d", metronome.count() + 1, metronome.division());
    // }

    if (force || settings::prev().song != settings::curr().song) {
        lcd.draw_stringf(0, 0, "Song: %d/%d", settings::curr().song + 1, NUM_SONGS);
    }

    if (force || settings::prev().channel != settings::curr().channel) {
        lcd.draw_stringf(15, 0, "Ch: %d/%d", settings::curr().channel + 1, NUM_CHANNELS);
    }
}
