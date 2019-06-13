#ifndef CONSTANTS_H
#define CONSTANTS_H

static const uint E =           0b0001;
static const uint E_NW =    0b00100001;
static const uint E_SW =    0b01000001;
static const uint E_NW_SW = 0b01100001;

static const uint N =           0b0010;
static const uint N_SW =    0b01000010;
static const uint N_SE =    0b10000010;
static const uint N_SW_SE = 0b11000010;

static const uint W =           0b0100;
static const uint W_NE =    0b00010100;
static const uint W_SE =    0b10000100;
static const uint W_NE_SE = 0b10010100;

static const uint S =           0b1000;
static const uint S_NE =    0b00011000;
static const uint S_NW =    0b00101000;
static const uint S_NE_NW = 0b00111000;

static const uint NE =        0b0011;
static const uint NE_SW = 0b01000011;
static const uint SE =        0b1001;
static const uint SE_NW = 0b01001001;
static const uint NW =        0b0110;
static const uint NW_SE = 0b10000110;
static const uint SW =        0b1100;
static const uint SW_NE = 0b00011100;

static const uint WE = 0b0101;
static const uint NS = 0b1010;

static const uint NEW = 0b0111;
static const uint NSE = 0b1011;
static const uint SEW = 0b1101;
static const uint NSW = 0b1110;

static const uint NSEW = 0b1111;

static const uint O =                 0b0000;
static const uint O_NE =          0b00010000;
static const uint O_NW =          0b00100000;
static const uint O_NE_NW =       0b00110000;
static const uint O_SW =          0b01000000;
static const uint O_NE_SW =       0b01010000;
static const uint O_NW_SW =       0b01100000;
static const uint O_NE_NW_SW =    0b01110000;
static const uint O_SE =          0b10000000;
static const uint O_NE_SE =       0b10010000;
static const uint O_NW_SE =       0b10100000;
static const uint O_NE_NW_SE =    0b10110000;
static const uint O_SW_SE =       0b11000000;
static const uint O_NE_SW_SE =    0b11010000;
static const uint O_NW_SW_SE =    0b11100000;
static const uint O_NE_NW_SW_SE = 0b11110000;

static const uint BACKGROUND = 0b11111111;

#endif
