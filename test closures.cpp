#include <stdio.h>

struct TestClosure {
    int totalTestCases;
    int passed;
    int failed;
    int defectsLogged;
    char projectName[50];
    char closureDate[20];
};

void generateClosureReport(struct TestClosure t) {
    printf("\n===== TEST CLOSURE REPORT =====\n");
    printf("Project Name   : %s\n", t.projectName);
    printf("Closure Date   : %s\n", t.closureDate);
    printf("Total Test Cases Executed : %d\n", t.totalTestCases);
    printf("Passed Test Cases         : %d\n", t.passed);
    printf("Failed Test Cases         : %d\n", t.failed);
    printf("Defects Logged            : %d\n", t.defectsLogged);

    // Simple analysis
    float passRate = ((float)t.passed / t.totalTestCases) * 100;
    float failRate = ((float)t.failed / t.totalTestCases) * 100;

    printf("\n--- Analysis ---\n");
    printf("Pass Percentage : %.2f%%\n", passRate);
    printf("Fail Percentage : %.2f%%\n", failRate);

    if (failRate > 20) {
        printf("Conclusion: High failure rate. More testing needed before release.\n");
    } else {
        printf("Conclusion: Testing completed successfully. Ready for closure.\n");
    }

    printf("===== END OF REPORT =====\n");
}

int main() {
    struct TestClosure t;

    printf("Enter Project Name: ");
    scanf(" %[^\n]", t.projectName);

    printf("Enter Closure Date (dd-mm-yyyy): ");
    scanf(" %[^\n]", t.closureDate);

    printf("Enter Total Test Cases Executed: ");
    scanf("%d", &t.totalTestCases);

    printf("Enter Number of Passed Test Cases: ");
    scanf("%d", &t.passed);

    printf("Enter Number of Failed Test Cases: ");
    scanf("%d", &t.failed);

    printf("Enter Number of Defects Logged: ");
    scanf("%d", &t.defectsLogged);

    // Generate the final test closure report
    generateClosureReport(t);

    return 0;
}
