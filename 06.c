#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define VERTICES 100
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    int matrix[VERTICES][VERTICES];
} AdjMatrix;

typedef struct {
    Node* list[VERTICES];
} AdjList;

Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

void initMatrix(AdjMatrix* graph) {
    memset(graph->matrix, 0, sizeof(graph->matrix));
}

void initList(AdjList* graph) {
    for (int i = 0; i < VERTICES; i++) {
        graph->list[i] = NULL;
    }
}

int addEdgeMatrix(AdjMatrix* graph, int src, int dest, int* comparisons) {
    *comparisons = 1; 
    if (graph->matrix[src][dest] == 0) {
        graph->matrix[src][dest] = 1;
        graph->matrix[dest][src] = 1; 
        return 1;
    }
    return 0;
}

int addEdgeList(AdjList* graph, int src, int dest, int* comparisons) {
    *comparisons = 0;
    
    Node* temp = graph->list[src];
    while (temp != NULL) {
        (*comparisons)++;
        if (temp->vertex == dest) return 0; 
        temp = temp->next;
    }
    
    Node* newNode = createNode(dest);
    newNode->next = graph->list[src];
    graph->list[src] = newNode;
    
    newNode = createNode(src);
    newNode->next = graph->list[dest];
    graph->list[dest] = newNode;
    
    return 1;
}

int removeEdgeMatrix(AdjMatrix* graph, int src, int dest, int* comparisons) {
    *comparisons = 1;
    if (graph->matrix[src][dest] == 1) {
        graph->matrix[src][dest] = 0;
        graph->matrix[dest][src] = 0;
        return 1;
    }
    return 0;
}

int removeEdgeList(AdjList* graph, int src, int dest, int* comparisons) {
    *comparisons = 0;
    
    Node* temp = graph->list[src];
    Node* prev = NULL;
    while (temp != NULL) {
        (*comparisons)++;
        if (temp->vertex == dest) {
            if (prev == NULL) {
                graph->list[src] = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    
    temp = graph->list[dest];
    prev = NULL;
    while (temp != NULL) {
        (*comparisons)++;
        if (temp->vertex == src) {
            if (prev == NULL) {
                graph->list[dest] = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    
    return 1;
}

int checkConnectionMatrix(AdjMatrix* graph, int src, int dest, int* comparisons) {
    *comparisons = 1;
    return graph->matrix[src][dest];
}

int checkConnectionList(AdjList* graph, int src, int dest, int* comparisons) {
    *comparisons = 0;
    Node* temp = graph->list[src];
    while (temp != NULL) {
        (*comparisons)++;
        if (temp->vertex == dest) return 1;
        temp = temp->next;
    }
    return 0;
}

void printAdjacentMatrix(AdjMatrix* graph, int vertex, int* comparisons) {
    *comparisons = 0;
    for (int i = 0; i < VERTICES; i++) {
        (*comparisons)++;
        if (graph->matrix[vertex][i] == 1) {
        }
    }
}

void printAdjacentList(AdjList* graph, int vertex, int* comparisons) {
    *comparisons = 0;
    Node* temp = graph->list[vertex];
    while (temp != NULL) {
        (*comparisons)++;
        temp = temp->next;
    }
}

void generateRandomGraph(int edges, AdjMatrix* matrixGraph, AdjList* listGraph) {
    initMatrix(matrixGraph);
    initList(listGraph);
    
    int edgeCount = 0;
    int comp;
    
    while (edgeCount < edges) {
        int src = rand() % VERTICES;
        int dest = rand() % VERTICES;
        
        if (src != dest) {
            if (addEdgeMatrix(matrixGraph, src, dest, &comp)) {
                addEdgeList(listGraph, src, dest, &comp);
                edgeCount++;
            }
        }
    }
}

size_t calculateMatrixMemory(AdjMatrix* graph) {
    return sizeof(AdjMatrix);
}

size_t calculateListMemory(AdjList* graph) {
    size_t memory = sizeof(AdjList);
    for (int i = 0; i < VERTICES; i++) {
        Node* temp = graph->list[i];
        while (temp != NULL) {
            memory += sizeof(Node);
            temp = temp->next;
        }
    }
    return memory;
}

void freeList(AdjList* graph) {
    for (int i = 0; i < VERTICES; i++) {
        Node* temp = graph->list[i];
        while (temp != NULL) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
}

void testPerformance(const char* caseName, int edgeCount) {
    AdjMatrix matrixGraph;
    AdjList listGraph;
    
    printf("케이스 %s:\n", caseName);
    
    generateRandomGraph(edgeCount, &matrixGraph, &listGraph);
    
    size_t matrixMem = calculateMatrixMemory(&matrixGraph);
    size_t listMem = calculateListMemory(&listGraph);
    
    int matrixInsertComp = 0, listInsertComp = 0;
    int matrixDeleteComp = 0, listDeleteComp = 0;
    int comp;
    
    for (int i = 0; i < 10; i++) {
        int src = rand() % VERTICES;
        int dest = rand() % VERTICES;
        if (src != dest) {
            addEdgeMatrix(&matrixGraph, src, dest, &comp);
            matrixInsertComp += comp;
            addEdgeList(&listGraph, src, dest, &comp);
            listInsertComp += comp;
        }
    }
    
    for (int i = 0; i < 10; i++) {
        int src = rand() % VERTICES;
        int dest = rand() % VERTICES;
        if (src != dest) {
            removeEdgeMatrix(&matrixGraph, src, dest, &comp);
            matrixDeleteComp += comp;
            removeEdgeList(&listGraph, src, dest, &comp);
            listDeleteComp += comp;
        }
    }
    
    int matrixCheckComp = 0, listCheckComp = 0;
    for (int i = 0; i < 10; i++) {
        int src = rand() % VERTICES;
        int dest = rand() % VERTICES;
        checkConnectionMatrix(&matrixGraph, src, dest, &comp);
        matrixCheckComp += comp;
        checkConnectionList(&listGraph, src, dest, &comp);
        listCheckComp += comp;
    }
    
    int matrixPrintComp = 0, listPrintComp = 0;
    for (int i = 0; i < 10; i++) {
        int vertex = rand() % VERTICES;
        printAdjacentMatrix(&matrixGraph, vertex, &comp);
        matrixPrintComp += comp;
        printAdjacentList(&listGraph, vertex, &comp);
        listPrintComp += comp;
    }
    
    printf("\n[인접 행렬]\n");
    printf("메모리: %zu Bytes\n", matrixMem);
    printf("간선 삽입 비교: %d번\n", matrixInsertComp);
    printf("간선 삭제 비교: %d번\n", matrixDeleteComp);
    printf("두 정점 연결 확인 비교: %d번\n", matrixCheckComp);
    printf("인접 노드 출력 비교: %d번\n", matrixPrintComp);
    
    printf("\n[인접 리스트]\n");
    printf("메모리: %zu Bytes\n", listMem);
    printf("간선 삽입 비교: %d번\n", listInsertComp);
    printf("간선 삭제 비교: %d번\n", listDeleteComp);
    printf("두 정점 연결 확인 비교: %d번\n", listCheckComp);
    printf("인접 노드 출력 비교: %d번\n\n", listPrintComp);
    
    freeList(&listGraph);
}

int main() {
    srand(time(NULL));
    
    printf("=== 그래프 성능 비교 ===\n\n");
    
    testPerformance("1 (희소그래프)", SPARSE_EDGES);
    
    testPerformance("2 (밀집그래프)", DENSE_EDGES);
    
    return 0;
}
