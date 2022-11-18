// #include "display.hpp"


// // void Menu::draw_circle(uint16_t usC, uint16_t usP, uint16_t SR, color_t usColor)
// // {
// //     int16_t f     = 1 - SR;
// //     int16_t ddF_x = 1;
// //     int16_t ddF_y = -2 * SR;
// //     int16_t x     = 0;
// //     int16_t y     = SR;

// //     lcd.draw_pixel(usC, usP + SR, usColor);
// //     lcd.draw_pixel(usC, usP - SR, usColor);
// //     lcd.draw_pixel(usC + SR, usP, usColor);
// //     lcd.draw_pixel(usC - SR, usP, usColor);

// //     while (x < y) {
// //         if (f >= 0) {
// //             y--;
// //             ddF_y += 2;
// //             f += ddF_y;
// //         }
// //         x++;
// //         ddF_x += 2;
// //         f += ddF_x;

// //         lcd.draw_pixel(usC + x, usP + y, usColor);
// //         lcd.draw_pixel(usC - x, usP + y, usColor);
// //         lcd.draw_pixel(usC + x, usP - y, usColor);
// //         lcd.draw_pixel(usC - x, usP - y, usColor);
// //         lcd.draw_pixel(usC + y, usP + x, usColor);
// //         lcd.draw_pixel(usC - y, usP + x, usColor);
// //         lcd.draw_pixel(usC + y, usP - x, usColor);
// //         lcd.draw_pixel(usC - y, usP - x, usColor);
// //     }
// // }
// // void Menu::init()
// // {
// //     lcd.init();
// //     lcd.clear();
// // }

// // void Menu::record(bool playback, bool countin) {}
// // // Play selected song.
// // void Menu::toggle_play_stop() {}
// // void Menu::clear_all_song() {}
// // void Menu::set_song(int song) {}
// // void Menu::clear() {} // Delete all music data in the selected song.
// // void Menu::set_channel() {}
// // void Menu::set_volume(uint16_t volume) {} // Volume, 0-100.
// // void Menu::toggle_mute() {}
// // void Menu::toggle_solo() {}
// // void Menu::change_instrument() {}
// // void Menu::clear_channel_data() {} // Delete music data in the selected channel.
// // void Menu::Export() {}
// // void Menu::Import() {}
// // void Menu::set_transpose(int16_t semitones) {}
// // void Menu::set_tempo(uint16_t bpm) {}
// // void Menu::set_time_signature(uint16_t n, uint16_t d) {}
// // void Menu::set_bitrate(uint32_t br) {}
// // void Menu::set_power_saving(bool on) {}
// // void Menu::set_fx(Effect) {}
// // void Menu::set_reverb(uint16_t wet) {} // 0-100.
// // void Menu::set_delay(uint16_t wet) {}  // 0-100.
// // void Menu::set_eq(Equaliser) {}
// // void Menu::enter()
// // {
// //     lcd.clear();
// //     switch (page) {
// //         case HOME:
// //             page     = (PAGE_NAME)(selected + 1);
// //             selected = 0;
// //             break;
// //         case SONG:
// //             switch (selected) {
// //                 case 0:
// //                     // start recording
// //                     record(0, 0);
// //                     break;
// //                 case 1:
// //                     // play/stop selected song
// //                     toggle_play_stop();
// //                     break;
// //                 case 2:
// //                     // Clear all song
// //                     clear_all_song();
// //                     break;

// //                 default: set_song(selected - 3); break;
// //             }
// //             break;
// //         case CHANNEL:
// //             switch (selected) {
// //                 case 0:
// //                     /* Set Channel */
// //                     set_channel();
// //                     break;
// //                 case 1:
// //                     /* Adjust_volume */
// //                     set_volume(100);
// //                     break;
// //                 case 2:
// //                     /* Toggle_mute */
// //                     toggle_mute();
// //                     break;
// //                 case 3:
// //                     /* Toggle_solo */
// //                     toggle_solo();
// //                     break;
// //                 case 4:
// //                     /* Change_instrument */
// //                     change_instrument();
// //                     break;
// //                 case 5:
// //                     /* Clear */           // Delete music data in the selected channel.
// //                     clear_channel_data(); // Delete music data in the selected channel.
// //                     break;

// //                 default: break;
// //             }
// //             break;
// //         case EXPORT:
// //             switch (selected) {
// //                 case 0:
// //                     /* Export */
// //                     Export();
// //                     break;
// //                 case 1:
// //                     // Import
// //                     Import();
// //                     break;
// //                 default: break;
// //             }
// //             break;
// //         case SETTING:
// //             switch (selected) {
// //                 case 0:
// //                     /* Set_transpose */
// //                     set_transpose(0);
// //                     break;

// //                 case 1:
// //                     /* Set_tempo */
// //                     set_tempo(1);
// //                     break;
// //                 case 2:
// //                     /* Set_time_signature */
// //                     set_time_signature(0, 0);
// //                     break;
// //                 case 3:
// //                     /* Set_bitrate */
// //                     set_bitrate(0);
// //                     break;
// //                 case 4:
// //                     /* Set_power_saving */
// //                     set_power_saving(1);
// //                     break;
// //                 default: break;
// //             }
// //             break;
// //         case EFFECTS:
// //             switch (selected) {
// //                 case 0:
// //                     /* Set_fx */
// //                     set_fx(DEFAULT);
// //                     break;
// //                 case 1:
// //                     /* Set_reverb */
// //                     set_reverb(0); // 0-100.
// //                     break;
// //                 case 2:
// //                     /* Set_delay */
// //                     set_delay(0); // 0-100.
// //                     break;
// //                 case 3:
// //                     /* Set_eq */
// //                     set_eq(NO_FILTER);
// //                     break;

// //                 default: break;
// //             }
// //             break;
// //     }
// // }

// // void Menu::plus() {}
// // void Menu::minus() {}

// // void Menu::exit()
// // {
// //     lcd.clear();
// //     page     = HOME;
// //     selected = 0;
// // }

// // void Menu::tick()
// // {
// //     int row = 0;
// //     draw_circle(8, selected * 16 + 22, 3, WHITE);
// //     switch (page) {
// //         case HOME:
// //             lcd.draw_string(0, row++, "Please select the setting that you want to modify:", selected);
// //             lcd.draw_string(2, row++, "SONG %d", selected);
// //             lcd.draw_string(2, row++, "CHANNEL");
// //             lcd.draw_string(2, row++, "EXPORT");
// //             lcd.draw_string(2, row++, "SETTING");
// //             lcd.draw_string(2, row++, "EFFECTS");
// //             break;
// //         case SONG:
// //             /* code */

// //             // Play selected song.
// //             lcd.draw_string(0, row++, "Song Settings:");
// //             lcd.draw_string(2, row++, "Record");
// //             lcd.draw_string(2, row++, "Play/Stop");
// //             lcd.draw_string(2, row++, "Clear all song"); // Delete all music data in the selected song.
// //             lcd.draw_string(2, row++, "Song List: ");
// //             // display function for all song
// //             break;
// //         case CHANNEL:
// //             /* code */
// //             lcd.draw_string(0, row++, "Channel Settings:");
// //             lcd.draw_string(2, row++, "Set Channel");
// //             lcd.draw_string(2, row++, "Adjust_volume"); // Volume, 0-100.
// //             lcd.draw_string(2, row++, "Toggle_mute");
// //             lcd.draw_string(2, row++, "Toggle_solo");
// //             lcd.draw_string(2, row++, "Change_instrument");

// //             lcd.draw_string(2, row++, "Clear"); // Delete music data in the selected channel.
// //             break;
// //         case EXPORT:
// //             /* code */
// //             lcd.draw_string(0, row++, "FileIO:");
// //             lcd.draw_string(2, row++, "Export");
// //             lcd.draw_string(2, row++, "Import");
// //             break;
// //         case SETTING:
// //             /* code */
// //             lcd.draw_string(0, row++, "Basic Settings:");
// //             lcd.draw_string(2, row++, "Set_transpose");
// //             lcd.draw_string(2, row++, "Set_tempo");
// //             lcd.draw_string(2, row++, "Set_time_signature");
// //             lcd.draw_string(2, row++, "Set_bitrate");
// //             lcd.draw_string(2, row++, "Set_power_saving");
// //             break;
// //         case EFFECTS:
// //             lcd.draw_string(0, row++, "Effect Settings:");
// //             lcd.draw_string(2, row++, "Set_fx");
// //             lcd.draw_string(2, row++, "Set_reverb"); // 0-100.
// //             lcd.draw_string(2, row++, "Set_delay");  // 0-100.
// //             lcd.draw_string(2, row++, "Set_Equaliser");
// //             break;
// //         default: break;
// //     }
// // }