#pragma once

#include <cstdint>


////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////

enum Orientation {
    PIN_ON_TOP,
    PIN_ON_LEFT,
    PIN_ON_BOTTOM,
    PIN_ON_RIGHT,
};

inline constexpr Orientation LCD_ORIENTATION = PIN_ON_LEFT;


////////////////////////////////////////////////////////////////
/// COLOURS
////////////////////////////////////////////////////////////////

using color_t = uint32_t;

constexpr uint32_t rgb18(uint8_t r, uint8_t g, uint8_t b)
{
    // return (uint32_t(r & 0xFC) << 10) | (uint32_t(g & 0xFC) << 4) | (b >> 2);
    return (uint32_t(r) << 16) | (uint32_t(g) << 8) | b;
}

inline constexpr color_t BLACK       = rgb18(0, 0, 0);
inline constexpr color_t NAVY        = rgb18(0, 0, 128);
inline constexpr color_t DARKGREEN   = rgb18(0, 128, 0);
inline constexpr color_t DARKCYAN    = rgb18(0, 128, 128);
inline constexpr color_t MAROON      = rgb18(128, 0, 0);
inline constexpr color_t PURPLE      = rgb18(128, 0, 128);
inline constexpr color_t OLIVE       = rgb18(128, 128, 0);
inline constexpr color_t LIGHTGREY   = rgb18(192, 192, 192);
inline constexpr color_t DARKGREY    = rgb18(128, 128, 128);
inline constexpr color_t BLUE        = rgb18(0, 0, 255);
inline constexpr color_t GREEN       = rgb18(0, 255, 0);
inline constexpr color_t CYAN        = rgb18(0, 255, 255);
inline constexpr color_t RED         = rgb18(255, 0, 0);
inline constexpr color_t MAGENTA     = rgb18(255, 0, 255);
inline constexpr color_t YELLOW      = rgb18(255, 255, 0);
inline constexpr color_t WHITE       = rgb18(255, 255, 255);
inline constexpr color_t ORANGE      = rgb18(255, 165, 0);
inline constexpr color_t GREENYELLOW = rgb18(173, 255, 47);
inline constexpr color_t PINK        = MAGENTA;

////////////////////////////////////////////////////////////////
/// ORIENTATION
////////////////////////////////////////////////////////////////

static constexpr uint8_t TFT_ORIENTATION_CMD = 0x36;
static constexpr uint8_t TFT_EOF_MARKER      = 0x42;
static constexpr uint8_t TFT_EOL_MARKER      = 0x43;
static constexpr uint8_t TFT_DELAY_MARKER    = 0xFF;


template <Orientation ORIENTATION>
constexpr uint8_t orientation_cmd()
{
    switch (ORIENTATION) {
        case PIN_ON_TOP: return 0x48;
        case PIN_ON_LEFT: return 0x28;
        case PIN_ON_BOTTOM: return 0x88;
        case PIN_ON_RIGHT: return 0xE8;
    }
}

template <Orientation ORIENTATION>
static constexpr uint8_t ili9486_init_seq[] = {
    0x00, 0x01,
    // Interface Mode Control
    0xb0, 0x0, TFT_EOL_MARKER,
    // Sleep OUT
    0x11, TFT_DELAY_MARKER, 100, TFT_EOL_MARKER,
    // Interface Pixel Format, 16 bits / pixel
    0x3A, 0x66, TFT_EOL_MARKER,
    // Power control VRH[5:0] = 4.60 V
    0xC0, 0x23, TFT_EOL_MARKER,
    // Power control SAP[2:0];BT[3:0] = adjust power to lowest possible
    0xC1, 0x10, TFT_EOL_MARKER,
    // Power Control 3 (For Normal Mode)
    0xC2, 0x44, TFT_EOL_MARKER,
    // VCOM Control
    0xC5, 0x00, 0x00, 0x00, 0x00, TFT_EOL_MARKER,
    // PGAMCTRL(Positive Gamma Control)
    0xE0, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00, TFT_EOL_MARKER,
    // NGAMCTRL (Negative Gamma Correction)
    0xE1, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00, TFT_EOL_MARKER,
    // Digital Gamma Control 1
    0xE2, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00, TFT_EOL_MARKER,
    // Memory Access Control
    TFT_ORIENTATION_CMD, orientation_cmd<ORIENTATION>(), TFT_EOL_MARKER,
    // Exit sleep
    0x11, TFT_DELAY_MARKER, 100, TFT_EOL_MARKER, 0x29, TFT_DELAY_MARKER, 100, TFT_EOL_MARKER, 0x2c, TFT_EOL_MARKER,
    TFT_EOF_MARKER};


////////////////////////////////////////////////////////////////
/// CONSTANTS
////////////////////////////////////////////////////////////////

inline constexpr int LCD_WIDTH  = 480;
inline constexpr int LCD_HEIGHT = 320;

inline constexpr uint16_t CHAR_WIDTH  = 8;
inline constexpr uint16_t CHAR_HEIGHT = 16;

inline constexpr uint32_t BUF_CHARS   = 8; // Number of chars to buffer onto the LCD.
inline constexpr uint32_t PIXEL_BYTES = 3; // Number of bytes per pixel.
inline constexpr uint32_t BUF_SIZE =
    BUF_CHARS * CHAR_WIDTH * CHAR_HEIGHT * PIXEL_BYTES; // Number of bytes to buffer before TXing to the LCD.
