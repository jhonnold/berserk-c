#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "types.h"

int64_t perft(int depth, SearchData* data) {
  if (depth == 0)
    return 1;

  MoveList moveList[1];
  generateMoves(moveList, data);

  if (depth == 1)
    return moveList->count;

  int64_t nodes = 0;

  for (int i = 0; i < moveList->count; i++) {
    Move m = moveList->moves[i];

    makeMove(m, data->board);
    nodes += perft(depth - 1, data);
    undoMove(m, data->board);
  }

  return nodes;
}

void PerftTest(int depth, Board* board) {
  int64_t total = 0;

  printf("\nRunning performance test to depth %d\n\n", depth);
  struct timeval stop, start;
  gettimeofday(&start, NULL);

  MoveList moveList[1];
  SearchData data[1];
  data->board = board;
  data->ply = 0;

  generateMoves(moveList, data);

  for (int i = 0; i < moveList->count; i++) {
    Move m = moveList->moves[i];

    makeMove(m, board);
    int64_t nodes = perft(depth - 1, data);
    undoMove(m, board);

    printf("%s: %" PRId64 "\n", moveStr(m), nodes);
    total += nodes;
  }

  gettimeofday(&stop, NULL);

  int64_t duration = ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
  printf("\nNodes: %" PRId64 "\n", total);
  printf("Time: %.3fms\n", duration / 1000.0);
  printf("NPS: %" PRId64 "\n\n", total * 1000000 / duration);
}