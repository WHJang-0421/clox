#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} Opcode;

typedef struct {
    int count;
    int capacity;
    uint8_t * code;
    int line_count;
    int line_capacity;
    int* line_values;
    int* line_token_lengths;
    ValueArray constants;
} Chunk; // a dynamic array holding byte code, line number and constants

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);
int getLine(Chunk* chunk, int index);
#endif