#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "dequef.h"


int resize_dec(dequef* D);
int resize_inc(dequef* D);

/**
   Create an empty deque of floats.

   capacity is both the initial and minimum capacity.
   factor is the resizing factor, larger than 1.0.

   On success it returns the address of a new dequef.
   On failure it returns NULL.
**/
dequef* df_alloc(long capacity, double factor) {
   dequef* p_estrutura;
   /* Alocando dinamicamente um espaço na memória para guardar a dequef em p_estrutura */
   if((p_estrutura = (dequef*) malloc(sizeof(dequef))) == NULL){
      return NULL;
   }
   /* Alocando dinamicamente um espaço na memória para guardar a float em p_estrutura.data */
   if((p_estrutura->data = (float*) malloc(capacity*sizeof(float))) == NULL)
      return NULL;
   
   /* Configurando os valores iniciais da estrutura */
   p_estrutura->cap = capacity; //(*p).cap
   p_estrutura->factor = factor;
   p_estrutura->mincap = capacity;
   p_estrutura->first = 0;
   p_estrutura->size = 0;
   return p_estrutura;
}


/**
  Release a dequef and its data.
**/
void df_free(dequef* D) {
   free(D->data);
   free(D);
}



/**
   The size of the deque.
**/
long df_size(dequef* D) {
   return (long) D->size;
}



/**
   Add x to the end of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_push(dequef* D, float x){
   /* Se o vetor estiver cheio tenta aumentar o tamanho, se não for possível retorna 0 */
   if(D->size == D->cap){
      if(!resize_inc(D))
         return 0;
   }
   
   // 3 _ 1 2 // first = 2, size = 3, cap = 4, f+s = 5 > 4, f+s-c = 1
   // _ _ _ 1 // first = 3, size = 1, cap = 4, f+s = 4 = 4, f+s-c = 0
   // _ _ 1 2 // first = 2, size = 2, cap = 4, f+s = 4 = 4, f+s-c = 0
   // _ _ 1 _ // first = 2, size = 1, cap = 4, f+s = 3 < 4, 3
   // _ 1 2 _ // first = 1, size = 2, cap = 4, f+s = 3 < 4, 3
   
   /* Calcula a posição do novo elemento no fim dos elementos */
   long pos = D->first + D->size;
   if(pos >= D->cap)
      pos -= D->cap;
   D->data[pos] = x; // Adiciona o elemento na posição calculada
   D->size++; // Aumenta o tamanho da lista

   return 1;
}


/**
   Remove a float from the end of D and return it.

   If the deque has capacity/(factor^2) it tries to reduce the array size to
   capacity/factor.  If capacity/factor is smaller than the minimum capacity,
   the minimum capacity is used instead.  If it is not possible to resize, then
   the array size remains unchanged.

   It returns the float removed from D.
   What happens if D is empty before the call?
**/
float df_pop(dequef* D) {
   D->size--; // Decrementa o tamanho do vetor
   
   /* Salva o item de retorno */
   float return_item = D->data[D->size];

   /* Verifica se está no tamanho da politica de redimensionamento */
   if(D->size <= (long)(D->cap / pow(D->factor, 2))){
      resize_dec(D);
   }
   return return_item;
}



/**
   Add x to the beginning of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_inject(dequef* D, float x) {
   /* Se o vetor estiver cheio tenta aumentar o tamanho, se não for possível retorna 0 */
   if(D->size == D->cap){
      if(!resize_inc(D))
         return 0;
   }

   /* Calcula a posição do elemento a ser adicionado */
   long pos = D->first - 1;
   if(pos < 0)
      pos = D->cap - 1;
   
   D->first = pos;   // Sinaliza a nova posição do primeiro elemento
   D->data[pos] = x; // Adiciona o novo elemento no início
   D->size++;        // Aumenta o tamanho do vetor

   return 1;
}



/**
   Remove a float from the beginning of D and return it.

   If the deque has capacity/(factor^2) elements, this function tries to reduce
   the array size to capacity/factor.  If capacity/factor is smaller than the
   minimum capacity, the minimum capacity is used instead.

   If it is not possible to resize, then the array size remains unchanged.

   It returns the float removed from D.
   What happens if D is empty before the call?
**/
float df_eject(dequef* D) {
   /* Salva o item de retorno */
   float return_item = D->data[D->first];
   /* Calcula em qual posição está o segundo elemento (novo primeiro) */
   D->first++;
   if(D->first >= D->cap)
      D->first = 0;
   /* Decrementa o tamanho do vetor e diminui conforme a politica de redimensionamento */
   D->size--;
   if(D->size <= (long)(D->cap / pow(D->factor, 2))){
      resize_dec(D);
   }
   return return_item;
}



/**
   Return D[i].

   If i is not in [0,|D|-1]] what happens then?
**/
float df_get(dequef* D, long i) {
   /* Calcula a posição do item desejado no vetor data */
   long pos = D->first + i;
   if(pos >= D->cap)
      pos -= D->cap;
   return D->data[pos];
}



/**
   Set D[i] to x.

   If i is not in [0,|D|-1]] what happens then?
**/
void df_set(dequef* D, long i, float x) {
   /* Calcula a posição do item desejado no vetor data */
   long pos = D->first + i;
   if(pos >= D->cap)
      pos -= D->cap;
   D->data[pos] = x; // Atualiza o tem na posição desejada
}



/**
   Print the elements of D in a single line.
**/
void df_print(dequef* D) {
   printf("deque (%ld):", D->size);
   long pos;
   for(long i = 0; i < D->size; i++){
      pos = D->first + i;
      if(pos >= D->cap)
         pos -= D->cap;
      printf(" %.1f", D->data[pos]);
   }
   printf("\n");
}

/* Aumenta o tamanho de D.data para capacity*factor
   Retorna 1 no sucesso
   Retorna 0 na falha */
int resize_inc(dequef* D){
   /* Tenta alocar um novo espaço na memória do tamanho desejado (fator * capacity) */
   float* aux = D->data;
   aux = malloc((long) (ceil(D->factor * D->cap) * sizeof(float)));
   /* Caso falhe em alocar retorna 0 */
   if(aux == NULL){
      return 0;
   }
   /* Copia o vetor antigo para o novo vetor já na ordem correta */
   long pos;
   for(long i = 0; i < D->size; i++){
      pos = D->first + i;
      if(pos >= D->cap)
         pos -= D->cap;
      aux[i] = D->data[pos];
   }
   
   /* Libera o espaço de memória do vetor antigo */
   free(D->data);

   // Atualiza com a nova capacidade, primeiro elemento e o ponteiro do novo array
   D->cap = (long) ceil(D->factor * D->cap);
   D->first = 0;
   D->data = aux;

   return 1;
}

/* Aumenta o tamanho de D.data para max(capacity/factor, mincap)
   Retorna 1 no sucesso
   Retorna 0 na falha */
int resize_dec(dequef* D){
   float* aux;

   /* Calcula o novo tamanho desejado de acordo com a politica de redimensionamento */
   long new_cap = (long)ceil(D->cap/D->factor);
   if(new_cap < D->mincap) // Verifica se o novo tamanho é menor que o tamanho mínimo
      new_cap = D->mincap;

   /* Tenta alocar um novo espaço na memória do tamanho desejado */   
   aux = (float*) malloc((long) (new_cap * sizeof(float)));
   
   /* Caso falhe em alocar retorna 0 */
   if(aux == NULL){
      return 0;
   }
   
   /* Copia o vetor antigo para o novo vetor já na ordem correta */
   long pos;
   for(long i = 0; i < D->size; i++){
      pos = D->first + i;
      if(pos >= D->cap)
         pos -= D->cap;
      aux[i] = D->data[pos];
   }
   
   /* Libera o espaço de memória do vetor antigo */
   free(D->data);

   // Atualiza com a nova capacidade, primeiro elemento e o ponteiro do novo array
   D->cap = new_cap;
   D->data = aux;
   D->first = 0;
   
   return 1;
}
