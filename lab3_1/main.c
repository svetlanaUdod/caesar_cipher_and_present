#include "present.h"

void encryptFile(FILE *input_file, FILE *output_file) {
    uint8_t **roundKeys = generateKeys(key);
    uint64_t block = 0;

    while (fread(&block, 1, sizeof(uint64_t), input_file) > 0) {
        uint64_t res = encrypt(block, roundKeys);
        fwrite(&res, sizeof(uint64_t), 1, output_file);
        block = 0;
    }
    free(roundKeys);
}

void decryptFile(FILE *input_file, FILE *output_file) {

    uint8_t **roundKeys = generateKeys(key);
    uint64_t block = 0;

    while (fread(&block, 1, sizeof(uint64_t), input_file) > 0) {
        uint64_t res = decrypt(block, roundKeys);
        fwrite(&res, sizeof(uint64_t), 1, output_file);
        block = 0;
    }
    free(roundKeys);
}

int main(int argc, char **argv) {
    if (argc == 4) {

        FILE *input_file = fopen(argv[2], "r");
        FILE *output_file = fopen(argv[3], "w");

        switch (argv[1][0]) {
            case 'd':
                decryptFile(input_file, output_file);
                break;
            case 'e':
                encryptFile(input_file, output_file);
                break;
            default:
                printf("Incorrect option");
                return 1;
        }
        fclose(input_file);
        fclose(output_file);

        return 0;
    }
    if ((argc == 2) && (argv[1][0] == 'h')) {

        while (!feof(stdin)) {
            char *cur = (char *) calloc(MAX_LEN, sizeof(char));
            fgets(cur, MAX_LEN, stdin);
            printf("hash: %lu\n", hash(cur, strlen(cur)));
            free(cur);
        }

        return 0;
    }


    printf("Error: found %d arguments. Needs exactly 1 or 3.\n", argc - 1);
    printf("To hash a string, just enter only it\n");
    printf("To encrypt, enter: e inputFile outputFile\n");
    printf("To dencrypt, enter: d inputFile outputFile\n");
    return 1;

}
