#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "types.h"

extern const BitBoard HOME_RANKS[];
extern const BitBoard THIRD_RANKS[];
extern const int PAWN_DIRECTIONS[];

extern const int HASH;
extern const int GOOD_CAPTURE;
extern const int BAD_CAPTURE;
extern const int KILLER1;
extern const int KILLER2;
extern const int COUNTER;

void addMove(MoveList* moveList, Move move);
void generateMoves(MoveList* moveList, Board* board, int ply);
void generateQuiesceMoves(MoveList* moveList, Board* board);
void printMoves(MoveList* moveList);
Move parseMove(char* moveStr, Board* board);
char* moveStr(Move move);
void bubbleTopMove(MoveList* moveList, int from);
void addKiller(Board* board, Move move, int ply);
void addCounter(Board* board, Move move, Move parent);
void addHistoryHeuristic(Board* board, Move move, int depth);
void addBFHeuristic(Board* board, Move move, int depth);

#endif
