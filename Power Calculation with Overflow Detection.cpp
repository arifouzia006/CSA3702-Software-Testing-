#include <stdio.h>
#include <limits.h>  // for INT_MAX and LONG_LONG_MAX

// Function to calculate power with overflow detection
long long power(int base, int exp) {
    long long result = 1;

    for (int i = 0; i < exp; i++) {
        // Check for overflow before multiplying
        if (base != 0 && result > LLONG_MAX / base) {
            printf("?? Overflow detected at step %d!\n", i + 1);
            return -1; // Indicate overflow
        }
        result *= base;
    }
    return result;
}

int main() {
    int base, exp;
    printf("Enter base and exponent: ");
    scanf("%d %d", &base, &exp);

    if (exp < 0) {
        printf("Negative exponents not supported with integers.\n");
        return 1;
    }

    long long result = power(base, exp);
    if (result != -1) {
        printf("%d^%d = %lld\n", base, exp, result);
    } else {
        printf("Calculation failed due to overflow.\n");
    }

    return 0;
}
