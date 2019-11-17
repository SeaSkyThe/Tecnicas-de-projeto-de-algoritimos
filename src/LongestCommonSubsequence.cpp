#include "stdc++.h"
#define TAM 100
char table_simbolos[TAM][TAM]; //Variavel que ira guardar as tabelas de simbolos
// 'c' - CIMA
// 'e' - ESQUERDA
// 'd' - DIAGONAL
void print_subseq(char *A, char *B ,int size_a, int size_b){
    if(size_a == 0 || size_b == 0){
        return;
    }

    if(table_simbolos[size_a][size_b] == 'd'){ //se for diagonal vai pra diagonal esquerda
        print_subseq(A, B, size_a - 1, size_b - 1);
        printf("%c", A[size_a - 1]);
    }
    else if(table_simbolos[size_a][size_b] == 'c') // se for cima, vai pra cima
        print_subseq(A, B, size_a - 1, size_b);
    else //se nao, é esquerda, e vai para ESQUERDA
        print_subseq(A, B, size_a, size_b - 1);
}

int lcs(char* A, char* B, int size_a, int size_b){
    int table_num[size_a + 1][size_b + 1];



    //inicializando tabelas
    for(int k = 0; k <= size_a; k++) {
        for (int l = 0; l <= size_b; l++) {
            table_num[k][l] = 0;
            table_simbolos[k][l] = '\0';
        }
    }

    for(int i = 0; i <= size_a; i++){
        for(int j = 0; j <= size_b; j++){
            if(i == 0 || j == 0){
                table_num[i][j] = 0;
            }
            else if(A[i - 1] == B[j - 1]){
                table_num[i][j] = table_num[i - 1][j - 1] + 1;
                table_simbolos[i][j] = 'd';  //DIAGONAL
            }
            else{
                if(table_num[i - 1][j] >= table_num[i][j-1]){
                    table_num[i][j] = table_num[i - 1][j];
                    table_simbolos[i][j] = 'c'; //CIMA
                }
                else{
                    table_num[i][j] = table_num[i][j-1];
                    table_simbolos[i][j] = 'e';  //ESQUERDA
                }
            }

        }
    }

    //Printando tabela NUM
    printf("\n\nMATRIZ COM NUMEROS: \n\n");
    printf("    ");
    for(int k = 0; k < size_b; k++){
        printf("%2c ", B[k]);
    }
    printf("\n\n");
    for(int k = 0; k <= size_a; k++) {
        printf("%c   ", A[k - 1]);
        for (int l = 0; l <= size_b; l++) {
            if(l != 0 && k != 0)
                printf("%2d ", table_num[k][l]);
        }
        printf("\n");

    }

    //Printando tabela de simbolos
    printf("\n\nMATRIZ COM SIMBOLOS: \n\n");
    printf("   ");
    for(int k = 0; k < size_b; k++){
        printf("%2c ", B[k]);
    }
    printf("\n\n");
    for(int k = 0; k <= size_a; k++) {
        printf("%c", A[k - 1]);
        for (int l = 0; l <= size_b; l++) {
            printf("%2c ", table_simbolos[k][l]);
        }
        printf("\n");

    }
    return table_num[size_a][size_b];
}



int main(void){

    char A[50];
    char B[50];


    printf("Digite a sequencia A: ");
    scanf("%s", A);
    printf("\nDigite a sequencia B: ");
    scanf("%s", B);

    int size_a = strlen(A);
    int size_b = strlen(B);

    int tamanho_subseq = lcs(A, B, size_a, size_b);

    printf("\n\nA maior subsequencia comum e: ");
    print_subseq(A, B, size_a, size_b);
    printf("\nTamanho da maior subsequencia comum: %d\n", tamanho_subseq);

    return 0;
}
