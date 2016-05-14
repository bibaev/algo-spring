#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> z_function(std::string s) {
    auto n = s.length();
    std::vector<int> z(n);
    for (auto i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }

    return z;
}

int main() {
    std::string str1;
    std::string str2;
    std::cin >> str1 >> str2;
    auto str2_len = str2.length();

    auto z = z_function(str2 + "#" + str1);

    std::vector<size_t> res;
    for (int i = str2_len + 1; i < z.size(); ++i) {
        if(z[i] == str2_len) {
            res.push_back(i - str2_len);
        }
    }

    std::cout << res.size() << std::endl;

    std::for_each(res.begin(), res.end(), [](int val) { std::cout << val << ' '; });
    std::cout << std::endl; 
}