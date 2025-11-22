#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    long long product;
} Student;

Student* generate_sample_students(int count) {
    const char* firstNames[] = {"김", "이", "박", "최", "정", "강", "조", "윤", "장", "임"};
    const char* lastNames[] = {"철수", "영희", "민수", "지은", "현우", "수진", "준호", "서연", "동현", "유나"};
    
    Student* arr = malloc(sizeof(Student) * count);
    if (!arr) {
        perror("메모리 할당 실패");
        return NULL;
    }
    
    for (int i = 0; i < count; i++) {
        arr[i].id = i + 1;
        snprintf(arr[i].name, MAX_NAME_LEN, "%s%s", 
                 firstNames[rand() % 10], lastNames[rand() % 10]);
        arr[i].gender = (rand() % 2) ? 'M' : 'F';
        arr[i].korean = rand() % 101;
        arr[i].english = rand() % 101;
        arr[i].math = rand() % 101;
        arr[i].product = (long long)arr[i].korean * arr[i].english * arr[i].math;
    }
    
    return arr;
}
Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);

    if (!arr) {
        perror("메모리 할당 실패");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) {
                perror("재할당 실패");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = temp;
        }

        Student s;
        char* token = strtok(line, ",");
        s.id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(s.name, token, MAX_NAME_LEN - 1);
        s.name[MAX_NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        s.gender = token[0];

        token = strtok(NULL, ",");
        s.korean = atoi(token);

        token = strtok(NULL, ",");
        s.english = atoi(token);

        token = strtok(NULL, ",");
        s.math = atoi(token);

        s.product = (long long)s.korean * s.english * s.math;

        arr[count++] = s;
    }

    fclose(fp);

    Student* tight = realloc(arr, sizeof(Student) * count);
    if (tight) arr = tight;

    *out_count = count;
    return arr;
}

int sequentialSearch(Student students[], int n, long long target, long long* comparisons) {
    *comparisons = 0;
    
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (students[i].product == target) {
            return i;
        }
    }
    
    return -1;
}

long long partition(Student arr[], int low, int high, long long* comparisons) {
    long long pivot = arr[high].product;
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        (*comparisons)++;
        if (arr[j].product <= pivot) {
            i++;
            Student temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    Student temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void quickSortWithCount(Student arr[], int low, int high, long long* comparisons) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons);
        quickSortWithCount(arr, low, pi - 1, comparisons);
        quickSortWithCount(arr, pi + 1, high, comparisons);
    }
}

int binarySearch(Student students[], int n, long long target, long long* comparisons) {
    *comparisons = 0;
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        (*comparisons)++;
        int mid = left + (right - left) / 2;
        
        if (students[mid].product == target) {
            return mid;
        }
        
        if (students[mid].product < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    printf("=== 학생 데이터 탐색 비교 ===\n\n");
    
    int student_count = 0;
    Student* students = NULL;
    
    if (argc > 1) {
        printf("파일에서 데이터 로드 중: %s\n", argv[1]);
        students = load_students(argv[1], &student_count);
    }
    
    if (!students) {
        printf("파일이 없어 샘플 데이터 생성 중...\n");
        student_count = 1000;
        students = generate_sample_students(student_count);
    }
    
    if (!students) {
        fprintf(stderr, "오류: 학생 데이터를 생성할 수 없습니다.\n");
        return 1;
    }
    
    printf("[1단계] 데이터 준비 완료\n");
    printf("학생 수: %d명\n\n", student_count);
    
    printf("샘플 데이터 (처음 5명):\n");
    printf("%-5s %-20s %-7s %-5s %-5s %-5s %-10s\n", 
           "ID", "이름", "성별", "국어", "영어", "수학", "곱");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < (student_count < 5 ? student_count : 5); i++) {
        printf("%-5d %-20s %-7c %-5d %-5d %-5d %-10lld\n",
               students[i].id, students[i].name, students[i].gender,
               students[i].korean, students[i].english, students[i].math,
               students[i].product);
    }
    printf("\n");
    
    long long target = rand() % 1000001;
    printf("[2단계] 탐색할 값: %lld\n\n", target);
    
    printf("[3단계] 순차 탐색 수행\n");
    long long seqComparisons;
    int seqResult = sequentialSearch(students, student_count, target, &seqComparisons);
    
    if (seqResult != -1) {
        printf("결과: 찾음!\n");
        printf("  학생 ID: %d\n", students[seqResult].id);
        printf("  이름: %s\n", students[seqResult].name);
        printf("  성적곱: %lld\n", students[seqResult].product);
    } else {
        printf("결과: 찾지 못함\n");
    }
    printf("비교 횟수: %lld회\n\n", seqComparisons);
    
    printf("[4단계] 정렬 후 이진 탐색 수행\n");
    Student* sortedStudents = malloc(sizeof(Student) * student_count);
    memcpy(sortedStudents, students, sizeof(Student) * student_count);
    
    long long sortComparisons = 0;
    quickSortWithCount(sortedStudents, 0, student_count - 1, &sortComparisons);
    printf("정렬 완료 (Quick Sort)\n");
    printf("정렬 비교 횟수: %lld회\n", sortComparisons);
    
    long long binComparisons;
    int binResult = binarySearch(sortedStudents, student_count, target, &binComparisons);
    
    if (binResult != -1) {
        printf("결과: 찾음!\n");
        printf("  학생 ID: %d\n", sortedStudents[binResult].id);
        printf("  이름: %s\n", sortedStudents[binResult].name);
        printf("  성적곱: %lld\n", sortedStudents[binResult].product);
    } else {
        printf("결과: 찾지 못함\n");
    }
    printf("이진 탐색 비교 횟수: %lld회\n", binComparisons);
    printf("정렬 + 탐색 총 비교: %lld회\n\n", sortComparisons + binComparisons);
    
    printf("=== 결과 비교 ===\n");
    printf("┌─────────────────────────┬──────────────┐\n");
    printf("│ 방법                    │ 비교 횟수    │\n");
    printf("├─────────────────────────┼──────────────┤\n");
    printf("│ 순차 탐색               │ %12lld │\n", seqComparisons);
    printf("│ 정렬 + 이진 탐색        │ %12lld │\n", sortComparisons + binComparisons);
    printf("│  - 정렬 (Quick Sort)    │ %12lld │\n", sortComparisons);
    printf("│  - 이진 탐색            │ %12lld │\n", binComparisons);
    printf("└─────────────────────────┴──────────────┘\n\n");
    
    if (seqComparisons < sortComparisons + binComparisons) {
        printf("💡 이 경우 순차 탐색이 %.1f배 빠름\n", 
               (double)(sortComparisons + binComparisons) / seqComparisons);
        printf("   (1회 탐색이므로 정렬 비용 > 탐색 이득)\n\n");
    } else {
        printf("💡 정렬 + 이진 탐색이 %.1f배 빠름\n", 
               (double)seqComparisons / (sortComparisons + binComparisons));
    }
    
    printf("참고: 여러 번 탐색 시 정렬은 1회만 하므로\n");
    printf("      이진 탐색이 훨씬 유리합니다.\n");
    
    free(students);
    free(sortedStudents);
    
    return 0;
}
