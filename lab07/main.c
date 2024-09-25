#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct node {
    double data;
    struct node *next;
};
typedef struct node node;


/*void createList(node* head){
  (*head).next = NULL; // head->next
}*/


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
      //removeNode(head, p);
      printf("r %d", p);
    }

    /* Imprimir a lista */
    else if (strcmp(cmd,"p") == 0) {
      listPrint(head);
      //if(insertNode(head, -1, 0) > 0){
        
        //printf("%d\n", insertNode(head, -1, 0));
      //}
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
      //moveList(head, i, t, p);
      printf("x %d %d %d\n", i, t, p);
    }
    
    /* Terminar o programa e liberar a memória */
    else if (strcmp(cmd,"t") == 0){
      listFree(head);
      break;
    }
  }
}
