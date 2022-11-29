#include "display/menu.hpp"
#include "speaker.hpp"

//////////////////////////
// ----- HomePage ----- //
//////////////////////////

void HomePage::draw(const urect& bounds, bool force)
{
    extern LCD_ lcd;

    if (force) {
        lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 0, "Home");
    }

    if (force || prev_index != index.data) {
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

    size_t row = 18;
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
        prev_v_index          = v_index;
        redraw_header         = true;
        redraw_selected_index = true;
    }


    if (redraw_header) {
        with_bg_if(!is_ch(v_index), BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }
    }

    size_t y = bounds.y;

    if (redraw_all) {
        for (int i = 0; i < NUM_CHANNELS; i++) {
            y += CHAR_HEIGHT;
            with_bg_if(is_ch(v_index) && i == to_ch(v_index), BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Channel %d:", i + 1);
            }
            channel[i].draw(urect(bounds.x + CHAR_WIDTH * 12, y, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
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
    }
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
{
    volume.set_value(50);
    volume.set_inc(10);
    volume.set_range(0, 100);
    transpose.set_value(0);
    transpose.set_range(-12, 12);
}

bool SettingsPage::on_w()
{
    if (select_index > 0) {
        select_index -= 1;
        return true;
    }
    return false;
}

bool SettingsPage::on_s()
{
    if (select_index + 1 < max_index) {
        select_index += 1;
        return true;
    }
    return false;
}

bool SettingsPage::on_a()
{
    switch (select_index) {
        case VOLUME: return truef(volume.down());
        case TRANSPOSE: return truef(transpose.down());
        default: return false;
    }
}

bool SettingsPage::on_d()
{
    switch (select_index) {
        case VOLUME: return truef(volume.up());
        case TRANSPOSE: return truef(transpose.up());
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
        with_bg_if(select_index == DEFAULT, BLUE)
        {
            lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Settings");
        }
    }

    // Volume row.
    y += CHAR_HEIGHT;
    if (force) {
        with_bg_if(select_index == VOLUME, BLUE) { lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Volume:"); }
    }
    with_fg_if(select_index == VOLUME, BLUE, DARKGREY)
    {
        volume.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8), force_subdraw);
    }

    // Transpose row.
    y += CHAR_HEIGHT;
    if (force) {
        with_bg_if(select_index == TRANSPOSE, BLUE)
        {
            lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Transpose:");
        }
    }
    with_fg_if(select_index == TRANSPOSE, BLUE, DARKGREY)
    {
        transpose.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8), force_subdraw);
    }
}


////////////////////////////////
// ----- MenuController ----- //
////////////////////////////////

void MenuController::update(bool force)
{
    switch (page) {
        case PageName::HOME: draw_delegate(home_page, force); break;
        case PageName::SONG: draw_delegate(song_page, force); break;
        case PageName::SETTING: draw_delegate(settings_page, force); break;
        default: break;
    }
}

void MenuController::loop()
{
    switch (page) {
        case PageName::HOME:
            callback_delegate(home_page);
            draw_delegate(home_page, false);
            break;
        case PageName::SONG:
            callback_delegate(song_page);
            draw_delegate(song_page, false);
            break;
        case PageName::SETTING:
            callback_delegate(settings_page);
            draw_delegate(settings_page, false);
            break;
        default: break;
    }
}

void MenuController::go_to_page(PageName page)
{
    extern LCD_ lcd;
    if (this->page != page) {
        this->page = page;
        lcd.clear();
        draw(); // Force refresh.
        switch (page) {
            case PageName::HOME: home_page.reset_selection(); break;
            case PageName::SETTING: settings_page.reset_selection(); break;
            default: break;
        }
    }
}
