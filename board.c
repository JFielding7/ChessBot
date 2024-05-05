//
// Created by joe on 4/30/24.
//

#include <stdbool.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>

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
        location next_pawns = remaining & (remaining - 1);
        location pawn = next_pawns ^ remaining;
        location advance1 = pawn << BOARD_LENGTH;
        add_if_legal_advance(*board, advance1, pawns ^ pawn ^ advance1, all_pieces, move_list, idx);
        if (pawn < RANK2_LIMIT && !(all_pieces & advance1)) {
            location advance2 = pawn << (BOARD_LENGTH << 1);
            add_if_legal_advance(*board, advance2, pawns ^ pawn ^ advance2, all_pieces, move_list, idx);
        }
        remaining = next_pawns;
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

void white_knight_moves(board_t* board, location white_pieces, board_t* move_list, int* idx) {
    location knights = board->white_knights;
    location remaining = knights;
    while (remaining) {
        location next_knights = remaining & (remaining - 1);
        location knight = next_knights ^ remaining;
        for (int i = 0; i < 8; i++) {
            if (!(KNIGHT_BOUNDARIES[i] & knight)) {
                location move = i < 4 ? knight >> KNIGHT_MOVES[i] : knight << KNIGHT_MOVES[i];
                if (!(move & white_pieces)) {
                    add_white_knight_move(*board, move, knights ^ knight, move_list, idx);
                }
            }
        }
        remaining = next_knights;
    }
}

void add_white_bishop_move(board_t board, location move, location bishops, board_t* move_list, int* idx) {
    board.white_bishops = bishops | move;
    board.black_pawns ^= move;
    board.black_knights ^= move;
    board.black_bishops ^= move;
    board.black_rooks ^= move;
    board.black_queen ^= move;
    move_list[(*idx)++] = board;
}

void white_bishop_moves(board_t* board, location white_pieces, location black_pieces, board_t* move_list, int* idx) {
    location bishops = board->white_bishops;
    location remaining_bishops = bishops;
    while (remaining_bishops) {
        location next_bishops = remaining_bishops & (remaining_bishops - 1);
        location bishop = next_bishops ^ remaining_bishops;
        for (int i = 0; i < 4; i++) {
            int delta = BISHOP_MOVES[i];
            location curr = bishop;
            location other_bishops = curr ^ bishops;
            while (!(curr & BISHOP_BOUNDARIES[i]) && !(curr & black_pieces)) {
                curr = i < 2 ? curr >> delta : curr << delta;
                if (curr & white_pieces) break;
                add_white_bishop_move(*board, curr, other_bishops, move_list, idx);
            }
        }
        remaining_bishops = next_bishops;
    }
}


void legal_moves(board_t* board) {
    board_t moves_list[MAX_MOVES];
    int idx = 0;
    int* idx_ptr = &idx;
    location white_pieces = board->white_pawns ^ board->white_knights ^ board->white_bishops
            ^ board->white_rooks ^ board->white_queen ^ board->white_king;
    location black_pieces = board->black_pawns ^ board->black_knights ^ board->black_bishops
            ^ board->black_rooks ^ board->black_queen ^ board->black_king;
    white_pawn_moves(board, white_pieces ^ black_pieces, black_pieces, moves_list, idx_ptr);
    white_knight_moves(board, white_pieces, moves_list, idx_ptr);
    white_bishop_moves(board, white_pieces, black_pieces, moves_list, idx_ptr);
    for (int i = 0; i < idx; i++) {
        puts("");
        decode_board(&moves_list[i]);
    }
    printf("%d", idx);
}

board_t* fen_to_bitboard(const char* fen) {
    board_t* board = malloc(sizeof (board_t));
    board->white_pawns   = 0;
    board->white_knights = 0;
    board->white_bishops = 0;
    board->white_rooks   = 0;
    board->white_queen   = 0;
    board->white_king    = 0;
    board->black_pawns   = 0;
    board->black_knights = 0;
    board->black_bishops = 0;
    board->black_rooks   = 0;
    board->black_queen   = 0;
    board->black_king    = 0;
    int square = 56;
    int i = 0;
    int length = strlen(fen);
    while (i < length) {
        printf("%d %c\n", square, fen[i]);
        location curr = 1lu << square;
        switch (fen[i]) {
            case 'p': board->black_pawns |= curr;
                break;
            case 'n': board->black_knights |= curr;
                break;
            case 'b': board->black_bishops |= curr;
                break;
            case 'r': board->black_rooks |= curr;
                break;
            case 'q': board->black_queen |= curr;
                break;
            case 'k': board->black_king |= curr;
                break;
            case 'P': board->white_pawns |= curr;
                break;
            case 'N': board->white_knights |= curr;
                break;
            case 'B': board->white_bishops |= curr;
                break;
            case 'R': board->white_rooks |= curr;
                break;
            case 'Q': board->white_queen |= curr;
                break;
            case 'K': board->white_king |= curr;
                break;
            case '/': square -= 17;
                break;
            default: square += fen[i] - 49;
        }
        square++;
        i++;
    }
    return board;
}

void decode_board(board_t* board) {
    for (int i = 7; i > -1; i--) {
        for (int j = 0; j < 8; j++) {
            location curr = 1lu << ((i << 3) + j);
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
    board_t* board = fen_to_bitboard("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R");
    decode_board(board);
    return 0;
}
