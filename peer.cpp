#include <stdio.h>
#include <string.h>

struct TestCase {
    int id;
    char description[100];
    char expectedResult[50];
    char status[20]; // Approved / Needs Improvement
    char comments[100];
};

int main() {
    int n, i;
    printf("Enter number of test cases to review: ");
    scanf("%d", &n);

    struct TestCase testCases[n];

    // Input test cases
    for(i = 0; i < n; i++) {
        testCases[i].id = i + 1;
        getchar(); // clear buffer
        printf("\nEnter description for Test Case %d: ", i + 1);
        fgets(testCases[i].description, sizeof(testCases[i].description), stdin);

        printf("Enter expected result for Test Case %d: ", i + 1);
        fgets(testCases[i].expectedResult, sizeof(testCases[i].expectedResult), stdin);

        strcpy(testCases[i].status, "Pending");
        strcpy(testCases[i].comments, "Not Reviewed");
    }

    // Peer review process
    printf("\n--- Peer Review Process ---\n");
    for(i = 0; i < n; i++) {
        int choice;
        printf("\nReviewing Test Case %d\n", testCases[i].id);
        printf("Description: %s", testCases[i].description);
        printf("Expected Result: %s", testCases[i].expectedResult);

        printf("Enter review decision (1 = Approve, 2 = Needs Improvement): ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        if(choice == 1) {
            strcpy(testCases[i].status, "Approved");
            strcpy(testCases[i].comments, "Looks good");
        } else {
            strcpy(testCases[i].status, "Needs Improvement");
            printf("Enter reviewer comments: ");
            fgets(testCases[i].comments, sizeof(testCases[i].comments), stdin);
        }
    }

    // Display review summary
    printf("\n--- Review Summary ---\n");
    for(i = 0; i < n; i++) {
        printf("\nTest Case %d\n", testCases[i].id);
        printf("Description: %s", testCases[i].description);
        printf("Expected Result: %s", testCases[i].expectedResult);
        printf("Review Status: %s\n", testCases[i].status);
        printf("Reviewer Comments: %s\n", testCases[i].comments);
    }

    return 0;
}
