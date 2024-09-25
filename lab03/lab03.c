#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int max(int * v0, int inicio, int fim){
    int maximo = v0[inicio]; // Supondo o primeiro elemento como o maior
    /* Iterando entre cada elemento da lista e verificando o maior no intervalo fornecido */
    for (int i = inicio+1; i <= fim; i++){
        if (v0[i] > maximo){
            maximo = v0[i];
        }
    }
    return maximo;
}

void calcularR(int n, int * v0, int * r, int rn){
    int i;
    int count_exceed = rn;

    // Calcula o máximo para cada pedaço de rn elementos do vetor principal
    for(i=0; i<rn-1; i++){
        r[i] = max(v0, i*rn, (i+1)*rn - 1); // Começando no elemento incial da cadeia e indo até o último (primeiro da próxima cadeia - 1)
    }

    while(rn*(rn-1)+count_exceed > n) count_exceed--; // Calcula quantos termos "a mais" temos na matriz principal que não formam um grupo de rn elementos (inicialmente é de rn, considerando o pior caso)
    r[i] = max(v0, (rn-1)*rn, rn*(rn-1)+count_exceed); // Calcula qual o máximo do pedaço não completo
}

int main(){
    int n, elemento_i, elemento_jx, i, comeco, final, maximo, aux_v[3];
    scanf("%d", &n);
    int *v = (int *)malloc(n * sizeof(int));  // Alocação dinâmica de memória
    if (v == NULL) {  // Verifica se a alocação foi bem-sucedida
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    int rn = (int) ceil(sqrt(n)); // Calcula o tamanho de R
    int size_rn = (int) floor((float)n/(float)rn);
    int r[rn], indice_r;
    char op;
    /* Leitura do vetor de n elementos */
    for(i=0; i < n; i++){
        scanf("%d", &v[i]);
    }

    /* Calcular R */
    calcularR(n, v, r, rn);

    /* Leitura das operações */
    while(scanf("%c %d %d", &op, &elemento_i, &elemento_jx) != EOF){
        if(op == 'a'){
            indice_r = (int) floor(elemento_i/rn);
            v[elemento_i] = elemento_jx;
            r[indice_r] = max(v, indice_r*rn, (indice_r+1)*rn - 1);  // Atualiza R após alteração no vetor principal
        }
        else if(op == 'm'){
            comeco = (int) ceil((float)elemento_i/(float)rn);   // primeiro elemento comparado no vetor R
            final = (int) floor((float)elemento_jx/(float)rn);                // último elemento comparado no vetor R
            if(comeco <= final-1)
                aux_v[0] = max(r, comeco, final-1);                 // Elementos comparados através de R
            else
                aux_v[0] = 0;
            if(elemento_i <= comeco*size_rn-1)
                aux_v[1] = max(v, elemento_i, comeco*size_rn-1);         // Demais elementos ANTES dos elementos comparados através de R
            else
                aux_v[1] = 0;
            if(final*size_rn <= elemento_jx)
                aux_v[2] = max(v, final*size_rn, elemento_jx);              // Demais elementos DEPOIS dos elementos comparados através de R
            else
                aux_v[2] = 0;

            maximo = max(aux_v, 0, 2);                          // Juntando as comparações e comparando novamente os elementos de antes, depois e através de R

            printf("%d\n", maximo);
        }
    }
    free(v);
    return 0;
}
