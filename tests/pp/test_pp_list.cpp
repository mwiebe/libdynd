//
// Copyright (C) 2011-14 Irwin Zaid, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include "inc_gtest.hpp"

#include <dynd/pp/arithmetic.hpp>
#include <dynd/pp/list.hpp>
#include <dynd/pp/logical.hpp>
#include <dynd/pp/token.hpp>

using namespace std;

TEST(PPList, IsEmpty) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(()));

    EXPECT_FALSE(DYND_PP_IS_EMPTY((A)));
    EXPECT_FALSE(DYND_PP_IS_EMPTY((0)));

    EXPECT_FALSE(DYND_PP_IS_EMPTY((,)));
    EXPECT_FALSE(DYND_PP_IS_EMPTY((A, B)));
    EXPECT_FALSE(DYND_PP_IS_EMPTY((0, 1)));

    EXPECT_FALSE(DYND_PP_IS_EMPTY((,,,,,,,)));
    EXPECT_FALSE(DYND_PP_IS_EMPTY((C, A, F, H, B, E, G, D)));
    EXPECT_FALSE(DYND_PP_IS_EMPTY((5, 1, 4, 3, 7, 0, 2, 6)));
}

TEST(PPList, Len) {
    EXPECT_EQ(DYND_PP_LEN(()), 0);

    EXPECT_EQ(DYND_PP_LEN((A)), 1);
    EXPECT_EQ(DYND_PP_LEN((0)), 1);

    EXPECT_EQ(DYND_PP_LEN((,)), 2);
    EXPECT_EQ(DYND_PP_LEN((A, B)), 2);
    EXPECT_EQ(DYND_PP_LEN((0, 1)), 2);

    EXPECT_EQ(DYND_PP_LEN((,,,,,,,)), 8);
    EXPECT_EQ(DYND_PP_LEN((C, A, F, H, B, E, G, D)), 8);
    EXPECT_EQ(DYND_PP_LEN((5, 1, 4, 3, 7, 0, 2, 6)), 8);

    EXPECT_EQ(DYND_PP_LEN(DYND_PP_INTS), DYND_PP_LEN_MAX);
}

TEST(PPList, First) {
    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_FIRST(())));

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_FIRST((A)), A));
    EXPECT_EQ(DYND_PP_FIRST((0)), 0);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_FIRST((A, B)), A));
    EXPECT_EQ(DYND_PP_FIRST((0, 1)), 0);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_FIRST((C, A, F, H, B, E, G, D)), C));
    EXPECT_EQ(DYND_PP_FIRST((5, 1, 4, 3, 7, 0, 2, 6)), 5);

    EXPECT_EQ(DYND_PP_FIRST(DYND_PP_RANGE(DYND_PP_LEN_MAX)), 0);
}

TEST(PPList, Rest) { // change to pop_first?
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_REST(())));

    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_REST((A))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_REST((0))));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REST((A, B)), (B)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REST((0, 1)), (1)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REST((C, A, F, H, B, E, G, D)), (A, F, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REST((5, 1, 4, 3, 7, 0, 2, 6)), (1, 4, 3, 7, 0, 2, 6)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REST(DYND_PP_RANGE(DYND_PP_LEN_MAX)), DYND_PP_RANGE(1, DYND_PP_LEN_MAX)));
}

TEST(PPList, Last) {
//    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_LAST(())));

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_LAST((A)), A));
    EXPECT_EQ(DYND_PP_LAST((0)), 0);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_LAST((A, B)), B));
    EXPECT_EQ(DYND_PP_LAST((0, 1)), 1);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_LAST((C, A, F, H, B, E, G, D)), D));
    EXPECT_EQ(DYND_PP_LAST((5, 1, 4, 3, 7, 0, 2, 6)), 6);

    EXPECT_EQ(DYND_PP_LAST(DYND_PP_RANGE(DYND_PP_LEN_MAX)), DYND_PP_INT_MAX);
}

TEST(PPList, Cat) {
    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_CAT(())));

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((A)), A));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((0)), 0));

#define DYND_PP_AB_EQ_AB
#define DYND_PP_A0_EQ_A0

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((A, B)), AB));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((A, 0)), A0));
    EXPECT_EQ(DYND_PP_CAT((0, 1)), 1);
    EXPECT_EQ(DYND_PP_CAT((1, 0)), 10);
    EXPECT_EQ(DYND_PP_CAT((0x0, D)), 0x0D);

#undef DYND_PP_AB_EQ_AB
#undef DYND_PP_A0_EQ_A0

#define DYND_PP_ABC_EQ_ABC
#define DYND_PP_qy7Z_EQ_qy7Z
#define DYND_PP__dw11__EQ__dw11_

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((A, B, C)), ABC));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((qy, 7, Z)), qy7Z));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((_, d, w11_)), _dw11_));

#undef DYND_PP_ABC_EQ_ABC
#undef DYND_PP_q7Z_EQ_q7Z
#undef DYND_PP__dw11__EQ__dw11_

#define DYND_PP_CAFHBEGD_EQ_CAFHBEGD

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_CAT((C, A, F, H, B, E, G, D)), CAFHBEGD));
    EXPECT_EQ(DYND_PP_CAT((5, 1, 4, 3, 7, 0, 2, 6)), 51437026);

#undef DYND_PP_CAFHBEGD_EQ_CAFHBEGD
}

TEST(PPList, Range) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(0)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(1, 0)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(1, 0, 5)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(1, 1)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(1, 1, 2)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(8, 3)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(DYND_PP_LEN_MAX, 8)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(DYND_PP_LEN_MAX, DYND_PP_LEN_MAX)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(DYND_PP_LEN_MAX, DYND_PP_LEN_MAX, 3)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_RANGE(DYND_PP_LEN_MAX, DYND_PP_LEN_MAX, DYND_PP_LEN_MAX)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(1), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(1, 2), (1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(7, 8), (7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(3, 7, 4), (3)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(2), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(3, 6, 2), (3, 5)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(1, 6, 2), (1, 3, 5)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(0, 8, 3), (0, 3, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(1, 8, 3), (1, 4, 7)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(0, 8, 2), (0, 2, 4, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(1, 8, 2), (1, 3, 5, 7)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(8), (0, 1, 2, 3, 4, 5, 6, 7)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_RANGE(DYND_PP_LEN_MAX), DYND_PP_INTS));
    EXPECT_EQ(DYND_PP_LEN(DYND_PP_RANGE(0, DYND_PP_LEN_MAX, 2)), DYND_PP_LEN_MAX / 2 + DYND_PP_LEN_MAX % 2);
    EXPECT_EQ(DYND_PP_LEN(DYND_PP_RANGE(1, DYND_PP_LEN_MAX, 2)), DYND_PP_LEN_MAX / 2);
}

TEST(PPList, Get) {
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(0, (A)), A));
    EXPECT_EQ(DYND_PP_GET(0, (0)), 0);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(0, (A, B)), A));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(1, (A, B)), B));
    EXPECT_EQ(DYND_PP_GET(0, (0, 1)), 0);
    EXPECT_EQ(DYND_PP_GET(1, (0, 1)), 1);

    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(0, (C, A, F, H, B, E, G, D)), C));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(1, (C, A, F, H, B, E, G, D)), A));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(2, (C, A, F, H, B, E, G, D)), F));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(3, (C, A, F, H, B, E, G, D)), H));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(4, (C, A, F, H, B, E, G, D)), B));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(5, (C, A, F, H, B, E, G, D)), E));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(6, (C, A, F, H, B, E, G, D)), G));
    EXPECT_TRUE(DYND_PP_EQ(DYND_PP_GET(7, (C, A, F, H, B, E, G, D)), D));
    EXPECT_EQ(DYND_PP_GET(0, (5, 1, 4, 3, 7, 0, 2, 6)), 5);
    EXPECT_EQ(DYND_PP_GET(1, (5, 1, 4, 3, 7, 0, 2, 6)), 1);
    EXPECT_EQ(DYND_PP_GET(2, (5, 1, 4, 3, 7, 0, 2, 6)), 4);
    EXPECT_EQ(DYND_PP_GET(3, (5, 1, 4, 3, 7, 0, 2, 6)), 3);
    EXPECT_EQ(DYND_PP_GET(4, (5, 1, 4, 3, 7, 0, 2, 6)), 7);
    EXPECT_EQ(DYND_PP_GET(5, (5, 1, 4, 3, 7, 0, 2, 6)), 0);
    EXPECT_EQ(DYND_PP_GET(6, (5, 1, 4, 3, 7, 0, 2, 6)), 2);
    EXPECT_EQ(DYND_PP_GET(7, (5, 1, 4, 3, 7, 0, 2, 6)), 6);

    EXPECT_EQ(DYND_PP_GET(0, DYND_PP_RANGE(DYND_PP_LEN_MAX)), 0);
    EXPECT_EQ(DYND_PP_GET(DYND_PP_INT_MAX, DYND_PP_RANGE(DYND_PP_LEN_MAX)), DYND_PP_INT_MAX);
}

TEST(PPList, Set) {
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, B, (A)), (B)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, 1, (0)), (1)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, B, (A, B)), (B, B)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(1, A, (A, A)), (A, A)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, 1, (0, 1)), (1, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(1, 0, (0, 1)), (0, 0)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, D, (C, A, F, H, B, E, G, D)), (D, A, F, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(1, G, (C, A, F, H, B, E, G, D)), (C, G, F, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(2, E, (C, A, F, H, B, E, G, D)), (C, A, E, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(3, B, (C, A, F, H, B, E, G, D)), (C, A, F, B, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(4, H, (C, A, F, H, B, E, G, D)), (C, A, F, H, H, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(5, F, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, F, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(6, A, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, E, A, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(7, C, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, E, G, C)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, 6, (5, 1, 4, 3, 7, 0, 2, 6)), (6, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(1, 2, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 2, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(2, 0, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 0, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(3, 7, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 7, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(4, 3, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 3, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(5, 4, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 4, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(6, 1, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 1, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(7, 5, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 5)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(0, DYND_PP_INT_MAX, DYND_PP_RANGE(DYND_PP_LEN_MAX)),
        DYND_PP_PREPEND(DYND_PP_INT_MAX, DYND_PP_RANGE(1, DYND_PP_LEN_MAX))));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SET(DYND_PP_INT_MAX, 0, DYND_PP_RANGE(DYND_PP_LEN_MAX)),
        DYND_PP_APPEND(0, DYND_PP_RANGE(DYND_PP_INT_MAX))));
}

TEST(PPList, Del) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_DEL(0, (A))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_DEL(0, (0))));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(0, (A, B)), (B)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(1, (A, B)), (A)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(0, (0, 1)), (1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(1, (0, 1)), (0)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(0, (C, A, F, H, B, E, G, D)), (A, F, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(1, (C, A, F, H, B, E, G, D)), (C, F, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(2, (C, A, F, H, B, E, G, D)), (C, A, H, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(3, (C, A, F, H, B, E, G, D)), (C, A, F, B, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(4, (C, A, F, H, B, E, G, D)), (C, A, F, H, E, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(5, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, G, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(6, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, E, D)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(7, (C, A, F, H, B, E, G, D)), (C, A, F, H, B, E, G)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(0, (5, 1, 4, 3, 7, 0, 2, 6)), (1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(1, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(2, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(3, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(4, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(5, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(6, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(7, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(0, DYND_PP_RANGE(DYND_PP_LEN_MAX)),
        DYND_PP_RANGE(1, DYND_PP_LEN_MAX)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_DEL(DYND_PP_INT_MAX, DYND_PP_RANGE(DYND_PP_LEN_MAX)),
        DYND_PP_RANGE(DYND_PP_INT_MAX)));
}

TEST(PPList, Map) {
    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_JOIN_MAP(DYND_PP_ID, (,), ())));
    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_JOIN_MAP(DYND_PP_INC, (;), ())));

    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_ID, (,), (0))), (0)));

    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_ID, (,), (A, B))), (A, B)));

#define MAC(A) DYND_PP_CAT((x, A, y))
#define DYND_PP_xAy_EQ_xAy
#define DYND_PP_xBy_EQ_xBy
    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(MAC, (,), (A, B))), (xAy, xBy)));
#undef MAC
#undef DYND_PP_xAy_EQ_xAy
#undef DYND_PP_xBy_EQ_xBy

    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_ID, (,), (0, 1))), (0, 1)));

#define MAC(A) DYND_PP_REDUCE(DYND_PP_ADD, DYND_PP_RANGE(A))
    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(MAC, (,), (2, 3, 4))), (1, 3, 6)));
#undef MAC

    EXPECT_TRUE(DYND_PP_IS_NULL(DYND_PP_JOIN_MAP(DYND_PP_TO_NULL, (), (C, A, F, H, B, E, G, D))));
    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_ID, (,), (C, A, F, H, B, E, G, D))),
        (C, A, F, H, B, E, G, D)));

    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_DEC, (,), (5, 1, 4, 3, 7, 1, 2, 6))),
        (4, 0, 3, 2, 6, 0, 1, 5)));
    EXPECT_TRUE(DYND_PP_JOIN_MAP(DYND_PP_NOT, (||), (0, 1, 2, 3, 4, 5, 6, 7)));
    EXPECT_FALSE(DYND_PP_JOIN_MAP(DYND_PP_NOT, (&&), (1, 1, 2, 3, 4, 5, 6, 7)));
    EXPECT_EQ(DYND_PP_JOIN_MAP(DYND_PP_ID, (+), (0, 1, 2, 3, 4, 5, 6, 7)), 28);
    EXPECT_EQ(DYND_PP_JOIN_MAP(DYND_PP_INC, (+), (0, 1, 2, 3, 4, 5, 6, 7)), 36);

    EXPECT_TRUE(DYND_PP_ALL_EQ((DYND_PP_JOIN_MAP(DYND_PP_ID, (,), DYND_PP_RANGE(DYND_PP_LEN_MAX))),
        DYND_PP_RANGE(DYND_PP_LEN_MAX)));

#define MAC(A) (1 << A)
    EXPECT_EQ(DYND_PP_JOIN_MAP(MAC, (+), DYND_PP_RANGE(DYND_PP_LEN_MAX)), MAC(DYND_PP_LEN_MAX) - 1);
#undef MAC

#define MAC(A) DYND_PP_JOIN_MAP_1(DYND_PP_INC, (+), DYND_PP_RANGE(DYND_PP_INC(A)))
    int a[] = {DYND_PP_JOIN_MAP(MAC, (,), DYND_PP_RANGE(DYND_PP_LEN_MAX))};
    EXPECT_EQ(a[0], 1);
    for (int i = 1; i < DYND_PP_LEN_MAX; i++) {
        EXPECT_EQ(a[i], a[i - 1] + i + 1);
    }
#undef MAC
}

TEST(PPList, SliceFrom) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(0, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(1, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(2, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(3, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(4, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(5, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(6, ())));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(7, ())));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(0, (0)), (0)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(1, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(2, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(3, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(4, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(5, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(6, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(7, (0))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(DYND_PP_INT_MAX, (0))));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(0, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(1, (0, 1)), (1)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(2, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(3, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(4, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(5, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(6, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(7, (0, 1))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(DYND_PP_INT_MAX, (0, 1))));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(0, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(1, (2, 1, 4, 7, 3)), (1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(2, (2, 1, 4, 7, 3)), (4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(3, (2, 1, 4, 7, 3)), (7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(4, (2, 1, 4, 7, 3)), (3)));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(5, (2, 1, 4, 7, 3))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(6, (2, 1, 4, 7, 3))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(7, (2, 1, 4, 7, 3))));
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(DYND_PP_INT_MAX, (2, 1, 4, 7, 3))));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(0, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(1, (5, 1, 4, 3, 7, 0, 2, 6)), (1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(2, (5, 1, 4, 3, 7, 0, 2, 6)), (4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(3, (5, 1, 4, 3, 7, 0, 2, 6)), (3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(4, (5, 1, 4, 3, 7, 0, 2, 6)), (7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(5, (5, 1, 4, 3, 7, 0, 2, 6)), (0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(6, (5, 1, 4, 3, 7, 0, 2, 6)), (2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_FROM(7, (5, 1, 4, 3, 7, 0, 2, 6)), (6)));
//    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_FROM(DYND_PP_LEN_MAX, (5, 1, 4, 3, 7, 0, 2, 6))));
}

TEST(PPList, SliceTo) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_TO(0, (0))));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(1, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(2, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(3, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(4, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(5, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(6, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(7, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(DYND_PP_INT_MAX, (0)), (0)));

    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_TO(0, (0, 1))));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(1, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(2, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(3, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(4, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(5, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(6, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(7, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(DYND_PP_INT_MAX, (0, 1)), (0, 1)));

    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_TO(0, (2, 1, 4, 7, 3))));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(1, (2, 1, 4, 7, 3)), (2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(2, (2, 1, 4, 7, 3)), (2, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(3, (2, 1, 4, 7, 3)), (2, 1, 4)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(4, (2, 1, 4, 7, 3)), (2, 1, 4, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(5, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(6, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(7, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(DYND_PP_INT_MAX, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));

    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_SLICE_TO(0, (5, 1, 4, 3, 7, 0, 2, 6))));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(1, (5, 1, 4, 3, 7, 0, 2, 6)), (5)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(2, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(3, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(4, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(5, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(6, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(7, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(8, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_TO(DYND_PP_LEN_MAX, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 6)));
}

TEST(PPList, SliceWith) {
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(1, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(2, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(3, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(4, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(5, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(6, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(7, (0)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(DYND_PP_INT_MAX, (0)), (0)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(1, (0, 1)), (0, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(2, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(3, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(4, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(5, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(6, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(7, (0, 1)), (0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(DYND_PP_INT_MAX, (0, 1)), (0)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(1, (2, 1, 4, 7, 3)), (2, 1, 4, 7, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(2, (2, 1, 4, 7, 3)), (2, 4, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(3, (2, 1, 4, 7, 3)), (2, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(4, (2, 1, 4, 7, 3)), (2, 3)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(5, (2, 1, 4, 7, 3)), (2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(6, (2, 1, 4, 7, 3)), (2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(7, (2, 1, 4, 7, 3)), (2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(DYND_PP_INT_MAX, (2, 1, 4, 7, 3)), (2)));

    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(1, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(2, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 4, 7, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(3, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 3, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(4, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(5, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(6, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(7, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(8, (5, 1, 4, 3, 7, 0, 2, 6)), (5)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE_WITH(DYND_PP_LEN_MAX, (5, 1, 4, 3, 7, 0, 2, 6)), (5)));
}

TEST(PPList, Slice) {
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(0, 8, 1, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(1, 7, 1, (5, 1, 4, 3, 7, 0, 2, 6)), (1, 4, 3, 7, 0, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(2, 6, 1, (5, 1, 4, 3, 7, 0, 2, 6)), (4, 3, 7, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(3, 5, 1, (5, 1, 4, 3, 7, 0, 2, 6)), (3, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(0, 8, 2, (5, 1, 4, 3, 7, 0, 2, 6)), (5, 4, 7, 2)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(1, 7, 2, (5, 1, 4, 3, 7, 0, 2, 6)), (1, 3, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(2, 6, 2, (5, 1, 4, 3, 7, 0, 2, 6)), (4, 7)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_SLICE(3, 5, 2, (5, 1, 4, 3, 7, 0, 2, 6)), (3)));
}

TEST(PPList, Repeat) {
    EXPECT_TRUE(DYND_PP_IS_EMPTY(DYND_PP_REPEAT(1, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REPEAT(1, 1), (1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REPEAT(1, 2), (1, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REPEAT(1, 3), (1, 1, 1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_REPEAT(1, 4), (1, 1, 1, 1)));
}

TEST(PPList, Reduce) {
    EXPECT_EQ(DYND_PP_REDUCE(DYND_PP_ADD, (0, 0)), 0);
    EXPECT_EQ(DYND_PP_REDUCE(DYND_PP_SUB, (5, 2)), 3);

    EXPECT_TRUE(DYND_PP_REDUCE(DYND_PP_OR, (0, 2, 5, 2)));

    EXPECT_EQ(DYND_PP_REDUCE(DYND_PP_ADD, (1, 1, 1, 1, 1, 1, 1)), 7);
    EXPECT_EQ(DYND_PP_REDUCE(DYND_PP_SUB, (7, 0, 4, 0, 0, 2, 0, 1)), 0);
    EXPECT_FALSE(DYND_PP_REDUCE(DYND_PP_AND, (5, 1, 4, 3, 7, 0, 2, 6)));

    EXPECT_TRUE(DYND_PP_REDUCE(DYND_PP_OR, DYND_PP_RANGE(DYND_PP_LEN_MAX)));
}

TEST(PPList, All) {
    EXPECT_FALSE(DYND_PP_ALL((0)));
    EXPECT_TRUE(DYND_PP_ALL((1)));
    EXPECT_TRUE(DYND_PP_ALL((3)));

    EXPECT_FALSE(DYND_PP_ALL((0, 0)));
    EXPECT_FALSE(DYND_PP_ALL((0, 1)));
    EXPECT_FALSE(DYND_PP_ALL((1, 0)));
    EXPECT_TRUE(DYND_PP_ALL((1, 1)));
    EXPECT_FALSE(DYND_PP_ALL((0, 3)));
    EXPECT_TRUE(DYND_PP_ALL((4, 7)));

    EXPECT_FALSE(DYND_PP_ALL((0, 0, 0, 0, 0, 0, 0, 0)));
    EXPECT_TRUE(DYND_PP_ALL((1, 1, 1, 1, 1, 1, 1, 1)));
    EXPECT_FALSE(DYND_PP_ALL((5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ALL((5, 1, 4, 3, 7, 1, 2, 6)));

    EXPECT_FALSE(DYND_PP_ALL(DYND_PP_RANGE(DYND_PP_LEN_MAX)));
    EXPECT_TRUE(DYND_PP_ALL(DYND_PP_PREPEND(1, DYND_PP_RANGE(1, DYND_PP_LEN_MAX))));
}

TEST(PPList, Any) {
    EXPECT_FALSE(DYND_PP_ANY((0)));
    EXPECT_TRUE(DYND_PP_ANY((1)));
    EXPECT_TRUE(DYND_PP_ANY((3)));

    EXPECT_FALSE(DYND_PP_ANY((0, 0)));
    EXPECT_TRUE(DYND_PP_ANY((0, 1)));
    EXPECT_TRUE(DYND_PP_ANY((1, 0)));
    EXPECT_TRUE(DYND_PP_ANY((1, 1)));
    EXPECT_TRUE(DYND_PP_ANY((0, 3)));
    EXPECT_TRUE(DYND_PP_ANY((4, 7)));

    EXPECT_FALSE(DYND_PP_ANY((0, 0, 0, 0, 0, 0, 0, 0)));
    EXPECT_TRUE(DYND_PP_ANY((1, 1, 1, 1, 1, 1, 1, 1)));
    EXPECT_TRUE(DYND_PP_ANY((5, 1, 4, 3, 7, 0, 2, 6)));
    EXPECT_TRUE(DYND_PP_ANY((5, 1, 4, 3, 7, 1, 2, 6)));

    EXPECT_TRUE(DYND_PP_ANY(DYND_PP_RANGE(DYND_PP_LEN_MAX)));
    EXPECT_TRUE(DYND_PP_ANY(DYND_PP_PREPEND(1, DYND_PP_RANGE(1, DYND_PP_LEN_MAX))));
}

/*
TEST(PPList, Outer) {
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_OR, (0), (1)), (1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_OR, (1), (0)), (1)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_AND, (1), (0, 0)), (0, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_AND, (0, 0), (1)), (0, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_XOR, (0, 1), (0, 1)), (0, 1, 1, 0)));
    EXPECT_TRUE(DYND_PP_ALL_EQ(DYND_PP_OUTER(DYND_PP_ADD, (1, 4), (2, 3)), (3, 4, 6, 7)));
}
*/
