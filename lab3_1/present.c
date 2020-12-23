//
// Created by admin on 22.12.2020.
//
#include "present.h"

static void rotateKeyLeft(uint8_t *pKey) {
    uint16_t buff[2];
    uint16_t *pBlock;
    uint8_t block;
    uint8_t const rotationPoint = 3;
    uint8_t const unrotatedBlocks = 1;
    uint8_t const lsbOffset = 2;
    uint8_t const msbOffset = 1;

    pBlock = (uint16_t *) pKey;
    for (block = 0u; block < 2; block++) {
        buff[block] = pBlock[block];
    }
    for (block = 0u; block < rotationPoint; block++) {
        pBlock[block] = (pBlock[block + lsbOffset] << 13) | (pBlock[block + msbOffset] >> 3);
    }

    pBlock[rotationPoint] = (buff[0] << 13) | (pBlock[KEY_BLOCK_SIZE_16 - 1] >> 3);
    for (block = 0u; block < unrotatedBlocks; block++) {
        pBlock[block + 4] = (buff[block + 1] << 13) | (buff[block] >> 3);
    }
}

uint8_t **generateKeys(const uint8_t *key) {
    uint8_t **roundKeys = (uint8_t **) malloc(ROUND_COUNT * (sizeof(uint8_t *)));
    uint8_t *tmpkey = (uint8_t *) malloc(KEY_BLOCK_SIZE_8 * (sizeof(uint8_t)));
    uint8_t highNibble;
    uint8_t lowNibble;

    roundKeys[0] = (uint8_t *) malloc(KEY_BLOCK_SIZE_8 * (sizeof(uint8_t)));
    memcpy(roundKeys[0], key, KEY_BLOCK_SIZE_8);
    memcpy(tmpkey, key, KEY_BLOCK_SIZE_8);

    for (int i = 1; i < ROUND_COUNT; ++i) {
        rotateKeyLeft(tmpkey);
        highNibble = (tmpkey[KEY_BLOCK_SIZE_8 - 1] & 0xF0u) >> 4;
        highNibble = sbox[highNibble];
        lowNibble = tmpkey[KEY_BLOCK_SIZE_8 - 1] & 0x0Fu;
        tmpkey[KEY_BLOCK_SIZE_8 - 1] = (highNibble << 4) | lowNibble;
        tmpkey[2] ^= i >> 1;
        tmpkey[1] ^= i << 7;
        roundKeys[i] = (uint8_t *) malloc(KEY_BLOCK_SIZE_8 * (sizeof(uint8_t)));
        memcpy(roundKeys[i], tmpkey, KEY_BLOCK_SIZE_8);
    }
    free(tmpkey);

    return roundKeys;
}

uint64_t sBoxlayer(uint64_t block) {
    uint8_t *tmp = (uint8_t *) &block;
    for (int j = 0; j < sizeof(uint64_t); ++j) {
        uint8_t rPart = sbox[(tmp[j] & 0x0Fu)];
        uint8_t lPart = sbox[(tmp[j] & 0xF0u) >> 4];
        tmp[j] = lPart << 4 | rPart;
    }
    return block;
}

uint64_t invSBoxlayer(uint64_t block) {
    uint8_t *tmp = (uint8_t *) &block;
    for (int j = 0; j < sizeof(uint64_t); ++j) {
        uint8_t rPart = invsbox[(tmp[j] & 0x0Fu)];
        uint8_t lPart = invsbox[(tmp[j] & 0xF0u) >> 4];
        tmp[j] = lPart << 4 | rPart;
    }
    return block;
}


uint64_t pLayer(uint64_t block) {
    uint64_t pBlock = 0;

    for (int i = 0; i < BLOCK_SIZE; ++i) {
        int distance = BLOCK_SIZE - i - 1;
        pBlock |= ((block >> distance & 0x1) << (BLOCK_SIZE - pbox[i] - 1));
    }

    return pBlock;

}

uint64_t invPLayer(uint64_t block) {
    uint64_t pBlock = 0;

    for (int i = 0; i < BLOCK_SIZE; ++i) {
        int distance = BLOCK_SIZE - pbox[i] - 1;
        pBlock = (pBlock << 1) | ((block >> distance) & 0x1);
    }

    return pBlock;

}

uint64_t addRoundKey(int round, uint64_t block, uint8_t **keys) {
    uint64_t *key = keys[round] + (KEY_SIZE - BLOCK_SIZE) / 8;
    block = block ^ (*key);
    return block;

}

uint64_t encrypt(uint64_t block, uint8_t **keys) {

    for (int i = 0; i < ROUND_COUNT - 1; ++i) {
        block = addRoundKey(i, block, keys);
        block = sBoxlayer(block);
        block = pLayer(block);
    }

    block = addRoundKey(ROUND_COUNT - 1, block, keys);

    return block;
}

uint64_t decrypt(uint64_t block, uint8_t **keys) {
    for (int i = ROUND_COUNT - 1; i > 0; --i) {
        block = addRoundKey(i, block, keys);
        block = invPLayer(block);
        block = invSBoxlayer(block);

    }
    block = addRoundKey(0, block, keys);
    return block;
}

uint64_t hash(uint8_t *message, int length) {
    int mod = length % KEY_BLOCK_SIZE_8;
    if (mod > 0) {
        length = length + (KEY_BLOCK_SIZE_8 - mod);
        uint8_t *tmp = (uint8_t *) calloc(length, sizeof(uint8_t));
        memcpy(tmp, message, length);
        message = tmp;
    }
    uint64_t h = h0;

    for (int i = 0; i < length/KEY_BLOCK_SIZE_8; ++i) {
        uint8_t **keys = generateKeys(message + KEY_BLOCK_SIZE_8 * i);
        h = encrypt(h, keys);
        free(keys);
    }
    if (mod > 0) free(message);

    return h;
}


