// Evgenii Khovalyg DSAI-02

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    double x;
    double y;
    double d;
} Point;

bool compare(const Point& a, const Point& b) {
    return a.d < b.d;
}

template<typename T, typename CompareFunc>
void EvgeniiKhovalyg_insertion_srt(vector<T>& arr, CompareFunc compare) {
    const int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template<typename T>
void EvgeniiKhovalyg_bucket_srt(vector<T>& points) {
    if (points.empty())
        return;

    const int n = points.size();
    int m = max(1, static_cast<int>(sqrt(n)));

    vector<vector<Point>> buckets(m);

    for (const auto& p : points) {
        const int bucket = min(m - 1, static_cast<int>(p.d * m));
        buckets[bucket].push_back(p);
    }

    for (auto& bucket : buckets) {
        if (!bucket.empty()) {
            EvgeniiKhovalyg_insertion_srt(bucket, compare);
        }
    }

    points.clear();
    for (const auto& bucket : buckets) {
        points.insert(points.end(), bucket.begin(), bucket.end());
    }
}

int main() {
    int n;
    cin >> n;
    vector<Point> points;
    points.reserve(n);

    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y;
        const double dist = sqrt(x * x + y * y);
        points.emplace_back(Point{x, y, dist});
    }

    EvgeniiKhovalyg_bucket_srt(points);

    for (const auto & point : points) {
        printf("%.4f %.4f\n", point.x, point.y);
    }

    return 0;
}