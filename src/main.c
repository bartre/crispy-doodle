#include "chess_engine.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    char *fen = "r2Nkb1r/2p2ppp/pp2p3/8/2PPpB2/1P2P3/P4PPP/R2QK2R b KQkq - 0 13";
    printf("%s\n", fen);

    const board_t board = ce_fen2board(fen);

    ce_print_board(&board);
    char *fen_out = calloc(128, 1);
    ce_board2fen(&board, fen_out, 128);
    printf("%s\n", fen_out);
    free(fen_out);
    return 0;
}