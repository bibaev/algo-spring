#include <iostream>
#include <vector>
#include <algorithm>

const int k = 32768;
std::vector<int> rmq(4 * k);

void build_rmq(std::vector<int> const& a, int v, int tl, int tr) {
    if(tl == tr) {
        rmq[v] = a[tl];
    }
    else {
        auto tm = (tl + tr) / 2;
        build_rmq(a, v * 2, tl, tm);
        build_rmq(a, v * 2 + 1, tm + 1, tr);
        rmq[v] = rmq[v * 2] + rmq[v * 2 + 1];
    }
}

int sum(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 0;
    }

    if(l == tl && r == tr) {
        return rmq[v];
    }

    auto tm = (tl + tr) / 2;
    return sum(v * 2, tl, tm, l, std::min(r, tm)) + sum(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r);
}

void update(int v, int tl, int tr, int pos, int value) {
    if (tl == tr) {
        rmq[v] = value;
    }
    else {
        auto tm = (tl + tr) / 2;
        if (pos <= tm) {
            update(v * 2, tl, tm, pos, value);
        }
        else {
            update(v * 2 + 1, tm + 1, tr, pos, value);
        }
        rmq[v] = rmq[v * 2] + rmq[v * 2 + 1];
    }
}

int main() {
    int n;

    std::vector<uint16_t> result;
    std::vector<int> count(k);
    while(scanf_s("%d", &n) == 1) {
        result.resize(n, 0);
        result.assign(n, 0);
        count.assign(k, 0);
        rmq.assign(rmq.size(), 0);
        build_rmq(count, 1, 0, k - 1);
        int x, y;
        for (auto i = 0; i < n; ++i) {
            std::cin >> x >> y;
            count[x] += 1;
            update(1, 0, k - 1, x, count[x]);
            ++result[sum(1, 0, k - 1, 0, x) - 1];
        }

        std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });
    }
}
