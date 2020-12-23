//
// Created by admin on 22.12.2020.
//

#ifndef LAB3_1_PRESENT_H
#define LAB3_1_PRESENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUND_COUNT 32
#define BLOCK_SIZE 64
#define KEY_SIZE 80
#define KEY_BLOCK_SIZE_16 5
#define KEY_BLOCK_SIZE_8 10
#define MAX_LEN 80

static uint8_t const sbox[] = {0x0Cu, 0x05u, 0x06u, 0x0Bu, \
                                 0x09u, 0x00u, 0x0Au, 0x0Du, \
                                 0x03u, 0x0Eu, 0x0Fu, 0x08u, \
                                 0x04u, 0x07u, 0x01u, 0x02u};
static uint8_t pbox[] = {0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51,\
                    4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55,\
                    8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59,\
                    12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

static uint8_t const invsbox[] = {0x05u, 0x0Eu, 0x0Fu, 0x08u, \
                                     0x0Cu, 0x01u, 0x02u, 0x0Du, \
                                     0x0Bu, 0x04u, 0x06u, 0x03u, \
                                     0x00u, 0x07u, 0x09u, 0x0Au};
static uint8_t const key[] = {0x00u, 0x00u, 0x00u, 0x00u, 0x00u, \
                                0x00u, 0x00u, 0x00u, 0x00u, 0x00u};
static uint64_t const h0=0x050E0F080C01020Du;

uint8_t ** generateKeys(const uint8_t *key);
uint64_t sBoxlayer(uint64_t block);
uint64_t invSBoxlayer(uint64_t block);
uint64_t pLayer(uint64_t block);
uint64_t invPLayer(uint64_t block);
uint64_t addRoundKey(int round,uint64_t block,uint8_t **keys);
uint64_t encrypt(uint64_t block, uint8_t **keys);
uint64_t decrypt(uint64_t block, uint8_t **keys);
uint64_t hash(uint8_t *message, int length);

#endif //LAB3_1_PRESENT_H
