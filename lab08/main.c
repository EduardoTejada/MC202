#include <stdio.h>
#include <stdlib.h>

#define MAX_CONJUNTOS 129

typedef struct item {
    int inteiro;
    struct item *proximo;
    struct item *anterior;
} item;

typedef struct lista {
    item *inicio;
    item *fim;
} lista;

typedef struct map {
    int conjunto_id;
    lista conjunto;
} map;

void imprimir(lista * l, int index)
{
    printf("C%d = {", index);
    item *atual = l->inicio;

    while (atual != NULL) {
	printf("%d", atual->inteiro);
	if (atual->proximo != NULL) {
	    printf(", ");
	}
	atual = atual->proximo;
    }
    printf("}\n");
}

void inicializa(lista * l)
{
    l->inicio = NULL;
    l->fim = NULL;
}

void liberar(lista * l)
{
    if (l == NULL) {
	return;
    }
    item *atual = l->inicio;
    while (atual != NULL) {
	item *temp = atual;
	atual = atual->proximo;
	free(temp);
    }
}

int insere(lista * l, int inteiro)
{
    item *novo = (item *) malloc(sizeof(item));
    if (novo == NULL) {
	return 0;
    }
    novo->inteiro = inteiro;
    novo->proximo = NULL;

    if (l->inicio == NULL) {
	novo->anterior = NULL;
	l->inicio = novo;
	l->fim = novo;
	return 1;
    }

    item *atual = l->inicio, *anterior = NULL;
    while (atual != NULL && inteiro >= atual->inteiro) {
	if (inteiro == atual->inteiro) {
	    free(novo);
	    return 0;		// Element already exists
	}
	anterior = atual;
	atual = atual->proximo;
    }

    if (atual == NULL) {
	novo->anterior = l->fim;
	l->fim->proximo = novo;
	l->fim = novo;
    } else {
	novo->anterior = anterior;
	novo->proximo = atual;
	if (anterior != NULL) {
	    anterior->proximo = novo;
	} else {
	    l->inicio = novo;
	}
	atual->anterior = novo;
    }

    return 1;
}

void remove_item(lista * l, int inteiro)
{
    item *atual = l->inicio;
    while (atual != NULL) {
	if (atual->inteiro == inteiro) {
	    if (atual->anterior != NULL) {
		atual->anterior->proximo = atual->proximo;
	    } else {
		l->inicio = atual->proximo;
	    }
	    if (atual->proximo != NULL) {
		atual->proximo->anterior = atual->anterior;
	    } else {
		l->fim = atual->anterior;
	    }
	    free(atual);
	    break;
	}
	atual = atual->proximo;
    }
}

int esta_no_conjunto(lista * l, int inteiro)
{
    item *atual = l->inicio;
    while (atual != NULL) {
	if (atual->inteiro == inteiro) {
	    return 1;
	}
	atual = atual->proximo;
    }
    return 0;
}

void uniao_conjuntos(lista * l1, lista * l2, lista * l3)
{
    for (item * atual = l2->inicio; atual != NULL; atual = atual->proximo) {
	insere(l1, atual->inteiro);
    }
    for (item * atual = l3->inicio; atual != NULL; atual = atual->proximo) {
	insere(l1, atual->inteiro);
    }
}

void diferenca_conjuntos(lista * l1, lista * l2, lista * l3)
{
    for (item * atual = l2->inicio; atual != NULL; atual = atual->proximo) {
	if (!esta_no_conjunto(l3, atual->inteiro)) {
	    insere(l1, atual->inteiro);
	}
    }
}

void intersecao_conjuntos(lista * l1, lista * l2, lista * l3)
{
    for (item * atual = l2->inicio; atual != NULL; atual = atual->proximo) {
	if (esta_no_conjunto(l3, atual->inteiro)) {
	    insere(l1, atual->inteiro);
	}
    }
}

int busca_conjunto(map * conjuntos, int num_conjuntos, int conjunto_id)
{
    for (int i = 0; i < num_conjuntos; i++) {
	if (conjuntos[i].conjunto_id == conjunto_id) {
	    return i;
	}
    }
    return -1;
}

int inicializa_se_necessario(map * conjuntos, int *num_conjuntos,
			     int conjunto_id)
{
    int idx = busca_conjunto(conjuntos, *num_conjuntos, conjunto_id);
    if (idx == -1) {
	// Inicializar novo conjunto
	idx = *num_conjuntos;
	conjuntos[idx].conjunto_id = conjunto_id;
	inicializa(&conjuntos[idx].conjunto);
	(*num_conjuntos)++;
    }
    return idx;
}

int reinicializa(map * conjuntos, int *num_conjuntos, int conjunto_id)
{
    int idx = busca_conjunto(conjuntos, *num_conjuntos, conjunto_id);
    if (idx == -1) {
	// Inicializar novo conjunto
	idx = *num_conjuntos;
	conjuntos[idx].conjunto_id = conjunto_id;
	inicializa(&conjuntos[idx].conjunto);
	(*num_conjuntos)++;
    } else {
	liberar(&conjuntos[idx].conjunto);
	inicializa(&conjuntos[idx].conjunto);
    }
    return idx;
}

int main()
{
    char comando, falhou = 0;
    map conjuntos[MAX_CONJUNTOS];
    int num_conjuntos = 0;

    scanf(" %c", &comando);
    while (1) {
	if (comando == 't') {
	    break;		// Exit condition
	}

	if (comando == 'c') {
	    int conjunto_id;
	    scanf("%d", &conjunto_id);
	    reinicializa(conjuntos, &num_conjuntos, conjunto_id);
	}

	else if (comando == 'i') {
	    int conjunto_id, quantidade_insercoes;
	    int numero;
	    scanf("%d %d", &conjunto_id, &quantidade_insercoes);
	    int idx =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto_id);
	    while (quantidade_insercoes) {
		//scanf("%s", string_numero);
		scanf("%d", &numero);
		insere(&conjuntos[idx].conjunto, numero);
		quantidade_insercoes--;
	    }
	    falhou = numero;	//string_numero[0];
	}

	else if (comando == 'r') {
	    int conjunto_id, quantidade_remocoes;
	    //char string_numero[50];
	    int numero;
	    scanf("%d %d", &conjunto_id, &quantidade_remocoes);
	    int idx =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto_id);
	    while (quantidade_remocoes) {
		//scanf("%s", string_numero);
		scanf("%d", &numero);
		int num_removido = numero;	//_atoi(string_numero);
		if (esta_no_conjunto
		    (&conjuntos[idx].conjunto, num_removido)) {
		    remove_item(&conjuntos[idx].conjunto, num_removido);
		}
		quantidade_remocoes--;
	    }
	    falhou = numero;	//string_numero[0];
	}

	else if (comando == 'u') {
	    int conjunto1, conjunto2, conjunto3;
	    scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
	    int idx1 = reinicializa(conjuntos, &num_conjuntos, conjunto1);
	    int idx2 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto2);
	    int idx3 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto3);
	    uniao_conjuntos(&conjuntos[idx1].conjunto,
			    &conjuntos[idx2].conjunto,
			    &conjuntos[idx3].conjunto);
	}

	else if (comando == 'n') {
	    int conjunto1, conjunto2, conjunto3;
	    scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
	    int idx1 = reinicializa(conjuntos, &num_conjuntos, conjunto1);
	    int idx2 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto2);
	    int idx3 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto3);
	    intersecao_conjuntos(&conjuntos[idx1].conjunto,
				 &conjuntos[idx2].conjunto,
				 &conjuntos[idx3].conjunto);
	}

	else if (comando == 'm') {
	    int conjunto1, conjunto2, conjunto3;
	    scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
	    int idx1 = reinicializa(conjuntos, &num_conjuntos, conjunto1);
	    int idx2 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto2);
	    int idx3 =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto3);
	    diferenca_conjuntos(&conjuntos[idx1].conjunto,
				&conjuntos[idx2].conjunto,
				&conjuntos[idx3].conjunto);
	}

	else if (comando == 'e') {
	    int conjunto_id, inteiro;
	    scanf("%d %d", &conjunto_id, &inteiro);
	    int idx =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto_id);
	    if (esta_no_conjunto(&conjuntos[idx].conjunto, inteiro))
		printf("%d esta em C%d\n", inteiro, conjunto_id);
	    else
		printf("%d nao esta em C%d\n", inteiro, conjunto_id);
	}

	else if (comando == 'p') {
	    int conjunto_id;
	    scanf("%d", &conjunto_id);
	    int idx =
		inicializa_se_necessario(conjuntos, &num_conjuntos,
					 conjunto_id);
	    imprimir(&conjuntos[idx].conjunto, conjunto_id);
	}

	if (falhou) {
	    comando = falhou;
	    falhou = 0;
	} else {
	    scanf(" %c", &comando);
	}
    }

    for (int i = 0; i < num_conjuntos; i++) {
	liberar(&conjuntos[i].conjunto);
    }

    return 0;
}