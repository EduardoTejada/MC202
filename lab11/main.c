#include <stdio.h>
#include <stdlib.h>

enum Tipos { TIPO_SEQUENCIAL, TIPO_MTF, TIPO_TRANSPOSE, TIPO_COUNT };

struct Node {
    int dado;
    struct Node *prox;
    int contador;
};
typedef struct Node Node;

struct lista {
    Node *inicio;
};
typedef struct lista lista;

lista criarLista()
{
    lista L;
    L.inicio = (Node *) malloc(sizeof(Node));
    L.inicio->dado = -1;
    return L;
}

void adicionarElementos(lista L, int n)
{
    Node *aux = L.inicio;
    for (int i = 1; i <= n; i++) {
	aux->prox = (Node *) malloc(sizeof(Node));
	aux = aux->prox;
	aux->dado = i;
	aux->contador = 0;
    }
    aux->prox = NULL;
}

void reconstituirElementos(lista L, int n)
{
    Node *aux = L.inicio->prox;
    for (int i = 1; i <= n; i++) {
	aux->dado = i;
	aux = aux->prox;
    }
}

void liberarLista(lista L)
{
    Node *aux = L.inicio;
    Node *aux1 = L.inicio->prox;
    while (aux != NULL) {
	aux1 = aux;
	aux = aux->prox;
	free(aux1);
    }
}

void imprimirLista(lista L)
{
    Node *aux = L.inicio->prox;
    int c = 0;
    printf("\n");
    while (aux != NULL) {
	printf(" %d ", aux->dado);
	aux = aux->prox;
	c++;
	if (c > 10000)
	    break;
    }
    c = 0;
    printf("\n");
    aux = L.inicio->prox;
    while (aux != NULL) {
	printf(" %d ", aux->contador);
	aux = aux->prox;
	c++;
	if (c > 10000)
	    break;
    }
    printf("\n");
}

int buscarChaveSequencial(lista L, int chave)
{
    Node *aux = L.inicio;
    int comparacoes = 0;
    while (aux->prox != NULL) {
	aux = aux->prox;
	if (aux->dado == chave)
	    return (comparacoes + 1);
	comparacoes++;
    }
    aux->prox = (Node *) malloc(sizeof(Node));
    aux->prox->dado = chave;
    aux->prox->prox = NULL;
    return comparacoes;
}

void adicionarInicio(lista L, int chave)
{
    Node *novo = (Node *) malloc(sizeof(Node));
    novo->dado = chave;
    novo->prox = L.inicio->prox;
    L.inicio->prox = novo;
}

int buscarChaveMTF(lista L, int chave)
{
    Node *aux = L.inicio->prox;
    Node *aux_ant = L.inicio;
    int comparacoes = 0;
    while (aux != NULL) {
	if (aux->dado == chave) {
	    aux_ant->prox = aux->prox;
	    aux->prox = L.inicio->prox;
	    L.inicio->prox = aux;
	    return (comparacoes + 1);
	}
	comparacoes++;
	aux_ant = aux;
	aux = aux->prox;
    }
    adicionarInicio(L, chave);
    return comparacoes;
}

int buscarChaveTranspose(lista L, int chave)
{
    Node *aux = L.inicio->prox;
    Node *aux_ant = L.inicio;
    Node *aux_ant_ant = L.inicio;
    int comparacoes = 0;
    while (aux != NULL) {
	if (aux->dado == chave) {
	    if (comparacoes == 1) {
		L.inicio->prox->prox = aux->prox;
		aux->prox = L.inicio->prox;
		L.inicio->prox = aux;
	    } else if (comparacoes > 1) {
		aux_ant->prox = aux->prox;
		aux->prox = aux_ant;
		aux_ant_ant->prox = aux;
	    }
	    return (comparacoes + 1);
	}
	comparacoes++;
	aux_ant_ant = aux_ant;
	aux_ant = aux;
	aux = aux->prox;
    }
    adicionarInicio(L, chave);
    return comparacoes;
}

void moverRegistro(lista L, Node * no, Node * no_ant)
{
    Node *aux = L.inicio->prox;
    Node *aux_ant = L.inicio;
    while (aux != NULL) {
	if (aux->contador <= no->contador) {
	    if (aux == no)
		return;
	    no_ant->prox = no->prox;
	    aux_ant->prox = no;
	    no->prox = aux;
	    return;
	}
	aux_ant = aux;
	aux = aux->prox;
    }
}

int buscarChaveCount(lista L, int chave)
{
    Node *aux = L.inicio->prox;
    Node *aux_ant = L.inicio;
    int comparacoes = 0;
    while (aux != NULL) {
	if (aux->dado == chave) {
	    aux->contador += 1;
	    if (comparacoes > 0)
		moverRegistro(L, aux, aux_ant);
	    return (comparacoes + 1);
	}
	comparacoes++;
	aux_ant = aux;
	aux = aux->prox;
    }
    Node *novo = (Node *) malloc(sizeof(Node));
    novo->dado = chave;
    novo->contador = 1;
    novo->prox = NULL;
    aux_ant->prox = novo;
    if (comparacoes > 0)
	moverRegistro(L, novo, aux_ant);
    return comparacoes;
}

int busca(lista L, int r, int *chaves, int tipo)
{
    int contagem = 0;
    switch (tipo) {
    case TIPO_SEQUENCIAL:
	for (int i = 0; i < r; i++) {
	    contagem += buscarChaveSequencial(L, chaves[i]);
	}
	break;
    case TIPO_MTF:
	for (int i = 0; i < r; i++) {
	    contagem += buscarChaveMTF(L, chaves[i]);
	}
	break;
    case TIPO_TRANSPOSE:
	for (int i = 0; i < r; i++) {
	    contagem += buscarChaveTranspose(L, chaves[i]);
	}
	break;
    case TIPO_COUNT:
	for (int i = 0; i < r; i++) {
	    contagem += buscarChaveCount(L, chaves[i]);
	}
	break;
    }
    return contagem;
}

int main(void)
{
    int n, r;
    scanf("%d", &n);
    scanf("%d", &r);
    int chaves[r];
    for (int i = 0; i < r; i++) {
	scanf("%d", &chaves[i]);
    }
    lista L = criarLista();
    adicionarElementos(L, n);

    printf("Sequencial: %d\n", busca(L, r, chaves, TIPO_SEQUENCIAL));
    liberarLista(L);
    L = criarLista();
    adicionarElementos(L, n);

    printf("MTF: %d\n", busca(L, r, chaves, TIPO_MTF));
    liberarLista(L);
    L = criarLista();
    adicionarElementos(L, n);

    printf("Transpose: %d\n", busca(L, r, chaves, TIPO_TRANSPOSE));
    liberarLista(L);
    L = criarLista();
    adicionarElementos(L, n);

    printf("Count: %d\n", busca(L, r, chaves, TIPO_COUNT));

    liberarLista(L);
    return 0;
}