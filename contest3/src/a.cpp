#include <iostream>
#include <list>
#include <string>

int main() {
    std::list<char> l;
    std::string s;
    std::cin >> s;
    for(auto c : s) {
        l.push_back(c);
    }

    auto it = l.begin();
    auto next = it;
    ++next;
    while(next != l.end()) {
        while(next != l.end() && *it != *next) {
            ++it; ++next;
        }

        if(next != l.end()) {
            next = l.erase(next);
            it = l.erase(it);
            if(it != l.begin()) {
                --it;
            }
            else {
                ++next;
            }
        }
    }

    for(auto ch : l) {
        std::cout << ch;
    }

    std::cout << std::endl;
}
