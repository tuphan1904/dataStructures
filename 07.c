#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define VERTICES 10
#define EDGES 20
#define MAX_WEIGHT 20
#define INF 9999

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

void printMatrix(int matrix[VERTICES][VERTICES], const char* name) {
    printf("\n%s:\n", name);
    printf("    ");
    for (int i = 0; i < VERTICES; i++) {
        printf("%4d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < VERTICES; i++) {
        printf("%2d: ", i);
        for (int j = 0; j < VERTICES; j++) {
            if (matrix[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("%4d ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

void generateRandomGraph(int graph[VERTICES][VERTICES]) {
    // Khởi tạo
    for (int i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
    
    int edgeCount = 0;
    printf("생성된 간선들:\n");
    
    while (edgeCount < EDGES) {
        int src = rand() % VERTICES;
        int dest = rand() % VERTICES;
        
        if (src != dest && graph[src][dest] == INF) {
            int weight = (rand() % MAX_WEIGHT) + 1; // 1 ~ MAX_WEIGHT
            graph[src][dest] = weight;
            graph[dest][src] = weight; // Undirected graph
            printf("간선 %d: %d ↔ %d (가중치: %d)\n", edgeCount + 1, src, dest, weight);
            edgeCount++;
        }
    }
}

void printPath(int start, int end, int next[VERTICES][VERTICES]) {
    if (next[start][end] == -1) {
        printf("경로 없음");
        return;
    }
    
    printf("%d", start);
    while (start != end) {
        start = next[start][end];
        printf(" → %d", start);
    }
}

void floydWarshall(int graph[VERTICES][VERTICES]) {
    int dist[VERTICES][VERTICES];
    int next[VERTICES][VERTICES];
    
    for (int i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            dist[i][j] = graph[i][j];
            if (graph[i][j] != INF && i != j) {
                next[i][j] = j;
            } else {
                next[i][j] = -1;
            }
        }
    }
    
    for (int k = 0; k < VERTICES; k++) {
        for (int i = 0; i < VERTICES; i++) {
            for (int j = 0; j < VERTICES; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }
    
    printf("\n=== 모든 정점 쌍의 최단 경로 (총 45쌍) ===\n\n");
    
    int pairCount = 0;
    for (int i = 0; i < VERTICES; i++) {
        for (int j = i + 1; j < VERTICES; j++) {
            pairCount++;
            printf("[%2d] %d → %d: ", pairCount, i, j);
            
            if (dist[i][j] == INF) {
                printf("연결 안됨\n");
            } else {
                printf("거리 %2d, 경로: ", dist[i][j]);
                printPath(i, j, next);
                printf("\n");
            }
        }
    }
    
    printf("\n=== 통계 요약 ===\n");
    int connectedPairs = 0;
    int totalDistance = 0;
    int maxDistance = 0;
    int minDistance = INF;
    
    for (int i = 0; i < VERTICES; i++) {
        for (int j = i + 1; j < VERTICES; j++) {
            if (dist[i][j] != INF) {
                connectedPairs++;
                totalDistance += dist[i][j];
                if (dist[i][j] > maxDistance) maxDistance = dist[i][j];
                if (dist[i][j] < minDistance) minDistance = dist[i][j];
            }
        }
    }
    
    printf("총 쌍 수: 45개\n");
    printf("연결된 쌍: %d개\n", connectedPairs);
    printf("연결 안된 쌍: %d개\n", 45 - connectedPairs);
    if (connectedPairs > 0) {
        printf("평균 거리: %.1f\n", (double)totalDistance / connectedPairs);
        printf("최단 거리: %d\n", minDistance);
        printf("최장 거리: %d\n", maxDistance);
    }
}

int main() {
    srand(time(NULL));
    
    int graph[VERTICES][VERTICES];
    
    printf("=== 그래프: 정점 %d개, 간선 %d개 ===\n\n", VERTICES, EDGES);
    
    generateRandomGraph(graph);
    floydWarshall(graph);
    
    return 0;
}
