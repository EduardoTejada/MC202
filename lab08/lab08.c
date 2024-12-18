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

void imprimir(lista *l, int index) {
    printf("C%d = {", index);
    for (item *atual = l->inicio; atual != NULL; atual = atual->proximo) {
        printf("%d%s", atual->inteiro, atual->proximo ? ", " : "");
    }
    printf("}\n");
}

void inicializa(lista *l) {
    l->inicio = NULL;
    l->fim = NULL;
}

void liberar(lista *l) {
    if (!l) return;
    item *atual = l->inicio;
    while (atual) {
        item *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

int insere(lista *l, int inteiro) {
    item *novo = (item *) malloc(sizeof(item));
    if (!novo) return 0; // Failed to allocate memory

    novo->inteiro = inteiro;
    novo->proximo = NULL;

    if (!l->inicio) { // Empty list
        novo->anterior = NULL;
        l->inicio = l->fim = novo;
        return 1;
    }

    item *atual = l->inicio;
    while (atual && inteiro > atual->inteiro) {
        if (inteiro == atual->inteiro) {
            free(novo);
            return 0; // Element already exists
        }
        atual = atual->proximo;
    }

    novo->anterior = atual ? atual->anterior : l->fim;
    if (atual) {
        if (atual->anterior) atual->anterior->proximo = novo;
        else l->inicio = novo;
        atual->anterior = novo;
        novo->proximo = atual;
    } else { // Insert at the end
        l->fim->proximo = novo;
        l->fim = novo;
    }

    return 1;
}

void remove_item(lista *l, int inteiro) {
    for (item *atual = l->inicio; atual != NULL; atual = atual->proximo) {
        if (atual->inteiro == inteiro) {
            if (atual->anterior) atual->anterior->proximo = atual->proximo;
            else l->inicio = atual->proximo;
            if (atual->proximo) atual->proximo->anterior = atual->anterior;
            else l->fim = atual->anterior;
            free(atual);
            break;
        }
    }
}

int esta_no_conjunto(lista *l, int inteiro) {
    for (item *atual = l->inicio; atual != NULL; atual = atual->proximo) {
        if (atual->inteiro == inteiro) return 1;
    }
    return 0;
}

void uniao_conjuntos(lista *l1, lista *l2, lista *l3) {
    liberar(l1);
    inicializa(l1);
    for (item *atual = l2->inicio; atual != NULL; atual = atual->proximo) {
        insere(l1, atual->inteiro);
    }
    for (item *atual = l3->inicio; atual != NULL; atual = atual->proximo) {
        insere(l1, atual->inteiro);
    }
}

void diferenca_conjuntos(lista *l1, lista *l2, lista *l3) {
    liberar(l1);
    inicializa(l1);
    for (item *atual = l2->inicio; atual != NULL; atual = atual->proximo) {
        if (!esta_no_conjunto(l3, atual->inteiro)) {
            insere(l1, atual->inteiro);
        }
    }
}

void intersecao_conjuntos(lista *l1, lista *l2, lista *l3) {
    liberar(l1);
    inicializa(l1);
    for (item *atual = l2->inicio; atual != NULL; atual = atual->proximo) {
        if (esta_no_conjunto(l3, atual->inteiro)) {
            insere(l1, atual->inteiro);
        }
    }
}

int busca_conjunto(map *conjuntos, int num_conjuntos, int conjunto_id) {
    for (int i = 0; i < num_conjuntos; i++) {
        if (conjuntos[i].conjunto_id == conjunto_id) {
            return i;
        }
    }
    return -1;
}

int inicializa_se_necessario(map *conjuntos, int *num_conjuntos, int conjunto_id) {
    int idx = busca_conjunto(conjuntos, *num_conjuntos, conjunto_id);
    if (idx == -1) {
        idx = (*num_conjuntos)++;
        conjuntos[idx].conjunto_id = conjunto_id;
        inicializa(&conjuntos[idx].conjunto);
    }
    return idx;
}

int reinicializa(map *conjuntos, int *num_conjuntos, int conjunto_id) {
    int idx = busca_conjunto(conjuntos, *num_conjuntos, conjunto_id);
    if (idx == -1) {
        // Inicializar novo conjunto
        idx = *num_conjuntos;
        conjuntos[idx].conjunto_id = conjunto_id;
        inicializa(&conjuntos[idx].conjunto);
        (*num_conjuntos)++;
    }
    else{
        liberar(&conjuntos[idx].conjunto);
        inicializa(&conjuntos[idx].conjunto);
    }
    return idx;
}

void processar_insercoes(map *conjuntos, int *num_conjuntos, int conjunto_id, int quantidade) {
    int idx = inicializa_se_necessario(conjuntos, num_conjuntos, conjunto_id);
    for (int i = 0; i < quantidade; i++) {
        int numero;
        scanf("%d", &numero);
        insere(&conjuntos[idx].conjunto, numero);
    }
}

void processar_remocoes(map *conjuntos, int *num_conjuntos, int conjunto_id, int quantidade) {
    int idx = inicializa_se_necessario(conjuntos, num_conjuntos, conjunto_id);
    for (int i = 0; i < quantidade; i++) {
        int numero;
        scanf("%d", &numero);
        if (esta_no_conjunto(&conjuntos[idx].conjunto, numero)) {
            remove_item(&conjuntos[idx].conjunto, numero);
        }
    }
}

int main() {
    char comando;
    map conjuntos[MAX_CONJUNTOS];
    int num_conjuntos = 0;

    while (scanf(" %c", &comando), comando != 't') {
        if (comando == 'c') {
            int conjunto_id;
            scanf("%d", &conjunto_id);
            reinicializa(conjuntos, &num_conjuntos, conjunto_id);
        } else if (comando == 'i') {
            int conjunto_id, quantidade;
            scanf("%d %d", &conjunto_id, &quantidade);
            int idx = inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto_id);
            processar_insercoes(conjuntos, &num_conjuntos, conjunto_id, quantidade);
        } else if (comando == 'r') {
            int conjunto_id, quantidade;
            scanf("%d %d", &conjunto_id, &quantidade);
            int idx = inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto_id);
            processar_remocoes(conjuntos, &num_conjuntos, conjunto_id, quantidade);
        } else if (comando == 'u') {
            int conjunto1, conjunto2, conjunto3;
            scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
            uniao_conjuntos(&conjuntos[reinicializa(conjuntos, &num_conjuntos, conjunto1)].conjunto,
                            &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto2)].conjunto,
                            &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto3)].conjunto);
        } else if (comando == 'n') {
            int conjunto1, conjunto2, conjunto3;
            scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
            intersecao_conjuntos(&conjuntos[reinicializa(conjuntos, &num_conjuntos, conjunto1)].conjunto,
                                 &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto2)].conjunto,
                                 &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto3)].conjunto);
        } else if (comando == 'm') {
            int conjunto1, conjunto2, conjunto3;
            scanf("%d %d %d", &conjunto1, &conjunto2, &conjunto3);
            diferenca_conjuntos(&conjuntos[reinicializa(conjuntos, &num_conjuntos, conjunto1)].conjunto,
                                &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto2)].conjunto,
                                &conjuntos[inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto3)].conjunto);
        }
        else if (comando == 'e') {
            int conjunto_id, inteiro;
            scanf("%d %d", &conjunto_id, &inteiro);
            int idx = inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto_id);
            if (esta_no_conjunto(&conjuntos[idx].conjunto, inteiro))
                printf("%d esta em C%d\n", inteiro, conjunto_id);
            else
                printf("%d nao esta em C%d\n", inteiro, conjunto_id);
        } 
        
        else if (comando == 'p') {
            int conjunto_id;
            scanf("%d", &conjunto_id);
            int idx = inicializa_se_necessario(conjuntos, &num_conjuntos, conjunto_id);
            imprimir(&conjuntos[idx].conjunto, conjunto_id);
        }
    }

    for (int i = 0; i < num_conjuntos; i++) {
        liberar(&conjuntos[i].conjunto);
    }

    return 0;
}
