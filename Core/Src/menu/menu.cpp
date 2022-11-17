#pragma once

#include "display/menu.hpp"


void SongPage::load(SongData& data)
{
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
    draw(urect(bounds.x, bounds.y + CHAR_HEIGHT, bounds.w, bounds.h - CHAR_HEIGHT));

    for (int i = 0; i < NUM_CHANNELS; i++) {
        with_bg_if(is_ch(v_index) && i == to_ch(v_index), BLUE)
        {
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT + i, "Channel %d:", i + 1);
        }
        channel[i].draw(
            urect(bounds.x + CHAR_WIDTH * 12, bounds.y + CHAR_HEIGHT * i, bounds.w - CHAR_WIDTH * 12, CHAR_HEIGHT));
    }
}

void SongPage::update(const urect& bounds)
{
    extern LCD_ lcd;
    lcd.draw_stringf(0, 12, "song index: %d", song_index);
    lcd.draw_stringf(0, 13, "select index: %d", v_index);

    if (prev_song_index != song_index) {
        prev_song_index = song_index;
        lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "Song %d", song_index + 1);
    }

    size_t ch_y = bounds.y + CHAR_HEIGHT;

    if (prev_v_index != v_index) {
        prev_v_index = v_index;

        if (is_ch(prev_v_index)) {
            // Deselect previous index.
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(prev_v_index),
                             "Channel %d:", to_ch(prev_v_index) + 1);
        }
        if (is_ch(v_index)) {
            // Select current index.
            with_bg(BLUE)
            {
                lcd.draw_stringf(bounds.x / CHAR_WIDTH, ch_y / CHAR_HEIGHT + to_ch(v_index),
                                 "Channel %d:", to_ch(v_index) + 1);
            }
        }
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

void SongPage::playpause() {}
void SongPage::record() {}