enum operation_t {ADD, SUB, MUL, DIV};
enum lexem_kind_t {OP, REG, NUM};

struct lexem_t {
    lexem_kind_t kind; 
    union {
        operation_t op;
        int reg;
        int num;
    } lex;

};

int main() {
    lexem_t line[4] = {{lexem_kind_t::OP, operation_t::ADD}
                    , {lexem_kind_t::REG}
                    , {lexem_kind_t::REG}
                    , {lexem_kind_t::REG}};
    line[1].lex.reg = 0;
    line[2].lex.reg = 1;
    line[3].lex.reg = 2;
}