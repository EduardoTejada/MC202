#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 100005

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} AdjList;

typedef struct {
    int vertex;
    int dist;
} QueueNode;

typedef struct {
    QueueNode *data;
    int front, rear, size, capacity;
} Queue;

Queue* createQueue(int capacity) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->data = (QueueNode*) malloc(queue->capacity * sizeof(QueueNode));
    return queue;
}

bool isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

bool isEmpty(Queue* queue) {
    return (queue->size == 0);
}

void enqueue(Queue* queue, int vertex, int dist) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->data[queue->rear].vertex = vertex;
    queue->data[queue->rear].dist = dist;
    queue->size = queue->size + 1;
}

QueueNode dequeue(Queue* queue) {
    QueueNode item = { -1, -1 };
    if (isEmpty(queue)) return item;
    item = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

bool is_prime(int num) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

void addEdge(AdjList* graph, int u, int v) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = graph[u].head;
    graph[u].head = newNode;

    newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = u;
    newNode->next = graph[v].head;
    graph[v].head = newNode;
}

int bfs(AdjList* graph, int n, int start, int end) {
    bool visited[MAXN] = { false };
    Queue* queue = createQueue(MAXN);
    enqueue(queue, start, 0);
    visited[start] = true;

    while (!isEmpty(queue)) {
        QueueNode current = dequeue(queue);
        int node = current.vertex;
        int dist = current.dist;

        if (node == end) {
            free(queue->data);
            free(queue);
            return dist;
        }

        Node* temp = graph[node].head;
        while (temp) {
            int neighbor = temp->vertex;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                enqueue(queue, neighbor, dist + 1);
            }
            temp = temp->next;
        }
    }

    free(queue->data);
    free(queue);
    return -1;
}

int main() {
    int N, M, S, T;
    scanf("%d %d %d %d", &N, &M, &S, &T);

    AdjList* graph = (AdjList*) malloc((N + 1) * sizeof(AdjList));
    for (int i = 1; i <= N; ++i) {
        graph[i].head = NULL;
    }

    for (int i = 0; i < M; ++i) {
        int ai, bi;
        scanf("%d %d", &ai, &bi);
        addEdge(graph, ai, bi);
    }

    int min_edges = bfs(graph, N, S, T);

    if (min_edges == -1) {
        printf("-1\n");
    } else {
        while (!is_prime(min_edges)) {
            min_edges++;
        }
        printf("%d\n", min_edges);
    }

    // Cleanup dynamically allocated memory
    for (int i = 1; i <= N; ++i) {
        Node* temp = graph[i].head;
        while (temp) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph);

    return 0;
}
