#include <stdio.h>

// Global counter for comparisons
long long comparisons = 0;

// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function (choosing first element as pivot -> can cause poor performance)
int partition(int arr[], int low, int high) {
    int pivot = arr[low]; // always pick first element as pivot
    int i = low + 1;
    int j = high;

    while (1) {
        while (i <= high && arr[i] <= pivot) {
            comparisons++;
            i++;
        }
        while (j >= low && arr[j] > pivot) {
            comparisons++;
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        } else {
            break;
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

// Quicksort recursive function
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

int main() {
    int n, i;
    
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    comparisons = 0; // reset counter
    quicksort(arr, 0, n - 1);

    printf("\nSorted array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\nTotal comparisons: %lld\n", comparisons);

    // Worst-case simulation: when input is already sorted
    // QuickSort with first element as pivot -> O(n^2)
    return 0;
}
