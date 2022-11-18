#include "display/menu.hpp"

//////////////////////////
// ----- HomePage ----- //
//////////////////////////

void HomePage::draw(const urect& bounds)
{
    extern LCD_ lcd;
    lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 0, "Home");
    lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 1, "Song Menu");
    lcd.draw_string(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + 2, "Settings Menu");
}
void HomePage::update(const urect& bounds)
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

void SongPage::draw(const urect& bounds)
{
    extern LCD_ lcd;
    with_bg_if(!is_ch(v_index), BLUE)
    {
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
    }

    size_t y = bounds.y;

    for (int i = 0; i < NUM_CHANNELS; i++) {
        y += CHAR_HEIGHT;
        with_bg_if(is_ch(v_index) && i == to_ch(v_index), BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Channel %d:", i + 1);
        }
        channel[i].draw(urect(bounds.x + CHAR_WIDTH * 12, y, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
    }
}

void SongPage::update(const urect& bounds)
{
    extern LCD_ lcd;

    if (prev_song_index != song_index) {
        prev_song_index = song_index;
        with_bg(BLUE) { lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1); }
    }

    size_t ch_y = bounds.y + CHAR_HEIGHT;

    if (prev_v_index != v_index) {
        with_bg_if(v_index == 0, BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
        }

        if (is_ch(prev_v_index)) {
            // Deselect previous index.
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(prev_v_index),
                             "Channel %d:", to_ch(prev_v_index) + 1);
        }
        prev_v_index = v_index;
    }
    if (is_ch(v_index)) {
        with_bg_if(channel[to_ch(v_index)].selection() == ChannelWidget::DEFAULT, BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(v_index),
                             "Channel %d:", to_ch(v_index) + 1);
        }

        channel[to_ch(v_index)].update(urect(bounds.x + CHAR_WIDTH * 12, ch_y + CHAR_HEIGHT * to_ch(v_index),
                                             bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
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

void SettingsPage::draw(const urect& bounds)
{
    extern LCD_ lcd;

    size_t col2 = bounds.x + CHAR_WIDTH * 12;
    size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
    size_t y    = bounds.y;

    with_bg_if(select_index == DEFAULT, BLUE) { lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Settings"); }

    y += CHAR_HEIGHT;
    with_bg_if(select_index == VOLUME, BLUE) { lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Volume:"); }
    with_fg_if(select_index == VOLUME, BLUE, DARKGREY) { volume.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8)); }

    y += CHAR_HEIGHT;
    with_bg_if(select_index == TRANSPOSE, BLUE)
    {
        lcd.draw_string(bounds.x / CHAR_WIDTH, y / CHAR_HEIGHT, "Transpose:");
    }
    with_fg_if(select_index == TRANSPOSE, BLUE, DARKGREY) { transpose.draw(urect(col2, y + 4, w2, CHAR_HEIGHT - 8)); }
}

void SettingsPage::update(const urect& bounds)
{
    if (prev_select_index != select_index) {
        prev_select_index = select_index;
        draw(bounds);
    } else {
        extern LCD_ lcd;
        size_t col2 = bounds.x + CHAR_WIDTH * 12;
        size_t w2   = (bounds.w - CHAR_WIDTH * 12) * 3 / 5;
        size_t y    = bounds.y;
        y += CHAR_HEIGHT;
        with_fg_if(select_index == VOLUME, BLUE, DARKGREY) { volume.update(urect(col2, y + 4, w2, CHAR_HEIGHT - 8)); }
        y += CHAR_HEIGHT;
        with_fg_if(select_index == TRANSPOSE, BLUE, DARKGREY)
        {
            transpose.update(urect(col2, y + 4, w2, CHAR_HEIGHT - 8));
        }
    }
}


////////////////////////////////
// ----- MenuController ----- //
////////////////////////////////

void MenuController::draw()
{
    switch (page) {
        case PageName::HOME: draw_delegate(home_page); break;
        case PageName::SONG: draw_delegate(song_page); break;
        case PageName::SETTING: draw_delegate(settings_page); break;
        default: break;
    }
}

void MenuController::loop()
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

void MenuController::go_to_page(PageName page)
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
