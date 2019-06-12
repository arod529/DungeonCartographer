#ifndef CONSTANTS_H
#define CONSTANTS_H

static const uint E_WALL =  0b0001;
static const uint E_HALL = 0b10001;
static const uint N_WALL =  0b0010;
static const uint N_HALL = 0b10010;
static const uint W_WALL =  0b0100;
static const uint W_HALL = 0b10100;
static const uint S_WALL =  0b1000;
static const uint S_HALL = 0b11000;

static const uint NE_WALL =  0b0011;
static const uint NE_HALL = 0b10011;
static const uint SE_WALL =  0b1001;
static const uint SE_HALL = 0b11001;
static const uint NW_WALL =  0b0110;
static const uint NW_HALL = 0b10110;
static const uint SW_WALL =  0b1100;
static const uint SW_HALL = 0b11100;

static const uint WE_WALL = 0b0101;
static const uint NS_HALL = 0b1010;

static const uint NEW_WALL = 0b0111;
static const uint NSE_WALL = 0b1011;
static const uint SEW_WALL = 0b1101;
static const uint NSW_WALL = 0b1110;

static const uint NSEW_WALL = 0b1111;

static const uint OPEN       =    0b0000;
static const uint E_OPEN     =   0b10000;
static const uint N_OPEN     =  0b100000;
static const uint W_OPEN     =  0b110000;
static const uint S_OPEN     = 0b1010000;
static const uint NSEW_OPEN  = 0b1010000;
static const uint BACKGROUND = 0b1110000;

#endif