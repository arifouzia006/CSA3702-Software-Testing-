#include <stdio.h>

int sumOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    // Repeatedly reduce until single digit
    while (num >= 10) {
        num = sumOfDigits(num);
    }

    printf("Single digit result = %d\n", num);
    return 0;
}
