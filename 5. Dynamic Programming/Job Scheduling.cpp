// Evgenii Khovalyg

#include <iostream>
#include <vector>

using namespace std;

struct Job {
    int s, f, p;
};

using JobIterator = vector<Job>::iterator;

void sort(JobIterator l, JobIterator r) {
    const int len = static_cast<int>(distance(l, r));
    if (len == 1) {
        return;
    }
    const auto mid = next(l, len / 2);
    sort(l, mid);
    sort(mid, r);

    auto i = l;
    auto j = mid;
    vector<Job> v;
    v.reserve(len);
    while (i != mid && j != r) {
        if (i->f < j->f) {
            v.push_back(*i);
            ++i;
        } else {
            v.push_back(*j);
            ++j;
        }
    }
    while (i != mid) {
        v.push_back(*i);
        ++i;
    }
    while (j != r) {
        v.push_back(*j);
        ++j;
    }
    move(v.begin(), v.end(), l);
}

void initialize(vector<Job> &jobs, const int n) {
    for (int i = 0; i < n; i++) {
        cin >> jobs[i].s;
    }
    for (int i = 0; i < n; i++) {
        cin >> jobs[i].f;
    }
    for (int i = 0; i < n; i++) {
        cin >> jobs[i].p;
    }
}

long long findMaxProfit(const vector<Job> &jobs, const int n) {
    if (n == 0) {
        return 0;
    }

    long long maxProfit[n];
    long long result = 0;
    for (int i = 0; i < n; i++) {
        maxProfit[i] = 0;
        for (int j = 0; j < i; j++) {
            if (jobs[j].f <= jobs[i].s && maxProfit[i] < maxProfit[j]) {
                maxProfit[i] = maxProfit[j];
            }
        }
        maxProfit[i] += jobs[i].p;
        result = max(maxProfit[i], result);
    }
    return result;
}

int main() {
    int n;
    cin >> n;

    vector<Job> jobs(n);
    initialize(jobs, n);
    sort(jobs.begin(), jobs.end());
    cout << findMaxProfit(jobs, n) << endl;
}
