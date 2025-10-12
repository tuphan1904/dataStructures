#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define MAX_VALUE 1000

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

int linearSearchList(Node* head, int target, int* comparisons) {
    *comparisons = 0;
    int position = 0;
    Node* current = head;
    
    while (current != NULL) {
        (*comparisons)++;
        if (current->data == target) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1; 
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int binarySearch(int arr[], int size, int target, int* comparisons) {
    *comparisons = 0;
    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        (*comparisons)++;
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; 
}

void printArray(int arr[], int size) {
    printf("Mảng dữ liệu:\n");
    for (int i = 0; i < size; i++) {
        printf("%4d ", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");
}

void printList(Node* head) {
    printf("Linked List:\n");
    Node* current = head;
    int count = 0;
    while (current != NULL) {
        printf("%4d ", current->data);
        count++;
        if (count % 10 == 0) printf("\n");
        current = current->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int arr[SIZE];
    int sortedArr[SIZE];
    Node* head = NULL;
    
    srand(time(NULL));
    
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % (MAX_VALUE + 1);
        sortedArr[i] = arr[i];
        append(&head, arr[i]);
    }
    
    int target;
    printf("찾을 숫자 입력: ");
    scanf("%d", &target);
    printf("\n");
    
    int linearComp;
    int linearPos = linearSearchList(head, target, &linearComp);
    
    printf("선형 탐색 (Linear Search)\n");
    if (linearPos != -1) {
        printf("결과: 찾음\n");
        printf("위치: %d번째\n", linearPos);
    } else {
        printf("결과: 찾을 수 없음\n");
    }
    printf("비교 횟수: %d회\n\n", linearComp);
    
    qsort(sortedArr, SIZE, sizeof(int), compare);
    int binaryComp;
    int binaryPos = binarySearch(sortedArr, SIZE, target, &binaryComp);
    
    printf("이진 탐색 (Binary Search)\n");
    if (binaryPos != -1) {
        printf("결과: 찾음\n");
        printf("위치: %d번째 (정렬된 배열)\n", binaryPos);
    } else {
        printf("결과: 찾을 수 없음\n");
    }
    printf("비교 횟수: %d회\n", binaryComp);
    
    freeList(head);
    return 0;
}
