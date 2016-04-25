#include <iostream>

long long egcd(long long a, long long b, long long& x, long long& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    long long x1, y1;
    auto d = egcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

int main() {
    using namespace std;
    long long a, m;
    cin >> a >> m;
    long long x, y;
    auto d = egcd(a, m, x, y);
    if(d == 1) {
        if (x < 0) {
            x = x % m + m;
        }

        cout << x;
    }
    else {
        cout << -1;
    }
    cout << endl;

    return 0;
}