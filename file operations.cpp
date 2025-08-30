#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr;
    char filename[50], ch;

    // ---------- Writing to a file ----------
    printf("Enter the filename to write: ");
    scanf("%s", filename);

    fptr = fopen(filename, "w"); // Open for writing
    if (fptr == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    fprintf(fptr, "Hello, this is a file write test!\n");
    fprintf(fptr, "File operations in C are powerful.\n");

    fclose(fptr);
    printf("Data written successfully to %s\n", filename);

    // ---------- Reading from the file ----------
    fptr = fopen(filename, "r"); // Open for reading
    if (fptr == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    printf("\nContents of %s:\n", filename);
    while ((ch = fgetc(fptr)) != EOF) {
        putchar(ch);
    }

    fclose(fptr);

    return 0;
}
