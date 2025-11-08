#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_INPUT 256

void cmd_cd(char *path) {
    if (chdir(path) != 0) {
        perror("cd");
    }
}

void cmd_ls() {
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    dir = opendir(cwd);
    if (dir == NULL) {
        perror("ls");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
}

void cmd_touch(char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("touch");
        return;
    }
    fclose(f);
}

void cmd_rm(char *filename) {
    if (remove(filename) != 0) {
        perror("rm");
    }
}

void cmd_cat(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("cat");
        return;
    }
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        putchar(ch);
    }
    fclose(f);
}

int main() {
    char input[MAX_INPUT];
    char *cmd, *arg;

    while (1) {
        printf("cultra> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        input[strcspn(input, "\n")] = 0; // quitar salto de línea
        cmd = strtok(input, " ");
        arg = strtok(NULL, "");

        if (cmd == NULL) continue;

        if (strcmp(cmd, "exit") == 0) break;
        else if (strcmp(cmd, "cd") == 0 && arg) cmd_cd(arg);
        else if (strcmp(cmd, "ls") == 0) cmd_ls();
        else if (strcmp(cmd, "touch") == 0 && arg) cmd_touch(arg);
        else if (strcmp(cmd, "rm") == 0 && arg) cmd_rm(arg);
        else if (strcmp(cmd, "cat") == 0 && arg) cmd_cat(arg);
        else printf("Comando desconocido: %s\n", cmd);
    }

    return 0;
}
