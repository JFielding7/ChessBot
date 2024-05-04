//
// Created by joe on 4/30/24.
//

#include <stdbool.h>
#include <memory.h>
#include <malloc.h>

#include "board.h"

void add_if_legal_advance(board_t board, location pawn, location pawns, location pieces, board_t* move_list, int* idx) {
    if (!(pawn & pieces)) {
        board.white_pawns = pawns;
        move_list[(*idx)++] = board;
    }
}

void white_pawn_moves(board_t* board, location all_pieces, location enemy_pieces, board_t* move_list, int* idx) {
    location pawns = board->white_pawns;
    location remaining = pawns;
    while (remaining) {
        location other_pawns = remaining & (remaining - 1);
        location pawn = other_pawns ^ remaining;
        location advance1 = pawn << BOARD_LENGTH;
        add_if_legal_advance(*board, advance1, pawns ^ pawn ^ advance1, all_pieces, move_list, idx);
        if (pawn < RANK2_LIMIT) {
            location advance2 = pawn << (BOARD_LENGTH << 1);
            add_if_legal_advance(*board, advance2, pawns ^ pawn ^ advance2, all_pieces, move_list, idx);
        }
        remaining = other_pawns;
    }
}

void add_white_knight_move(board_t board, location move, location knights, board_t* move_list, int* idx) {
    board.white_knights = knights | move;
    board.black_pawns ^= move;
    board.black_knights ^= move;
    board.black_bishops ^= move;
    board.black_rooks ^= move;
    board.black_queen ^= move;
    move_list[(*idx)++] = board;
}

void white_knight_moves(board_t* board, location pieces, board_t* move_list, int* idx) {
    location knights = board->white_knights;
    location remaining = knights;
    while (remaining) {
        location other_knights = remaining & (remaining - 1);
        location knight = other_knights ^ remaining;
        for (int i = 0; i < 8; i++) {
            if (!(KNIGHT_BOUNDARIES[i] & knight)) {
                location move = knight << KNIGHT_MOVES[i];
                if (!(move & pieces)) {
                    add_white_knight_move(*board, move, knights ^ knight, move_list, idx);
                }
            }
        }
        remaining = other_knights;
    }
}

static int count = 0;

void legal_moves(board_t* board) {
    count++;
    board_t moves[MAX_MOVES];
    int idx = 0;
    int* idx_ptr = &idx;
    location white_pieces = board->white_pawns ^ board->white_knights ^ board->white_bishops
            ^ board->white_rooks ^ board->white_queen ^ board->white_king;
    location black_pieces = board->black_pawns ^ board->black_knights ^ board->black_bishops
            ^ board->black_rooks ^ board->black_queen ^ board->black_king;
    white_pawn_moves(board, white_pieces ^ black_pieces, black_pieces, moves, idx_ptr);
    white_knight_moves(board, white_pieces, moves, idx_ptr);
    for (int i = 0; i < idx; i++) {
        puts("");
        decode_board(&moves[i]);
    }
    printf("%d", idx);
}

void decode_board(board_t* board) {
    for (int i = 7; i > -1; i--) {
        for (int j = 0; j < 8; j++) {
            location curr = 1lu << ((i << 3) + j);
//            printf("%d", (i << 3) + j);
            if (board->white_pawns & curr) printf("♟ ");
            else if (board->white_knights & curr) printf("♞ ");
            else if (board->white_bishops & curr) printf("♝ ");
            else if (board->white_rooks & curr) printf("♜ ");
            else if (board->white_queen & curr) printf("♛ ");
            else if (board->white_king & curr) printf("♚ ");
            else if (board->black_pawns & curr) printf("♙ ");
            else if (board->black_knights & curr) printf("♘ ");
            else if (board->black_bishops & curr) printf("♗ ");
            else if (board->black_rooks & curr) printf("♖ ");
            else if (board->black_queen & curr) printf("♕ ");
            else if (board->black_king & curr) printf("♔ ");
            else printf("- ");
        }
        puts("");
    }
}

int main() {
    board_t* board = START_POSITION;
//    decode_board(board);
    legal_moves(board);
    return 0;
}
