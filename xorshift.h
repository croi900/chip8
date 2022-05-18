#include <stdint.h>

/* These state variables must be initialised so that they are not all zero. */
static const uint64_t STATE_MAGIC = 0x2545F4914F6CDD1D;
static uint64_t state = STATE_MAGIC * 3.21 / 1.2;


 
uint32_t next_int() {
    uint64_t x;
    uint32_t answer;
 
    x = state;
    x = x ^ (x >> 12);
    x = x ^ (x << 25);
    x = x ^ (x >> 27);
    state = x;
    answer = ((x * STATE_MAGIC) >> 32);
 
    return answer;
}

uint8_t xor_byte(){
    byte bn = (next_int() % 100) / 25;
    uint32_t mask = (byte)(-1)<<bn*8;
    return (next_int() & mask) >> bn*8;
}