#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;
    struct Node *firstChild;
    struct Node *nextSibling;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

Node* parseTree(const char *expr) {
    Node *root = NULL;
    Node *currentParent = NULL;
    Node *lastSibling = NULL;
    Node *stack[100]; 
    int top = -1;

    for (int i = 0; expr[i] != '\0'; i++) {
        char ch = expr[i];
        if (ch == ' ' || ch == '\n') continue;

        if (ch == '(') {
            stack[++top] = currentParent;
            lastSibling = NULL;
        } else if (ch == ')') {
            currentParent = stack[top--];
            lastSibling = NULL;
        } else {
            Node* newNode = createNode(ch);
            if (root == NULL) {
                root = newNode;
            } else {
                if (lastSibling == NULL) {
                    if (currentParent != NULL)
                        currentParent->firstChild = newNode;
                } else {
                    lastSibling->nextSibling = newNode;
                }
            }
            lastSibling = newNode;

            if (expr[i+1] == '(') {
                currentParent = newNode;
            }
        }
    }
    return root;
}

int getHeight(Node* root) {
    if (!root) return -1;
    int maxH = -1;
    for (Node* ch = root->firstChild; ch; ch = ch->nextSibling) {
        int h = getHeight(ch);
        if (h > maxH) maxH = h;
    }
    return maxH + 1;
}

int countNodes(Node* root) {
    if (!root) return 0;
    int sum = 1;
    for (Node* ch = root->firstChild; ch; ch = ch->nextSibling)
        sum += countNodes(ch);
    return sum;
}

int countLeaves(Node* root) {
    if (!root) return 0;
    if (!root->firstChild && isalpha(root->data)) return 1;
    int sum = 0;
    for (Node* ch = root->firstChild; ch; ch = ch->nextSibling)
        sum += countLeaves(ch);
    return sum;
}

int main() {
    char input[1000];
    if (!fgets(input, sizeof(input), stdin)) return 0;
    size_t L = strlen(input);
    if (L > 0 && input[L-1] == '\n') input[L-1] = '\0';

    for (size_t i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (!(isalpha((unsigned char)c) || c == '(' || c == ')' || isspace((unsigned char)c))) {
            printf("ERROR\n");
            return 0;
        }
    }

    Node* parentStack[1000];
    Node* lastChildStack[1000];
    int top = -1;
    int expecting_label = 0;
    Node* root = NULL;
    int error = 0;

    for (size_t i = 0; input[i] != '\0'; ) {
        char c = input[i];

        if (isspace((unsigned char)c)) { i++; continue; }

        if (c == '(') {
            expecting_label = 1;
            i++;
            continue;
        }

        if (isalpha((unsigned char)c)) {
            char label = c;

            if (expecting_label) {
                Node* n = createNode(label);
                if (!root) root = n;

                if (top >= 0) {
                    Node* parent = parentStack[top];
                    if (lastChildStack[top] == NULL) parent->firstChild = n;
                    else lastChildStack[top]->nextSibling = n;
                    lastChildStack[top] = n;
                }
                parentStack[++top] = n;
                lastChildStack[top] = NULL;
                expecting_label = 0;
            } else {
                Node* n = createNode(label);
                if (!root && top < 0) {
                    root = n;
                } else if (top >= 0) {
                    Node* parent = parentStack[top];
                    if (lastChildStack[top] == NULL) parent->firstChild = n;
                    else lastChildStack[top]->nextSibling = n;
                    lastChildStack[top] = n;
                } else {
                    if (root) { error = 1; break; }
                    root = n;
                }
            }
            i++; continue;
        }

        if (c == ')') {
            if (expecting_label) { 
                error = 1; break;
            }
            if (top < 0) { 
                error = 1; break;
            }
            top--;
            i++; continue;
        }

        error = 1;
        break;
    }

    if (expecting_label) error = 1;   
    if (top != -1) error = 1;         
    if (!root) error = 1;            

    if (error) {
        printf("ERROR\n");
        return 0;
    }

    int height = getHeight(root);
    int nodes = countNodes(root);
    int leaves = countLeaves(root);
    printf("%d, %d, %d\n", height, nodes, leaves);
    return 0;
}
