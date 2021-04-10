// Berserk is a UCI compliant chess engine written in C
// Copyright (C) 2021 Jay Honnold

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef TYPES_H
#define TYPES_H

#include <setjmp.h>
#include <stdint.h>

#define MAX_SEARCH_PLY 128
#define MAX_MOVES 256
#define MAX_GAME_PLY 1024

// Tune on doubles for more accuracy
#ifndef TUNE
typedef int Score;
typedef Score TScore[2];
#else
typedef double Score;
typedef Score TScore[2];
#endif

typedef uint64_t BitBoard;

typedef uint64_t TTValue;

typedef int Move;

// Move generation storage
// moves/scores idx's match
typedef struct {
  int count;
  Move moves[MAX_MOVES];
  int scores[MAX_MOVES];
} MoveList;

typedef struct {
  BitBoard pieces[12];     // individual piece data
  BitBoard occupancies[3]; // 0 - white pieces, 1 - black pieces, 2 - both
  int squares[64];         // piece per square
  BitBoard checkers;       // checking piece squares
  BitBoard pinners;        // pinned pieces
  uint64_t piecesCounts;   // "material key" - pieces left on the board

  int side;     // side to move
  int xside;    // side not to move
  int epSquare; // en passant square (a8 or 0 is not valid so that marks no active ep)
  int castling; // castling mask e.g. 1111 = KQkq, 1001 = Kq
  int moveNo;   // current game move number TODO: Is this still used?
  int halfMove; // half move count for 50 move rule

  uint64_t zobrist; // zobrist hash of the position

  // data that is hard to track, so it is "remembered" when search undoes moves
  uint64_t zobristHistory[MAX_GAME_PLY];
  int castlingHistory[MAX_GAME_PLY];
  int epSquareHistory[MAX_GAME_PLY];
  int captureHistory[MAX_GAME_PLY];
  int halfMoveHistory[MAX_GAME_PLY];
} Board;

// Tracking the principal variation
typedef struct {
  int count;
  Move moves[MAX_SEARCH_PLY];
} PV;

// A general data object for use during search
typedef struct {
  int score;     // analysis score result, from perspective of stm
  Move bestMove; // best move from analysis

  Board* board; // reference to board
  int ply;      // ply depth of active search

  // TODO: Put depth here as well? Just cause
  int nodes;    // node count
  int seldepth; // seldepth count

  Move skipMove[MAX_SEARCH_PLY]; // moves to skip during singular search
  int evals[MAX_SEARCH_PLY];     // static evals at ply stack
  Move moves[MAX_SEARCH_PLY];    // moves for ply stack

  Move killers[MAX_SEARCH_PLY][2]; // killer moves, 2 per ply
  Move counters[64 * 64];          // counter move butterfly table
  int hh[2][64 * 64];              // history heuristic butterfly table (side)
  int bf[2][64 * 64];              // butterfly heuristic butterfly table (side)
} SearchData;

typedef struct {
  long startTime;
  long endTime;
  int depth;
  int timeset;
  int movesToGo;
  int stopped;
  int quit;
} SearchParams;

typedef struct {
  TScore material; // raw material score
  TScore pawns;    // pawn bonuses, includes all pawn parameters (passers, doubled, etc..)
  TScore knights;  // knight bonuses
  TScore bishops;  // bishop bonuses
  TScore rooks;    // rook bonuses
  TScore queens;   // queen bonuses
  TScore kings;    // king bonuses

  TScore mobility;   // mobility of all pieces (except kings + pawns)
  TScore kingSafety; // king safety score
  TScore threats;    // active threats
  TScore tempo;      // tempo just didn't fit anywhere else

  // these are general data objects, for buildup during eval
  BitBoard attacks[6]; // attacks by piece type
  BitBoard allAttacks; // all attacks
  BitBoard attacks2;   // squares attacked twice
  int attackWeight;    // king safety attackers weight
  int attackCount;     // king safety sq attack count
  int attackers;       // king safety attackers count
} EvalData;

typedef struct ThreadData ThreadData;

struct ThreadData {
  int count, idx;
  ThreadData* threads;
  jmp_buf exit;

  SearchParams* params;
  SearchData data;

  Board board;
  PV pv;
};

enum { WHITE, BLACK, BOTH };

// clang-format off
enum {
  A8, B8, C8, D8, E8, F8, G8, H8,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A1, B1, C1, D1, E1, F1, G1, H1,
};
// clang-format on

enum { N = -8, E = 1, S = 8, W = -1, NE = -7, SE = 9, SW = 7, NW = -9 };

enum {
  PAWN_WHITE,
  PAWN_BLACK,
  KNIGHT_WHITE,
  KNIGHT_BLACK,
  BISHOP_WHITE,
  BISHOP_BLACK,
  ROOK_WHITE,
  ROOK_BLACK,
  QUEEN_WHITE,
  QUEEN_BLACK,
  KING_WHITE,
  KING_BLACK
};

enum { PAWN_TYPE, KNIGHT_TYPE, BISHOP_TYPE, ROOK_TYPE, QUEEN_TYPE, KING_TYPE };

enum { MG, EG };

#endif
