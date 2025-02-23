#include <stdio.h>

void *malloc(size_t);
void free(void *);

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

void add_to_list(Node **head, char ch) {
    Node *current = *head;
    while (current != NULL) {
        if (current->ch == ch) {
            current->count++;
            return;
        }
        if (current->next == NULL) break;
        current = current->next;
    }
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    newNode->ch = ch;
    newNode->count = 1;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        current->next = newNode;
    }
}

void free_list(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(Node *head) {
    while (head != NULL) {
        if (head->ch == '\n') {
            printf("\\n (ASCII 10):%d\n", head->count);
        } else {
            printf("%c (ASCII %d):%d\n", head->ch, (int)head->ch, head->count);
        }
        head = head->next;
    }
}

int main() {
    FILE *file = fopen("HW0.c", "r");
    if (!file) {
        printf("unable to open file\n");
        return 1;
    }
    Node *head = NULL;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        add_to_list(&head, ch);
    }
    fclose(file);
    print_list(head);
    free_list(head);
    return 0;
}
