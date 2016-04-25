#include <iostream>
#include <vector>

int gauss(std::vector<std::vector<int>>& matrix, std::vector<int>& right) {
    using namespace std;
    auto n = matrix.size();
    vector<int> swaps(n);
    for (size_t i = 0; i < n; ++i) {
        swaps[i] = i;
    }

    for (size_t col = 0, row = 0; col < n && row < n; ++col, ++row) {
        for (auto i = row; i < n; ++i) {
            if(matrix[i][col] != 0) {
                swap(matrix[i], matrix[row]);
                swap(right[i], right[row]);
                break;
            }
        }

        if(!matrix[row][col]) {
            return -1;
        }

        for (auto i = 0; i < n; ++i) {
            if(i != row && matrix[i][col] != 0) {
                for (auto j = col; j < n; ++j) {
                    matrix[i][j] ^= matrix[row][j];
                }
                right[i] ^= right[row];
            }
        }
    }

    int answer = 0;
    for (size_t i = 0; i < n; ++i) {
        if(right[i]) {
            ++answer;
        }
    }

    return answer;
}

int main() {
    using namespace std;
    size_t n;
    cin >> n;
    vector<vector<int>> matrix(n);
    vector<int> right(n);
    for (size_t i = 0; i < n; ++i) {
        matrix[i].resize(n);
        size_t k;
        cin >> k;
        for (size_t j = 0; j < k; ++j) {
            size_t col;
            cin >> col; --col;
            matrix[i][col] = 1;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        int val;
        cin >> val;
        right[i] = val;
    }

    auto res = gauss(matrix, right);
    if(res != -1) {
        cout << res << endl;
        for (size_t i = 0; i < n; ++i) {
            if(right[i] != 0) {
                cout << i + 1 << " ";
            }
        }
        cout << endl;
    }
    else {
        cout << -1 << endl;;
    }
}
