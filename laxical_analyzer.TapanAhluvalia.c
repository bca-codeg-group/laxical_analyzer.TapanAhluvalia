#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    CONSTANT,
    UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

Token getNextToken(FILE *fp) {
    Token token;
    token.type = UNKNOWN;
    memset(token.lexeme, 0, MAX_TOKEN_LENGTH);

    int ch = fgetc(fp);

    // Skip white spaces
    while (isspace(ch))
        ch = fgetc(fp);

    // Check for end of file
    if (ch == EOF) {
        token.type = UNKNOWN;
        return token;
    }

    // Check for operators
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>') {
        token.type = OPERATOR;
        token.lexeme[0] = ch;
        return token;
    }

    // Check for constants
    if (isdigit(ch)) {
        token.type = CONSTANT;
        int i = 0;
        while (isdigit(ch)) {
            token.lexeme[i++] = ch;
            ch = fgetc(fp);
        }
        ungetc(ch, fp);
        return token;
    }

    // Check for keywords or identifiers
    if (isalpha(ch)) {
        int i = 0;
        while (isalpha(ch) || isdigit(ch) || ch == '_') {
            token.lexeme[i++] = ch;
            ch = fgetc(fp);
        }
        ungetc(ch, fp);

        if (strcmp(token.lexeme, "if") == 0 || strcmp(token.lexeme, "else") == 0 || strcmp(token.lexeme, "while") == 0) {
            token.type = KEYWORD;
        } else {
            token.type = IDENTIFIER;
        }
        return token;
    }

    // Unknown token
    token.type = UNKNOWN;
    token.lexeme[0] = ch;
    return token;
}

int main() {
    FILE *fp;
    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    Token token;
    do {
        token = getNextToken(fp);
        switch (token.type) {
            case KEYWORD:
                printf("Keyword: %s\n", token.lexeme);
                break;
            case IDENTIFIER:
                printf("Identifier: %s\n", token.lexeme);
                break;
            case OPERATOR:
                printf("Operator: %s\n", token.lexeme);
                break;
            case CONSTANT:
                printf("Constant: %s\n", token.lexeme);
                break;
            case UNKNOWN:
                printf("Unknown token: %s\n", token.lexeme);
                break;
        }
    } while (token.type != UNKNOWN && token.type != EOF);

    fclose(fp);

    return 0;
}
