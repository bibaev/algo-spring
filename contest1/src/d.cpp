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
	if (rmq[v].color == UNKNOWN) {
		push_draw(v, tm - tl, tr - tm, rmq);
	}

	draw(2 * v, tl, tm, l, r, color, rmq);
	draw(2 * v + 1, tm, tr, l, r, color, rmq);

	rmq[v].black_count = rmq[2 * v].black_count + rmq[2 * v + 1].black_count;
	rmq[v].black_len = rmq[2 * v].black_len + rmq[2 * v + 1].black_len;
	rmq[v].l_color = rmq[2 * v].l_color;
	rmq[v].r_color = rmq[2 * v + 1].r_color;

	if (rmq[2 * v].r_color == WHITE && rmq[2 * v + 1].l_color == BLACK) {
		++rmq[v].black_count;
	}
}

void push_draw(int v, int left_length, int right_length, vector<node>& rmq) {
	rmq[2 * v].draw_all(left_length, rmq[v].color);
	rmq[2 * v + 1].draw_all(right_length, rmq[v].color);
	rmq[v].color = UNKNOWN;
}

std::pair<int, int> update(vector<node>& rmq, color_t color, int from, int length) {
	draw(1, 0, 1000001, from, from + length, color, rmq);
	return std::make_pair(rmq[1].black_count, rmq[1].black_len);
}

int main() {
    int n;
    std::cin >> n;

    char line_type;
    int from, length;
    std::vector<std::pair<int, int>> result(n);
	std::vector<node> rmq(1000000 * 4);
    for (auto i = 0; i < n; ++i) {
        std::cin >> line_type;
        std::cin >> from >> length;
		from += MAX_COORD_MOD + 1;
        result.emplace_back(update(rmq, line_type == 'W' ? WHITE : BLACK, from, from + length));
    }

    std::for_each(result.begin(), result.end(),
            [](std::pair<int, int> const& value) {std::cout << value.first << " " << value.second << std::endl; });

    return 0;
}
