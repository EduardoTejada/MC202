#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct node {
    double data;
    struct node *next;
};
typedef struct node node;


node* createList(){
    node n0;
    n0.next = NULL;
    return &n0;
}



int main(void) {

  char cmd[15];
  double x;
  int i, p, t;
  node* head = NULL;

  while (1) {
    scanf("%s",cmd);

    if (strcmp(cmd,"c") == 0) {
      
      if (head != NULL)
        listFree(head);

      head = createList();
      while (head == NULL) {
        head = createList();
      }
    }

    else if (strcmp(cmd,"i ") == 0) {
      scanf("%d %lf",&p,&x);
      insertNode(head, p, x);
    }

    else if (strcmp(cmd,"r ") == 0) {
      removeNode(head, p);
    }

    else if (strcmp(cmd,"p ") == 0) {
      if(listSize(head) > 0)
        listPrint(head);
    }

    else if (strcmp(cmd,"v ") == 0) {
      scanf("%d %d",&i,&t);
      reverseList(head, i, t);
    }

    else if (strcmp(cmd,"x ") == 0) {
      scanf("%d %d %d",&i,&t, &p);
      moveList(head, i, t, p);
    }
    else if (strcmp(cmd,"t") == 0){
      endList(head);
    }
  }
}
