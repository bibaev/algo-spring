#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);
    std::cin >> n;
    std::vector<int> values(n);
    for (auto i = 0; i < n; ++i) {
        std::cin >> values[i];
    }
    
    auto max = 0;
    int value, sum;
    int l, r;
    int maxL, maxR;
    maxL = maxR = 0;
    for (auto i = 0; i < n; ++i) {
        value = values[i];
        sum = value;
        for (r = i + 1; r < n && values[r] >= value; ++r) {
            sum += values[r];
        }

        for (l = i - 1; l >= 0 && values[l] >= value; --l) {
            sum += values[l];
        }

        if (max < value * sum) {
            maxL = l + 1;
            maxR = r - 1;
            max = value * sum;
        }
    }

    std::cout << max << std::endl << maxL + 1 << " " << maxR + 1 << std::endl;

    return 0;
}
