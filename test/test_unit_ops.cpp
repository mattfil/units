#include "test.hpp"
#include "units/unit_definitions.hpp"
#include "units/units_decl.hpp"

using namespace units;
TEST(unitOps, Simple)
{
    EXPECT_EQ(m, m);
    EXPECT_EQ(m * m, m * m);
    EXPECT_NE(m * m, s * s);
    EXPECT_EQ(V / km, mV / m);
    static_assert((V / km).is_exactly_the_same(mV / m), "units not static equivalent");
}

TEST(unitOps, hash)
{
    auto h1 = std::hash<unit>()(N);
    auto u2 = one / N;
    auto h2 = std::hash<unit>()(u2.inv());
    EXPECT_EQ(h1, h2);
}

TEST(unitOps, Inv)
{
    EXPECT_EQ(m.inv(), one / m);
    static_assert((m.inv()).is_exactly_the_same(one / m), "inverse units not static equivalent");
    EXPECT_EQ(m.inv().inv(), m);
    EXPECT_EQ(N.inv(), one / N);

    EXPECT_EQ(gal.inv().inv(), gal);
}

TEST(unitOps, MultipleOps)
{
    auto u1 = kW / gal;
    auto u2 = u1 / kW;
    auto u3 = u2.inv();
    EXPECT_EQ(u3, gal);
}

TEST(unitOps, power)
{
    auto m2 = m.pow(2);
    EXPECT_EQ(m * m, m2);
    auto m4 = m.pow(4);
    EXPECT_EQ(m * m * m * m, m4);
    auto m4_b = m2.pow(2);
    EXPECT_EQ(m4_b, m * m * m * m);
    EXPECT_EQ(m4_b, m2 * m2);

    EXPECT_EQ(m.inv(), m.pow(-1));
    EXPECT_EQ(m.inv().inv(), m.pow(-1).pow(-1));
}

TEST(unitOps, root)
{
    auto m2 = m.pow(2);
    EXPECT_EQ(m, m2.root(2));
    auto m4 = m.pow(4);
    EXPECT_EQ(m * m, m4.root(2));
    EXPECT_EQ(m, m4.root(4));

    auto ft2 = ft.pow(2);
    EXPECT_EQ(ft, ft2.root(2));
    auto ft4 = ft.pow(4);
    EXPECT_EQ(ft * ft, ft4.root(2));
    EXPECT_EQ(ft, ft4.root(4));
}

TEST(unitOps, nan)
{
    EXPECT_TRUE(isnan(invalid));
    EXPECT_FALSE(isnan(defunit));
    auto zunit = unit(0.0, m);
    auto zunit2 = unit(0.0, kg);
    auto nunit = zunit2 / zunit;
    EXPECT_TRUE(isnan(nunit));
    EXPECT_TRUE(isnan(unit_cast(precise::invalid)));
}

TEST(unitOps, inf)
{
    EXPECT_FALSE(isinf(invalid));
    EXPECT_FALSE(isinf(defunit));
    EXPECT_FALSE(isinf(V));
    auto zunit = unit(0.0, m);
    auto nunit = kg / zunit;
    EXPECT_TRUE(isinf(nunit));
    EXPECT_TRUE(isinf(unit_cast(precise_unit(nunit))));
}

TEST(unitOps, cast)
{
    EXPECT_EQ(ft, unit_cast(precise::ft));
    EXPECT_EQ(gal, unit_cast(precise::gal));
    EXPECT_TRUE(is_unit_cast_lossless(precise::m));
    EXPECT_FALSE(is_unit_cast_lossless(precise::gal));
}

TEST(preciseUnitOps, Simple)
{
    EXPECT_EQ(precise::m, precise::m);
    EXPECT_EQ(precise::m * precise::m, precise::m * precise::m);
    EXPECT_NE(precise::m * precise::m, precise::s * precise::s);
    EXPECT_EQ(precise::V / precise::km, precise::mV / precise::m);
}

TEST(preciseUnitOps, Hash)
{
    auto h1 = std::hash<precise_unit>()(precise::N);
    auto u2 = precise::one / precise::N;
    auto h2 = std::hash<precise_unit>()(u2.inv());
    EXPECT_EQ(h1, h2);
}

TEST(preciseUnitOps, Inv)
{
    EXPECT_EQ(precise::m.inv(), precise::one / precise::m);
    EXPECT_EQ(precise::m.inv().inv(), precise::m);
    EXPECT_EQ(precise::N.inv(), precise::one / precise::N);

    EXPECT_EQ(precise::gal.inv().inv(), precise::gal);
}

TEST(preciseUnitOps, MultipleOps)
{
    auto u1 = precise::kW / precise::gal;
    auto u2 = u1 / precise::kW;
    auto u3 = u2.inv();
    EXPECT_EQ(u3, precise::gal);
}

TEST(preciseUnitOps, Power)
{
    auto m2 = precise::m.pow(2);
    EXPECT_EQ(precise::m * precise::m, m2);
    auto m4 = m.pow(4);
    EXPECT_EQ(precise::m * precise::m * precise::m * precise::m, m4);
    auto m4_b = m2.pow(2);
    EXPECT_EQ(m4_b, precise::m * precise::m * precise::m * precise::m);
    EXPECT_EQ(m4_b, m2 * m2);
}

TEST(preciseUnitOps, nan)
{
    EXPECT_TRUE(isnan(precise::invalid));
    EXPECT_FALSE(isnan(precise::defunit));
    auto zunit = precise_unit(0.0, m);
    auto zunit2 = precise_unit(0.0, kg);
    auto nunit = zunit2 / zunit;
    EXPECT_TRUE(isnan(nunit));

    EXPECT_TRUE(isnan(precise_unit(invalid)));
}

TEST(preciseUnitOps, inf)
{
    EXPECT_FALSE(isinf(precise::invalid));
    EXPECT_FALSE(isinf(precise::defunit));
    EXPECT_FALSE(isinf(V));
    auto zunit = precise_unit(0.0, precise::m);
    auto nunit = precise::kg / zunit;
    EXPECT_TRUE(isinf(nunit));
    EXPECT_TRUE(isinf(precise_unit(nunit)));
}

TEST(preciseUnitOps, cast)
{
    EXPECT_NE(precise_unit(ft), precise::ft);
    EXPECT_EQ(precise_unit(m), precise::m);
}

TEST(invalidOps, saturate)
{
    for (int ii = -8; ii < 8; ++ii)
    {
        auto nunit = precise::s.pow(ii);
        auto nunit2 = nunit.pow(2);
        EXPECT_EQ(nunit2.base_units().kg(), 0);
        EXPECT_EQ(nunit2.base_units().meter(), 0);
    }
}

TEST(specialOps, rootHertz)
{
    auto res = precise::special::ASD.pow(2);
    EXPECT_EQ(res, precise::m.pow(2) / precise::s.pow(4) / precise::Hz);
    EXPECT_FALSE(precise::special::ASD.is_error());
}

TEST(customUnits, definition)
{
    auto cunit1 = precise::generate_custom_unit(4);
    auto cunit2 = precise::generate_custom_unit(7);
    EXPECT_FALSE(cunit1 == cunit2);
}

TEST(customUnits, testAllInv)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_unit_inverted(cunit1.base_units()));

        EXPECT_FALSE(precise::custom::is_custom_count_unit(cunit1.base_units()));
        auto cunit2 = cunit1.inv();
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit2.base_units()));
        EXPECT_TRUE(precise::custom::is_custom_unit_inverted(cunit2.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_count_unit(cunit2.base_units()));
        auto cunit3 = cunit2.inv();
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit3.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_unit_inverted(cunit3.base_units()));

        EXPECT_FALSE(cunit1 == cunit2) << "Error with false comparison 1 index " << ii;
        EXPECT_FALSE(cunit2 == cunit3) << "Error with false comparison 2 index " << ii;
        EXPECT_TRUE(cunit1 == cunit3) << "Error with inversion " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit2.base_units()))
          << "Error with custom unit detection of inverse " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit3.base_units()))
          << "Error with custom unit detection inv inv" << ii;
    }
}

TEST(customUnits, uniqueness)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1inv = cunit1.inv();

        EXPECT_EQ(precise::custom::custom_unit_number(cunit1.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1.base_units()), ii);
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            EXPECT_FALSE(cunit1 == cunit2) << "Error with false comparison 1 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1 == cunit2.inv()) << "Error with false comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1inv == cunit2) << "Error with false inv comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1inv == cunit2.inv())
              << "Error with false inv comparison 2 inv index " << ii << "," << jj;
        }
    }
}

TEST(customUnits, uniquenesspermeter)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1adj = cunit1 / precise::meter;

        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.inv().base_units()), ii);
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit/meter detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1adj.base_units()))
          << "Error with custom unit/meter detection of inverse " << ii;
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            auto cunit2adj = cunit2 / precise::m;
            EXPECT_FALSE(cunit1 == cunit2adj)
              << "Error with false per meter comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2)
              << "Error with false per meter comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2adj)
              << "Error with false per meter comparison 2 per meter index " << ii << "," << jj;
        }
        EXPECT_TRUE(cunit1adj * precise::m == cunit1) << "Error with true per meter multiply" << ii;
    }
}

TEST(customUnits, uniquenesspermeter2)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1adj = cunit1 / precise::meter.pow(2);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.inv().base_units()), ii);

        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit/meter2 detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1adj.base_units()))
          << "Error with custom unit/meter2 detection of inverse " << ii;
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            auto cunit2adj = cunit2 / precise::m.pow(2);
            EXPECT_FALSE(cunit1 == cunit2adj)
              << "Error with false per meter2 comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2)
              << "Error with false per meter2 comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2adj)
              << "Error with false per meter2 comparison 2 per meter2 index " << ii << "," << jj;
        }
        EXPECT_TRUE(cunit1adj * precise::m * precise::m == cunit1) << "Error with true per meter2 multiply" << ii;
    }
}

TEST(customUnits, uniquenesspermeter3)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1adj = cunit1 / precise::meter.pow(3);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.inv().base_units()), ii);
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit/meter3 detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1adj.base_units()))
          << "Error with custom unit/meter3 detection of inverse " << ii;
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            auto cunit2adj = cunit2 / precise::m.pow(3);
            EXPECT_FALSE(cunit1 == cunit2adj)
              << "Error with false per meter3 comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2)
              << "Error with false per meter3 comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2adj)
              << "Error with false per meter3 comparison 2 per meter3 index " << ii << "," << jj;
        }
        EXPECT_TRUE(cunit1adj * precise::m.pow(3) == cunit1) << "Error with true per meter3 multiply" << ii;
    }
}

TEST(customUnits, uniquenessperkg)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1adj = cunit1 / precise::kg;
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.inv().base_units()), ii);
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit/kg detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1adj.base_units()))
          << "Error with custom unit/kg detection of inverse " << ii;
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            auto cunit2adj = cunit2 / precise::kg;
            EXPECT_FALSE(cunit1 == cunit2adj)
              << "Error with false per kg comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2) << "Error with false per kg comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2adj)
              << "Error with false per kg comparison 2 per kg index " << ii << "," << jj;
        }
        EXPECT_TRUE(cunit1adj * precise::kg == cunit1) << "Error with true per kg multiply" << ii;
    }
}

TEST(customUnits, uniquenesspersecond)
{
    for (unsigned short ii = 0; ii < 1024; ++ii)
    {
        auto cunit1 = precise::generate_custom_unit(ii);
        auto cunit1adj = cunit1 / precise::s;
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_unit_number(cunit1adj.inv().base_units()), ii);
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1.base_units()))
          << "Error with custom unit/s detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_unit(cunit1adj.base_units()))
          << "Error with custom unit/s detection of inverse " << ii;
        for (unsigned short jj = 0; jj < 1024; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_unit(jj);
            auto cunit2adj = cunit2 / precise::s;
            EXPECT_FALSE(cunit1 == cunit2adj)
              << "Error with false per sec comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2) << "Error with false per sec comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1adj == cunit2adj)
              << "Error with false per sec comparison 2 per sec index " << ii << "," << jj;
        }
        EXPECT_TRUE(cunit1adj * precise::s == cunit1) << "Error with true per sec multiply" << ii;
    }
}

TEST(customCountUnits, definition)
{
    auto cunit1 = precise::generate_custom_count_unit(4);
    auto cunit2 = precise::generate_custom_count_unit(7);
    EXPECT_FALSE(cunit1 == cunit2);
}

TEST(customCountUnits, testAllInv)
{
    for (unsigned short ii = 0; ii < 16; ++ii)
    {
        auto cunit1 = precise::generate_custom_count_unit(ii);
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit1.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_count_unit_inverted(cunit1.base_units()));

        EXPECT_FALSE(precise::custom::is_custom_unit(cunit1.base_units()));
        auto cunit2 = cunit1.inv();
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit2.base_units()));
        EXPECT_TRUE(precise::custom::is_custom_count_unit_inverted(cunit2.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_unit(cunit2.base_units()));
        auto cunit3 = cunit2.inv();
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit3.base_units()));
        EXPECT_FALSE(precise::custom::is_custom_count_unit_inverted(cunit3.base_units()));

        EXPECT_FALSE(cunit1 == cunit2) << "Error with false comparison 1 index " << ii;
        EXPECT_FALSE(cunit2 == cunit3) << "Error with false comparison 2 index " << ii;
        EXPECT_TRUE(cunit1 == cunit3) << "Error with inversion " << ii;
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit1.base_units()))
          << "Error with custom unit detection " << ii;
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit2.base_units()))
          << "Error with custom unit detection of inverse " << ii;
        EXPECT_TRUE(precise::custom::is_custom_count_unit(cunit3.base_units()))
          << "Error with custom unit detection inv inv" << ii;
    }
}

TEST(customCountUnits, uniqueness)
{
    for (unsigned short ii = 0; ii < 16; ++ii)
    {
        auto cunit1 = precise::generate_custom_count_unit(ii);
        auto cunit1inv = cunit1.inv();

        EXPECT_EQ(precise::custom::custom_count_unit_number(cunit1.base_units()), ii);
        EXPECT_EQ(precise::custom::custom_count_unit_number(cunit1.base_units()), ii);
        for (unsigned short jj = 0; jj < 16; ++jj)
        {
            if (ii == jj)
            {
                continue;
            }
            auto cunit2 = precise::generate_custom_count_unit(jj);
            EXPECT_FALSE(cunit1 == cunit2) << "Error with false comparison 1 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1 == cunit2.inv()) << "Error with false comparison 1 inv index " << ii << "," << jj;
            EXPECT_FALSE(cunit1inv == cunit2) << "Error with false inv comparison 2 index " << ii << "," << jj;
            EXPECT_FALSE(cunit1inv == cunit2.inv())
              << "Error with false inv comparison 2 inv index " << ii << "," << jj;
        }
    }
}