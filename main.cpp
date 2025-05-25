#include <bits/stdc++.h>

typedef long long long_long;

struct ComparisonCounter {
    static long_long comparisons_count;

    bool operator()(const std::string &str1, const std::string &str2) const {
        int len1 = str1.size(), len2 = str2.size();
        int length = std::min(len1, len2);
        for (int i = 0; i < length; ++i) {
            ++comparisons_count;
            if (str1[i] < str2[i]) return true;
            if (str1[i] > str2[i]) return false;
        }
        return len1 < len2;
    }
};

long_long ComparisonCounter::comparisons_count = 0;

void standardSort(std::vector<std::string> &data) {
    ComparisonCounter::comparisons_count = 0;
    std::sort(data.begin(), data.end(), ComparisonCounter());
}

int prefixCompare(const std::string &a, const std::string &b) {
    int min_len = std::min(a.size(), b.size()), i = 0;
    while (i < min_len) {
        ++ComparisonCounter::comparisons_count;
        if (a[i] != b[i]) return (a[i] < b[i]) ? -1 : 1;
        ++i;
    }
    if (a.size() == b.size()) return 0;
    return (a.size() < b.size()) ? -1 : 1;
}

bool comparePrefix(const std::string &a, const std::string &b) {
    return prefixCompare(a, b) < 0;
}

void mergeSortRecursive(int start, int end, std::vector<std::string> &data, std::vector<std::string> &temp) {
    if (end - start <= 1) return;
    int mid = (start + end) / 2;
    mergeSortRecursive(start, mid, data, temp);
    mergeSortRecursive(mid, end, data, temp);

    int i = start, j = mid, k = start;
    while (i < mid && j < end) {
        if (comparePrefix(data[i], data[j])) temp[k++] = std::move(data[i++]);
        else temp[k++] = std::move(data[j++]);
    }
    while (i < mid) temp[k++] = std::move(data[i++]);
    while (j < end) temp[k++] = std::move(data[j++]);

    for (int i = start; i < end; ++i) data[i] = std::move(temp[i]);
}

void mergeSort(std::vector<std::string> &data) {
    ComparisonCounter::comparisons_count = 0;
    int n = data.size();
    std::vector<std::string> temp(n);
    if (n > 1) mergeSortRecursive(0, n, data, temp);
}

// Реализация быстрой сортировки с разбиением по признаку символа
void quickSortRecursive(int low, int high, int depth, std::vector<std::string> &data) {
    if (low >= high) return;

    int pivotIndex = low + (high - low) / 2;
    std::string pivot = data[pivotIndex];
    int i = low, j = high;

    while (i <= j) {
        while (data[i][depth] < pivot[depth]) i++;
        while (data[j][depth] > pivot[depth]) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }

    if (low < j) quickSortRecursive(low, j, depth, data);
    if (i < high) quickSortRecursive(i, high, depth, data);
}

void quickSort(std::vector<std::string> &data) {
    ComparisonCounter::comparisons_count = 0;
    if (!data.empty()) quickSortRecursive(0, data.size() - 1, 0, data);
}

const int ALPHABET_SIZE = 256;

void msdSortRecursive(int start, int end, int depth, std::vector<std::string> &data, std::vector<std::string> &temp) {
    if (start >= end) return;

    std::vector<int> count(ALPHABET_SIZE + 2, 0);
    for (int i = start; i <= end; ++i) {
        int char_code = (depth < data[i].size()) ? (unsigned char)data[i][depth] + 1 : 0;
        ++count[char_code + 1];
    }

    for (int i = 0; i <= ALPHABET_SIZE; ++i) {
        count[i + 1] += count[i];
    }

    for (int i = start; i <= end; ++i) {
        int char_code = (depth < data[i].size()) ? (unsigned char)data[i][depth] + 1 : 0;
        temp[start + count[char_code]++] = std::move(data[i]);
        ++ComparisonCounter::comparisons_count;
    }

    for (int i = start; i <= end; ++i) {
        data[i] = std::move(temp[i]);
    }

    for (int i = 0; i <= ALPHABET_SIZE; ++i) {
        int subStart = start + count[i];
        int subEnd = start + count[i + 1] - 1;
        if (subStart <= subEnd)
            msdSortRecursive(subStart, subEnd, depth + 1, data, temp);
    }
}

void msdSort(std::vector<std::string> &data) {
    ComparisonCounter::comparisons_count = 0;
    int n = data.size();
    std::vector<std::string> temp(n);
    if (n > 1) msdSortRecursive(0, n - 1, 0, data, temp);
}

// Гибридный MSD сорт с порогом для переключения на быструю сортировку
const int SWITCH_THRESHOLD = 74;

void hybridMSDSortRecursive(int start, int end, int depth, std::vector<std::string> &data, std::vector<std::string> &temp) {
    if (start >= end) return;

    if (end - start + 1 < SWITCH_THRESHOLD) {
        quickSortRecursive(start, end, depth, data);
        return;
    }

    std::vector<int> count(ALPHABET_SIZE + 2, 0);
    for (int i = start; i <= end; ++i) {
        int char_code = (depth < data[i].size()) ? (unsigned char)data[i][depth] + 1 : 0;
        ++count[char_code + 1];
    }

    for (int i = 0; i <= ALPHABET_SIZE; ++i) {
        count[i + 1] += count[i];
    }

    for (int i = start; i <= end; ++i) {
        int char_code = (depth < data[i].size()) ? (unsigned char)data[i][depth] + 1 : 0;
        temp[start + count[char_code]++] = std::move(data[i]);
        ++ComparisonCounter::comparisons_count;
    }

    for (int i = start; i <= end; ++i) {
        data[i] = std::move(temp[i]);
    }

    for (int i = 0; i <= ALPHABET_SIZE; ++i) {
        int subStart = start + count[i];
        int subEnd = start + count[i + 1] - 1;
        if (subStart <= subEnd)
            hybridMSDSortRecursive(subStart, subEnd, depth + 1, data, temp);
    }
}

void hybridMSDSort(std::vector<std::string> &data) {
    ComparisonCounter::comparisons_count = 0;
    int n = data.size();
    std::vector<std::string> temp(n);
    if (n > 1) hybridMSDSortRecursive(0, n - 1, 0, data, temp);
}

class StringGenerator {
public:
    static const std::string CHARSET;

    StringGenerator(unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count())
            : rng(seed), lenDist(10, 200), charDist(0, (int)CHARSET.size() - 1) {}

    std::vector<std::string> generateRandom(int count) {
        std::vector<std::string> result(count);
        for (int i = 0; i < count; ++i) result[i] = generateString();
        return result;
    }

    std::vector<std::string> generateReversed(int count) {
        auto result = generateRandom(count);
        std::sort(result.begin(), result.end(), std::greater<std::string>());
        return result;
    }

    std::vector<std::string> generateNearlySorted(int count, int swaps = 10) {
        auto result = generateRandom(count);
        std::sort(result.begin(), result.end());
        std::uniform_int_distribution<int> posDist(0, count - 1);
        for (int i = 0; i < swaps; ++i)
            std::swap(result[posDist(rng)], result[posDist(rng)]);
        return result;
    }

private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> lenDist, charDist;

    std::string generateString() {
        int length = lenDist(rng);
        std::string str(length, ' ');
        for (int i = 0; i < length; ++i) str[i] = CHARSET[charDist(rng)];
        return str;
    }
};

const std::string StringGenerator::CHARSET =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#*:,^&*()-";

class SortingSortTester {
public:
    struct TestResult {
        int size;
        std::string setType;
        std::string algorithm;
        double duration_ms;
        long_long comparisons;
    };

    static TestResult runTest(std::vector<std::string> data, std::string setType, std::string algoName,
                              std::function<void(std::vector<std::string>&)> sortingFunction)
    {
        ComparisonCounter::comparisons_count = 0;
        auto start = std::chrono::high_resolution_clock::now();
        sortingFunction(data);
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end - start).count();
        return { (int)data.size(), setType, algoName, duration, ComparisonCounter::comparisons_count };
    }

    std::vector<TestResult> runAllTests() {
        std::vector<TestResult> results;
        StringGenerator generator;
        std::vector<std::pair<std::string, std::vector<std::string>>> datasets;

        for (const auto& type : {"random", "reversed", "almost"}) {
            for (int size = 100; size <= 3000; size += 100) {
                std::vector<std::string> data;
                if (type == "random") data = generator.generateRandom(size);
                if (type == "reversed") data = generator.generateReversed(size);
                if (type == "almost") data = generator.generateNearlySorted(size, 10);
                datasets.emplace_back(type, std::move(data));
            }
        }

        for (auto& dataset : datasets) {
            auto& type = dataset.first;
            auto arr = dataset.second;

            results.push_back(runTest(arr, type, "std::sort", standardSort));
            results.push_back(runTest(arr, type, "mergesort_LCP", mergeSort));
            results.push_back(runTest(arr, type, "quick3way", quickSort));
            results.push_back(runTest(arr, type, "msd_radix", msdSort));
            results.push_back(runTest(arr, type, "msd_hybrid", hybridMSDSort));
        }

        return results;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto results = SortingSortTester{}.runAllTests();

    std::ofstream outputFile("results.csv");

    outputFile << "size,type,algo,duration_ms,comparisons\n";

    std::cout << "size,type,algo,duration_ms,comparisons\n";
    for (auto& result : results) {
        outputFile << result.size << ',' << result.setType << ',' << result.algorithm << ','
                   << std::fixed << std::setprecision(3) << result.duration_ms << ',' << result.comparisons << "\n";

        std::cout << result.size << ',' << result.setType << ',' << result.algorithm << ','
                  << std::fixed << std::setprecision(3) << result.duration_ms << ',' << result.comparisons << "\n";
    }

    outputFile.close();

    std::cout << "Results have been written to results.csv" << std::endl;
    return 0;
}
