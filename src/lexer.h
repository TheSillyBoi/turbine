#ifndef INCLUDE_SRC_LEXER_H_
#define INCLUDE_SRC_LEXER_H_

typedef enum Symbol { LABEL, INSTRUCTION, REGISTER, MEMORY, VALUE } Symbol;

typedef struct Token {
  Symbol symbol;
  char *data;
} Token;

#endif // INCLUDE_SRC_LEXER_H_
