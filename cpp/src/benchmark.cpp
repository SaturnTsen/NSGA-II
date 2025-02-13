#include "benchmark.h"
#include "individual.h"
#include <cassert>
#include <cstddef>
#include <stdexcept>

namespace benchmark {
    using individual::individual_t;

    int lotzk(const int k, const individual::span &x) {
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

    objective::val_t lotz(const individual_t &x) {
        return objective::val_t{(double)lotzk(0, x), (double)lotzk(1, x)};
    }

    int mlotzk(const int m, const int k, const individual::span &x) {
        assert(k >= 0 && k < m);
        int n2 = 2 * x.size() / m;
        return lotzk(k % 2, x.subspan((k / 2) * n2, n2));
    }

    objective::val_t mlotz(const int m, const individual_t &x) {
        int n = x.size();
        assert(m > 1 && m % 2 == 0);
        assert(n % (m / 2) == 0);

        objective::val_t v(m);
        for (int k = 0; k < m; ++k) {
            v[k] = mlotzk(m, k, x);
        }
        return v;
    }

    // mLOTZ functor
    mlotz_functor::mlotz_functor(const size_t m) : m(m) { assert(m > 1 && m % 2 == 0); }

    objective::val_t mlotz_functor::operator()(const individual_t &x) {
        return mlotz(m, x);
    }

    bool is_lotz_pareto_front(const individual_t &x) {
        return lotzk(0, x) + lotzk(1, x) == x.size();
    }

    bool is_mlotz_pareto_front(const int m, const individual_t &x) {
        // TODO prove this in the report
        const int n = x.size();
        assert(m > 1 && n % (m / 2) == 0);
        const int len_span = n / (m / 2);
        for (int k = 0; k < m; k += 2) {
            if (mlotzk(m, k, x) + mlotzk(m, k + 1, x) != len_span) {
                return false;
            }
        }
        return true;
    }
} // namespace benchmark