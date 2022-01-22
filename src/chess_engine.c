#include "chess_engine.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *pos2str_lut[NUM_POS] =
    {
        "a8",
        "b8",
        "c8",
        "d8",
        "e8",
        "f8",
        "g8",
        "h8",
        "a7",
        "b7",
        "c7",
        "d7",
        "e7",
        "f7",
        "g7",
        "h7",
        "a6",
        "b6",
        "c6",
        "d6",
        "e6",
        "f6",
        "g6",
        "h6",
        "a5",
        "b5",
        "c5",
        "d5",
        "e5",
        "f5",
        "g5",
        "h5",
        "a4",
        "b4",
        "c4",
        "d4",
        "e4",
        "f4",
        "g4",
        "h4",
        "a3",
        "b3",
        "c3",
        "d3",
        "e3",
        "f3",
        "g3",
        "h3",
        "a2",
        "b2",
        "c2",
        "d2",
        "e2",
        "f2",
        "g2",
        "h2",
        "a1",
        "b1",
        "c1",
        "d1",
        "e1",
        "f1",
        "g1",
        "h1",
        "INVALID_POS"};

position_t ce_str2pos(const char *s)
{
    char column = s[0];
    char row = s[1];

    if (((column < 'a') || (column > 'h')) && ((column < 'A') || (column > 'H')))
    {
        return INVALID_POS;
    }
    if ((row < '0') || (row > '8'))
    {
        return INVALID_POS;
    }

    position_t pos = (position_t)(64 - (8 * (row - '0')));
    pos += (isupper(column) ? column - 'A' : column - 'a');

    return pos;
}

int ce_board2fen(const board_t *const board, char *const fen, const unsigned int len)
{
    int offset = 0;
    int free = 0;
    for (position_t pos = A8; pos <= H1; pos++)
    {
        const char piece = board->board[pos];
        if (piece != '_')
        {
            if (free != 0)
            {
                fen[offset++] = free + '0';
                free = 0;
            }
            fen[offset++] = piece;
        }
        else
        {
            free++;
        }

        if (((pos + 1) % 8 == 0) && (pos != A8))
        {
            if (free != 0)
            {
                fen[offset++] = free + '0';
                free = 0;
            }
            if (pos != H1)
            {
                fen[offset++] = '/';
            }
        }
    }
    fen[offset++] = ' ';
    fen[offset++] = board->white_to_move ? 'w' : 'b';
    fen[offset++] = ' ';

    if ((board->white_can_castle_kingside == false) &&
        (board->white_can_castle_queenside == false) &&
        (board->black_can_castle_kingside == false) &&
        (board->black_can_castle_queenside == false))
    {
        fen[offset++] = '-';
    }
    else
    {
        if (board->white_can_castle_kingside == true)
        {
            fen[offset++] = 'K';
        }
        if (board->white_can_castle_queenside == true)
        {
            fen[offset++] = 'Q';
        }
        if (board->black_can_castle_kingside == true)
        {
            fen[offset++] = 'k';
        }
        if (board->black_can_castle_queenside == true)
        {
            fen[offset++] = 'q';
        }
    }
    fen[offset++] = ' ';
    if (board->enpassant != INVALID_POS)
    {
        memcpy(&fen[offset], pos2str_lut[board->enpassant], 2);
        offset += 2;
    }
    else
    {
        fen[offset++] = '-';
    }
    fen[offset++] = ' ';
    offset += sprintf(&fen[offset], "%d", board->halfmoves);
    fen[offset++] = ' ';
    offset += sprintf(&fen[offset], "%d", board->fullmoves);

    return offset;
}

board_t ce_fen2board(const char *const fen)
{
    board_t board;
    memset(board.board, '_', sizeof(board.board));
    board.enpassant = INVALID_POS;
    board.white_can_castle_kingside = false;
    board.white_can_castle_queenside = false;
    board.black_can_castle_kingside = false;
    board.black_can_castle_queenside = false;
    board.halfmoves = 0;
    board.fullmoves = 0;

    char positions[64];
    char move;
    char castling[5];
    char enpassant[3];

    sscanf(fen, "%s %c %s %s %d %d", positions, &move, castling, enpassant, &board.halfmoves, &board.fullmoves);

    int column = 0;
    int row = 0;
    for (int i = 0; (i < strlen(positions)); i++)
    {
        if (positions[i] > '0' && positions[i] <= '8')
        {
            for (int j = 0; j < positions[i] - '0'; j++)
            {
                column++;
            }
        }
        else if (positions[i] == '/')
        {
            row++;
            column = 0;
        }
        else
        {
            board.board[(row * 8) + column] = positions[i];
            column++;
        }
    }
    board.white_to_move = move == 'w' ? true : false;

    for (int i = 0; (i < strlen(castling)); i++)
    {
        switch (castling[i])
        {
        case 'K':
            board.white_can_castle_kingside = true;
            break;
        case 'Q':
            board.white_can_castle_queenside = true;
            break;
        case 'k':
            board.black_can_castle_kingside = true;
            break;
        case 'q':
            board.black_can_castle_queenside = true;
            break;
        default:
            break;
        }
    }

    if (enpassant[0] != '-')
    {
        board.enpassant = ce_str2pos(enpassant);
    }
    return board;
}

void ce_print_board(const board_t *board)
{

    for (int row = 0; row < 8; row++)
    {
        printf("%d  ", 8 - row);

        for (int column = 0; column < 8; column++)
        {
            {
                printf("%c ", board->board[(row * 8) + column]);
            }
        }
        printf("\n");
    }
    printf("   A B C D E F G H\n\n");

    printf("%s\n", board->white_to_move ? "White to move" : "Black to move");
    if (board->enpassant != INVALID_POS)
    {
        printf("En passant possible at %s\n", pos2str_lut[(int)board->enpassant]);
    }
    if (board->white_can_castle_kingside)
    {
        printf("White can castle kingside\n");
    }
    if (board->white_can_castle_queenside)
    {
        printf("White can castle queenside\n");
    }
    if (board->black_can_castle_kingside)
    {
        printf("Black can castle kingside\n");
    }
    if (board->white_can_castle_queenside)
    {
        printf("Black can castle queenside\n");
    }
    printf("Half moves: %d, full moves: %d\n", board->halfmoves, board->fullmoves);
}

int ce_horizontal_offset(const position_t current, const position_t new_pos)
{
    int horizontal_current = current % 8;
    int horizontal_new = new_pos % 8;

    return horizontal_new - horizontal_current;
}

int ce_vertical_offset(const position_t current, const position_t new_pos)
{
    int vert_current = (64 - (current + 1)) / 8;
    int vert_new = (64 - (new_pos + 1)) / 8;

    return vert_new - vert_current;
}

bool ce_move_valid(const board_t *board, const position_t current, const position_t new_pos)
{
    bool valid = false;
    char piece = board->board[current];

    if (current == new_pos)
    {
        return false;
    }
    if ((current == INVALID_POS) || (current == NUM_POS) || (new_pos == INVALID_POS) || (new_pos == NUM_POS))
    {
        return false;
    }

    switch (piece)
    {
    case 'p':
    {
    }
    break;
    case 'r':
    {
    }
    break;
    case 'n':
    {
    }
    break;
    case 'b':
    {
    }
    break;
    case 'q':
    {
    }
    case 'k':
    {
    }
    break;

    case 'P':
    {
    }
    break;
    case 'R':
    {
    }
    break;
    case 'N':
    {
    }
    break;
    case 'B':
    {
    }
    break;
    case 'Q':
    {
    }
    case 'K':
    {
    }
    break;
    default:
        break;
    }

    return valid;
}