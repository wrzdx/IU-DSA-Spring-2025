// Evgenii Khovalyg

#include <algorithm>
#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;


int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> v(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> v[i][j];
        }
    }
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, LONG_LONG_MAX));
    dp[0][1] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + v[i][j];
        }
    }
    vector<int> path;
    for (int i = n, j = m; i != 0 || j != 1;) {
        path.push_back(v[i][j]);
        if (dp[i][j] - v[i][j] == dp[i - 1][j]) {
            i--;
        } else {
            j--;
        }
    }
    cout << dp[n][m] << endl;
    for (size_t i = path.size() - 1; i != -1; --i) {
        cout << path[i] << " ";
    }
}
