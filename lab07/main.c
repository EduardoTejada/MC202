#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Definição da estrutura do nó da lista encadeada
struct node {
    double data; // Valor armazenado no nó
    struct node *next; // Ponteiro para o próximo nó na lista
};
typedef struct node node;

// Declaração das funções utilizadas no programa
void listPrint(node* aux); // Função para imprimir a lista
void listFree(node* aux); // Função para liberar a memória da lista
node* insertNode(node* head, int p, double x); // Função para inserir um nó na posição p
node* removeNode(node* head, int p); // Função para remover o nó na posição p
node* moveList(node* head, int i, int t, int p); // Função para mover trecho da lista de i até t para posição p
node* reverseList(node* head, int i, int t); // Função para reverter trecho da lista de i até t

int main(void) {
    char cmd[15]; // Variável para armazenar o comando do usuário
    double x; // Variável para armazenar o valor a ser inserido na lista
    int i, p, t; // Variáveis para armazenar posições e trechos na lista
    node* head = NULL; // Ponteiro para o início da lista, inicialmente vazio

    while (1) {
        // Lê o comando do usuário
        scanf("%s", cmd);

        // Comando para criar uma nova lista
        if (strcmp(cmd, "c") == 0) {
            if (head != NULL)
                listFree(head); // Libera a memória da lista atual, se existir
            head = NULL;
        }

        // Comando para inserir um nó na posição p
        else if (strcmp(cmd, "i") == 0) {
            scanf("%d %lf", &p, &x);
            head = insertNode(head, p, x);
        }

        // Comando para remover o nó na posição p
        else if (strcmp(cmd, "r") == 0) {
            scanf("%d", &p);
            head = removeNode(head, p);
        }

        // Comando para imprimir a lista
        else if (strcmp(cmd, "p") == 0) {
            listPrint(head);
        }

        // Comando para reverter trecho da lista de i até t
        else if (strcmp(cmd, "v") == 0) {
            scanf("%d %d", &i, &t);
            head = reverseList(head, i, t);
        }

        // Comando para mover trecho da lista de i até t para posição p
        else if (strcmp(cmd, "x") == 0) {
            scanf("%d %d %d", &i, &t, &p);
            head = moveList(head, i, t, p);
        }

        // Comando para terminar o programa e liberar a memória
        else if (strcmp(cmd, "t") == 0) {
            listFree(head);
            break;
        }
    }
    return 0;
}

// Função para reverter trecho da lista de i até t
node* reverseList(node* head, int i, int t) {
    int count = 0;
    node* aux = head;

    // Conta o número de nós na lista
    while (aux != NULL) {
        count++;
        aux = aux->next;
    }

    // Verifica se os índices estão fora do alcance
    if (t > count - 1 || i > count - 1 || i > t) {
        return head;
    }

    // Caso especial para lista de tamanho 1
    if (count == 1)
        return head;

    aux = head;
    node* aux1 = aux->next;

    // Caso especial para lista de tamanho 2
    if (count == 2) {
        if (i == 0 && t == 1) {
            aux1->next = aux;
            head = aux1;
            aux->next = NULL;
        }
        return head;
    }

    node* aux2 = aux1->next;
    count = 0;

    // Ponteiros para os nós que delimitam o trecho a ser revertido
    node* p_i_minus_1 = NULL;
    node* p_i = NULL;
    node* p_t_plus_1 = NULL;
    node* p_t = NULL;

    // Encontra os nós que delimitam o trecho a ser revertido
    while (1) {
        if (count == i - 1) {
            p_i_minus_1 = aux;
        } else if (count == i) {
            p_i = aux;
        }
        if (count == t) {
            p_t = aux;
        } else if (count == t + 1) {
            p_t_plus_1 = aux;
        }

        // Reverte os ponteiros dos nós entre i e t
        if (count >= i && count < t) {
            aux1->next = aux;
        }

        // Passo de incremento para o próximo nó
        aux = aux1;
        aux1 = aux2;

        // Verifica fim da lista
        if (aux2 == NULL) {
            if (aux1 == NULL) {
                if (aux == NULL)
                    break;
            }
        } else
            aux2 = aux2->next; // Passo de incremento para o próximo nó

        count++;
    }

    // Atualiza os ponteiros para reverter o trecho
    if (i == 0) {
        head = p_t;
    } else {
        p_i_minus_1->next = p_t;
    }
    p_i->next = p_t_plus_1;
    return head;
}

// Função para mover trecho da lista de i até t para posição p
node* moveList(node* head, int i, int t, int p) {
    node* aux = head;
    node* p_i_minus_1 = NULL;
    node* p_i = NULL;
    node* p_p_minus_1 = NULL;
    node* p_p = NULL;
    node* p_t_plus_1 = NULL;
    node* p_t = NULL;
    int count = 0;

    // Verifica condições inválidas
    if (i > t || head == NULL || (p >= i && p <= t + 1))
        return head;

    // Encontra os nós que delimitam os trechos a serem movidos
    while (1) {
        if (count == i - 1) {
            p_i_minus_1 = aux;
        } else if (count == i) {
            p_i = aux;
        }
        if (count == p - 1) {
            p_p_minus_1 = aux;
        } else if (count == p) {
            p_p = aux;
        }
        if (count == t) {
            p_t = aux;
        } else if (count == t + 1) {
            p_t_plus_1 = aux;
        }
        count++;
        
        // Verifica fim da lista
        if (aux->next == NULL)
            break;
        
        // Passo de incremento para o próximo nó
        aux = aux->next;
    }

    // Verifica condições de saída
    if ((p_i_minus_1 == NULL && i > 0) || ((p_i == NULL) && (i > 0)) || (p_t == NULL) || count < t || count < i)
        return head;

    // Atualiza os ponteiros para mover o trecho
    if (i == 0) {
        head = p_t_plus_1;
    } else {
        p_i_minus_1->next = p_t_plus_1;
    }

    if (p_p == NULL) { // Caso especial p fora do alcance
        aux->next = p_i;
        p_t->next = NULL;
        return head;
    } else if (p == 0)
        head = p_i;
    else
        p_p_minus_1->next = p_i;

    p_t->next = p_p;
    return head;
}

// Função para remover o nó na posição p
node* removeNode(node* head, int p) {
    int count = 0;

    // Verifica se a lista está vazia ou se a posição é inválida
    if (head == NULL || p < 0) {
        return head;
    }

    node* aux = head;
    if (p == 0) {
        head = head->next;
        free(aux); // Libera a memória do nó removido
        return head;
    }

    node* aux1 = aux->next;
    while (1) {
        // Encontra nó a ser removido
        if (count == p - 1) {
            aux->next = aux1->next;
            free(aux1); // Libera a memória do nó removido
            return head;
        }
        aux = aux1;
        aux1 = aux1->next; // Passo de incremento para o próximo nó
        if (aux1 == NULL) // Verifica fim da lista
            return head;
        count++;
    }
}

// Função para imprimir a lista
void listPrint(node* aux) {
    // Itera sobre todos os nós e imprime seus valores
    while (aux != NULL) {
        printf("%.2lf ", aux->data);
        aux = aux->next;
    }
    printf("\n");
}

// Função para inserir um nó na posição p
node* insertNode(node* head, int p, double x) {
    int count = 0;

    // Verifica se a posição é inválida
    if (p < 0)
        return head;

    node* new_node = (node*) malloc(sizeof(node)); // Aloca memória para o novo nó
    if (new_node == NULL) {
        return head;
    }
    new_node->data = x;

    // Insere o nó no início da lista se a lista estiver vazia ou se p for 0
    if (head == NULL) {
        head = new_node;
        new_node->next = NULL;
        return head;
    }

    if (p == 0) {
        new_node->next = head;
        head = new_node;
        return head;
    }

    node* aux = head;
    // Itera até encontrar a posição correta para inserir o nó
    while (1) {
        count++;
        if (count == p) {
            new_node->next = aux->next;
            aux->next = new_node;
            return head;
        }
        if (aux->next != NULL)
            aux = aux->next;
        else
            break;
    }
    // Adiciona o nó no final da lista se a posição p for maior que o tamanho da lista
    aux->next = new_node;
    new_node->next = NULL;
    return head;
}

// Função para liberar a memória de todos os nós da lista
void listFree(node* aux) {
    if (aux == NULL) {
        return;
    }
    node* aux1 = aux->next;
    // Itera sobre todos os nós e libera a memória alocada
    while (aux1 != NULL) {
        free(aux);
        aux = aux1;
        aux1 = aux1->next;
    }
}
