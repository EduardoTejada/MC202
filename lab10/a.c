#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int dado;
    struct Node *esquerda;
    struct Node *direita;
} Node;

int minimo(int* lista, int tamanho) {
    int menor = lista[0];
    int menor_pos = 0;
    for(int i = 1; i < tamanho; i++) { // Começar de 1 para comparar corretamente
        if(lista[i] < menor) {
            menor = lista[i];
            menor_pos = i;
        }
    }
    return menor_pos;
}

void enqueue(Node** fila, Node* item, int* primeiro, int* tamanho, int n) {
    int pos = (*primeiro + *tamanho) % n;
    fila[pos] = item;
    (*tamanho)++;
}

Node* dequeue(Node** fila, int* primeiro, int* tamanho, int n) {
    Node* rem = fila[*primeiro];
    *primeiro = (*primeiro) < n-1 ? (*primeiro)+1 : 0;
    (*tamanho)--;
    return rem;
}

void largura(Node* raiz, int n) {
    Node* Q[n];
    int primeiro = 0, tamanho = 0, nivel = 0, parcial = 1; // Inicializar parcial como 1
    enqueue(Q, raiz, &primeiro, &tamanho, n);
    while(tamanho != 0) {
        Node* p = dequeue(Q, &primeiro, &tamanho, n);
        printf("%d", p->dado);
        if(tamanho == 0 || parcial == 0)
            printf("\n");
        else
            printf(" ");
        parcial = 0;
        if(p->esquerda != NULL) {
            enqueue(Q, p->esquerda, &primeiro, &tamanho, n);
            parcial++;
        }
        if(p->direita != NULL) {
            enqueue(Q, p->direita, &primeiro, &tamanho, n);
            parcial++;
        }
    }
}

// Ajustes na função cartesianar para lidar corretamente com sublistas e índices
void cartesianar(int* lista, int primeiro, int ultimo, Node* no) {
    if(ultimo - primeiro == 0) {
        no->dado = lista[primeiro];
        no->esquerda = NULL;
        no->direita = NULL;
        return;
    }

    int min = primeiro + minimo(lista + primeiro, ultimo - primeiro + 1);
    no->dado = lista[min];

    if(min > primeiro) {
        int lista_esquerda[min - primeiro];
        for(int i = 0; i < min - primeiro; i++) {
            lista_esquerda[i] = lista[primeiro + i];
        }
        no->esquerda = (Node*) malloc(sizeof(Node));
        cartesianar(lista_esquerda, 0, min - primeiro - 1, no->esquerda);
    } else {
        no->esquerda = NULL;
    }

    if(min < ultimo) {
        int lista_direita[ultimo - min];
        for(int i = 0; i < ultimo - min; i++) {
            lista_direita[i] = lista[min + 1 + i];
        }
        no->direita = (Node*) malloc(sizeof(Node));
        cartesianar(lista_direita, 0, ultimo - min - 1, no->direita);
    } else {
        no->direita = NULL;
    }
}

// Remoção do ponto e vírgula após if na função desmallocar
void desmallocar(Node* no) {
    if(no == NULL) return;

    desmallocar(no->esquerda);
    desmallocar(no->direita);

    free(no);
}

// Correção no loop de leitura de elementos no main
int main(void) {
    int n;
    scanf("%d", &n);
    if(n == 0) return 0;
    int lista[n];
    for(int i = 0; i < n; i++) { // Correção na inicialização do índice i
        scanf("%d", &lista[i]);
    }
    Node* raiz = (Node*) malloc(sizeof(Node));

    cartesianar(lista, 0, n - 1, raiz);

    largura(raiz, n);

    desmallocar(raiz);
    return 0;
}
