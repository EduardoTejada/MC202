#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PESSOAS 100000
#define MAX_NOME 51

struct pessoa {
    char *nome;
    int conhecido_por;
    int conhece_alguem;
    struct pessoa *prox;
};
typedef struct pessoa pessoa;


void copiarString1(char *dest, char *src, char *fim)
{
    if (fim == NULL)
	return;
    char *atual = src;
    int i = 0;
    while (atual != fim) {
	dest[i] = *atual;
	i++;
	atual++;
    }
    dest[i] = '\0';
}

void copiarString2(char *dest, char *inicio)
{
    if (inicio == NULL)
	return;
    char *atual = inicio;
    int i = 0;
    while (*atual != '\0') {
	dest[i] = *atual;
	i++;
	atual++;
    }
    dest[i] = '\0';
}

int hash(char *nome)
{
    int valor = 0;
    for (int i = 0; i < (int) strlen(nome); i++) {
	valor += nome[i];
	valor = (valor * nome[i]) % MAX_PESSOAS;
    }
    return valor;
}

pessoa *buscar(pessoa ** tabela, char *nome)
{
    int valor = hash(nome);
    pessoa *aux = tabela[valor];
    while (aux != NULL) {
	if (strcmp(aux->nome, nome) == 0) {
	    return aux;
	}
	aux = aux->prox;
    }
    return NULL;
}

/*pessoa* inserir_pessoa(pessoa** tabela, char* nome){
    int valor = hash(nome);
    pessoa* aux = tabela[valor];
    pessoa* aux1 = NULL;
    while (aux != NULL){
        aux1 = aux;
        aux = aux->prox;
    }
    if(aux1 != NULL) aux1->prox = aux;
    aux = (pessoa*) malloc(sizeof(pessoa));
    aux->nome = malloc((strlen(nome)+1)*sizeof(char));
    strcpy(aux->nome, nome);
    aux->prox = NULL;
    aux->conhecido_por = 0;
    aux->conhece_alguem = 0;
    
    tabela[valor] = aux;

    return aux;
}*/
pessoa *inserir_pessoa(pessoa ** tabela, char *nome)
{
    int valor = hash(nome);
    pessoa *nova = (pessoa *) malloc(sizeof(pessoa));
    nova->nome = malloc((strlen(nome) + 1) * sizeof(char));
    strcpy(nova->nome, nome);
    nova->prox = tabela[valor];
    nova->conhecido_por = 0;
    nova->conhece_alguem = 0;
    tabela[valor] = nova;
    return nova;
}

int main(void)
{
    int tamanho = 0;
    pessoa *tabela[MAX_PESSOAS];
    for (int i = 0; i < MAX_PESSOAS; i++)
	tabela[i] = NULL;
    //printf("a> %p\n", tabela);
    pessoa *p1, *p2;
    char *fim;
    char entrada[2 * MAX_NOME + 10], nome1[MAX_NOME], nome2[MAX_NOME];


    while (fgets(entrada, sizeof(entrada), stdin) != NULL) {	//{scanf("%112[^\n]\n", entrada) == 1 && tamanho < MAX_PESSOAS){
	if (entrada[0] == '\n')
	    break;
	fim = strstr(entrada, " conhece ");
	copiarString1(nome1, entrada, fim);
	fim += sizeof(char) * (strlen(" conhece "));
	copiarString2(nome2, fim);
	nome2[strcspn(nome2, "\n")] = '\0';
	//printf("%s %s\n", nome1, nome2);

	p1 = buscar(tabela, nome1);
	p2 = buscar(tabela, nome2);
	if (!p1) {
	    p1 = inserir_pessoa(tabela, nome1);
	    tamanho++;
	}
	if (!p2) {
	    p2 = inserir_pessoa(tabela, nome2);
	    tamanho++;
	}
	//printf("p1: %p p2: %p\n", p1, p2);
	//printf("p1: %d p2: %d\n", hash(nome1), hash(nome2));
	//printf("p1: %s p2: %s\n", p1->nome, p2->nome);
	//printf("\n");
	p1->conhece_alguem = 1;
	(p2->conhecido_por)++;
    }
    pessoa *aux;
    int achou = 0;
    for (int i = 0; i < MAX_PESSOAS; i++) {
	aux = tabela[i];
	while (aux != NULL) {
	    if (achou == 0 && aux->conhece_alguem == 0
		&& aux->conhecido_por == tamanho - 1) {
		printf("%s e' celebridade.\n", aux->nome);
		achou = 1;
	    }
	    pessoa *temp = aux;
	    aux = aux->prox;
	    free(temp->nome);
	    free(temp);
	}
    }
    if (!achou) {
	printf("Nao ha' celebridade.\n");
    }				/*
				   for (int i = 0; i < MAX_PESSOAS; i++) {
				   aux = tabela[i];
				   while (aux != NULL) {
				   pessoa* temp = aux;
				   aux = aux->prox;
				   free(temp->nome);
				   free(temp);
				   }
				   } */
    /*
       for(int i = 0; i < MAX_PESSOAS; i++)
       printf("%s", (*tabela[i]).nome); */
    return 0;
}

