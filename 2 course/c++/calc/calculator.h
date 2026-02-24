#include <cstdint>

enum Register {
    REG_A = 0,
    REG_B = 1,
    REG_C = 2,
    REG_D = 3
};

struct Opcode {
    static const uint8_t MOVI = 0;
    static const uint8_t ADD = 8;
    static const uint8_t SUB = 9;
    static const uint8_t MUL = 10;
    static const uint8_t DIV = 11;
    static const uint8_t IN = 48;
    static const uint8_t OUT = 49;
};