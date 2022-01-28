//
// Created by alice on 28.01.22.
//

// Google Test framework
#include <gtest/gtest.h>

#include <iostream>
#include <numeric>

#include "fldr.hpp"
#include "linear_congruential_generator.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


using namespace FLDR;

std::vector<int> get_distribution(int length) {
    std::vector<int> distribution(length);
    for (std::size_t i{}; i < distribution.size(); ++i) {
        distribution[i] = static_cast<int>(i % 3);
    }
    return distribution;
}

constexpr int distribution(int i) {
    return i % 3;
}

LCG lcg{42};

bool flip() {
    static int k = 62;
    static unsigned long flip_word = 0;
    static int flip_pos = 0;

    if (flip_pos == 0) {
        flip_word = lcg.next();
        flip_pos = k;
    }
    --flip_pos;
    return (flip_word >> flip_pos) & 1;
}


TEST(fldr, basic_test) {
    constexpr int distribution_size = 0xFF;

    using FLDR = FastLoadedDiceRoller<decltype(distribution),
            distribution_size,
            sum_distribution(distribution, distribution_size)>;
    constexpr FLDR fldr(distribution);

    constexpr std::size_t NUM_SAMPLES{10};

    std::vector<int> result;

    for (std::size_t i{}; i < NUM_SAMPLES; ++i) {
        result.push_back(fldr.fldr_sample(flip));
    }

    EXPECT_EQ(result, (std::vector<int>{{49, 50, 193, 56, 182, 14, 158, 62, 166, 221} }));
}


TEST(fldr, bigger_test) {
    constexpr int distribution_size = 0xFF;
    using FLDR = FastLoadedDiceRoller<decltype(distribution),
            distribution_size,
            sum_distribution(distribution, distribution_size)>;
    constexpr FLDR fldr(distribution);

    constexpr std::size_t NUM_SAMPLES{10'000'000};

    std::vector<int> result;

    for (std::size_t i{}; i < NUM_SAMPLES; ++i) {
        result.push_back(fldr.fldr_sample(flip));
    }

    EXPECT_EQ(std::accumulate(result.begin(), result.end(), 0UL), 0);
}
