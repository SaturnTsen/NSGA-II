#include <stdexcept>
#include <cassert>
#include <span>
#include "benchmark.h"

namespace benchmark {
    using individual::individual_t;

    int lotzk(int k, individual::span x) {
        int n = x.size();
        if (k == 0) {
            int i = 0;
            while (i < n && x[i]) {
                ++i;
            }
            return i;
        } else if (k == 1) {
            int i = 0;
            while (i < n && !x[n - 1 - i]) {
                ++i;
            }
            return i;
        } else {
            throw std::invalid_argument("Invalid objective index for LOTZ");
        }
    }

    objective::val_t lotz(individual_t &x) {
        return objective::val_t{(float)lotzk(0, x), (float)lotzk(1, x)};
    }


    int mlotzk(int m, int k, individual::span x) {
        int n2 = 2 * x.size() / m;
        return lotzk(k % 2, x.subspan((k / 2) * n2, n2));
    }

    objective::val_t mlotz(int m, individual_t &x) {
        int n = x.size();
        assert(m % 2 == 0);
        assert(n % (m / 2) == 0);

        objective::val_t v(m);
        for (int k = 0; k < m; ++k) {
            v[k] = mlotzk(m, k, x);
        }
        return v;
    }
}