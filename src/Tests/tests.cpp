#ifndef ARDUINO

#include "../BiQuadFilter.h"
#include "../SOSFilter.h"
#include "../Cascade.h"

#include <catch.hpp>
using Catch::Matchers::WithinULP;

#include <iostream>
using namespace std;

TEST_CASE("BiQuadDF2 test", "[BIQUADDF2]")
{
    const double b[] = {0.500000000000000, 0, -0.500000000000000};
    const double a[] = {0.500000000000000, 0.422665745365229, 0.262062811036003};
    const double gain = 0.5;

    const double signal[] = {1, 2, 3, 2, 1, 0, 0, 0, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const size_t signalLength = sizeof(signal) / sizeof(signal[0]);

    const double expected[] = {0.50000000000000, 0.577334254634772, 0.249898362843829, -0.513842630947802, -0.696610777660332, -0.141814884268856, -0.0150078552899556, 0.0870154271214089, 1.93430912076473, 0.319260572540564, -1.78370198691754, -0.159513186566006, -0.430274566724766, -0.0526704109495565, 0.270041881951755, -0.200668994726211, 0.0280959510918313, 0.0814253694714912, -0.0835572368023246, 0.0279564411207387, 0.0201620286695239};

    BiQuadFilterDF2 filter(b, a, gain);
    for (size_t i = 0; i < signalLength; i++)
    {
        REQUIRE_THAT(
            filter.filter(signal[i]),
            WithinULP(expected[i], 1 << 7));
    }
}

TEST_CASE("BiQuadDF1 test", "[BIQUADDF1]")
{
    const double b[] = {0.500000000000000, 0, -0.500000000000000};
    const double a[] = {0.500000000000000, 0.422665745365229, 0.262062811036003};
    const double gain = 0.5;

    const double signal[] = {1, 2, 3, 2, 1, 0, 0, 0, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const size_t signalLength = sizeof(signal) / sizeof(signal[0]);

    const double expected[] = {0.50000000000000, 0.577334254634772, 0.249898362843829, -0.513842630947802, -0.696610777660332, -0.141814884268856, -0.0150078552899556, 0.0870154271214089, 1.93430912076473, 0.319260572540564, -1.78370198691754, -0.159513186566006, -0.430274566724766, -0.0526704109495565, 0.270041881951755, -0.200668994726211, 0.0280959510918313, 0.0814253694714912, -0.0835572368023246, 0.0279564411207387, 0.0201620286695239};

    BiQuadFilterDF1 filter(b, a, gain);
    for (size_t i = 0; i < signalLength; i++)
    {
        REQUIRE_THAT(
            filter.filter(signal[i]),
            WithinULP(expected[i], 1 << 7));
    }
}

TEST_CASE("Cascade test", "[CASCADE]")
{
    /*
    SOS Matrix:                                                                                       
    1  0  -1  1  -1.290129716329686315745561842049937695265  0.717874255231761404516532820707652717829
    1  0  -1  1  -0.688415850612193125357407552655786275864  0.631975601790225605114414975105319172144
                                                                                                    
    Scale Values:                                                                                     
    0.235753158418643082860910453746328130364                                                         
    0.235753158418643082860910453746328130364
    */

    const double b1[] = {1, 0, -1};
    const double b2[] = {1, 0, -1};
    const double a1[] = {1, -1.290129716329686315745561842049937695265, 0.717874255231761404516532820707652717829};
    const double a2[] = {1, -0.688415850612193125357407552655786275864, 0.631975601790225605114414975105319172144};
    const double gain1 = 0.235753158418643082860910453746328130364;
    const double gain2 = 0.235753158418643082860910453746328130364;

    const double signal[] = {1, 2, 3, 2, 1, 0, 0, 0, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const size_t signalLength = sizeof(signal) / sizeof(signal[0]);

    const double expected[] = {
        55.5795517043658e-003, 221.125779046022e-003, 368.700189236404e-003, 196.235397222664e-003,
        -394.852138278890e-003, -949.063457160015e-003, -848.804476542809e-003, -50.3366148133755e-003,
        1.01423752153364e+000, 1.66072468315129e+000, 1.05723023392403e+000, -662.965610458666e-003,
        -2.07431865232857e+000, -1.87822565793893e+000, -254.914590070918e-003, 1.33907848244674e+000,
        1.70141405938642e+000, 887.767638240748e-003, -182.060273821692e-003, -726.499592715404e-003,
        -639.303044095792e-003};
    BiQuadFilterDF2 filter1(b1, a1, gain1);
    BiQuadFilterDF2 filter2(b2, a2, gain2);
    Cascade<2> filter({&filter1, &filter2});
    for (size_t i = 0; i < signalLength; i++)
    {
        REQUIRE_THAT(
            filter.filter(signal[i]),
            WithinULP(expected[i], 1 << 7));
    }
}

TEST_CASE("SOSFilter test", "[SOSFILTER]")
{
    /*
    SOS Matrix:                                                                                       
    1  0  -1  1  -1.290129716329686315745561842049937695265  0.717874255231761404516532820707652717829
    1  0  -1  1  -0.688415850612193125357407552655786275864  0.631975601790225605114414975105319172144
                                                                                                    
    Scale Values:                                                                                     
    0.235753158418643082860910453746328130364                                                         
    0.235753158418643082860910453746328130364
    */

    SOSFilter<2> filter({
        {1, 0, -1},
        {1, 0, -1}
    }, {
        {1, -1.290129716329686315745561842049937695265, 0.717874255231761404516532820707652717829},
        {1, -0.688415850612193125357407552655786275864, 0.631975601790225605114414975105319172144}
    }, {
        0.235753158418643082860910453746328130364,
        0.235753158418643082860910453746328130364
    });

    const double signal[] = {1, 2, 3, 2, 1, 0, 0, 0, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const size_t signalLength = sizeof(signal) / sizeof(signal[0]);

    const double expected[] = {
        55.5795517043658e-003, 221.125779046022e-003, 368.700189236404e-003, 196.235397222664e-003,
        -394.852138278890e-003, -949.063457160015e-003, -848.804476542809e-003, -50.3366148133755e-003,
        1.01423752153364e+000, 1.66072468315129e+000, 1.05723023392403e+000, -662.965610458666e-003,
        -2.07431865232857e+000, -1.87822565793893e+000, -254.914590070918e-003, 1.33907848244674e+000,
        1.70141405938642e+000, 887.767638240748e-003, -182.060273821692e-003, -726.499592715404e-003,
        -639.303044095792e-003};

    for (size_t i = 0; i < signalLength; i++)
    {
        REQUIRE_THAT(
            filter.filter(signal[i]),
            WithinULP(expected[i], 1 << 7));
    }
}

TEST_CASE("SOSFilter test Matrix", "[SOSFILTER]")
{
    /*
    SOS Matrix:                                                                                       
    1  0  -1  1  -1.290129716329686315745561842049937695265  0.717874255231761404516532820707652717829
    1  0  -1  1  -0.688415850612193125357407552655786275864  0.631975601790225605114414975105319172144
                                                                                                    
    Scale Values:                                                                                     
    0.235753158418643082860910453746328130364                                                         
    0.235753158418643082860910453746328130364
    */

    SOSFilter<2> filter({
        {1,  0,  -1,  1,  -1.290129716329686315745561842049937695265,  0.717874255231761404516532820707652717829},
        {1,  0,  -1,  1,  -0.688415850612193125357407552655786275864,  0.631975601790225605114414975105319172144}
    }, {
        0.235753158418643082860910453746328130364,
        0.235753158418643082860910453746328130364
    });

    const double signal[] = {1, 2, 3, 2, 1, 0, 0, 0, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const size_t signalLength = sizeof(signal) / sizeof(signal[0]);

    const double expected[] = {
        55.5795517043658e-003, 221.125779046022e-003, 368.700189236404e-003, 196.235397222664e-003,
        -394.852138278890e-003, -949.063457160015e-003, -848.804476542809e-003, -50.3366148133755e-003,
        1.01423752153364e+000, 1.66072468315129e+000, 1.05723023392403e+000, -662.965610458666e-003,
        -2.07431865232857e+000, -1.87822565793893e+000, -254.914590070918e-003, 1.33907848244674e+000,
        1.70141405938642e+000, 887.767638240748e-003, -182.060273821692e-003, -726.499592715404e-003,
        -639.303044095792e-003};

    for (size_t i = 0; i < signalLength; i++)
    {
        REQUIRE_THAT(
            filter.filter(signal[i]),
            WithinULP(expected[i], 1 << 7));
    }
}

#endif