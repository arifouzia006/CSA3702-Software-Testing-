/* vc_manager_git.c
   Simple Git wrapper to manage test scripts, defect logs, and test results.
   Compile: gcc -o vc_manager_git vc_manager_git.c
   Run: ./vc_manager_git
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_BUF 1024
#define PATH_MAX_LEN 512

void run_cmd(const char *cmd) {
    int rc = system(cmd);
    if (rc != 0) {
        printf("[WARNING] Command failed (exit %d): %s\n", rc, cmd);
    }
}

void run_cmd_capture(const char *cmd) {
    char buf[512];
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        printf("Failed to run command: %s\n", cmd);
        return;
    }
    while (fgets(buf, sizeof(buf), fp)) {
        fputs(buf, stdout);
    }
    pclose(fp);
}

void init_repo() {
    printf("Initializing git repository (if not already)...\n");
    run_cmd("git rev-parse --is-inside-work-tree > /dev/null 2>&1 || git init");
}

void add_and_commit() {
    char msg[256];
    printf("Enter commit message: ");
    if (!fgets(msg, sizeof(msg), stdin)) return;
    msg[strcspn(msg, "\n")] = 0; // trim newline

    // Stage common artifact directories if they exist
    run_cmd("test -d tests && git add tests || true");
    run_cmd("test -d defects && git add defects || true");
    run_cmd("test -d results && git add results || true");

    // If user wants to add arbitrary paths
    char extra[PATH_MAX_LEN];
    printf("Add extra path (or press Enter to skip): ");
    if (!fgets(extra, sizeof(extra), stdin)) return;
    extra[strcspn(extra, "\n")] = 0;
    if (strlen(extra) > 0) {
        char cmd[CMD_BUF];
        snprintf(cmd, sizeof(cmd), "git add \"%s\"", extra);
        run_cmd(cmd);
    }

    char cmd[CMD_BUF];
    snprintf(cmd, sizeof(cmd), "git commit -m \"%s\" || echo \"Nothing to commit or commit failed\"", msg);
    run_cmd(cmd);
}

void create_branch() {
    char branch[128];
    printf("Enter branch name to create/switch to: ");
    if (!fgets(branch, sizeof(branch), stdin)) return;
    branch[strcspn(branch, "\n")] = 0;
    if (strlen(branch) == 0) return;

    char cmd[CMD_BUF];
    snprintf(cmd, sizeof(cmd), "git checkout -b \"%s\" 2>/dev/null || git checkout \"%s\"", branch, branch);
    run_cmd(cmd);
    printf("Now on branch '%s'\n", branch);
}

void create_tag() {
    char tag[128], msg[256];
    printf("Enter tag name: ");
    if (!fgets(tag, sizeof(tag), stdin)) return;
    tag[strcspn(tag, "\n")] = 0;
    if (strlen(tag) == 0) return;
    printf("Enter tag message (optional): ");
    if (!fgets(msg, sizeof(msg), stdin)) return;
    msg[strcspn(msg, "\n")] = 0;

    char cmd[CMD_BUF];
    if (strlen(msg) > 0) {
        snprintf(cmd, sizeof(cmd), "git tag -a \"%s\" -m \"%s\" || echo \"tag failed\"", tag, msg);
    } else {
        snprintf(cmd, sizeof(cmd), "git tag \"%s\" || echo \"tag failed\"", tag);
    }
    run_cmd(cmd);
    printf("Created tag '%s'\n", tag);
}

void show_log() {
    printf("Recent commits (git log --oneline -n 20):\n");
    run_cmd_capture("git --no-pager log --oneline -n 20");
}

void push_remote() {
    char remote[256], branch[128];
    printf("Enter remote name (e.g., origin) or press Enter to skip: ");
    if (!fgets(remote, sizeof(remote), stdin)) return;
    remote[strcspn(remote, "\n")] = 0;
    if (strlen(remote) == 0) return;
    printf("Enter branch to push (or press Enter to push current branch): ");
    if (!fgets(branch, sizeof(branch), stdin)) return;
    branch[strcspn(branch, "\n")] = 0;

    char cmd[CMD_BUF];
    if (strlen(branch) == 0) {
        snprintf(cmd, sizeof(cmd), "git push %s", remote);
    } else {
        snprintf(cmd, sizeof(cmd), "git push %s %s", remote, branch);
    }
    run_cmd(cmd);
}

int main() {
    int choice = 0;
    while (1) {
        printf("\n=== Test Assets Git Manager ===\n");
        printf("1. Init repository\n");
        printf("2. Add & commit (tests/ defects/ results/ or extra)\n");
        printf("3. Create/switch branch\n");
        printf("4. Create tag\n");
        printf("5. Show recent log\n");
        printf("6. Push to remote\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        while (getchar() != '\n'); // flush newline

        if (choice == 1) init_repo();
        else if (choice == 2) add_and_commit();
        else if (choice == 3) create_branch();
        else if (choice == 4) create_tag();
        else if (choice == 5) show_log();
        else if (choice == 6) push_remote();
        else if (choice == 7) break;
        else printf("Invalid choice\n");
    }
    printf("Exiting.\n");
    return 0;
}
