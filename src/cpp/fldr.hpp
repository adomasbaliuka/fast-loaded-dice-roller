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

#include <cstdlib>
#include <vector>

namespace FLDR {

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
    struct FastLoadedDiceRoller {
        explicit FastLoadedDiceRoller(std::vector<int> distribution) {
            int *a = distribution.data();
            n = distribution.size(); // TODO check length. This is a narrowing conversion.

            m = 0;
            for (std::size_t i = 0; i < n; i++) {
                m += a[i];
            }
            k = ceil_log2(m);
            r = (1 << k) - m;

            h = static_cast<int *>(calloc(k, sizeof(*h)));
            H = static_cast<int *>(calloc((n + 1) * k, sizeof(*H)));

            int d;
            for (int j = 0; j < k; j++) {
                d = 0;
                for (int i = 0; i < n; i++) {
                    H[i * k + j] = -1;
                    bool w = (a[i] >> ((k - 1) - j)) & 1;
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
        };

        ~FastLoadedDiceRoller() {
            free(h);
            free(H);
        }

        FastLoadedDiceRoller(FastLoadedDiceRoller const& rhs) = delete; // copy constructor
        FastLoadedDiceRoller& operator=(FastLoadedDiceRoller const& rhs) = delete; // copy assignment

        template<typename FlipFunction>
        int fldr_sample(FlipFunction flip) {
            if (n == 1) {
                return 0;
            }

            int c = 0;
            int d = 0;
            while (true) {
                int b = flip();
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

        static constexpr int ceil_log2(unsigned long long x) {
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
            int i{};

            for (i = 0; i < 6; i++) {
                int k = (((x & t[i]) == 0) ? 0 : j);
                y += k;
                x >>= k;
                j >>= 1;
            }

            return y;
        }

        int n;
        int m;
        int k;
        int r;
        int *h;
        int *H;
    };

}

#endif
