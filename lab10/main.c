#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int dado;
    struct Node *esquerda;
    struct Node *direita;
} Node;

int minimo(int *lista, int inicio, int fim)
{
    int menor = lista[inicio];
    int menor_pos = inicio;
    for (int i = inicio + 1; i <= fim; i++) {
	if (lista[i] < menor) {
	    menor = lista[i];
	    menor_pos = i;
	}
    }
    return menor_pos;
}

void enqueue(Node ** fila, Node * item, int *primeiro, int *tamanho, int n)
{
    int pos = (*primeiro + *tamanho) % n;
    fila[pos] = item;
    (*tamanho)++;
}

Node *dequeue(Node ** fila, int *primeiro, int *tamanho, int n)
{
    Node *rem = fila[*primeiro];
    *primeiro = (*primeiro) < n - 1 ? (*primeiro) + 1 : 0;
    (*tamanho)--;

    return rem;
}

void largura(Node * raiz, int n)
{
    Node *Q[n];
    int primeiro = 0, tamanho = 0;
    enqueue(Q, raiz, &primeiro, &tamanho, n);
    while (tamanho != 0) {
	int nivel_count = tamanho;
	while (nivel_count > 0) {
	    Node *p = dequeue(Q, &primeiro, &tamanho, n);
	    printf("%d ", p->dado);
	    if (p->esquerda != NULL) {
		enqueue(Q, p->esquerda, &primeiro, &tamanho, n);
	    }
	    if (p->direita != NULL) {
		enqueue(Q, p->direita, &primeiro, &tamanho, n);
	    }
	    nivel_count--;
	}
	printf("\n");
    }
}


void cartesianar(int *lista, int primeiro, int ultimo, Node * no)
{
    // Caso base - somente um elemento na lista
    if (ultimo - primeiro == 0) {
	no->dado = primeiro;
	no->esquerda = NULL;
	no->direita = NULL;
	return;
    }
    // Salvando a posição do minimo no dado do nó
    int min = minimo(lista, primeiro, ultimo);
    no->dado = min;
    // Copiando a lista à esquerda do minimo
    if (min > primeiro) {
	// Criando o nó à esquerda
	no->esquerda = (Node *) malloc(sizeof(Node));
	// Chamando a função para calcular a árvore à esquerda do mínimo
	cartesianar(lista, primeiro, min - 1, no->esquerda);
    } else {
	no->esquerda = NULL;
    }

    // Copiando a lista à direita do minimo
    if (min < ultimo) {
	// Criando o nó à direita
	no->direita = (Node *) malloc(sizeof(Node));
	// Chamando a função para calcular a árvore à esquerda do mínimo
	cartesianar(lista, min + 1, ultimo, no->direita);
    } else {
	no->direita = NULL;
    }
    return;
}


void desmallocar(Node * no)
{
    if (no == NULL)
	return;

    desmallocar(no->esquerda);
    desmallocar(no->direita);

    free(no);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    while (n != 0) {
	int lista[n];
	for (int i = 0; i < n; i++) {
	    scanf("%d", &lista[i]);
	}
	Node *raiz = (Node *) malloc(sizeof(Node));
	cartesianar(lista, 0, n - 1, raiz);
	largura(raiz, n);

	desmallocar(raiz);

	printf("\n");

	scanf("%d", &n);
    }

    return 0;
}