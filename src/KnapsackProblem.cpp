#include <stdio.h>
#include <stdlib.h>

int tabela[100][100];
int tabela_0_1[100][100];

void knapsack(int capacidade, int peso[], int valor[], int n){

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

}

void menu(){
    system("@cls || clear");
    printf("1 - Inserir dados;");
    printf("\n2 - Resolver o Knapsack Problem;");
    printf("\n3 - Resultado;");
    printf("\n0 - Sair;");
    printf("\n\nEscolha a opcao desejada: ");
}

void pausa(){ //Pause
    printf("\n\nPressione qualquer tecla para continuar...");
    getchar();
    getchar();
}

void resultado(int n, int capacidade, int peso[], int valor[]){
    // Tabela com valores
    system("@cls || clear");
    printf("Tabela com valores:\n\n");
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            printf(" %3d ", tabela[i][j]);
        }
        printf("\n");
    }

    printf("\n\n\nTabela de 0s e 1s:\n\n");
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= capacidade; j++){
            printf(" %3d ", tabela_0_1[i][j]);
        }
        printf("\n");
    }

    printf("\n\n\nO Valor Maximo e: %d\n\n", tabela[n][capacidade]);
    printf("Com os seguintes itens: | Peso | Valor |\n");
    int coluna_atual = capacidade;
    for(int i = n + 1; i >= 1; i--){
        if(tabela_0_1[i][coluna_atual] == 1){
            printf("                        | %4d | %5d |\n", peso[i-1], valor[i-1]);
            coluna_atual = coluna_atual - peso[i-1];
        }
    }
    printf("\nOcupando um total de %d da capacidade da mochila(%d)", capacidade - coluna_atual, capacidade);
    if(coluna_atual > 0){
        printf("\nSobrando %d de capacidade", coluna_atual);
    }
}

int main(){
    int escolha = -1;
    int n, capacidade;
    int peso[100], valor[100];
    n = capacidade = 0;
    //Dados
    while(escolha) {
        menu();
        scanf("%d", &escolha);
        switch (escolha) {

            case 1:
                system("@cls || clear");
                printf("Escolha do numero de elementos (n): ");
                scanf("%d", &n);
                for(int i = 0; i < n; i++){
                    system("@cls || clear");
                    printf("Digite o Peso e o Valor respectivamente (Peso Valor): ");
                    scanf("%d %d", &peso[i], &valor[i]);
                }
                system("@cls || clear");
                printf("Qual a capacidade da mochila? (Capacidade): ");
                scanf("%d", &capacidade);

                //Dados
                system("@cls || clear");
                printf("Numero de elementos: %d", n);
                printf("\n\n| Peso | Valor |\n");
                for(int i = 0; i < n; i++){
                    printf("| %4d | %5d |\n", peso[i], valor[i]);
                }
                printf("\n\nCapacidade da mochila: %d", capacidade);
                pausa();
            break;

            case 2:
                system("@cls || clear");
                printf("Resolvendo o problema da mochila...");
                knapsack(capacidade, peso, valor, n);
                printf("\n\nProblema resolvido :)");
                pausa();
            break;

            case 3:
                resultado(n, capacidade, peso, valor);
                pausa();
            break;
        }
    }
    system("@cls || clear");
    printf("BYE BYE :)\n\n");

}
