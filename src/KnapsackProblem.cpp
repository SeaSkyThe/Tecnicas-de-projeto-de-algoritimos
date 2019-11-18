#include <stdio.h>
#include <stdlib.h>

void knapsack(int capacidade, int peso[], int valor[], int n){
    int tabela[n+1][capacidade+1];
    int tabela_0_1[n+1][capacidade+1];

// Inicialização da tabela
// ----------------------------------------------------------------------------
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            tabela[i][j] = 0;
            tabela_0_1[i][j] = 0;
        }
    }

// ----------------------------------------------------------------------------
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            if(i==0 || j == 0)
                tabela[i][j] = 0;
            else if((peso[i-1] <= j) && (valor[i - 1] + tabela[i - 1][j - peso[i - 1]] > tabela[i - 1][j])){
                tabela[i][j] = valor[i - 1] + tabela[i - 1][j - peso[i - 1]];
                tabela_0_1[i][j] = 1;
            }
            else{
                tabela[i][j] = tabela[i - 1][j];
                tabela_0_1[i][j] = 0;
            }
        }
    }


// Print Tabela
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            printf(" %2d ", tabela[i][j]);
        }
        printf("\n");
    }

printf("\n\n");
// Print Tabela_0_1
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            printf(" %2d ", tabela_0_1[i][j]);
        }
        printf("\n");
    }

// Print Do resultado
printf("\nO Valor Maximo e: %d\n\n", tabela[n][capacidade]);
printf("Com os seguintes itens: | Valor | Peso |\n");
int coluna_atual = capacidade;
for(int i = n + 1; i >= 1; i--){
    if(tabela_0_1[i][coluna_atual] == 1){
        printf("                        | %5d | %4d |\n", valor[i-1], peso[i-1]);
        coluna_atual = coluna_atual - peso[i-1];
    }
}
printf("\nOcupando um total de %d da capacidade da mochila(%d)", capacidade - coluna_atual, capacidade);
if(coluna_atual > 0){
    printf("\nSobrando %d de capacidade", coluna_atual);
}
printf("\n\n");
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
}

int main(){
    // 1)
        // int n = 4;
        // int peso[] = {4, 2, 1, 3};
        // int valor[]= {50, 40, 30, 45};
        // int capacidade = 5;
    // ----------------------------------------------------------------------------
    //
    // 2)
        // int n = 7;
        // int peso[]= {4, 6, 5, 7, 3, 1, 6};
        // int valor[] = {12, 10, 8, 11, 14, 7, 9};
        // int capacidade = 18;
    // // ----------------------------------------------------------------------------

    // 3)
        int n = 6;
        int peso[]= {1, 2, 3, 5, 6, 8};
        int valor[] = {3, 6, 7, 9, 11, 18};
        int capacidade = 15;
    // ----------------------------------------------------------------------------
    knapsack(capacidade, peso, valor, n);
}
