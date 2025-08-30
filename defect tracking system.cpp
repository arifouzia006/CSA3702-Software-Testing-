#include <stdio.h>
#include <string.h>

struct Defect {
    int id;
    char description[100];
    char severity[20];   // Low, Medium, High
    char status[20];     // New, In Progress, Resolved, Closed
    char assignedTo[50];
};

int main() {
    struct Defect defects[50];
    int choice, count = 0, i, id;

    while (1) {
        printf("\n--- Defect Tracking System ---\n");
        printf("1. Log New Defect\n");
        printf("2. Update Defect Status\n");
        printf("3. View All Defects\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        if (choice == 1) {
            defects[count].id = count + 1;
            printf("Enter defect description: ");
            fgets(defects[count].description, sizeof(defects[count].description), stdin);

            printf("Enter severity (Low/Medium/High): ");
            fgets(defects[count].severity, sizeof(defects[count].severity), stdin);

            printf("Assign to developer: ");
            fgets(defects[count].assignedTo, sizeof(defects[count].assignedTo), stdin);

            strcpy(defects[count].status, "New");

            printf("Defect logged successfully with ID: %d\n", defects[count].id);
            count++;
        }
        else if (choice == 2) {
            printf("Enter defect ID to update: ");
            scanf("%d", &id);
            getchar();

            if (id <= 0 || id > count) {
                printf("Invalid Defect ID!\n");
                continue;
            }

            printf("Current Status: %s\n", defects[id - 1].status);
            printf("Enter new status (New/In Progress/Resolved/Closed): ");
            fgets(defects[id - 1].status, sizeof(defects[id - 1].status), stdin);

            printf("Defect ID %d status updated successfully!\n", id);
        }
        else if (choice == 3) {
            printf("\n--- Defect List ---\n");
            for (i = 0; i < count; i++) {
                printf("\nDefect ID: %d\n", defects[i].id);
                printf("Description: %s", defects[i].description);
                printf("Severity: %s", defects[i].severity);
                printf("Assigned To: %s", defects[i].assignedTo);
                printf("Status: %s\n", defects[i].status);
            }
            if (count == 0)
                printf("No defects logged yet.\n");
        }
        else if (choice == 4) {
            printf("Exiting Defect Tracking System...\n");
            break;
        }
        else {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
