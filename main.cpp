#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <iomanip>
using namespace std;

const int N = 1000000;

// ================= QUICK SORT =================

void quickSort(vector<float>& a, int left, int right) {
    if (left >= right)
        return;

    float pivot = a[(left + right) / 2];

    int i = left;
    int j = right;

    while (i <= j) {
        while (a[i] < pivot)
            i++;

        while (a[j] > pivot)
            j--;

        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort(a, left, j);

    if (i < right)
        quickSort(a, i, right);
}

// ================= HEAP SORT =================

void heapify(vector<float>& a, int n, int i) {
    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest])
        largest = left;

    if (right < n && a[right] > a[largest])
        largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);

        heapify(a, n, largest);
    }
}

void heapSort(vector<float>& a) {
    int n = a.size();

    // Tạo Max Heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }

    // Đưa phần tử lớn nhất về cuối
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);

        heapify(a, i, 0);
    }
}

// ================= MERGE SORT =================

void merge(vector<float>& a, int left, int mid, int right) {
    vector<float> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) {
            temp.push_back(a[i]);
            i++;
        }
        else {
            temp.push_back(a[j]);
            j++;
        }
    }

    while (i <= mid) {
        temp.push_back(a[i]);
        i++;
    }

    while (j <= right) {
        temp.push_back(a[j]);
        j++;
    }

    for (int k = 0; k < temp.size(); k++) {
        a[left + k] = temp[k];
    }
}

void mergeSort(vector<float>& a, int left, int right) {
    if (left >= right)
        return;

    int mid = (left + right) / 2;

    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);

    merge(a, left, mid, right);
}

// ================= TẠO DỮ LIỆU =================

void generateData() {
    mt19937 gen(20261004);
    uniform_real_distribution<float> dist(-1000000, 1000000);

    for (int d = 1; d <= 10; d++) {

        vector<float> a(N);

        // Dãy 1: tăng dần
        if (d == 1) {
            for (int i = 0; i < N; i++) {
                a[i] = i;
            }
        }

        // Dãy 2: giảm dần
        else if (d == 2) {
            for (int i = 0; i < N; i++) {
                a[i] = N - i;
            }
        }

        // Dãy 3 -> 10: ngẫu nhiên
        else {
            for (int i = 0; i < N; i++) {
                a[i] = dist(gen);
            }
        }

        string filename = "data" + to_string(d) + ".bin";

        ofstream file(filename, ios::binary);

        file.write(
            (char*)a.data(),
            N * sizeof(float)
        );

        file.close();

        cout << "Da tao " << filename << endl;
    }
}

// ================= ĐỌC DỮ LIỆU =================

bool loadData(string filename, vector<float>& a) {
    ifstream file(filename, ios::binary);

    if (!file)
        return false;

    a.resize(N);

    file.read(
        (char*)a.data(),
        N * sizeof(float)
    );

    file.close();

    return true;
}

// ================= KIỂM TRA =================

bool checkSorted(vector<float>& a) {
    for (int i = 1; i < a.size(); i++) {
        if (a[i - 1] > a[i])
            return false;
    }

    return true;
}

// ================= MAIN =================

int main() {

    // Tạo 10 bộ dữ liệu
    generateData();

    ofstream result("result.csv");

    result << "Dataset,QuickSort,HeapSort,MergeSort,STL_sort\n";

    for (int d = 1; d <= 10; d++) {

        string filename = "data" + to_string(d) + ".bin";

        vector<float> original;

        if (!loadData(filename, original)) {
            cout << "Khong doc duoc " << filename << endl;
            return 1;
        }

        cout << "\n===== DATASET " << d << " =====\n";

        // ------------------------------------------------
        // QUICK SORT
        // ------------------------------------------------

        vector<float> a = original;

        auto start = chrono::steady_clock::now();

        quickSort(a, 0, N - 1);

        auto finish = chrono::steady_clock::now();

        double quickTime =
            chrono::duration<double, milli>(finish - start).count();

        cout << "QuickSort: "
            << fixed << setprecision(2)
            << quickTime << " ms";

        if (!checkSorted(a))
            cout << " Loi!";

        cout << endl;


        // ------------------------------------------------
        // HEAP SORT
        // ------------------------------------------------

        a = original;

        start = chrono::steady_clock::now();

        heapSort(a);

        finish = chrono::steady_clock::now();

        double heapTime =
            chrono::duration<double, milli>(finish - start).count();

        cout << "HeapSort: "
            << heapTime << " ms";

        if (!checkSorted(a))
            cout << " Loi!";

        cout << endl;


        // ------------------------------------------------
        // MERGE SORT
        // ------------------------------------------------

        a = original;

        start = chrono::steady_clock::now();

        mergeSort(a, 0, N - 1);

        finish = chrono::steady_clock::now();

        double mergeTime =
            chrono::duration<double, milli>(finish - start).count();

        cout << "MergeSort: "
            << mergeTime << " ms";

        if (!checkSorted(a))
            cout << " Loi!";

        cout << endl;


        // ------------------------------------------------
        // STL SORT
        // ------------------------------------------------

        a = original;

        start = chrono::steady_clock::now();

        sort(a.begin(), a.end());

        finish = chrono::steady_clock::now();

        double stlTime =
            chrono::duration<double, milli>(finish - start).count();

        cout << "STL sort: "
            << stlTime << " ms";

        if (!checkSorted(a))
            cout << " Loi!";

        cout << endl;


        // Ghi kết quả vào file CSV
        result << d << ","
            << quickTime << ","
            << heapTime << ","
            << mergeTime << ","
            << stlTime << "\n";
    }

    result.close();

    cout << "\nDa luu ket qua vao result.csv\n";

    return 0;
}