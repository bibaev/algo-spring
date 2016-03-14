#include <iostream>
#include <vector>
#include <algorithm>

enum color {
    WHITE,
    BLACK
};

std::pair<int, int>&& update(color color, int from, int to) {
    return std::make_pair(0, 0);
}

int main() {
    int n;
    std::cin >> n;

    char line_type;
    int from, to;
    std::vector<std::pair<int, int>> result(n);
    for (auto i = 0; i < n; ++i) {
        std::cin >> line_type;
        std::cin >> from >> to;
        result.emplace_back(update(/* rmq, */line_type == 'W' ? WHITE : BLACK, from, to));
    }

    std::for_each(result.begin(), result.end(),
            [](std::pair<int, int> const& value) {std::cout << value.first << " " << value.second << std::endl; });

    return 0;
}
