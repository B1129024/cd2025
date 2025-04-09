#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_TOKEN, MAIN_TOKEN, IF_TOKEN, ELSE_TOKEN, WHILE_TOKEN,
    ID_TOKEN, LITERAL_TOKEN,
    ASSIGN_TOKEN, EQUAL_TOKEN, GREATEREQUAL_TOKEN, LESSEQUAL_TOKEN,
    GREATER_TOKEN, LESS_TOKEN,
    PLUS_TOKEN, MINUS_TOKEN,
    LEFTPAREN_TOKEN, REFTPAREN_TOKEN, LEFTBRACE_TOKEN, REFTBRACE_TOKEN,
    SEMICOLON_TOKEN
} TokenType;

const char *token_names[] = {
    "TYPE_TOKEN", "MAIN_TOKEN", "IF_TOKEN", "ELSE_TOKEN", "WHILE_TOKEN",
    "ID_TOKEN", "LITERAL_TOKEN",
    "ASSIGN_TOKEN", "EQUAL_TOKEN", "GREATEREQUAL_TOKEN", "LESSEQUAL_TOKEN",
    "GREATER_TOKEN", "LESS_TOKEN",
    "PLUS_TOKEN", "MINUS_TOKEN",
    "LEFTPAREN_TOKEN", "REFTPAREN_TOKEN", "LEFTBRACE_TOKEN", "REFTBRACE_TOKEN",
    "SEMICOLON_TOKEN"
};

typedef struct Node {
    char value[100];
    TokenType type;
    struct Node *next;
} Node;

void add_to_list(Node **head, const char *val, TokenType type) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return;
    strcpy(newNode->value, val);
    newNode->type = type;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node *cur = *head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = newNode;
    }
}

void print_list(Node *head) {
    while (head != NULL) {
        printf("%s: %s\n", head->value, token_names[head->type]);
        head = head->next;
    }
}

void free_list(Node *head) {
    Node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

TokenType check_keyword(const char *str) {
    if (strcmp(str, "int") == 0) return TYPE_TOKEN;
    if (strcmp(str, "main") == 0) return MAIN_TOKEN;
    if (strcmp(str, "if") == 0) return IF_TOKEN;
    if (strcmp(str, "else") == 0) return ELSE_TOKEN;
    if (strcmp(str, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

void scan(FILE *file, Node **head) {
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (is_whitespace(c)) continue;

        if (is_letter(c)) {
            char buffer[100];
            int i = 0;
            buffer[i++] = c;
            while ((c = fgetc(file)) != EOF && (is_letter(c) || is_digit(c))) {
                if (i < 99) buffer[i++] = c;
            }
            buffer[i] = '\0';
            if (c != EOF) ungetc(c, file);
            add_to_list(head, buffer, check_keyword(buffer));
        } else if (is_digit(c)) {
            char buffer[100];
            int i = 0;
            buffer[i++] = c;
            while ((c = fgetc(file)) != EOF && is_digit(c)) {
                if (i < 99) buffer[i++] = c;
            }
            buffer[i] = '\0';
            if (c != EOF) ungetc(c, file);
            add_to_list(head, buffer, LITERAL_TOKEN);
        } else {
            char next = fgetc(file);
            char temp[3] = {c, 0, 0};
            switch (c) {
                case '=':
                    if (next == '=') {
                        temp[1] = '=';
                        add_to_list(head, temp, EQUAL_TOKEN);
                    } else {
                        if (next != EOF) ungetc(next, file);
                        add_to_list(head, temp, ASSIGN_TOKEN);
                    }
                    break;
                case '>':
                    if (next == '=') {
                        temp[1] = '=';
                        add_to_list(head, temp, GREATEREQUAL_TOKEN);
                    } else {
                        if (next != EOF) ungetc(next, file);
                        add_to_list(head, temp, GREATER_TOKEN);
                    }
                    break;
                case '<':
                    if (next == '=') {
                        temp[1] = '=';
                        add_to_list(head, temp, LESSEQUAL_TOKEN);
                    } else {
                        if (next != EOF) ungetc(next, file);
                        add_to_list(head, temp, LESS_TOKEN);
                    }
                    break;
                case '+':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, PLUS_TOKEN);
                    break;
                case '-':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, MINUS_TOKEN);
                    break;
                case '(':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, LEFTPAREN_TOKEN);
                    break;
                case ')':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, REFTPAREN_TOKEN);
                    break;
                case '{':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, LEFTBRACE_TOKEN);
                    break;
                case '}':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, REFTBRACE_TOKEN);
                    break;
                case ';':
                    if (next != EOF) ungetc(next, file);
                    add_to_list(head, temp, SEMICOLON_TOKEN);
                    break;
                default:
                    if (next != EOF) ungetc(next, file);
                    break; // 不處理 unknown token
            }
        }
    }
}

int main() {
    FILE *file = fopen("main.c", "r");  // 修改這行可以用 input.c 測試
    if (!file) {
        printf("unable to open file\n");
        return 1;
    }

    Node *head = NULL;
    scan(file, &head);
    fclose(file);

    print_list(head);
    free_list(head);
    return 0;
}
