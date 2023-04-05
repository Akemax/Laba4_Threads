#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <cstdlib>

using namespace std;

vector<vector<int>> matrix;
vector<vector<int>> prefix_sum;
int max_sum = -1, max_i = -1, max_j = -1, max_k = -1;

void find_max_submatrix(int n, int k) {

    prefix_sum.resize(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            prefix_sum[i][j] = matrix[i][j];
            if (i > 0) {
                prefix_sum[i][j] += prefix_sum[i - 1][j];
            }
            if (j > 0) {
                prefix_sum[i][j] += prefix_sum[i][j - 1];
            }
            if (i > 0 && j > 0) {
                prefix_sum[i][j] -= prefix_sum[i - 1][j - 1];
            }
        }
    }
    std::cout << "\n";

    for (int i = 0; i < n; i++) {
        std::cout << "\n";
        for (int j = 0; j < n; j++) {
            std::cout << prefix_sum[i][j] << " ";
        }
    }

#pragma omp parallel for collapse(2)
    for (int i = 0; i <= n - k; i++) {
        for (int j = 0; j <= n - k; j++) {
            int sum = prefix_sum[i + k - 1][j + k - 1];
            if (i > 0) {
                sum -= prefix_sum[i - 1][j + k - 1];
            }
            if (j > 0) {
                sum -= prefix_sum[i + k - 1][j - 1];
            }
            if (i > 0 && j > 0) {
                sum += prefix_sum[i - 1][j - 1];
            }

#pragma omp critical
            {
                if (sum > max_sum) {
                    max_sum = sum;
                    max_i = i;
                    max_j = j;
                    max_k = k;
                }
            }
        }
    }
}

void print_max_submatrix() {

    cout << "\n" << endl;
    cout << "Max sum: " << max_sum << "\n" << endl;
    cout << "Submatrix size: " << max_k << "x" << max_k << endl;
    cout << endl;
    cout << "Submatrix:" << endl;
    for (int i = max_i; i < max_i + max_k; i++) {
        for (int j = max_j; j < max_j + max_k; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}


int main() {

    auto start_time = std::chrono::steady_clock::now();

    int n = 10, k = 4;

    matrix.resize(n, vector<int>(n)); // Створення двовимірного масиву n-розміру
    
    srand(time(NULL));

    for (int i = 0; i < n; i++) {   // Функція створення значень у масиві
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < n; i++) {   // Функція виводу масиву для наглядного прикладу
        std::cout << "\n";
        for (int j = 0; j < n; j++) {
            std::cout << matrix[i][j] << " ";
        }
    }

    find_max_submatrix(n, k);

    print_max_submatrix();

    auto end_time = std::chrono::steady_clock::now(); // Кінцевий час

    auto duration_ms = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() / 1e6;

    std::cout << "\nElapsed time: " << duration_ms << " ms\n"; // Виведення часу виконання
}



