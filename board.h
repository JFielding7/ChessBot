//
// Created by joe on 5/3/24.
//

#ifndef CHESSBOT_BOARD_H
#define CHESSBOT_BOARD_H

#define START_POSITION malloc(sizeof (board_t));\
board->white_pawns   = 0b0000000000000000000000000000000000000000000000001111111100000000;\
board->white_knights = 0b0000000000000000000000000000000000000000000000000000000001000010;\
board->white_bishops = 0b0000000000000000000000000000000000000000000000000000000000100100;\
board->white_rooks   = 0b0000000000000000000000000000000000000000000000000000000010000001;\
board->white_queen   = 0b0000000000000000000000000000000000000000000000000000000000001000;\
board->white_king    = 0b0000000000000000000000000000000000000000000000000000000000010000;\
board->black_pawns   = 0b0000000011111111000000000000000000000000000000000000000000000000;\
board->black_knights = 0b0100001000000000000000000000000000000000000000000000000000000000;\
board->black_bishops = 0b0010010000000000000000000000000000000000000000000000000000000000;\
board->black_rooks   = 0b1000000100000000000000000000000000000000000000000000000000000000;\
board->black_queen   = 0b0000100000000000000000000000000000000000000000000000000000000000;\
board->black_king    = 0b0001000000000000000000000000000000000000000000000000000000000000

#define location unsigned long
#define SQUARES 64
#define BOARD_LENGTH 8
#define MAX_MOVES 4096
#define RANK2 1 << 16


typedef struct board_s {
    unsigned long white_pawns;
    unsigned long white_knights;
    unsigned long white_bishops;
    unsigned long white_rooks;
    unsigned long white_queen;
    unsigned long white_king;
    unsigned long black_pawns;
    unsigned long black_knights;
    unsigned long black_bishops;
    unsigned long black_rooks;
    unsigned long black_queen;
    unsigned long black_king;
    unsigned int state;
} board_t;

#endif //CHESSBOT_BOARD_H
