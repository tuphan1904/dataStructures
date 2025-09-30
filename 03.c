#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Stack {
    Node* data[100];
    int top;
} Stack;

Node* createNode(char data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void initStack(Stack* s) {
    s->top = -1;
}

void push(Stack* s, Node* node) {
    s->data[++(s->top)] = node;
}

Node* pop(Stack* s) {
    return s->data[(s->top)--];
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

Node* parseTree(char* str, int* pos) {
    while (str[*pos] == ' ') (*pos)++;
    
    if (isalpha(str[*pos])) {
        Node* node = createNode(str[*pos]);
        (*pos)++;
        
        while (str[*pos] == ' ') (*pos)++;
        
        if (str[*pos] == '(') {
            (*pos)++; 
            
            Node* children[100];
            int childCount = 0;
            
            while (str[*pos] != '\0' && str[*pos] != ')') {
                while (str[*pos] == ' ') (*pos)++;
                if (str[*pos] == ')') break;
                
                Node* child = parseTree(str, pos);
                if (child) {
                    children[childCount++] = child;
                }
                while (str[*pos] == ' ') (*pos)++;
            }
            
            if (str[*pos] == ')') {
                (*pos)++; 
            }
            
            node->left = (childCount > 0) ? children[0] : NULL;
            node->right = (childCount > 1) ? children[1] : NULL;
        }
        
        return node;
    }
    
    if (str[*pos] == '(') {
        (*pos)++;
        return parseTree(str, pos);
    }
    
    return NULL;
}

void preorderIterative(Node* root) {
    if (root == NULL) return;
    
    Stack s;
    initStack(&s);
    push(&s, root);
    
    printf("pre-order: ");
    while (!isEmpty(&s)) {
        Node* node = pop(&s);
        printf("%c ", node->data);
        
        if (node->right) push(&s, node->right);
        if (node->left) push(&s, node->left);
    }
    printf("\n");
}

void inorderIterative(Node* root) {
    if (root == NULL) return;
    
    Stack s;
    initStack(&s);
    Node* current = root;
    
    printf("in-order: ");
    while (!isEmpty(&s) || current != NULL) {
        while (current != NULL) {
            push(&s, current);
            current = current->left;
        }
        
        current = pop(&s);
        printf("%c ", current->data);
        current = current->right;
    }
    printf("\n");
}

void postorderIterative(Node* root) {
    if (root == NULL) return;
    
    Stack s1, s2;
    initStack(&s1);
    initStack(&s2);
    
    push(&s1, root);
    
    while (!isEmpty(&s1)) {
        Node* node = pop(&s1);
        push(&s2, node);
        
        if (node->left) push(&s1, node->left);
        if (node->right) push(&s1, node->right);
    }
    
    printf("post-order: ");
    while (!isEmpty(&s2)) {
        printf("%c ", pop(&s2)->data);
    }
    printf("\n");
}

int main() {
    char input[1000];
    
    printf("Input:\n");
    fgets(input, sizeof(input), stdin);
    
    input[strcspn(input, "\n")] = 0;
    
    int pos = 0;
    Node* root = parseTree(input, &pos);
    
    printf("\nOutput:\n");
    preorderIterative(root);
    inorderIterative(root);
    postorderIterative(root);
    
    return 0;
}
