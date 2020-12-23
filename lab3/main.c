#include "ceasar.h"

int main(int argc, char *argv[]) {
    char *cur = NULL;
    char *dst = NULL;
    int key;

    if (argc != 3) {
        printf("Error: found %d arguments. Needs exactly 2.\n", argc - 1);
        return 1;
    }

    key = strtol(argv[2], NULL, 10);
    cur = (char *) calloc(MAX_LEN, sizeof(char));
    dst = (char *) calloc(MAX_LEN, sizeof(char));

    if (strlen(argv[1]) == 1) {
        switch (argv[1][0]) {
            case 'd':
                key = -key;
                break;
            case 'e':
                break;
            default:
                printf("Incorrect option");
        }
    }

    while (!feof(stdin)) {
        fgets(cur, MAX_LEN, stdin);
        caesar(dst, cur, key);
        printf("%s \n", dst);
    }
    return 0;
}
