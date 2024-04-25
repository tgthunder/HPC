#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

void sequential_bubble_sort(int arr[], int size) {
    int array[size];
    for (int i = 0; i < size; i++) {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    // Bubble sort implementation...
    double end = omp_get_wtime();

    cout << "Sequential Bubble Sort:\n";
    cout << "Time Required: " << end - start << endl;
}

void parallel_bubble_sort(int arr[], int size) {
    int array[size];
    for (int i = 0; i < size; i++) {
        array[i] = arr[i];
    }

    double start = omp_get_wtime();
    // Parallel bubble sort implementation...
    double end = omp_get_wtime();

    cout << "Parallel Bubble Sort:\n";
    cout << "Time Required: " << end - start << endl;
}

void merge(int array[], int low, int mid, int high, int size) {
    // Merge function implementation...
}

void mergesort(int array[], int low, int high, int size) {
    // Merge sort function implementation...
}

void perform_merge_sort(int arr[], int size) {
    // Merge sort function call...
}

void p_mergesort(int array[], int low, int high, int size) {
    if (low < high) {
        int mid = (low + high) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            p_mergesort(array, low, mid, size);
#pragma omp section
            p_mergesort(array, mid + 1, high, size);
        }
        merge(array, low, mid, high, size);
    }
}

void perform_p_merge_sort(int arr[], int size) {
    // Parallel merge sort function call...
}

int main(int argc, char const *argv[]) {
    int SIZE;
    int MAX = 1000;
    cout << "Enter size of array: ";
    cin >> SIZE;
    int array[SIZE];
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % MAX;
    }
    sequential_bubble_sort(array, SIZE);
    parallel_bubble_sort(array, SIZE);
    perform_merge_sort(array, SIZE);
    perform_p_merge_sort(array, SIZE);
    return 0;
}
