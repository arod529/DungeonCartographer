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

typedef unsigned int uint;

//---------------------------
//----- Event Constants -----
//---------------------------
static const int SCROLL_UP = -1;
static const int SCROLL_DOWN = 1;

//--------------------------------
//----- Tileset id constants -----
//--------------------------------
static const uint CORNER_BIT_OFFSET = 4;

static const uint E =           0b0001;
static const uint E_NW =    0b00010001;
static const uint E_SW =    0b00100001;
static const uint E_NW_SW = 0b00110001;

static const uint N =           0b0010;
static const uint N_SW =    0b00100010;
static const uint N_SE =    0b01000010;
static const uint N_SW_SE = 0b01100010;

static const uint W =           0b0100;
static const uint W_NE =    0b10000100;
static const uint W_SE =    0b01000100;
static const uint W_NE_SE = 0b11000100;

static const uint S =           0b1000;
static const uint S_NE =    0b10001000;
static const uint S_NW =    0b00011000;
static const uint S_NE_NW = 0b10011000;

static const uint NE =        0b0011;
static const uint NE_SW = 0b00100011;
static const uint SE =        0b1001;
static const uint SE_NW = 0b00011001;
static const uint NW =        0b0110;
static const uint NW_SE = 0b01000110;
static const uint SW =        0b1100;
static const uint SW_NE = 0b10001100;

static const uint WE = 0b0101;
static const uint NS = 0b1010;

static const uint NEW = 0b0111;
static const uint NSE = 0b1011;
static const uint SEW = 0b1101;
static const uint NSW = 0b1110;

static const uint NSEW = 0b1111;

static const uint O =                 0b0000;
static const uint O_NE =          0b10000000;
static const uint O_NW =          0b00010000;
static const uint O_NE_NW =       0b10010000;
static const uint O_SW =          0b00100000;
static const uint O_NE_SW =       0b10100000;
static const uint O_NW_SW =       0b00110000;
static const uint O_NE_NW_SW =    0b10110000;
static const uint O_SE =          0b01000000;
static const uint O_NE_SE =       0b11000000;
static const uint O_NW_SE =       0b01010000;
static const uint O_NE_NW_SE =    0b11010000;
static const uint O_SW_SE =       0b01100000;
static const uint O_NE_SW_SE =    0b11100000;
static const uint O_NW_SW_SE =    0b01110000;
static const uint O_NE_NW_SW_SE = 0b11110000;

static const uint BACKGROUND = 0b11111111;

#endif
