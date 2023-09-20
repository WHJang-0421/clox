#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->line_count = 0;
    chunk->line_capacity = 0;
    chunk->line_token_lengths = NULL;
    chunk->line_values = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->count++;
    // code for updating line
    if (chunk->line_count == 0 || (line != chunk->line_values[chunk->line_count-1])) {
        if (chunk->line_capacity < chunk->line_count + 1) {
            int oldLineCapacity = chunk->line_capacity;
            chunk->line_capacity = GROW_CAPACITY(oldLineCapacity);
            chunk->line_token_lengths = GROW_ARRAY(int, chunk->line_token_lengths, oldLineCapacity, chunk->line_capacity);
            chunk->line_values = GROW_ARRAY(int, chunk->line_values, oldLineCapacity, chunk->line_capacity);
        }
        chunk->line_values[chunk->line_count] = line;
        chunk->line_token_lengths[chunk->line_count] = 1;
        chunk->line_count++;
    } else {
        chunk->line_token_lengths[chunk->line_count-1]++;
    }
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->line_token_lengths, chunk->line_capacity);
    FREE_ARRAY(int, chunk->line_values, chunk->line_capacity);
    freeValueArray(&chunk->constants);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int index) {
    int code_idx = 0;
    int line_idx = 0;
    for (; line_idx < chunk->line_capacity; line_idx++) {
        for (int i = 0; i < chunk->line_token_lengths[line_idx]; i++) {
            code_idx++;
            if (code_idx > index) {
                return chunk->line_values[line_idx];
            }
        }
    }
    return 2;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int constant = addConstant(chunk, value);
    if (constant < 256) {
        // add OP-CONSTANT
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, constant, line);
    } else {
        // get the third, second, first byte
        uint8_t a = (constant >> 16) & 0xff;
        uint8_t b = (constant >> 8) & 0xff;
        uint8_t c = constant & 0xff;
        // add OP-CONSTANT-LONG
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, a, line);
        writeChunk(chunk, b, line);
        writeChunk(chunk, c, line);
    }
}