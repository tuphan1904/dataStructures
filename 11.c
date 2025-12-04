#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define MAX_CAPACITY 2000

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

typedef struct AVLNode {
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;


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
        arr[i].id = 1000 + i;
        snprintf(arr[i].name, MAX_NAME_LEN, "%s%s", first[rand() % 10], last[rand() % 10]);
        arr[i].gender = (rand() % 2) ? 'M' : 'F';
        arr[i].korean = rand() % 101;
        arr[i].english = rand() % 101;
        arr[i].math = rand() % 101;
    }
    return arr;
}


long long unsorted_search(Student arr[], int n, int id) {
    long long comp = 0;
    for (int i = 0; i < n; i++) {
        comp++;
        if (arr[i].id == id) return comp;
    }
    return comp;
}

long long unsorted_insert(Student arr[], int* n, Student s) {
    arr[(*n)++] = s;
    return 0; 
}

long long unsorted_delete(Student arr[], int* n, int id) {
    long long comp = 0;
    for (int i = 0; i < *n; i++) {
        comp++;
        if (arr[i].id == id) {
            arr[i] = arr[--(*n)]; 
            return comp;
        }
    }
    return comp;
}


long long sorted_search(Student arr[], int n, int id) {
    long long comp = 0;
    int left = 0, right = n - 1;
    
    while (left <= right) {
        comp++;
        int mid = left + (right - left) / 2;
        
        if (arr[mid].id == id) return comp;
        if (arr[mid].id < id) left = mid + 1;
        else right = mid - 1;
    }
    return comp;
}

long long sorted_insert(Student arr[], int* n, Student s) {
    long long comp = 0;
    int pos = 0;
    
    int left = 0, right = *n;
    while (left < right) {
        comp++;
        int mid = left + (right - left) / 2;
        if (arr[mid].id < s.id) left = mid + 1;
        else right = mid;
    }
    pos = left;
    
    for (int i = *n; i > pos; i--) {
        arr[i] = arr[i - 1];
    }
    
    arr[pos] = s;
    (*n)++;
    return comp;
}

long long sorted_delete(Student arr[], int* n, int id) {
    long long comp = sorted_search(arr, *n, id);
    
    int pos = -1;
    int left = 0, right = *n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].id == id) {
            pos = mid;
            break;
        }
        if (arr[mid].id < id) left = mid + 1;
        else right = mid - 1;
    }
    
    if (pos == -1) return comp;
    
    for (int i = pos; i < *n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*n)--;
    return comp;
}


int height(AVLNode* node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* createAVLNode(Student s) {
    AVLNode* node = malloc(sizeof(AVLNode));
    node->data = s;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
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

int getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* avl_insert_helper(AVLNode* node, Student s, long long* comp) {
    if (node == NULL) return createAVLNode(s);
    
    (*comp)++;
    if (s.id < node->data.id)
        node->left = avl_insert_helper(node->left, s, comp);
    else if (s.id > node->data.id)
        node->right = avl_insert_helper(node->right, s, comp);
    else
        return node;
    
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    
    if (balance > 1 && s.id < node->left->data.id)
        return rotateRight(node);
    
    if (balance < -1 && s.id > node->right->data.id)
        return rotateLeft(node);
    
    if (balance > 1 && s.id > node->left->data.id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && s.id < node->right->data.id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

long long avl_search(AVLNode* node, int id) {
    long long comp = 0;
    while (node != NULL) {
        comp++;
        if (id == node->data.id) return comp;
        if (id < node->data.id) node = node->left;
        else node = node->right;
    }
    return comp;
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode* avl_delete_helper(AVLNode* root, int id, long long* comp) {
    if (root == NULL) return root;
    
    (*comp)++;
    if (id < root->data.id)
        root->left = avl_delete_helper(root->left, id, comp);
    else if (id > root->data.id)
        root->right = avl_delete_helper(root->right, id, comp);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = avl_delete_helper(root->right, temp->data.id, comp);
        }
    }
    
    if (root == NULL) return root;
    
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    
    return root;
}

void freeAVL(AVLNode* node) {
    if (node == NULL) return;
    freeAVL(node->left);
    freeAVL(node->right);
    free(node);
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    int student_count = 0;
    Student* students = NULL;
    
    const char* default_file = "students.csv"; 

    if (argc > 1) {
        students = load_students(argv[1], &student_count);
    }
    
    if (!students) {
        student_count = 200;
        students = generate_sample_students(student_count);
    }
    
    printf("초기 데이터: %d명\n\n", student_count);
    
    Student* unsorted = malloc(sizeof(Student) * MAX_CAPACITY);
    Student* sorted = malloc(sizeof(Student) * MAX_CAPACITY);
    AVLNode* avl_root = NULL;
    
    int unsorted_n = student_count;
    int sorted_n = student_count;
    
    memcpy(unsorted, students, sizeof(Student) * student_count);
    memcpy(sorted, students, sizeof(Student) * student_count);
    
    for (int i = 0; i < sorted_n - 1; i++) {
        for (int j = 0; j < sorted_n - i - 1; j++) {
            if (sorted[j].id > sorted[j + 1].id) {
                Student temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < student_count; i++) {
        long long comp = 0;
        avl_root = avl_insert_helper(avl_root, students[i], &comp);
    }
    
    printf("1. 삽입 연산\n");
    Student new_student = {9999, "신규학생", 'M', 90, 85, 88};
    
    long long comp1 = unsorted_insert(unsorted, &unsorted_n, new_student);
    long long comp2 = sorted_insert(sorted, &sorted_n, new_student);
    long long comp3 = 0;
    avl_root = avl_insert_helper(avl_root, new_student, &comp3);
    
    printf("비정렬 배열: %lld회\n", comp1);
    printf("정렬 배열: %lld회\n", comp2);
    printf("AVL Tree: %lld회\n\n", comp3);
    
    printf("2. 검색 연산\n");
    int search_id = students[student_count / 2].id;
    
    comp1 = unsorted_search(unsorted, unsorted_n, search_id);
    comp2 = sorted_search(sorted, sorted_n, search_id);
    comp3 = avl_search(avl_root, search_id);
    
    printf("비정렬 배열: %lld회\n", comp1);
    printf("정렬 배열: %lld회\n", comp2);
    printf("AVL Tree: %lld회\n\n", comp3);
    
    printf("3. 삭제 연산\n");
    int delete_id = students[10].id;
    
    comp1 = unsorted_delete(unsorted, &unsorted_n, delete_id);
    comp2 = sorted_delete(sorted, &sorted_n, delete_id);
    comp3 = 0;
    avl_root = avl_delete_helper(avl_root, delete_id, &comp3);
    
    printf("비정렬 배열: %lld회\n", comp1);
    printf("정렬 배열: %lld회\n", comp2);
    printf("AVL Tree: %lld회\n", comp3);
    
    free(unsorted);
    free(sorted);
    free(students);
    freeAVL(avl_root);
    
    return 0;
}
