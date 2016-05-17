#include <iostream>
#include <vector>
#include <string>
std::vector<int> prefix_function(std::string s) {
    auto n = s.length();
    std::vector<int> pi(n);
    for (size_t i = 1; i < n; ++i) {
        size_t j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])  ++j;
        pi[i] = j;
    }
    return pi;
}

int main() {
    std::string str;
    std::cin >> str;
    auto p = prefix_function(str);

    std::vector<size_t> d(str.length(), 0);
    size_t arg_max = 0;
    for (size_t j = 1; j < str.length(); ++j) {
        if(p[j] != 0) {
            d[j] = d[p[j] - 1] + 1;
        }
        if(d[j] > d[arg_max]) {
            arg_max = j;
        }
    }

    std::cout << str.substr(0, arg_max + 1) << std::endl;
}
