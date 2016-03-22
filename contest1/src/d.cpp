#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const int MAX_COORD_MOD = 500000;

enum color_t {
    WHITE,
    BLACK,
    UNKNOWN
};

struct node {
public:
    node()
        : color(WHITE)
        , black_count(0)
        , black_len(0)
        , l_color(WHITE)
        , r_color(WHITE)
    {}

    void draw_all(int length, color_t new_color) {
        if (new_color == BLACK) {
            black_len = length;
        }
        else {
            black_len = 0;
        }

        black_count = 0;
        color = l_color = r_color = new_color;
    }

    color_t color;
    int black_count;
    int black_len;

    color_t l_color;
    color_t r_color;
};

void push_draw(int, int, int, vector<node>&);

void draw(int v, int tl, int tr, int l, int r, color_t color, vector<node>& rmq) {
    if (tr <= l || r <= tl) return;
    if (l <= tl && tr <= r) {
        rmq[v].draw_all(tr - tl, color);
        return;
    }

    int tm = (tl + tr) / 2;
    if (rmq[v].color != UNKNOWN) {
        push_draw(v, tm - tl, tr - tm, rmq);
    }

    auto left = 2 * v;
    auto right = 2 * v + 1;

    draw(2 * v, tl, tm, l, r, color, rmq);
    draw(2 * v + 1, tm, tr, l, r, color, rmq);

    rmq[v].black_count = rmq[left].black_count + rmq[right].black_count;
    rmq[v].black_len = rmq[left].black_len + rmq[right].black_len;
    rmq[v].l_color = rmq[left].l_color;
    rmq[v].r_color = rmq[right].r_color;

    if (rmq[left].r_color == WHITE && rmq[right].l_color == BLACK) {
        ++rmq[v].black_count;
    }
}

void push_draw(int v, int left_length, int right_length, vector<node>& rmq) {
    rmq[2 * v].draw_all(left_length, rmq[v].color);
    rmq[2 * v + 1].draw_all(right_length, rmq[v].color);
    rmq[v].color = UNKNOWN;
}

std::pair<int, int>&& update(vector<node>& rmq, color_t color, int from, int length) {
    from += 1;
    draw(1, 0, 2000000, from, from + length, color, rmq);
    return std::make_pair(rmq[1].black_count, rmq[1].black_len);
}

int main() {
    int n;
    cin >> n;

    char line_type;
    int from, length;
    vector<pair<int, int>> result;
    vector<node> rmq(8 * MAX_COORD_MOD);
    for (auto i = 0; i < n; ++i) {
        cin >> line_type;
        cin >> from >> length;
        from += MAX_COORD_MOD;
        result.emplace_back(update(rmq, line_type == 'W' ? WHITE : BLACK, from, length));
    }

    std::for_each(result.begin(), result.end(),
            [](pair<int, int> const& value) {cout << value.first << " " << value.second << endl; });

    return 0;
}
