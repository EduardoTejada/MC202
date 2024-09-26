#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct node {
    double data;
    struct node *next;
};
typedef struct node node;


void listPrint(node* aux);
void listFree(node* aux);
node* insertNode(node* head, int p, double x);
node* removeNode(node* head, int p);
node* moveList(node* head, int i, int t, int p);

int main(void) {
  char cmd[15];
  double x;
  int i, p, t;
  node* head;
  node* aux;

  while (1) {
    /* Criar uma lista encadeada */
    scanf("%s",cmd);

    if (strcmp(cmd,"c") == 0) {
      if (head != NULL)
        listFree(head);
      head = NULL;
    }

    /* Inserir x na posição p da lista*/
    else if (strcmp(cmd,"i") == 0) {
      scanf("%d %lf",&p,&x);
      aux = insertNode(head, p, x);
      head = (aux != NULL) ? aux: head;
    }

    /* Remover o nó na posição p */
    else if (strcmp(cmd,"r") == 0) {
      scanf("%d", &p);
      aux = removeNode(head, p);
      head = (aux != NULL) ? aux: head;
    }

    /* Imprimir a lista */
    else if (strcmp(cmd,"p") == 0) {
      listPrint(head);
    }

    /* Reverter os elementos do trecho i até t da lista inclusive, se estiver fora do alcance não faz nada */
    else if (strcmp(cmd,"v") == 0) {
      scanf("%d %d",&i,&t);
      //reverseList(head, i, t);
      printf("v %d %d\n", i, t);
    }

    /* Mover o trecho de i até t para a posição p */
    else if (strcmp(cmd,"x") == 0) {
      scanf("%d %d %d",&i,&t, &p);
      aux = moveList(head, i, t, p);
      head = (aux != NULL) ? aux: head;
    }
    
    /* Terminar o programa e liberar a memória */
    else if (strcmp(cmd,"t") == 0){
      listFree(head);
      break;
    }
  }
}


node* moveList(node* head, int i, int t, int p){
  node* aux = head;
  node* p_i_minus_1 = NULL;
  node* p_i = NULL;
  node* p_p_minus_1 = NULL;
  node* p_p = NULL;
  node* p_t_plus_1 = NULL;
  node* p_t = NULL;
  int count = 0;

  if(i > t)
    return head;

  while(1){
    if(count == i-1){
      p_i_minus_1 = aux;
      printf("i-1: %lf\n", aux->data);
    }
    else if(count == i){
      p_i = aux;
      printf("i: %lf\n", aux->data);
    }
    if(count == p-1){
      p_p_minus_1 = aux;
      printf("p-1: %lf\n", aux->data);
    }
    else if(count == p){
      p_p = aux;
      printf("p: %lf\n", aux->data);
    }
    if(count == t){
      p_t = aux;
      printf("t: %lf\n", aux->data);
    }
    else if(count == t+1){
      p_t_plus_1 = aux;
      printf("t+1: %lf\n", aux->data);
    }
    count++;
    
    aux = aux->next;
    if(aux == NULL)
      break;
  }

  if((p_i_minus_1 == NULL && i > 0) || (p_i == NULL) || (p_p == NULL) || 
  (p_p_minus_1 == NULL && p > 0) || (p_t == NULL) || count < p || count < t || count < i)
    return head;
  if(i == 0){
    head = p_i;
  }
  else if(i-1 == 0){
    head = p_t_plus_1;
  }else{
    p_i_minus_1->next = p_t_plus_1;
  }
  p_p_minus_1->next = p_i;
  p_t->next = p_p;
  return head;
}


node* removeNode(node* head, int p){
  int count = 0;

  if(head == NULL || p < 0){
    return head;
  }

  node* aux = head;
  if(p == 0){
    head = head->next;
    free(aux);
    return head;
  }

  node* aux1 = aux->next;
  while(1){
    if(count == p-1){
      aux->next = aux1->next;
      free(aux1);
      return head;
    }
    aux = aux1;
    aux1 = aux1->next;
    if(aux1 == NULL)
      return NULL;
    count++;
  }
}

void listPrint(node* aux){
  while(aux){
    printf("%.2lf", aux->data);
    aux = aux->next;
    if(aux == NULL)
      printf("\n");
    else
      printf(" ");
  }
}


node* insertNode(node* head, int p, double x){
  int count = 0;

  if(p < 0)
    return NULL;

  node* new_node = (node*) malloc(sizeof(node));
  if(new_node == NULL){
    return NULL;
  }
  new_node->data = x;
  
  if(head == NULL && p >= 0){
    head = new_node;
    new_node->next = NULL;
    return head;
  }

  if(p == 0){
    new_node->next = head;
    head = new_node;
    return head;
  }

  node* aux = head;
  while(1){
    count++;
    if(count == p){
      new_node->next = aux->next;
      aux->next = new_node;
      return head;
    }
    if((aux->next) == NULL)
      break;
    else
      aux = aux->next;
  }
  /* Adiciona no final da lista */
  aux->next = new_node;
  new_node->next = NULL;
  return head;
}


void listFree(node* aux){
  if(aux == NULL){
    printf("nada");
    return;
  }
  node* aux1 = aux->next;
  while(aux1 != NULL){
    free(aux);
    aux = aux1;
    aux1 = aux1->next;
  }
}