#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 51
#define HASH_SIZE 100003 // tamanho primo para minimizar colisões

typedef struct Pessoa {
    char nome[MAX_NOME];
    int conhecidos_por;
    int conhece_alguem;
    struct Pessoa *proximo;
} Pessoa;

Pessoa *hash_table[HASH_SIZE];
int num_pessoas = 0; // Contador global de pessoas

unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

Pessoa *encontrar_pessoa(const char *nome) {
    unsigned int idx = hash(nome);
    Pessoa *p = hash_table[idx];
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) {
            return p;
        }
        p = p->proximo;
    }
    return NULL;
}

Pessoa *adicionar_pessoa(const char *nome) {
    unsigned int idx = hash(nome);
    Pessoa *p = hash_table[idx];
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) {
            return p;
        }
        p = p->proximo;
    }
    Pessoa *nova_pessoa = (Pessoa *)malloc(sizeof(Pessoa));
    strcpy(nova_pessoa->nome, nome);
    nova_pessoa->conhecidos_por = 0;
    nova_pessoa->conhece_alguem = 0;
    nova_pessoa->proximo = hash_table[idx];
    hash_table[idx] = nova_pessoa;
    num_pessoas++; // Incrementa o contador de pessoas
    return nova_pessoa;
}

void adicionar_relacao(const char *nome1, const char *nome2) {
    Pessoa *p1 = adicionar_pessoa(nome1);
    Pessoa *p2 = adicionar_pessoa(nome2);
    p1->conhece_alguem = 1;
    p2->conhecidos_por++;
}

void encontrar_celebridade() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        Pessoa *p = hash_table[i];
        while (p != NULL) {
            if (p->conhecidos_por == num_pessoas - 1 && p->conhece_alguem == 0) {
                printf("%s e' celebridade.\n", p->nome);
                return;
            }
            p = p->proximo;
        }
    }
    printf("Nao ha' celebridade.\n");
}

int main() {
    char linha[2 * MAX_NOME + 10];

    while (fgets(linha, sizeof(linha), stdin)) {
        char nome1[MAX_NOME], nome2[MAX_NOME];
        if (sscanf(linha, "%s conhece %s", nome1, nome2) == 2) {
            adicionar_relacao(nome1, nome2);
        }
    }

    encontrar_celebridade();

    return 0;
}
