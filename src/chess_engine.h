#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

    typedef enum
    {
        A8 = 0,
        B8,
        C8,
        D8,
        E8,
        F8,
        G8,
        H8,
        A7,
        B7,
        C7,
        D7,
        E7,
        F7,
        G7,
        H7,
        A6,
        B6,
        C6,
        D6,
        E6,
        F6,
        G6,
        H6,
        A5,
        B5,
        C5,
        D5,
        E5,
        F5,
        G5,
        H5,
        A4,
        B4,
        C4,
        D4,
        E4,
        F4,
        G4,
        H4,
        A3,
        B3,
        C3,
        D3,
        E3,
        F3,
        G3,
        H3,
        A2,
        B2,
        C2,
        D2,
        E2,
        F2,
        G2,
        H2,
        A1,
        B1,
        C1,
        D1,
        E1,
        F1,
        G1,
        H1,
        INVALID_POS,
        NUM_POS
    } position_t;

    typedef struct
    {
        char board[64];
        bool white_to_move;
        position_t enpassant;
        bool white_can_castle_kingside;
        bool white_can_castle_queenside;
        bool black_can_castle_kingside;
        bool black_can_castle_queenside;
        unsigned int halfmoves;
        unsigned int fullmoves;
    } board_t;

    board_t ce_fen2board(const char *const fen);
    int ce_board2fen(const board_t *const board, char *const fen, const unsigned int len);

    void ce_print_board(const board_t *board);

    position_t ce_str2pos(const char *s);

    int ce_vertical_offset(const position_t current, const position_t new_pos);
    int ce_horizontal_offset(const position_t current, const position_t new_pos);

    bool ce_move_valid(const board_t *board, const position_t current, const position_t new_pos);

#ifdef __cplusplus
}
#endif