#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DATA_SIZE 10000
#define MAX_VALUE 1000000
#define TEST_COUNT 100

long long insertionSort(int arr[], int n) {
    long long comparisons = 0;
    
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && ++comparisons && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    
    return comparisons;
}

long long shellSortBasic(int arr[], int n) {
    long long comparisons = 0;
    
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap && ++comparisons && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            
            if (j >= gap) comparisons++; 
            
            arr[j] = temp;
        }
    }
    
    return comparisons;
}

long long shellSortKnuth(int arr[], int n) {
    long long comparisons = 0;
    
    int gaps[20];
    int gapCount = 0;
    
    int gap = 1;
    while (gap < n) {
        gaps[gapCount++] = gap;
        gap = gap * 3 + 1;
    }
    
    for (int g = gapCount - 1; g >= 0; g--) {
        gap = gaps[g];
        
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap && ++comparisons && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            
            if (j >= gap) comparisons++;
            
            arr[j] = temp;
        }
    }
    
    return comparisons;
}

void generateRandomData(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (MAX_VALUE + 1);
    }
}

int isSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

int main() {
    srand(time(NULL));
    
    int *originalData = (int*)malloc(DATA_SIZE * sizeof(int));
    int *tempData = (int*)malloc(DATA_SIZE * sizeof(int));
    
    long long totalInsertionComp = 0;
    long long totalShellBasicComp = 0;
    long long totalShellKnuthComp = 0;
    

    
    for (int test = 0; test < TEST_COUNT; test++) {
        generateRandomData(originalData, DATA_SIZE);
        
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        long long comp = insertionSort(tempData, DATA_SIZE);
        totalInsertionComp += comp;
        if (!isSorted(tempData, DATA_SIZE)) {
            printf("\n오류: 삽입 정렬 실패!\n");
            return 1;
        }
        
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        comp = shellSortBasic(tempData, DATA_SIZE);
        totalShellBasicComp += comp;
        if (!isSorted(tempData, DATA_SIZE)) {
            printf("\n오류: 쉘 정렬(기본) 실패!\n");
            return 1;
        }
        
        memcpy(tempData, originalData, DATA_SIZE * sizeof(int));
        comp = shellSortKnuth(tempData, DATA_SIZE);
        totalShellKnuthComp += comp;
        if (!isSorted(tempData, DATA_SIZE)) {
            printf("\n오류: 쉘 정렬(Knuth) 실패!\n");
            return 1;
        }
        
    
    }
    
    double avgInsertion = (double)totalInsertionComp / TEST_COUNT;
    double avgShellBasic = (double)totalShellBasicComp / TEST_COUNT;
    double avgShellKnuth = (double)totalShellKnuthComp / TEST_COUNT;
    
    printf("=== 결과 (평균 비교 횟수) ===\n\n");
    
    printf("1. 삽입 정렬 (Insertion Sort)\n");
    printf("   평균 비교 횟수: %.0f회\n", avgInsertion);
    printf("   시간 복잡도: O(n²)\n\n");
    
    printf("2. 쉘 정렬 - 기본 간격 (n/2, n/4, n/8...)\n");
    printf("   평균 비교 횟수: %.0f회\n", avgShellBasic);
    printf("   개선율: %.1f%%\n", (1.0 - avgShellBasic / avgInsertion) * 100);
    printf("   배수: 삽입 정렬의 %.1f배 빠름\n\n", avgInsertion / avgShellBasic);
    
    printf("3. 쉘 정렬 - 최적 간격 (Knuth's sequence)\n");
    printf("   간격 수열: 1, 4, 13, 40, 121, 364, 1093, 3280...\n");
    printf("   평균 비교 횟수: %.0f회\n", avgShellKnuth);
    printf("   개선율: %.1f%%\n", (1.0 - avgShellKnuth / avgInsertion) * 100);
    printf("   배수: 삽입 정렬의 %.1f배 빠름\n\n", avgInsertion / avgShellKnuth);
    
    printf("=== 쉘 정렬 간격 비교 ===\n");
    printf("기본 간격 대비 Knuth 개선: %.1f%%\n", 
           (1.0 - avgShellKnuth / avgShellBasic) * 100);
    printf("Knuth가 기본보다 %.1f배 빠름\n\n", avgShellBasic / avgShellKnuth);
    
    free(originalData);
    free(tempData);
    
    return 0;
}
