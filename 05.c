#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 1000
#define SEARCH_COUNT 1000
#define MAX_VALUE 10000

typedef struct BSTNode {
    int data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct AVLNode {
    int data;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

BSTNode* createBSTNode(int data) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

AVLNode* createAVLNode(int data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode* root, int data) {
    if (root == NULL) return createBSTNode(data);
    
    if (data < root->data)
        root->left = insertBST(root->left, data);
    else if (data > root->data)
        root->right = insertBST(root->right, data);
    
    return root;
}

int searchBST(BSTNode* root, int target, int* comparisons) {
    *comparisons = 0;
    BSTNode* current = root;
    
    while (current != NULL) {
        (*comparisons)++;
        if (target == current->data) return 1;
        else if (target < current->data) current = current->left;
        else current = current->right;
    }
    return 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode* node) {
    return (node == NULL) ? 0 : node->height;
}

int getBalance(AVLNode* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    
    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    
    return y;
}

AVLNode* insertAVL(AVLNode* node, int data) {
    if (node == NULL) return createAVLNode(data);
    
    if (data < node->data)
        node->left = insertAVL(node->left, data);
    else if (data > node->data)
        node->right = insertAVL(node->right, data);
    else
        return node;
    
    node->height = 1 + max(height(node->left), height(node->right));
    
    int balance = getBalance(node);
    
    if (balance > 1 && data < node->left->data)
        return rotateRight(node);
    
    if (balance < -1 && data > node->right->data)
        return rotateLeft(node);
    
    if (balance > 1 && data > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && data < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

int searchAVL(AVLNode* root, int target, int* comparisons) {
    *comparisons = 0;
    AVLNode* current = root;
    
    while (current != NULL) {
        (*comparisons)++;
        if (target == current->data) return 1;
        else if (target < current->data) current = current->left;
        else current = current->right;
    }
    return 0;
}

int searchArray(int arr[], int size, int target, int* comparisons) {
    *comparisons = 0;
    for (int i = 0; i < size; i++) {
        (*comparisons)++;
        if (arr[i] == target) return 1;
    }
    return 0;
}

void generateRandomData(int arr[], int size) {
    int used[MAX_VALUE + 1] = {0};
    int count = 0;
    
    while (count < size) {
        int num = rand() % (MAX_VALUE + 1);
        if (!used[num]) {
            arr[count++] = num;
            used[num] = 1;
        }
    }
}

void generateAscendingData(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void generateDescendingData(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - 1 - i;
    }
}

void generateFormulaData(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i * (i % 2 + 2);
    }
}

void freeBST(BSTNode* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

void freeAVL(AVLNode* root) {
    if (root == NULL) return;
    freeAVL(root->left);
    freeAVL(root->right);
    free(root);
}

void runTest(int data[], int size, const char* dataName) {
    int* arr = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) arr[i] = data[i];
    
    BSTNode* bstRoot = NULL;
    AVLNode* avlRoot = NULL;
    
    for (int i = 0; i < size; i++) {
        bstRoot = insertBST(bstRoot, data[i]);
        avlRoot = insertAVL(avlRoot, data[i]);
    }
    
    int searchTargets[SEARCH_COUNT];
    for (int i = 0; i < SEARCH_COUNT; i++) {
        searchTargets[i] = rand() % (MAX_VALUE + 1);
    }
    
    long long totalArrayComp = 0;
    long long totalBSTComp = 0;
    long long totalAVLComp = 0;
    
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int comp;
        searchArray(arr, size, searchTargets[i], &comp);
        totalArrayComp += comp;
        
        searchBST(bstRoot, searchTargets[i], &comp);
        totalBSTComp += comp;
        
        searchAVL(avlRoot, searchTargets[i], &comp);
        totalAVLComp += comp;
    }
    
    printf("Array: 데이터 %s에서 평균 %.2f회 탐색\n", 
           dataName, (double)totalArrayComp / SEARCH_COUNT);
    printf("BST: 데이터 %s에서 평균 %.2f회 탐색\n", 
           dataName, (double)totalBSTComp / SEARCH_COUNT);
    printf("AVL: 데이터 %s에서 평균 %.2f회 탐색\n\n", 
           dataName, (double)totalAVLComp / SEARCH_COUNT);
    
    free(arr);
    freeBST(bstRoot);
    freeAVL(avlRoot);
}

int main() {
    srand(time(NULL));
    
    int data[DATA_SIZE];
    
    generateRandomData(data, DATA_SIZE);
    runTest(data, DATA_SIZE, "(1)");
    
    generateAscendingData(data, DATA_SIZE);
    runTest(data, DATA_SIZE, "(2)");
    
    generateDescendingData(data, DATA_SIZE);
    runTest(data, DATA_SIZE, "(3)");
    
    generateFormulaData(data, DATA_SIZE);
    runTest(data, DATA_SIZE, "(4)");
    
    return 0;
}
