#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

struct solution {
    solution(int value, int number)
        : min(value)
        , sum(value)
        , left(number)
        , right(number)
    {}

    long long check(long long value) const {
        return std::min(value, min) * (sum + value) - min * sum;
    }

    bool is_trivial() const {
        return left == right;
    }

    void recalc(long long value, int ix) {
        if (ix != left - 1 && ix != right + 1)
            return;
        min = std::min(value, min);
        sum += value;
        if (left - 1 == ix) {
            --left;
        }
        else {
            ++right;
        }
    }

    long long res() const {
        return min * sum;
    }

    long long min;
    long long sum;
    int left;
    int right;
};

int main() {
    int n;
    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);
    std::cin >> n;
    std::vector<std::pair<int, int>> values(n);
    std::vector<int> vals(n);
    std::vector<std::shared_ptr<solution>> solutions(n);
    std::shared_ptr<solution> max_sol(new solution(0, 0));
    for (auto i = 0; i < n; ++i) {
        std::cin >> values[i].first;
        vals[i] = values[i].first;
        values[i].second = i;
        solutions[i] = std::make_shared<solution>(values[i].first, i);
        if (solutions[i]->res() > max_sol->res()) {
            max_sol = solutions[i];
        }
    }

    std::sort(values.begin(), values.end(), [](std::pair<int, int> const& left, std::pair<int, int> const& right) { return left.first > right.first; });

    for (auto i = 0; i < n; ++i) {
        auto ix = values[i].second;
        auto value = values[i].first;


    }

    std::cout << max_sol->res() << std::endl << max_sol->left + 1 << " " << max_sol->right + 1 << std::endl;
    return 0;
}
