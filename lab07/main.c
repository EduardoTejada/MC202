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
node* reverseList(node* head, int i, int t);

int main(void) {
  char cmd[15];
  double x;
  int i, p, t;
  node* head = NULL;

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
      head = insertNode(head, p, x);
    }

    /* Remover o nó na posição p */
    else if (strcmp(cmd,"r") == 0) {
      scanf("%d", &p);
      head = removeNode(head, p);
    }

    /* Imprimir a lista */
    else if (strcmp(cmd,"p") == 0) {
      listPrint(head);
    }

    /* Reverter os elementos do trecho i até t da lista inclusive, se estiver fora do alcance não faz nada */
    else if (strcmp(cmd,"v") == 0) {
      scanf("%d %d",&i,&t);
      head = reverseList(head, i, t);
    }

    /* Mover o trecho de i até t para a posição p */
    else if (strcmp(cmd,"x") == 0) {
      scanf("%d %d %d",&i,&t, &p);
      head = moveList(head, i, t, p);
    }
    
    /* Terminar o programa e liberar a memória */
    else if (strcmp(cmd,"t") == 0){
      listFree(head);
      break;
    }
  }
  return 0;
}


node* reverseList(node* head, int i, int t){
  int count = 0;
  node* aux = head;
  while(1){
    if(aux == NULL)
      break;
    count++;
    aux = aux->next;
  }
  
  if(t > count-1 || i > count-1 || i > t){
    return head;
  }
  if(count == 1)
    return head;
  
  aux = head;
  node* aux1 = aux->next;
  
  if(count == 2){
    if(i == 0 && t == 1){
      aux1->next = aux;
      head = aux1;
      aux->next = NULL;
    }
    return head;
  }

  node* aux2 = aux1->next;

  count = 0;

  node* p_i_minus_1 = NULL;
  node* p_i = NULL;
  node* p_t_plus_1 = NULL;
  node* p_t = NULL;

  while(1){
    if(count == i-1){
      p_i_minus_1 = aux;
      //printf("i-1: %.2lf\n", p_i_minus_1->data);
    }
    else if(count == i){
      p_i = aux;
      //printf("i: %.2lf\n", p_i->data);
    }
    if(count == t){
      p_t = aux;
      //printf("t: %.2lf\n", p_t->data);
    }
    else if(count == t+1){
      p_t_plus_1 = aux;
      //printf("t+1: %.2lf\n", p_t_plus_1->data);
    }

    if(count >= i && count < t){
      aux1->next = aux;
    }
    aux = aux1;
    aux1 = aux2;

    if(aux2 == NULL){
      if(aux1 == NULL){
        if(aux == NULL)
          break;
      }
    }
    else
      aux2 = aux2->next;
    
    count++; 
  }
  if(i == 0){
    head = p_t;
  }
  else{
    p_i_minus_1->next = p_t;
  }
  p_i->next = p_t_plus_1;
  return head;
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

  if(i > t || head == NULL || (p >= i && p <= t+1))
    return head;

  while(1){
    if(count == i-1){
      p_i_minus_1 = aux;
      //printf("i-1: %lf\n", aux->data);
    }
    else if(count == i){
      p_i = aux;
      //printf("i: %lf\n", aux->data);
    }
    if(count == p-1){
      p_p_minus_1 = aux;
      //printf("p-1: %lf\n", aux->data);
    }
    else if(count == p){
      p_p = aux;
      //printf("p: %lf\n", aux->data);
    }
    if(count == t){
      p_t = aux;
      //printf("t: %lf\n", aux->data);
    }
    else if(count == t+1){
      p_t_plus_1 = aux;
      //printf("t+1: %lf\n", aux->data);
    }
    count++;
    
    if(aux->next == NULL)
      break;
    aux = aux->next;
  }

  if((p_i_minus_1 == NULL && i > 0) || ((p_i == NULL) && (i > 0)) || (p_t == NULL) || count < t || count < i)
    return head;
  
  if(i == 0){
    head = p_t_plus_1;
  }
  else{
    p_i_minus_1->next = p_t_plus_1;
  }

  if(p_p == NULL){
    aux->next = p_i;
    p_t->next = NULL;
    return head;
  }
  else if(p == 0)
    head = p_i;
  else
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
      return head;
    count++;
  }
}

void listPrint(node* aux){
  while(aux != NULL){
    printf("%.2lf ", aux->data);
    aux = aux->next;
  }
  printf("\n");
}


node* insertNode(node* head, int p, double x){
  int count = 0;

  if(p < 0)
    return head;

  node* new_node = (node*) malloc(sizeof(node));
  if(new_node == NULL){
    return head;
  }
  new_node->data = x;
  
  if(head == NULL){
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
    //printf("teste: %.2lf\n", aux->data);
    if(count == p){
      new_node->next = aux->next;
      aux->next = new_node;
      return head;
    }
    if(aux->next != NULL)
      aux = aux->next;
    else
      break;
  }
  /* Adiciona no final da lista */
  aux->next = new_node;
  new_node->next = NULL;
  return head;
}


void listFree(node* aux){
  if(aux == NULL){
    return;
  }
  node* aux1 = aux->next;
  while(aux1 != NULL){
    free(aux);
    aux = aux1;
    aux1 = aux1->next;
  }
}