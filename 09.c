#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define TEST_COUNT 1000

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

typedef struct {
    long long comparisons;
    size_t memory;
} SortStats;

typedef struct TreeNode {
    Student data;
    struct TreeNode* left;
    struct TreeNode* right;
    int height; 
} TreeNode;


Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    char line[MAX_LINE_LEN];
    int capacity = 10, count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    if (!arr) { fclose(fp); return NULL; }

    fgets(line, sizeof(line), fp); 

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) { free(arr); fclose(fp); return NULL; }
            arr = temp;
        }

        char* token = strtok(line, ",");
        arr[count].id = atoi(token);
        token = strtok(NULL, ",");
        strncpy(arr[count].name, token, MAX_NAME_LEN - 1);
        arr[count].name[MAX_NAME_LEN - 1] = '\0';
        token = strtok(NULL, ",");
        arr[count].gender = token[0];
        token = strtok(NULL, ",");
        arr[count].korean = atoi(token);
        token = strtok(NULL, ",");
        arr[count].english = atoi(token);
        token = strtok(NULL, ",");
        arr[count].math = atoi(token);
        count++;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

Student* generate_sample_students(int count) {
    const char* first[] = {"김", "이", "박", "최", "정", "강", "조", "윤", "장", "임"};
    const char* last[] = {"철수", "영희", "민수", "지은", "현우", "수진", "준호", "서연", "동현", "유나"};
    
    Student* arr = malloc(sizeof(Student) * count);
    for (int i = 0; i < count; i++) {
        arr[i].id = i + 1;
        snprintf(arr[i].name, MAX_NAME_LEN, "%s%s", first[rand() % 10], last[rand() % 10]);
        arr[i].gender = (rand() % 2) ? 'M' : 'F';
        arr[i].korean = rand() % 101;
        arr[i].english = rand() % 101;
        arr[i].math = rand() % 101;
    }
    return arr;
}


int cmp_id_asc(Student* a, Student* b, long long* c) { (*c)++; return a->id - b->id; }
int cmp_id_desc(Student* a, Student* b, long long* c) { (*c)++; return b->id - a->id; }
int cmp_name_asc(Student* a, Student* b, long long* c) { (*c)++; return strcmp(a->name, b->name); }
int cmp_name_desc(Student* a, Student* b, long long* c) { (*c)++; return strcmp(b->name, a->name); }
int cmp_gender_asc(Student* a, Student* b, long long* c) { (*c)++; return a->gender - b->gender; }

int cmp_grade_sum(Student* a, Student* b, long long* c) {
    int sum1 = a->korean + a->english + a->math;
    int sum2 = b->korean + b->english + b->math;
    (*c)++;
    if (sum1 != sum2) return sum2 - sum1;
    (*c)++;
    if (a->korean != b->korean) return b->korean - a->korean;
    (*c)++;
    if (a->english != b->english) return b->english - a->english;
    (*c)++;
    return b->math - a->math;
}

SortStats bubbleSort(Student arr[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (cmp(&arr[j], &arr[j + 1], &s.comparisons) > 0) {
                Student t = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = t;
            }
    return s;
}

SortStats selectionSort(Student arr[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (cmp(&arr[j], &arr[min], &s.comparisons) < 0) min = j;
        if (min != i) { Student t = arr[i]; arr[i] = arr[min]; arr[min] = t; }
    }
    return s;
}

SortStats insertionSort(Student arr[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(&arr[j], &key, &s.comparisons) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        if (j >= 0) s.comparisons++;
        arr[j + 1] = key;
    }
    return s;
}

SortStats shellSort(Student arr[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && cmp(&arr[j - gap], &temp, &s.comparisons) > 0; j -= gap)
                arr[j] = arr[j - gap];
            if (j >= gap) s.comparisons++;
            arr[j] = temp;
        }
    }
    return s;
}

SortStats shellSortOptimized(Student arr[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    int gaps[20], gc = 0, gap = 1;
    while (gap < n) { gaps[gc++] = gap; gap = gap * 3 + 1; }
    
    for (int g = gc - 1; g >= 0; g--) {
        gap = gaps[g];
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && cmp(&arr[j - gap], &temp, &s.comparisons) > 0; j -= gap)
                arr[j] = arr[j - gap];
            if (j >= gap) s.comparisons++;
            arr[j] = temp;
        }
    }
    return s;
}

int partition_q(Student a[], int l, int h, int (*cmp)(Student*, Student*, long long*), long long* c) {
    Student p = a[h];
    int i = l - 1;
    for (int j = l; j < h; j++)
        if (cmp(&a[j], &p, c) <= 0) {
            i++;
            Student t = a[i]; a[i] = a[j]; a[j] = t;
        }
    Student t = a[i + 1]; a[i + 1] = a[h]; a[h] = t;
    return i + 1;
}

void quick_rec(Student a[], int l, int h, int (*cmp)(Student*, Student*, long long*), long long* c) {
    if (l < h) {
        int pi = partition_q(a, l, h, cmp, c);
        quick_rec(a, l, pi - 1, cmp, c);
        quick_rec(a, pi + 1, h, cmp, c);
    }
}

SortStats quickSort(Student a[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    quick_rec(a, 0, n - 1, cmp, &s.comparisons);
    return s;
}

int median3(Student a[], int l, int h, int (*cmp)(Student*, Student*, long long*), long long* c) {
    int m = (l + h) / 2;
    if (cmp(&a[l], &a[m], c) > 0) { Student t = a[l]; a[l] = a[m]; a[m] = t; }
    if (cmp(&a[l], &a[h], c) > 0) { Student t = a[l]; a[l] = a[h]; a[h] = t; }
    if (cmp(&a[m], &a[h], c) > 0) { Student t = a[m]; a[m] = a[h]; a[h] = t; }
    Student t = a[m]; a[m] = a[h - 1]; a[h - 1] = t;
    return h - 1;
}

int partition_opt(Student a[], int l, int h, int (*cmp)(Student*, Student*, long long*), long long* c) {
    int pivot_idx = median3(a, l, h, cmp, c);
    Student p = a[pivot_idx];
    Student t = a[pivot_idx]; a[pivot_idx] = a[h]; a[h] = t;
    
    int i = l - 1;
    for (int j = l; j < h; j++)
        if (cmp(&a[j], &p, c) <= 0) {
            i++;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
    t = a[i + 1]; a[i + 1] = a[h]; a[h] = t;
    return i + 1;
}

void quick_opt_rec(Student a[], int l, int h, int (*cmp)(Student*, Student*, long long*), long long* c) {
    if (l < h) {
        int pi = partition_opt(a, l, h, cmp, c);
        quick_opt_rec(a, l, pi - 1, cmp, c);
        quick_opt_rec(a, pi + 1, h, cmp, c);
    }
}

SortStats quickSortOptimized(Student a[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    if (n > 2) quick_opt_rec(a, 0, n - 1, cmp, &s.comparisons);
    return s;
}

void heapify(Student a[], int n, int i, int (*cmp)(Student*, Student*, long long*), long long* c) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && cmp(&a[l], &a[largest], c) > 0) largest = l;
    if (r < n && cmp(&a[r], &a[largest], c) > 0) largest = r;
    if (largest != i) {
        Student t = a[i]; a[i] = a[largest]; a[largest] = t;
        heapify(a, n, largest, cmp, c);
    }
}

SortStats heapSort(Student a[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    for (int i = n / 2 - 1; i >= 0; i--) heapify(a, n, i, cmp, &s.comparisons);
    for (int i = n - 1; i > 0; i--) {
        Student t = a[0]; a[0] = a[i]; a[i] = t;
        heapify(a, i, 0, cmp, &s.comparisons);
    }
    return s;
}

void merge(Student a[], int l, int m, int r, int (*cmp)(Student*, Student*, long long*), long long* c, size_t* mem) {
    int n1 = m - l + 1, n2 = r - m;
    Student* L = malloc(n1 * sizeof(Student));
    Student* R = malloc(n2 * sizeof(Student));
    *mem += (n1 + n2) * sizeof(Student);
    
    for (int i = 0; i < n1; i++) L[i] = a[l + i];
    for (int j = 0; j < n2; j++) R[j] = a[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (cmp(&L[i], &R[j], c) <= 0) a[k++] = L[i++];
        else a[k++] = R[j++];
    }
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
    
    free(L); free(R);
}

void merge_rec(Student a[], int l, int r, int (*cmp)(Student*, Student*, long long*), long long* c, size_t* mem) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_rec(a, l, m, cmp, c, mem);
        merge_rec(a, m + 1, r, cmp, c, mem);
        merge(a, l, m, r, cmp, c, mem);
    }
}

SortStats mergeSort(Student a[], int n, int (*cmp)(Student*, Student*, long long*)) {
    SortStats s = {0, 0};
    merge_rec(a, 0, n - 1, cmp, &s.comparisons, &s.memory);
    return s;
}

SortStats radixSort(Student a[], int n) {
    SortStats s = {0, 0};
    Student* output = malloc(n * sizeof(Student));
    s.memory += n * sizeof(Student);
    
    int max = a[0].id;
    for (int i = 1; i < n; i++) if (a[i].id > max) max = a[i].id;
    
    for (int exp = 1; max / exp > 0; exp *= 10) {
        int count[10] = {0};
        for (int i = 0; i < n; i++) count[(a[i].id / exp) % 10]++;
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
        for (int i = n - 1; i >= 0; i--) {
            int digit = (a[i].id / exp) % 10;
            output[count[digit] - 1] = a[i];
            count[digit]--;
        }
        for (int i = 0; i < n; i++) a[i] = output[i];
    }
    
    free(output);
    return s;
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║  정렬 알고리즘 성능 비교 (과제 A & B)   ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");
    
    int student_count = 0;
    Student* students = NULL;

    const char* default_file = "students.csv";
    
    if (argc > 1) {
        printf("📁 파일 로드: %s\n", argv[1]);
        students = load_students(argv[1], &student_count);
    }
    
    if (!students) {
        printf("📝 샘플 데이터 생성 (100명)\n");
        student_count = 100;
        students = generate_sample_students(student_count);
    }
    
    printf("✅ 학생 수: %d명\n", student_count);
    printf("🔄 테스트 횟수: %d회\n\n", TEST_COUNT);
    
    printf("=== 과제 A: ID 오름차순 정렬 비교 ===\n\n");
    
    Student* temp = malloc(sizeof(Student) * student_count);
    
    long long total_comp = 0;
    for (int t = 0; t < TEST_COUNT; t++) {
        memcpy(temp, students, sizeof(Student) * student_count);
        SortStats s = bubbleSort(temp, student_count, cmp_id_asc);
        total_comp += s.comparisons;
    }
    printf("Bubble Sort:    평균 %lld회 비교\n", total_comp / TEST_COUNT);
    
    total_comp = 0;
    for (int t = 0; t < TEST_COUNT; t++) {
        memcpy(temp, students, sizeof(Student) * student_count);
        SortStats s = quickSort(temp, student_count, cmp_id_asc);
        total_comp += s.comparisons;
    }
    printf("Quick Sort:     평균 %lld회 비교\n", total_comp / TEST_COUNT);
    
    total_comp = 0;
    for (int t = 0; t < TEST_COUNT; t++) {
        memcpy(temp, students, sizeof(Student) * student_count);
        SortStats s = shellSort(temp, student_count, cmp_id_asc);
        total_comp += s.comparisons;
    }
    printf("Shell (기본):   평균 %lld회 비교\n", total_comp / TEST_COUNT);
    
    printf("\n=== 과제 B: 최적화 비교 ===\n\n");
    
    total_comp = 0;
    for (int t = 0; t < TEST_COUNT; t++) {
        memcpy(temp, students, sizeof(Student) * student_count);
        SortStats s = shellSortOptimized(temp, student_count, cmp_id_asc);
        total_comp += s.comparisons;
    }
    printf("Shell (최적화): 평균 %lld회 비교 ✨\n", total_comp / TEST_COUNT);
    
    total_comp = 0;
    for (int t = 0; t < TEST_COUNT; t++) {
        memcpy(temp, students, sizeof(Student) * student_count);
        SortStats s = quickSortOptimized(temp, student_count, cmp_id_asc);
        total_comp += s.comparisons;
    }
    printf("Quick (최적화): 평균 %lld회 비교 ✨\n\n", total_comp / TEST_COUNT);
    
    printf("💡 전체 결과를 보려면 모든 정렬 & 기준 조합을 테스트하세요!\n");
    
    free(temp);
    free(students);
    
    return 0;
}
