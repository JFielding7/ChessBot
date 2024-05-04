//
// Created by joe on 4/30/24.
//

#include <stdbool.h>
#include <memory.h>
#include <malloc.h>

#include "board.h"

void add_if_legal_advance(board_t board, location pawn, location pawns, location pieces, board_t* move_list, int* idx) {
    if ((pawn & pieces) == 0) {
        puts("legal");
        board.white_pawns = pawns;
        move_list[(*idx)++] = board;
    }
}

void white_pawn_moves(board_t* board, location all_pieces, location enemy_pieces, board_t* move_list, int* idx) {
    location pawns = board->white_pawns;
    location remaining = board->white_pawns;
    while (remaining) {
        location other_pawns = remaining & (remaining - 1);
        location pawn = remaining ^ other_pawns;
        location advance1 = pawn << BOARD_LENGTH;
        add_if_legal_advance(*board, advance1, pawns ^ pawn ^ advance1, all_pieces, move_list, idx);
        if (pawn < RANK2) {
            location advance2 = pawn << (BOARD_LENGTH << 1);
            add_if_legal_advance(*board, advance2, pawns ^ pawn ^ advance2, all_pieces, move_list, idx);
        }
        remaining = other_pawns;
    }
}

void legal_moves(board_t* board) {
    board_t moves[MAX_MOVES];
    int idx = 0;
    int* idx_ptr = &idx;
    location white_pieces = board->white_pawns ^ board->white_knights ^ board->white_bishops
            ^ board->white_rooks ^ board->white_queen ^ board->white_king;
    location black_pieces = board->black_pawns ^ board->black_knights ^ board->black_bishops
            ^ board->black_rooks ^ board->black_queen ^ board->black_king;
    white_pawn_moves(board, white_pieces ^ black_pieces, black_pieces, moves, idx_ptr);
    printf("%d", idx);
}

void decode_board(board_t* board) {
    printf("%lb\n", board->black_pawns);
    for (int i = 0; i < SQUARES; i++) {
        location curr = 1lu << i;
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
        if ((i & 7) == 7) puts("");
    }
}

int main() {
    board_t* board = START_POSITION;
    decode_board(board);
    legal_moves(board);
    free(board);
    return 0;
}
