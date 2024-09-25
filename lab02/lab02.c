#include <stdio.h>
#include <math.h>

float pitagoras(float a, float b){
    return sqrt(a*a + b*b);
}

int coelhoFoge(float xc, float yc, float xr, float yr, float xb, float yb){
    /* Calcula a distancia do coelho pro buraco e a distancia da raposa pro buraco */
    float distancia_coelho = pitagoras(xc-xb, yc-yb);
    float distancia_raposa = pitagoras(xr-xb, yr-yb);

    /* Se a raposa estiver pelo menos 2 vezes mais longe que o coelho, ele escapa, retornando 1 */
    return (distancia_raposa/2.000 >= distancia_coelho);
}

int main(){
    /* Inicialização de Variaveis gerais */
    int n;
    char flag = 0;
    float xc, yc, xr, yr, xb, yb;

    /* Repete enquanto as entradas estiverem sendo recebidas corretamente */
    /* Leitura da quantidade de buracos, das coordenadas do coelho e da raposa*/
    while(scanf("%d %f %f %f %f", &n, &xc, &yc, &xr, &yr) == 5){
        flag = 0;

        /* Enquanto a quantidade de buracos for diferente de zero com pós decremento */
        while(n--){
            /* Leitura das coordenadas do buraco n */
            scanf("%f %f", &xb, &yb);
            /* Se o coelho não espacou antes e escapa agora mensagem e ative a flag */
            if(coelhoFoge(xc, yc, xr, yr, xb, yb) && !flag){
                printf("O coelho pode escapar pelo buraco (%.3f,%.3f).\n", xb, yb);
                flag = 1;
            }
        }
        /* Se acabaram os buracos e o coelho não escapou */
        if(!flag) printf("O coelho nao pode escapar.\n");
    }
    return 0;
}