#include <iostream>
#include <string>
#include <unordered_map>

struct automata_state {
    int length;
    int link;
    std::unordered_map<char, int> next;
};

struct automata_state st[200000];
int sz, last;

void init() {
    sz = last = 0;
    st[0].length = 0;
    st[0].link = -1;
    ++sz;
}

void extend(char ch) {
    auto cur = sz++;
    st[cur].length = st[last].length + 1;
    int p;
    for (p = last; p != -1 && !st[p].next.count(ch); p = st[p].link) {
        st[p].next[ch] = cur;
    }
    if (p == -1) {
        st[cur].link = 0;
    }
    else {
        auto q = st[p].next[ch];
        if (st[p].length + 1 == st[q].length) {
            st[cur].link = q;
        }
        else {
            auto clone = sz++;
            st[clone].length = st[p].length + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            for (; p != -1 && st[p].next[ch] == q; p = st[p].link) {
                st[p].next[ch] = clone;
            }
            st[q].link = st[cur].link = clone;
        }
    }

    last = cur;
}

std::string longnest_substr(std::string s, std::string t) {
    init();
    for (size_t i = 0; i < s.length(); ++i) {
        extend(s[i]);
    }

    size_t v = 0;
    size_t l = 0;
    size_t best = 0;
    size_t bestpos = 0;
    for (size_t i = 0; i < t.length(); ++i) {
        while (v && !st[v].next.count(t[i])) {
            v = st[v].link;
            l = st[v].length;
        }
        if (st[v].next.count(t[i])) {
            v = st[v].next[t[i]];
            ++l;
        }
        if (l > best) {
            best = l;
            bestpos = i;
        }
    }
    return t.substr(bestpos - best + 1, best);
}

int main() {
    int n;
    std::cin >> n;
    std::string str1, str2;

    std::cin >> str1 >> str2;

    std::cout << longnest_substr(str1, str2) << std::endl;
}