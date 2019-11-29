#include <stdio.h>
#include <stdlib.h>
#include "stdc++.h"


float value = 0.0;
int i;
int remaining = 0;

// Resolve o problema da mochila fracionaria
void fractional_knapsack(int capacidade, int peso[], int valor[], int n){

    int curret_weight = 0;

    for(i = 0; i < n; i++){
        if(curret_weight + peso[i] <= capacidade){ //Se ainda couber o item inteiro na mochila
            curret_weight += peso[i];
            value += valor[i];
        }
        else { //Se não pega uma fraçao do item apenas
            remaining = capacidade - curret_weight;
            value += valor[i] * ((float)remaining/peso[i]);
            break;
        }
    }
}


// Funcao que ordena o array pela relacao valor/peso
void array_valor_peso(int valor[], int peso[], int n){
    float relacao[n] = {0};

    for(int i = 0; i < n; i++){
        relacao[i] = (float)valor[i]/peso[i];
    }

    int aux_peso, aux_valor;
    for(int i = 0; i < n; i++){
        if(i+1 <= n-1)
            if(relacao[i] < relacao[i+1]){ //Bubble Sort para ordenar
                // Troca dos pesos e valores
                aux_peso = peso[i];
                aux_valor = valor[i];

                peso[i] = peso[i+1];
                valor[i] = valor[i+1];

                peso[i+1] = aux_peso;
                valor[i+1] = aux_valor;
            }
    }
}


// Menu de opcoes
void menu(){
    system("@cls || clear");
    printf("1 - Inserir dados;");
    printf("\n2 - Resolver o Knapsack Problem;");
    printf("\n3 - Resultado;");
    printf("\n0 - Sair;");
    printf("\n\nEscolha a opcao desejada: ");
}


// Funcao para pausar sem usar o system
void pausa(){
    printf("\n\nPressione qualquer tecla para continuar...");
    getchar();
    getchar();
}


void resultado(int n, int capacidade, int peso[], int valor[]){
    system("@cls || clear");
    printf("Valor final: %1.2f\n", value);
    printf("Com os itens:\n");
    for(int j = 0; j <= i; j++){
        if(j != i){
            printf("Item Inteiro: Peso | Valor\n");
            printf("              %4d | %5d \n", peso[j], valor[j]);
        }
        else if(remaining != 0) {
            printf("Item Fracionado: Peso | Valor\n");
            printf("%3d/%3d do item  %4d | %5d\n", remaining, peso[j], peso[j], valor[j]);
        }
    }
}

int main(){
    int escolha = -1;
    int n, capacidade;
    int peso[100], valor[100];
    n = capacidade = 0;

    while(escolha){
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
                array_valor_peso(valor, peso, n);
                fractional_knapsack(capacidade, peso, valor, n);
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
