//
// Created by alice on 28.01.22.
//

#ifndef FLDR_CPP_LINEAR_CONGRUENTIAL_GENERATOR_HPP
#define FLDR_CPP_LINEAR_CONGRUENTIAL_GENERATOR_HPP

#include <cstdint>
#include <vector>

// TODO this file should not be committed to version control!

// deterministic way to add noise to bitstrings, reproducible in Julia
// adapted from https://codereview.stackexchange.com/questions/195218/implementation-of-a-linear-congruential-generator
class LCG {
    static constexpr uint64_t A = 0x5851F42D4C957F2D;
    static constexpr uint64_t C = 0x14057B7EF767814F;

    uint64_t this_seed;

public:
    explicit LCG(std::uint64_t seed) noexcept
            : this_seed(seed) {
    }

    void seed(uint64_t const value) noexcept {
        this_seed = value;
    }

    // [0, 2 ^ 64 - 1)
    auto next() noexcept {
        this_seed = this_seed * A + C;
        return this_seed;
    }


    // [0, 1)
    double get() noexcept {
        return static_cast<double>(next() >> 11) * (1.0 / (UINT64_C(1) << 53));
    }

    template<typename T>
    void noise_bitstring_inplace(std::vector<T> &src, double err_prob) {
        for (std::size_t i = 0; i < src.size(); i++) {
            if (get() <= err_prob) {
                src[i] = !src[i];
            }
        }
    }
};


#endif //FLDR_CPP_LINEAR_CONGRUENTIAL_GENERATOR_HPP
