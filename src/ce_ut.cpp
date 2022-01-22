#include "gtest/gtest.h"
#include "chess_engine.h"
#include <string>
#include <tuple>

class FENParameterizedTestFixture : public ::testing::TestWithParam<std::string>
{
};

TEST_P(FENParameterizedTestFixture, FENEncodeDecode)
{
    std::string fen = GetParam();
    board_t b = ce_fen2board(fen.c_str());
    char fen_out[128] = {0};
    (void)ce_board2fen(&b, fen_out, 128);
    ASSERT_STREQ(fen.c_str(), fen_out);
}

INSTANTIATE_TEST_SUITE_P(
    FENParameterizedTest,
    FENParameterizedTestFixture,
    ::testing::Values(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r1b1k2r/ppp3pp/3b4/3p1p1q/4N3/2PBPP2/PP2K1P1/R1BQR3 b kq - 0 14",
        "2k4r/ppp3pp/8/8/4bb2/1KP5/PP4P1/R1Br4 w - - 0 24",
        "rnb2rk1/pp4pp/2pb4/2qnp1B1/2B4P/2PQ4/P2N1PP1/R3R1K1 w - - 3 15",
        "rnbq1rk1/ppp2ppp/3p1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQ1RK1 b - - 0 6",
        "5rk1/2p2pp1/3p3p/p2P4/P1P2Pq1/7r/3R4/5R1K w - - 1 31",
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
        "2k4r/ppp3pp/8/8/4bb2/1KP5/PP4P1/R1Br4 w - - 0 24",
        "2kr3r/ppp3pp/3b2b1/7q/4p3/2PK1P2/PP4P1/R1BQR3 w - - 0 20",
        "r1b1k2r/ppp3pp/3b4/3p3q/8/2PpPP2/PP1K2P1/R1BQR3 w kq - 0 16"));

class Str2PosParameterizedTestFixture : public ::testing::TestWithParam<std::tuple<std::string, position_t, bool>>
{
};

TEST_P(Str2PosParameterizedTestFixture, Str2Pos)
{
    ASSERT_TRUE((std::get<1>(GetParam()) == ce_str2pos(std::get<0>(GetParam()).c_str())) == std::get<2>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
    Str2PosParameterizedTest,
    Str2PosParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple("A8", A8, true),
        std::make_tuple("H1", H1, true),
        std::make_tuple("A9", INVALID_POS, true),
        std::make_tuple("Z1", INVALID_POS, true),
        std::make_tuple("  ", INVALID_POS, true),
        std::make_tuple("A ", INVALID_POS, true),
        std::make_tuple("H ", INVALID_POS, true),
        std::make_tuple(" 8", INVALID_POS, true),
        std::make_tuple(" 1", INVALID_POS, true),
        std::make_tuple("99", INVALID_POS, true),
        std::make_tuple("AA", INVALID_POS, true)));

class HortOffsetParameterizedTestFixture : public ::testing::TestWithParam<std::tuple<position_t, position_t, int>>
{
};

TEST_P(HortOffsetParameterizedTestFixture, HortOffset)
{
    position_t current_pos = std::get<0>(GetParam());
    position_t new_pos = std::get<1>(GetParam());
    int offset = std::get<2>(GetParam());
    ASSERT_EQ(offset, ce_horizontal_offset(current_pos, new_pos));
}

INSTANTIATE_TEST_SUITE_P(
    HortOffsetParameterizedTest,
    HortOffsetParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(A8, A8, 0),
        std::make_tuple(A8, B7, 1),
        std::make_tuple(A8, C6, 2),
        std::make_tuple(A8, D5, 3),
        std::make_tuple(A8, E4, 4),
        std::make_tuple(A8, F3, 5),
        std::make_tuple(A8, G2, 6),
        std::make_tuple(A8, H1, 7),

        std::make_tuple(A1, A1, 0),
        std::make_tuple(A1, B2, 1),
        std::make_tuple(A1, C3, 2),
        std::make_tuple(A1, D4, 3),
        std::make_tuple(A1, E5, 4),
        std::make_tuple(A1, F6, 5),
        std::make_tuple(A1, G7, 6),
        std::make_tuple(A1, H8, 7),

        std::make_tuple(H1, H1, 0),
        std::make_tuple(H1, G2, -1),
        std::make_tuple(H1, F3, -2),
        std::make_tuple(H1, E4, -3),
        std::make_tuple(H1, D5, -4),
        std::make_tuple(H1, C6, -5),
        std::make_tuple(H1, B7, -6),
        std::make_tuple(H1, A8, -7),

        std::make_tuple(H8, H1, 0),
        std::make_tuple(H8, G2, -1),
        std::make_tuple(H8, F3, -2),
        std::make_tuple(H8, E4, -3),
        std::make_tuple(H8, D3, -4),
        std::make_tuple(H8, C3, -5),
        std::make_tuple(H8, B2, -6),
        std::make_tuple(H8, A1, -7),

        std::make_tuple(D8, A4, -3),
        std::make_tuple(D7, B4, -2),
        std::make_tuple(D6, C4, -1),
        std::make_tuple(D5, D4, 0),
        std::make_tuple(E4, E5, 0),
        std::make_tuple(E3, F5, 1),
        std::make_tuple(E2, G5, 2),
        std::make_tuple(E1, H5, 3)

            ));

class VertOffsetParameterizedTestFixture : public ::testing::TestWithParam<std::tuple<position_t, position_t, int>>
{
};

TEST_P(VertOffsetParameterizedTestFixture, VertOffset)
{
    position_t current_pos = std::get<0>(GetParam());
    position_t new_pos = std::get<1>(GetParam());
    int offset = std::get<2>(GetParam());
    ASSERT_EQ(offset, ce_vertical_offset(current_pos, new_pos));
}

INSTANTIATE_TEST_SUITE_P(
    VertOffsetParameterizedTest,
    VertOffsetParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(A8, A8, 0),
        std::make_tuple(A8, B7, -1),
        std::make_tuple(A8, C6, -2),
        std::make_tuple(A8, D5, -3),
        std::make_tuple(A8, E4, -4),
        std::make_tuple(A8, F3, -5),
        std::make_tuple(A8, G2, -6),
        std::make_tuple(A8, H1, -7),

        std::make_tuple(A1, A1, 0),
        std::make_tuple(A1, B2, 1),
        std::make_tuple(A1, C3, 2),
        std::make_tuple(A1, D4, 3),
        std::make_tuple(A1, E5, 4),
        std::make_tuple(A1, F6, 5),
        std::make_tuple(A1, G7, 6),
        std::make_tuple(A1, H8, 7),

        std::make_tuple(H1, H1, 0),
        std::make_tuple(H1, G2, 1),
        std::make_tuple(H1, F3, 2),
        std::make_tuple(H1, E4, 3),
        std::make_tuple(H1, D5, 4),
        std::make_tuple(H1, C6, 5),
        std::make_tuple(H1, B7, 6),
        std::make_tuple(H1, A8, 7),

        std::make_tuple(H8, H1, -7),
        std::make_tuple(H8, G2, -6),
        std::make_tuple(H8, F3, -5),
        std::make_tuple(H8, E4, -4),
        std::make_tuple(H8, D5, -3),
        std::make_tuple(H8, C6, -2),
        std::make_tuple(H8, B7, -1),
        std::make_tuple(H8, A8, 0),

        std::make_tuple(D8, A4, -4),
        std::make_tuple(D7, B4, -3),
        std::make_tuple(D6, C4, -2),
        std::make_tuple(D5, D4, -1),
        std::make_tuple(E4, E5, 1),
        std::make_tuple(E3, F5, 2),
        std::make_tuple(E2, G5, 3),
        std::make_tuple(E1, H5, 4)

            ));