#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int chave;
    int custo;
} Node;


typedef struct {
    Node* hp;
    int* indices;
    int tamanho;
    int capacidade;
} Heap;


Heap* criarHeap(int tamanho){
    Heap* p = (Heap*) malloc(sizeof(Heap));
    p->hp = (Node*) malloc(tamanho * sizeof(Node));
    p->indices = (int*) malloc(tamanho * sizeof(int));
    for(int i = 0; i < tamanho; i++)
        p->indices[i] = -1;
    p->tamanho = 0;
    p->capacidade = tamanho;
    return p;
}

void freeHeap(Heap* minHeap) {
    free(minHeap->hp);
    free(minHeap->indices);
    free(minHeap);
}

void swap_valores(Node* a, Node* b){
    Node aux = *a;
    *a = *b;
    *b = aux;
}

void heapifyUp(Heap* heap, int filho){
    int pai = (filho-1)/2;
    while(filho > 0 && heap->hp[pai].custo > heap->hp[filho].custo){
        heap->indices[heap->hp[filho].chave] = pai;
        heap->indices[heap->hp[pai].chave] = filho;

        swap_valores(&heap->hp[filho], &heap->hp[pai]);

        filho = pai;
        pai = (filho-1)/2;
    }
}

void heapifyDown(Heap* heap, int pai){
    int menor = pai;
    int filho_esquerda = 2 * pai + 1;
    int filho_direita = 2 * pai + 2;

    if (filho_esquerda < heap->tamanho && heap->hp[filho_esquerda].custo < heap->hp[menor].custo) {
        menor = filho_esquerda;
    }

    if (filho_direita < heap->tamanho && heap->hp[filho_direita].custo < heap->hp[menor].custo) {
        menor = filho_direita;
    }

    if (menor != pai) {
        // Atualiza o índice das chaves no vetor de índice
        heap->indices[heap->hp[pai].chave] = menor;
        heap->indices[heap->hp[menor].chave] = pai;
        swap_valores(&heap->hp[pai], &heap->hp[menor]);
        heapifyDown(heap, menor);
    }
}

void inserirHeap(Heap* heap, int chave, int custo){
    if(heap->indices[chave] != -1 || heap->tamanho >= heap->capacidade)
        return;

    heap->tamanho++;
    int ultimo = heap->tamanho-1;
    heap->hp[ultimo].chave = chave;
    heap->hp[ultimo].custo = custo;
    heap->indices[chave] = ultimo;
    heapifyUp(heap, ultimo);
}

void removerMin(Heap* heap){
    if(heap->tamanho == 0){
        printf("heap vazio\n");
        return;
    }
    printf("minimo {%d,%d}\n", heap->hp->chave, heap->hp->custo);
    swap_valores(&heap->hp[0], &heap->hp[heap->tamanho - 1]);
    heap->indices[heap->hp[0].chave] = 0;
    heap->indices[heap->hp[heap->tamanho - 1].chave] = -1;
    heap->tamanho--;
    heapifyDown(heap, 0);
}

void diminuir(Heap* heap, int chave, int novo_custo){
    int pos = heap->indices[chave];
    if (pos == -1 || heap->hp[pos].custo <= novo_custo){
        return;
    }

    heap->hp[pos].custo = novo_custo;
    heapifyUp(heap, pos);
}


int main(void){
    Heap* heap = NULL;
    char cmd[15];
    int tamanho, chave, custo;

    while(1){
        scanf("%s", cmd);
        
        if (strcmp(cmd, "c") == 0) {
            scanf("%d", &tamanho);
            if(heap != NULL)
                freeHeap(heap);
            heap = criarHeap(tamanho);
        }
        else if(strcmp(cmd, "i") == 0){
            scanf("%d %d", &chave, &custo);
            inserirHeap(heap, chave, custo);
        }
        else if(strcmp(cmd, "m") == 0) {
            removerMin(heap);
        }
        else if(strcmp(cmd, "d") == 0){
            scanf("%d %d", &chave, &custo);
            diminuir(heap, chave, custo);
        }
        else if(strcmp(cmd, "t") == 0) {
            if(heap != NULL) freeHeap(heap);
            break;
        }
    }
    return 0;
}