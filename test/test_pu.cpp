#include "units/units.hpp"

#include "test.hpp"

using namespace units;

TEST(PU, basic)
{
    EXPECT_EQ(pu * V, puV);
    EXPECT_EQ(puHz, pu * Hz);
    EXPECT_TRUE(puHz.is_per_unit());
}

TEST(PU, Ops)
{
    EXPECT_EQ(puV * puA, pu * W);
    EXPECT_EQ(puV / puA, puOhm);
}

TEST(PU, Example1)
{
    EXPECT_EQ(convert(1.0, puMW, ohm, 10000.0, 100.0), 1.0);
    EXPECT_NEAR(convert(136.0, kV, puV, 500, 138000), 0.9855, test::tolerance * 100);
    // problem from text book
    auto basePower = 100000000;

    EXPECT_NEAR(convert(1.0, ohm, puOhm, basePower, 8000), 1.56, 0.01);
    EXPECT_NEAR(convert(24.0, ohm, puOhm, basePower, 80000), 0.375, 0.01);
    EXPECT_NEAR(convert(1.0, ohm, puOhm, basePower, 16000), 0.39, 0.01);
    EXPECT_NEAR(convert(1.0, puOhm, ohm, basePower, 8000), 0.64, 0.01);
    EXPECT_NEAR(convert(1.0, puOhm, ohm, basePower, 80000), 64, 0.01);
    EXPECT_NEAR(convert(1.0, puOhm, ohm, basePower, 16000), 2.56, 0.01);

    EXPECT_NEAR(convert(0.22, puA, A, 100000000, 80000), 275, 0.1);
}

TEST(PU, Conversions_just_pu)
{
    EXPECT_EQ(convert(1.0, pu, ohm, 5.0), 5.0);
    EXPECT_NEAR(convert(136.0, kV, pu, 500, 138000), 0.9855, test::tolerance * 100);
    EXPECT_EQ(convert(2.7, pu, puMW), 2.7);
}