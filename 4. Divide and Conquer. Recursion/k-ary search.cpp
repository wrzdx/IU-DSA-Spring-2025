// Evgenii Khovalyg DSAI-02

#include <iostream>
#include <vector>

using namespace std;

using It = std::vector<int>::const_iterator; // use template for conciseness

/**
 * @brief Execute k-ary search of number in the sorted vector.
 *
 * @param start Iterator to the vector beginning (for index calculating).
 * @param l Iterator to the left boundary of the vector subsequence.
 * @param r Iterator to the right boundary of the vector subsequence (not inclusive).
 * @param x The number to find.
 * @param k The number of parts into which the vector should be divided on every recursion stage.
 * @return Index of number `x` in the vector, if it is found, otherwise -1.
 */
int k_search(const It start, const It l, const It r, const int x, const int k) {
    // If the vector is empty, the number is not found
    if (l == r) {
        return -1;
    }

    const int len = r - l; // length of th current subsequence

    // If the subsequence length is 1, check the left element
    if (len == 1) {
        return (*l == x) ? (l - start) : -1;
    }

    // Divide vector subsequence into k parts, if it is possible
    if (const int part_size = len / k; part_size > 0) {
        for (It first_mid = l; first_mid + part_size <= r; first_mid += part_size) {
            // If `x` is between `first_mid` and `first_mid + part_size`,
            // search recursively in this subsequence
            if (x > *first_mid && x < *(first_mid + part_size)) {
                return k_search(start, first_mid, first_mid + part_size, x, k);
            }
        }
    }

    // If the length is less than `k`, try to find the element in the current range
    for (It it = l; it != r; ++it) {
        if (*it == x) {
            return it - start;
        }
    }

    // If `x` is not found return `-1`
    return -1;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int x;
        cin >> x;
        cout << k_search(v.begin(), v.begin(), v.end(), x, k) << "\n";
    }
    return 0;
}

