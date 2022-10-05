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


////////////////////////////////////////////////////////////////
/// COLOURS
////////////////////////////////////////////////////////////////

using color_t = uint16_t;

constexpr uint16_t rgb16(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t(r & 0xF8) << 8) | (uint16_t(g & 0xFC) << 3) | (b >> 3);
}

#define BLACK       rgb16(0, 0, 0)
#define NAVY        rgb16(0, 0, 128)
#define DARKGREEN   rgb16(0, 128, 0)
#define DARKCYAN    rgb16(0, 128, 128)
#define MAROON      rgb16(128, 0, 0)
#define PURPLE      rgb16(128, 0, 128)
#define OLIVE       rgb16(128, 128, 0)
#define LIGHTGREY   rgb16(192, 192, 192)
#define DARKGREY    rgb16(128, 128, 128)
#define BLUE        rgb16(0, 0, 255)
#define GREEN       rgb16(0, 255, 0)
#define CYAN        rgb16(0, 255, 255)
#define RED         rgb16(255, 0, 0)
#define MAGENTA     rgb16(255, 0, 255)
#define YELLOW      rgb16(255, 255, 0)
#define WHITE       rgb16(255, 255, 255)
#define ORANGE      rgb16(255, 165, 0)
#define GREENYELLOW rgb16(173, 255, 47)
#define PINK        MAGENTA

////////////////////////////////////////////////////////////////
/// ORIENTATION
////////////////////////////////////////////////////////////////

static constexpr uint8_t TFT_ORIENTATION_CMD = 0x36;
static constexpr uint8_t TFT_EOF_MARKER = 0x42;
static constexpr uint8_t TFT_EOL_MARKER = 0x43;
static constexpr uint8_t TFT_DELAY_MARKER = 0xFF;


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
    // Reset
    0x01,
    // Interface Mode Control
    0xB0, 0x0,
    // // Sleep OUT
    // 0x11,
    // Interface Pixel Format, 16 bits / pixel
    0x3A, 0x55,
    // Power control VRH[5:0] = 4.60 V
    0xC0, 0x23,
    // Power control SAP[2:0];BT[3:0] = adjust power to lowest possible
    0xC1, 0x10,
    // Power Control 3 (For Normal Mode)
    0xC2, 0x44,
    // VCOM Control
    0xC5, 0x00, 0x00, 0x00, 0x00,
    // PGAMCTRL(Positive Gamma Control)
    0xE0, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
    // NGAMCTRL (Negative Gamma Correction)
    0xE1, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
    // Digital Gamma Control 1
    0xE2, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
    // Memory Access Control
    orientation_cmd<ORIENTATION>(),
    // Display ON.
    0x29,
    // Sleep OUT
    0x11,
    //
    // 0x2c, TFT_EOL_MARKER,
};


////////////////////////////////////////////////////////////////
/// CONSTANTS
////////////////////////////////////////////////////////////////

inline constexpr int LCD_WIDTH = 320;
inline constexpr int LCD_HEIGHT = 480;

inline constexpr int CHAR_WIDTH = 8;
inline constexpr int CHAR_HEIGHT = 16;

inline constexpr uint32_t BUF_CHARS = 8; // Number of chars to buffer onto the LCD.
inline constexpr uint32_t BUF_SIZE =
    BUF_CHARS * CHAR_WIDTH * CHAR_HEIGHT * sizeof(color_t); // Number of bytes to buffer before TXing to the LCD.
