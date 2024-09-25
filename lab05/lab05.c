#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    short int n;
    double media = (double) 0, dp = (double) 0;
    scanf("%hd", &n);
    /* Alocando um espaço de memória para guardar n apontadores para double, portanto sendo do tipo
    apontador para apontador para double */
    double** vetor_p = (double **)malloc((n * sizeof(double*)));
    for(short int i = 0; i < n; i++){
        /* Para cada linha aloca a quantidade de items, do tipo double, correspondente a linha */
        vetor_p[i] = (double *)malloc((i+1) * sizeof(double));
        /* Lê a quantidade de items da linha e salva no novo vetor recem alocado */
        for(short int j = 0; j <= i; j++){
            double aux;
            scanf("%lf", &aux);
            vetor_p[i][j] = aux;
            media += aux; // Dentro do for a primeira parte do cálculo da média é apenas somar todos os valores
        }
    }
    /* Finaliza o cálculo da média dividindo a soma pela quantidade de items (N^2 + N)/2 */
    media /= ((double) n * (double) n + (double) n)/(double) 2;
    /* Primeira parte do cálculo do desvio padrão, soma os quadrados das diferenças de cada item pela
    média, usando dois for para iterar entre todos elementos da matriz */
    for(short int i = 0; i < n; i++){
        for(short int j = 0; j <= i; j++){
            dp += pow(vetor_p[i][j] - media, 2);
        }
    }
    /* Finaliza o cálculo do desvio padrão dividindo a soma dos quadrados das diferenças de cada item
    pela média pela quantidade de total de items (N^2+N)/2 e tirando a raíz quadrada */
    dp = sqrt(dp / (((double) n * (double) n + (double) n)/(double) 2));

    /* Cálcula e exibe Z = (Xi - media)/dp na tela colocando um espaço caso não seja o último item da linha
    e caso tenha acabado a linha printa uma nova linha */
    for(short int i = 0; i < n; i++){
        for(short int j = 0; j <= i; j++){
            printf("%.4lf", (vetor_p[i][j]-media)/dp);
            if(j < i)
                printf(" ");
        }
        printf("\n");
    }
    printf("\n%.4lf %.4lf", media, dp);
    printf("\n");
    // Libera todos espaços de memória previamente alocados
    for(short int i = 0; i < n; i++){
        free(vetor_p[i]);
    }
    free(vetor_p);
    return 0;
}