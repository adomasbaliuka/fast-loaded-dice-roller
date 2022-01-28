/*
  Name:     fldr.h
  Purpose:  Fast sampling of random integers.
  Author:   F. A. Saad
  Copyright (C) 2020 Feras A. Saad, All Rights Reserved.

  Released under Apache 2.0; refer to LICENSE.txt
*/

// The license requires this statement:
// Adomas Baliuka changed this code in 2022. The changes are porting the code to C++.


#ifndef FLDR_H
#define FLDR_H


#include <array>

namespace FLDR {

    template<typename Distribution>
    constexpr unsigned long sum_distribution(Distribution distribution, std::size_t distribution_size) {
        unsigned long sum{};
        for (std::size_t i = 0; i < distribution_size; i++) {
            sum += distribution(i);
        }

        return sum;
    }

    constexpr unsigned int ceil_log2(unsigned long long x) {
        constexpr unsigned long long t[6] = {
                0xFFFFFFFF00000000ull,
                0x00000000FFFF0000ull,
                0x000000000000FF00ull,
                0x00000000000000F0ull,
                0x000000000000000Cull,
                0x0000000000000002ull
        };

        int y = (((x & (x - 1)) == 0) ? 0 : 1);
        int j = 32;

        for (auto i : t) {
            int k_ = (((x & i) == 0) ? 0 : j);
            y += k_;
            x >>= k_;
            j >>= 1;
        }

        return y;
    }

    /*!
     *     Name:     fldr.h
     *     Purpose:  Fast sampling of random integers.
     *     Author:   F. A. Saad
     *     Copyright (C) 2020 Feras A. Saad, All Rights Reserved.
     *
     *     Released under Apache 2.0; refer to LICENSE.txt
     *
     *
     * The license requires this statement:
     * Adomas Baliuka changed this code in 2022. The changes are porting the code to C++.

     * @tparam int unsigned integer type.
     */
    template<typename Distribution, int distribution_size, unsigned long distribution_sum>
    struct FastLoadedDiceRoller {
        constexpr inline static int n{distribution_size};
        constexpr inline static int m{distribution_sum};
        constexpr inline static int k{ceil_log2(m)};
        constexpr inline static int r{(1 << k) - m};

        const std::array<int, k> h;
        const std::array<int, (n + 1) * k> H;

        constexpr explicit FastLoadedDiceRoller(Distribution distribution) :
                h(init(distribution).first),
                H(init(distribution).second)
        { }

        template<typename FlipFunction>
        constexpr int fldr_sample(FlipFunction flip) const {
            if (n == 1) {
                return 0;
            }
            // TODO check if `int` can (or should?!) be replaced by a larger integral type
            int c = 0;
            int d = 0;
            while (true) {
                bool b = flip(); // TODO check if this should be int or bool. (most likely makes no difference)
                d = 2 * d + (1 - b);
                if (d < h[c]) {
                    int z = H[d * k + c];
                    if (z < n) {
                        return z;
                    } else {
                        d = 0;
                        c = 0;
                    }
                } else {
                    d = d - h[c];
                    c = c + 1;
                }
            }
        }

    private:
        // initialize the two arrays h and H
        static constexpr auto init(Distribution distribution) {
            std::array<int, k> h{};
            std::array<int, (n + 1) * k> H{};

            int d{};
            for (int j = 0; j < k; j++) {
                d = 0;
                for (int i = 0; i < n; i++) {
                    H[i * k + j] = -1;
                    bool w = (distribution(i) >> ((k - 1) - j)) & 1;
                    h[j] += w;
                    if (w) {
                        H[d * k + j] = i;
                        d += 1;
                    }
                }
                H[n * k + j] = -1;
                bool w = (r >> ((k - 1) - j)) & 1;
                h[j] += w;
                if (w) {
                    H[d * k + j] = n;
                    d += 1;
                }
            }

            return std::make_pair(h, H);
        }
    };

}

#endif
