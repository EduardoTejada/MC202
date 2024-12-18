#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int key;
    int cost;
} HeapNode;

typedef struct {
    HeapNode* heap;
    int* index;
    int size;
    int capacity;
} MinHeap;

// Função para criar um heap de mínimo com capacidade n
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->heap = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    minHeap->index = (int*)malloc(capacity * sizeof(int));
    for (int i = 0; i < capacity; i++) {
        minHeap->index[i] = -1;
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

// Função para trocar dois nós no heap
void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Função para fazer heapify-up
void heapifyUp(MinHeap* minHeap, int idx) {
    while (idx > 0 && minHeap->heap[(idx - 1) / 2].cost > minHeap->heap[idx].cost) {
        minHeap->index[minHeap->heap[idx].key] = (idx - 1) / 2;
        minHeap->index[minHeap->heap[(idx - 1) / 2].key] = idx;
        swap(&minHeap->heap[idx], &minHeap->heap[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

// Função para fazer heapify-down
void heapifyDown(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->heap[left].cost < minHeap->heap[smallest].cost) {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->heap[right].cost < minHeap->heap[smallest].cost) {
        smallest = right;
    }

    if (smallest != idx) {
        // Atualiza o índice das chaves no vetor de índice
        minHeap->index[minHeap->heap[idx].key] = smallest;
        minHeap->index[minHeap->heap[smallest].key] = idx;
        swap(&minHeap->heap[idx], &minHeap->heap[smallest]);
        heapifyDown(minHeap, smallest);
    }
}

// Função para inserir um novo par (chave, custo) no heap
void insert(MinHeap* minHeap, int key, int cost) {
    if (minHeap->index[key] != -1) {
        return;
    }

    if (minHeap->size >= minHeap->capacity) {
        return;
    }

    minHeap->size++;
    int idx = minHeap->size - 1;
    minHeap->heap[idx].key = key;
    minHeap->heap[idx].cost = cost;
    minHeap->index[key] = idx;
    heapifyUp(minHeap, idx);
}

// Função para remover e retornar o mínimo do heap
HeapNode removeMin(MinHeap* minHeap) {
    if (minHeap->size == 0) {
        HeapNode emptyNode = { -1, -1 };
        printf("heap vazio\n");
        return emptyNode;
    }

    HeapNode root = minHeap->heap[0];
    minHeap->heap[0] = minHeap->heap[minHeap->size - 1];
    minHeap->index[minHeap->heap[0].key] = 0;
    minHeap->index[root.key] = -1;
    minHeap->size--;
    heapifyDown(minHeap, 0);

    printf("minimo {%d,%d}\n", root.key, root.cost);
    return root;root.key
}

// Função para diminuir o custo de uma chave
void decreaseKey(MinHeap* minHeap, int key, int newCost) {
    int idx = minHeap->index[key];
    if (idx == -1 || minHeap->heap[idx].cost <= newCost) {
        return;
    }

    minHeap->heap[idx].cost = newCost;
    heapifyUp(minHeap, idx);
}

// Função para liberar a memória alocada
void freeMinHeap(MinHeap* minHeap) {
    free(minHeap->heap);
    free(minHeap->index);
    free(minHeap);
}

// Função principal para interpretar os comandos
int main() {
    MinHeap* heap = NULL;
    char command;
    int n, key, cost;

    while (scanf(" %c", &command) != EOF) {
        switch (command) {
            case 'c':
                scanf("%d", &n);
                if (heap != NULL) {
                    freeMinHeap(heap);
                }
                heap = createMinHeap(n);
                break;
            case 'i':
                scanf("%d %d", &key, &cost);
                insert(heap, key, cost);
                break;
            case 'm':
                removeMin(heap);
                break;
            case 'd':
                scanf("%d %d", &key, &cost);
                decreaseKey(heap, key, cost);
                break;
            case 't':
                freeMinHeap(heap);
                return 0;
        }
    }

    return 0;
}
