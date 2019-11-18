/*************************************************************************
| Dungeon Cartographer, a grid based map drawing program.                |
| Copyright (C) 2019  Alexander L. Hopper                                |
|                                                                        |
| This program is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU General Public License as published by   |
| the Free Software Foundation, either version 3 of the License, or      |
| (at your option) any later version.                                    |
|                                                                        |
| This program is distributed in the hope that it will be useful,        |
| but WITHOUT ANY WARRANTY; without even the implied warranty of         |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          |
| GNU General Public License for more details.                           |
|                                                                        |
| You should have received a copy of the GNU General Public License      |
| along with this program.  If not, see <https://www.gnu.org/licenses/>. |
*************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef unsigned short int uint16; //guaranteed at least 16 bits

//---------------------------
//----- Event Constants -----
//---------------------------
static const int SCROLL_UP   = -1;
static const int SCROLL_DOWN =  1;

//--------------------------------
//----- Tileset id constants -----
//--------------------------------
//The 1st nibble represents the walls      S| W| N| E
//The 2nd nibble represents the corners   NE|SE|SW|NW
//The 3rd nibble represents the doors      S| W| N| E
static const uint16 CORNER_BIT_OFFSET = 4;
static const uint16 DOOR_BIT_OFFSET   = 8;

//East wall variants
static const uint16 E         =         0b0001;
static const uint16 E_NW      =     0b00010001;
static const uint16 E_SW      =     0b00100001;
static const uint16 E_NW_SW   =     0b00110001;
static const uint16 E_E       = 0b000100000001;
static const uint16 E_NW_E    = 0b000100010001;
static const uint16 E_SW_E    = 0b000100100001;
static const uint16 E_NW_SW_E = 0b000100110001;

//North wall variants
static const uint16 N         =         0b0010;
static const uint16 N_SW      =     0b00100010;
static const uint16 N_SE      =     0b01000010;
static const uint16 N_SW_SE   =     0b01100010;
static const uint16 N_N       = 0b001000000010;
static const uint16 N_SW_N    = 0b001000100010;
static const uint16 N_SE_N    = 0b001001000010;
static const uint16 N_SW_SE_N = 0b001001100010;

//West wall variants
static const uint16 W         =         0b0100;
static const uint16 W_NE      =     0b10000100;
static const uint16 W_SE      =     0b01000100;
static const uint16 W_NE_SE   =     0b11000100;
static const uint16 W_W       = 0b010000000100;
static const uint16 W_NE_W    = 0b010010000100;
static const uint16 W_SE_W    = 0b010001000100;
static const uint16 W_NE_SE_W = 0b010011000100;

//South wall variant
static const uint16 S         =         0b1000;
static const uint16 S_NE      =     0b10001000;
static const uint16 S_NW      =     0b00011000;
static const uint16 S_NE_NW   =     0b10011000;
static const uint16 S_S       = 0b100000001000;
static const uint16 S_NE_S    = 0b100010001000;
static const uint16 S_NW_S    = 0b100000011000;
static const uint16 S_NE_NW_S = 0b100010011000;

//North-East wall variants
static const uint16 NE       =         0b0011;
static const uint16 NE_E     = 0b000100000011;
static const uint16 NE_N     = 0b001000000011;
static const uint16 NE_NE    = 0b001000010011;

static const uint16 NE_SW    =     0b00100011;
static const uint16 NE_SW_E  = 0b000100100011;
static const uint16 NE_SW_N  = 0b001000100011;
static const uint16 NE_SW_NE = 0b001100100011;

//South-East wall variants
static const uint16 SE       =         0b1001;
static const uint16 SE_E     = 0b000100001001;
static const uint16 SE_S     = 0b100000001001;
static const uint16 SE_SE    = 0b100100001001;

static const uint16 SE_NW    =     0b00011001;
static const uint16 SE_NW_E  = 0b000100011001;
static const uint16 SE_NW_S  = 0b100000011001;
static const uint16 SE_NW_SE = 0b100100011001;

//North-West wall variants
static const uint16 NW       =         0b0110;
static const uint16 NW_N     = 0b001000000110;
static const uint16 NW_W     = 0b010000000110;
static const uint16 NW_NW    = 0b011000000110;

static const uint16 NW_SE    =     0b01000110;
static const uint16 NW_SE_N  = 0b001001000110;
static const uint16 NW_SE_W  = 0b010001000110;
static const uint16 NW_SE_NW = 0b011001000110;

//South-West wall variants
static const uint16 SW       =         0b1100;
static const uint16 SW_W     = 0b010000001100;
static const uint16 SW_S     = 0b100000001100;
static const uint16 SW_SW    = 0b110000001100;

static const uint16 SW_NE    =     0b10001100;
static const uint16 SW_NE_W  = 0b010010001100;
static const uint16 SW_NE_S  = 0b100010001100;
static const uint16 SW_NE_SW = 0b110010001100;

//West-East wall variants
static const uint16 EW    =         0b0101;
static const uint16 EW_E  = 0b000100000101;
static const uint16 EW_W  = 0b010000000101;
static const uint16 EW_EW = 0b010100000101;

//North-South wall variants
static const uint16 NS    =         0b1010;
static const uint16 NS_N  = 0b001000001010;
static const uint16 NS_S  = 0b100000001010;
static const uint16 NS_NS = 0b101000001010;

//North-East-West wall variants
static const uint16 NEW     =         0b0111;
static const uint16 NEW_E   = 0b000100000111;
static const uint16 NEW_N   = 0b001000000111;
static const uint16 NEW_W   = 0b010000000111;
static const uint16 NEW_NE  = 0b001100000111;
static const uint16 NEW_EW  = 0b010100000111;
static const uint16 NEW_NW  = 0b011000000111;
static const uint16 NEW_NEW = 0b011100000111;

//North-South-East wall variants
static const uint16 NSE     =         0b1011;
static const uint16 NSE_E   = 0b000100001011;
static const uint16 NSE_N   = 0b001000001011;
static const uint16 NSE_S   = 0b100000001011;
static const uint16 NSE_NE  = 0b001100001011;
static const uint16 NSE_SE  = 0b010100001011;
static const uint16 NSE_NS  = 0b101000001011;
static const uint16 NSE_NSE = 0b101100001011;

//South-East-West wall variants
static const uint16 SEW     =         0b1101;
static const uint16 SEW_E   = 0b000100001101;
static const uint16 SEW_W   = 0b010000001101;
static const uint16 SEW_S   = 0b100000001101;
static const uint16 SEW_EW  = 0b010100001101;
static const uint16 SEW_SE  = 0b100100001101;
static const uint16 SEW_SW  = 0b110000001101;
static const uint16 SEW_SEW = 0b110100001101;

//North-South-West wall variants
static const uint16 NSW     =         0b1110;
static const uint16 NSW_N   = 0b001000001110;
static const uint16 NSW_W   = 0b010000001110;
static const uint16 NSW_S   = 0b100000001110;
static const uint16 NSW_NW  = 0b011000001110;
static const uint16 NSW_NS  = 0b101000001110;
static const uint16 NSW_SW  = 0b110000001110;
static const uint16 NSW_NSW = 0b111000001110;

//North-South-East-West wall variants
static const uint16 NSEW      =         0b1111;
static const uint16 NSEW_E    = 0b000100001111;
static const uint16 NSEW_N    = 0b001000001111;
static const uint16 NSEW_W    = 0b010000001111;
static const uint16 NSEW_S    = 0b100000001111;
static const uint16 NSEW_NE   = 0b001100001111;
static const uint16 NSEW_EW   = 0b010100001111;
static const uint16 NSEW_SE   = 0b100100001111;
static const uint16 NSEW_NW   = 0b011000001111;
static const uint16 NSEW_NS   = 0b101000001111;
static const uint16 NSEW_SW   = 0b110000001111;
static const uint16 NSEW_NEW  = 0b011100001111;
static const uint16 NSEW_NSE  = 0b101100001111;
static const uint16 NSEW_NSW  = 0b111000001111;
static const uint16 NSEW_SEW  = 0b110100001111;
static const uint16 NSEW_NSEW = 0b111100001111;

//No wall variants
static const uint16 O =                 0b0000;
static const uint16 O_NE =          0b10000000;
static const uint16 O_NW =          0b00010000;
static const uint16 O_NE_NW =       0b10010000;
static const uint16 O_SW =          0b00100000;
static const uint16 O_NE_SW =       0b10100000;
static const uint16 O_NW_SW =       0b00110000;
static const uint16 O_NE_NW_SW =    0b10110000;
static const uint16 O_SE =          0b01000000;
static const uint16 O_NE_SE =       0b11000000;
static const uint16 O_NW_SE =       0b01010000;
static const uint16 O_NE_NW_SE =    0b11010000;
static const uint16 O_SW_SE =       0b01100000;
static const uint16 O_NE_SW_SE =    0b11100000;
static const uint16 O_NW_SW_SE =    0b01110000;
static const uint16 O_NE_NW_SW_SE = 0b11110000;

static const uint16 BACKGROUND = 0b111111111111;

#endif
