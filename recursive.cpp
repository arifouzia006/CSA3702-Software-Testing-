#include <stdio.h>

// Recursive factorial function
long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1; // base case
    } else {
        return n * factorial(n - 1); // recursive case
    }
}

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    if (num < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        printf("Factorial of %d = %lld\n", num, factorial(num));
    }
    return 0;
}
