#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int *valores;		// Valores não nulos
    int *col_index;		// Índice das colunas dos valores
    int *linha_ptr;		// Ponteiros das linhas
    int nlinhas;		// Número de linhas
    int ncols;			// Número de colunas
    int nnz;			// Número de valores não nulos
} CSR;

typedef struct {
    int linha;
    int col;
    int valor;
} Elemento;

int compararElementos(const void *a, const void *b)
{
    Elemento *elemA = (Elemento *) a;
    Elemento *elemB = (Elemento *) b;
    if (elemA->linha != elemB->linha) {
	return elemA->linha - elemB->linha;
    } else {
	return elemA->col - elemB->col;
    }
}

// Declaração antecipada da função freeCSR
void freeCSR(CSR * matriz);

// Função para criar uma matriz CSR
CSR *inicializaCSR(int nlinhas, int ncols, int nnz)
{
    if (nlinhas <= 0 || ncols <= 0 || nnz <= 0 || nlinhas > INT_MAX
	|| nnz > INT_MAX) {
	return NULL;
    }

    CSR *matriz = (CSR *) malloc(sizeof(CSR));
    if (matriz == NULL) {
	return NULL;
    }

    matriz->nlinhas = nlinhas;
    matriz->ncols = ncols;
    matriz->nnz = nnz;
    matriz->valores = (int *) malloc(nnz * sizeof(int));
    matriz->col_index = (int *) malloc(nnz * sizeof(int));
    matriz->linha_ptr = (int *) malloc((nlinhas + 1) * sizeof(int));

    if (!matriz->valores || !matriz->col_index || !matriz->linha_ptr) {
	freeCSR(matriz);
	return NULL;
    }

    return matriz;
}

// Função para liberar a memória da matriz CSR
void freeCSR(CSR * matriz)
{
    if (matriz != NULL) {
	free(matriz->valores);
	free(matriz->col_index);
	free(matriz->linha_ptr);
	free(matriz);
    }
}

// Função para preencher a matriz CSR com os dados de entrada
int completaCSR(CSR * matriz, Elemento * elementos, int nnz)
{
    // Inicializar linha_ptr com zeros
    for (int i = 0; i <= matriz->nlinhas; i++) {
	matriz->linha_ptr[i] = 0;
    }

    // Contar o número de valores não nulos em cada linha
    for (int i = 0; i < nnz; i++) {
	matriz->linha_ptr[elementos[i].linha + 1]++;
    }

    // Calcular os ponteiros de linha (linha_ptr)
    for (int i = 0; i < matriz->nlinhas; i++) {
	matriz->linha_ptr[i + 1] += matriz->linha_ptr[i];
    }

    // Preencher os valores e índices de coluna
    int *counter = (int *) calloc(matriz->nlinhas, sizeof(int));
    if (counter == NULL) {
	return 0;
    }

    for (int i = 0; i < nnz; i++) {
	int linha = elementos[i].linha;
	int dest = matriz->linha_ptr[linha] + counter[linha];

	matriz->col_index[dest] = elementos[i].col;
	matriz->valores[dest] = elementos[i].valor;

	counter[linha]++;
    }
    free(counter);
    return 1;
}

// Função para recuperar um valor da matriz CSR
int getValor(CSR * matriz, int linha, int col)
{
    if (linha >= matriz->nlinhas || col >= matriz->ncols) {
	return 0;		// Fora dos limites
    }
    for (int i = matriz->linha_ptr[linha];
	 i < matriz->linha_ptr[linha + 1]; i++) {
	if (matriz->col_index[i] == col) {
	    return matriz->valores[i];
	}
    }
    return 0;			// Retorna 0 se o valor não for encontrado (matriz esparsa)
}

int main()
{
    int k;
    scanf("%d", &k);

    // Arrays temporários para armazenar os dados de entrada
    Elemento *elementos = (Elemento *) malloc(k * sizeof(Elemento));
    if (elementos == NULL) {
	return 1;
    }
    // Ler os elementos não nulos
    int max_linha = 0, max_col = 0;
    for (int i = 0; i < k; i++) {
	scanf("%d %d %d", &elementos[i].linha, &elementos[i].col,
	      &elementos[i].valor);
	if (elementos[i].linha > max_linha)
	    max_linha = elementos[i].linha;
	if (elementos[i].col > max_col)
	    max_col = elementos[i].col;
    }

    int nlinhas = max_linha + 1;
    int ncols = max_col + 1;

    // Ordenar os elementos por linha e coluna
    qsort(elementos, k, sizeof(Elemento), compararElementos);

    // Criar e completar a matriz CSR
    CSR *matriz = inicializaCSR(nlinhas, ncols, k);
    if (matriz == NULL) {
	return 1;
    }
    if (!completaCSR(matriz, elementos, k)) {
	return 1;
    }
    // Consultar valores na matriz
    int linha, col;
    while (1) {
	scanf("%d %d", &linha, &col);
	if (linha == -1 && col == -1)
	    break;
	printf("(%d,%d) = %d\n", linha, col, getValor(matriz, linha, col));
    }

    // Liberar a memória alocada
    freeCSR(matriz);
    free(elementos);

    return 0;
}