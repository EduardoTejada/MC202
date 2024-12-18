#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct node {
    int key;
    char *nome;
    float pontos;
    struct node *left;
    struct node *right;
    struct node *parent;
};
typedef struct node node;


struct tree {
    node *raiz;
    int num_de_nos;
};
typedef struct tree tree;

/*
void enqueue(node** fila, node* item, int* primeiro, int* tamanho, int n){
    int pos = (*primeiro + *tamanho) % n;
    fila[pos] = item;
    (*tamanho)++;
}

node* dequeue(node** fila, int* primeiro, int* tamanho, int n){
    node* rem = fila[*primeiro];
    *primeiro = (*primeiro) < n-1 ? (*primeiro)+1 : 0;
    (*tamanho)--;

    return rem;
}

void print_largura(tree T){
    node* Q[T.num_de_nos];
    int primeiro = 0, tamanho = 0;
    enqueue(Q, T.raiz, &primeiro, &tamanho, T.num_de_nos);
    while(tamanho != 0){
        int nivel_count = tamanho;
        while(nivel_count > 0){
            node* p = dequeue(Q, &primeiro, &tamanho, T.num_de_nos);
            printf("%d %s %f    ", p->key, p->nome, p->pontos);
            if(p->left != NULL){
                enqueue(Q, p->left, &primeiro, &tamanho, T.num_de_nos);
            }
            if(p->right != NULL){
                enqueue(Q, p->right, &primeiro, &tamanho, T.num_de_nos);
            }
            nivel_count--;
        }
        printf("\n");
    }
}
*/

/*
  0 = fail, 1 = success
*/
int insert(node ** root, int keyy, char *nomee, float pontoss)
{

    node *neu = malloc(sizeof(node));
    if (neu == NULL)
	return 0;

    neu->key = keyy;
    neu->pontos = pontoss;
    neu->left = neu->right = NULL;
    neu->nome = (char *) malloc(strlen(nomee) + 1);	// Aloca memória suficiente para a string
    if (neu->nome == NULL) {
	free(neu);		// Libera o nó alocado se não conseguir alocar memória para o nome
	return 0;
    }
    strcpy(neu->nome, nomee);	// Copia a string

    if (*root == NULL) {
	*root = neu;
	return 1;
    }

    node *pp = NULL;
    node *p = *root;

    while (p != NULL) {
	if (p->key > keyy) {
	    pp = p;
	    p = p->left;
	} else if (p->key < keyy) {
	    pp = p;
	    p = p->right;
	} else {
	    return 1;
	}
    }

    if (pp->key > keyy)
	pp->left = neu;
    else
	pp->right = neu;

    neu->parent = pp;
    return 1;
}

tree criarArvore()
{
    tree T;
    T.num_de_nos = 0;
    T.raiz = NULL;
    return T;
}

void liberarArvore(node * raiz)
{
    if (raiz == NULL)
	return;

    liberarArvore(raiz->left);
    liberarArvore(raiz->right);

    free(raiz->nome);
    free(raiz);
}

node *buscar(node * raiz, int chave)
{
    if (raiz == NULL || raiz->key == chave)
	return raiz;
    if (chave < raiz->key)
	return buscar(raiz->left, chave);
    else
	return buscar(raiz->right, chave);
}

node *minimo(node * raiz)
{
    if (raiz == NULL)
	return NULL;

    while (raiz->left != NULL)
	raiz = raiz->left;

    return raiz;
}


node *maximo(node * raiz)
{
    if (raiz == NULL)
	return NULL;

    while (raiz->right != NULL)
	raiz = raiz->right;

    return raiz;
}


node *sucessor(node * u)
{
    if (u->right != NULL)
	return minimo(u->right);

    node *p = u->parent;
    while (p != NULL && u == p->right) {
	u = p;
	p = p->parent;
    }
    return p;
}

node *predecessor(node * u)
{
    if (u->left != NULL)
	return maximo(u->left);
    node *p = u->parent;
    while (p != NULL && u == p->left) {
	u = p;
	p = p->parent;
    }
    return p;
}

/* 
  0 = fail, 1 = sucess
*/
int remover(tree * T, int chave)
{
    node *u = buscar(T->raiz, chave);
    if (u == NULL)
	return 0;

    node *p = u->parent;

    if (u->left == NULL && u->right == NULL) {	// é uma folha
	if (p == NULL) {
	    T->raiz = NULL;
	} else if (u == p->right)
	    p->right = NULL;
	else
	    p->left = NULL;
	free(u->nome);
	free(u);
	T->num_de_nos--;
	return 1;
    }

    if (u->left == NULL || u->right == NULL) {	// tem apenas um filho
	node *f = u->left == NULL ? u->right : u->left;

	if (p == NULL) {
	    T->raiz = f;
	} else if (u == p->right)
	    p->right = f;
	else
	    p->left = f;

	f->parent = p;

	T->num_de_nos--;
	free(u->nome);
	free(u);
	T->num_de_nos--;
	return 1;
    }
    // no removido tem dois filhos
    node *suce = sucessor(u);

    u->key = suce->key;

    free(u->nome);
    u->nome = (char *) malloc(strlen(suce->nome) + 1);
    strcpy(u->nome, suce->nome);

    u->pontos = suce->pontos;

    if (suce->parent->left == suce)
	suce->parent->left = suce->right;
    else
	suce->parent->right = suce->right;

    if (suce->right != NULL)
	suce->right->parent = suce->parent;

    free(suce->nome);
    free(suce);
    /*
       if(p == NULL)
       T->raiz = pred;
       else{
       if(p->right == u)
       p->right = pred;
       else
       p->left = pred;
       }

       if(u->left != pred)
       pred->left = u->left;
       else
       pred->left = NULL;
       pred->right = u->right;

       if(pred->parent->right == pred)
       pred->parent->right = NULL;
       else
       pred->parent->left = NULL;

       pred->parent = p;

       free(u->nome);
       free(u);
     */
    T->num_de_nos--;
    return 1;
}


void buscar_primeiro_maior(node * raiz, int chave, node ** u)
{
    if (raiz != NULL) {
	buscar_primeiro_maior(raiz->left, chave, u);
	if (raiz->key > chave && (*u == NULL)) {
	    *u = raiz;
	    return;
	}
	buscar_primeiro_maior(raiz->right, chave, u);
    }
}


void buscar_ultimo_menor(node * raiz, int chave, node ** u)
{
    if (raiz != NULL) {
	buscar_ultimo_menor(raiz->right, chave, u);
	if (raiz->key < chave && (*u == NULL)) {
	    *u = raiz;
	    return;
	}
	buscar_ultimo_menor(raiz->left, chave, u);
    }
}


void buscar_intervalo(tree T, int chave1, int chave2)
{
    printf("clientes no intervalo [%d,%d]: ", chave1, chave2);

    if (chave1 > chave2 || T.raiz == NULL) {
	printf("nenhum\n");
	return;
    }
    node *u1 = buscar(T.raiz, chave1);
    if (u1 == NULL)
	buscar_primeiro_maior(T.raiz, chave1, &u1);
    node *u2 = buscar(T.raiz, chave2);
    if (u2 == NULL)
	buscar_ultimo_menor(T.raiz, chave2, &u2);

    node *max = maximo(T.raiz);

    if (chave1 == max->key || u1 == NULL || u2 == NULL) {
	printf("nenhum\n");
	return;
    }

    int Q[T.num_de_nos];

    int i = 0;
    while (u1->key <= chave2) {
	Q[i] = u1->key;
	i++;
	if (u1->key == max->key)
	    break;
	u1 = sucessor(u1);
    };

    if (i == 0) {
	printf("nenhum\n");
	return;
    }

    for (int k = 0; k < i; k++) {
	printf("%d ", Q[k]);
    }
    printf("\n");
}


void print_clientes(node * raiz)
{
    if (raiz != NULL) {
	print_clientes(raiz->left);
	printf(" %s (%d)", raiz->nome, raiz->key);
	print_clientes(raiz->right);
    }
}


int main(void)
{
    char cmd[150];
    int chave_lida;
    char nome_lido[100];
    float pontos_lidos;
    tree T;
    T.raiz = NULL;

    while (1) {
	scanf("%s", cmd);

	if (strcmp(cmd, "criar") == 0) {
	    liberarArvore(T.raiz);
	    T = criarArvore();
	}

	else if (strcmp(cmd, "inserir") == 0) {
	    scanf("%d, %100[^,], %f", &chave_lida, nome_lido,
		  &pontos_lidos);
	    if (!insert(&T.raiz, chave_lida, nome_lido, pontos_lidos))
		printf("memoria insuficiente\n");
	    else
		T.num_de_nos++;
	}

	else if (strcmp(cmd, "remover") == 0) {
	    scanf("%d", &chave_lida);
	    remover(&T, chave_lida);
	}

	else if (strcmp(cmd, "buscar-intervalo") == 0) {
	    int chave_lida2;
	    scanf("%d %d", &chave_lida, &chave_lida2);
	    buscar_intervalo(T, chave_lida, chave_lida2);
	}

	else if (strcmp(cmd, "buscar") == 0) {
	    scanf("%d", &chave_lida);
	    node *aux = buscar(T.raiz, chave_lida);
	    if (aux != NULL)
		printf("cliente %d: %s, %.2f pontos\n", aux->key,
		       aux->nome, aux->pontos);
	    else
		printf("nao ha cliente %d\n", chave_lida);
	}

	else if (strcmp(cmd, "minimo") == 0) {
	    if (T.raiz == NULL)
		printf("arvore vazia\n");
	    else
		printf("minimo: %d\n", minimo(T.raiz)->key);
	}

	else if (strcmp(cmd, "maximo") == 0) {
	    if (T.raiz == NULL)
		printf("arvore vazia\n");
	    else
		printf("maximo: %d\n", maximo(T.raiz)->key);
	}

	else if (strcmp(cmd, "sucessor") == 0) {
	    scanf("%d", &chave_lida);
	    node *aux = buscar(T.raiz, chave_lida);
	    if (aux && aux->key != maximo(T.raiz)->key)
		printf("sucessor de %d: %d\n", aux->key,
		       sucessor(aux)->key);
	    else
		printf("nao ha cliente %d\n", chave_lida);
	}

	else if (strcmp(cmd, "predecessor") == 0) {
	    scanf("%d", &chave_lida);
	    node *aux = buscar(T.raiz, chave_lida);
	    if (aux && aux->key != minimo(T.raiz)->key)
		printf("predecessor de %d: %d\n", aux->key,
		       predecessor(aux)->key);
	    else
		printf("nao ha cliente %d\n", chave_lida);
	}

	else if (strcmp(cmd, "imprimir") == 0) {
	    if (T.raiz == NULL)
		printf("arvore vazia\n");
	    else {
		printf("clientes:");
		print_clientes(T.raiz);
		printf(" \n");
		//print_largura(T);
	    }
	}

	else if (strcmp(cmd, "terminar") == 0) {
	    //print_largura(T);
	    liberarArvore(T.raiz);
	    break;
	}
    }
    return 0;
}