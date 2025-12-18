// Evgenii Khovalyg DSAI-02

#include <iostream>
#include <vector>

using namespace std;

struct Number {
    int value;
    int index;
};

void initialize(vector<Number> &v, const int n) {
    for (int i = 0; i < n; i++) {
        cin >> v[i].value;
        v[i].index = i;
    }
}

void findFreq(vector<int> &numberFreq, const vector<Number> &numbers) {
    for (const auto [value, index]: numbers) {
        ++numberFreq[value];
    }
}

template<typename T, typename IndexFunc>
void EvgeniiKhovalygCountingSort(typename vector<T>::iterator left,
                                 typename vector<T>::iterator right,
                                 const int n, IndexFunc getIndex) {
    if (left == right) {
        return;
    }
    const int range = n + 1;
    vector<int> count(range, 0);


    for (auto it = left; it != right; ++it) {
        const int index = getIndex(*it);
        ++count[index];
    }

    for (int i = 1; i < range; ++i) {
        count[i] += count[i - 1];
    }

    vector<T> result(right - left);
    for (auto it = right; it != left;) {
        --it;
        const int index = getIndex(*it);
        const int pos = --count[index];
        result[pos] = *it;
    }

    copy(result.begin(), result.end(), left);
}

template<typename T>
void EvgeniiKhovalygRadixSort(typename vector<T>::iterator left,
                              typename vector<T>::iterator right,
                              const int maxValue) {
    if (left == right) {
        return;
    }

    int maxDigits = 0;
    int temp = maxValue;
    while (temp > 0) {
        maxDigits++;
        temp /= 10;
    }
    if (maxValue == 0)
        maxDigits = 1;

    for (int digit = 0; digit < maxDigits; ++digit) {
        auto getDigitFunc = [digit](const T &item) {
            const int powersOfTen[] = {1, 10, 100, 1000};
            return (item.value / powersOfTen[digit]) % 10;
        };

        EvgeniiKhovalygCountingSort<T>(left, right, 10, getDigitFunc);
    }
}

template<typename T>
void EvgeniiKhovalygDoubleSort(vector<T> &bucket, const vector<int> &numberFreq,
                               const int minFreq) {
    if (bucket.size() == 0) {
        return;
    }
    // Sort by freq
    auto getIndexByFreq = [&numberFreq, minFreq](const T &number) {
        return numberFreq[number.value] - minFreq;
    };
    EvgeniiKhovalygCountingSort<T>(bucket.begin(), bucket.end(), bucket.size(),
                                   getIndexByFreq);
    // Sort by value in the subsequence with the same freq
    auto l = bucket.begin();
    const int maxValue = 1000;
    for (auto r = next(bucket.begin()); r != bucket.end(); ++r) {
        if (getIndexByFreq(*r) != getIndexByFreq(*prev(r))) {
            EvgeniiKhovalygRadixSort<T>(l, r, maxValue);
            l = r;
        }
    }
    EvgeniiKhovalygRadixSort<T>(l, bucket.end(), maxValue);
}

int findMaxFreq(const vector<int> &numberFreq) {
    int maxFreq = 0;
    for (const int freq: numberFreq) {
        if (freq > maxFreq) {
            maxFreq = freq;
        }
    }
    return maxFreq;
}

template<typename T>
void EvgeniiKhovalygBucketSort(vector<T> &numbers) {
    const int valueRange = 1000;
    vector<int> numberFreq(valueRange + 1);
    findFreq(numberFreq, numbers);
    const int maxFreq = findMaxFreq(numberFreq);


    const int bucketStep = 1;
    const int bucketSize = maxFreq / bucketStep + 1;
    vector<vector<T>> buckets(bucketSize, vector<T>());
    for (const T &number: numbers) {
        const int bucketNumber = numberFreq[number.value] / bucketStep;
        buckets[bucketNumber].push_back(number);
    }


    for (int i = 0; i < bucketSize; i++) {
        EvgeniiKhovalygDoubleSort<T>(buckets[i], numberFreq, bucketStep * i);
    }


    int pos = 0;
    for (const auto &bucket: buckets) {
        for (const auto &num: bucket) {
            numbers[pos++] = num;
        }
    }
}

int main() {
    int n;
    cin >> n;
    vector<Number> numbers(n);
    initialize(numbers, n);
    EvgeniiKhovalygBucketSort<Number>(numbers);
    for (const auto &[value, index]: numbers) {
        cout << value << ' ' << index << '\n';
    }
}
