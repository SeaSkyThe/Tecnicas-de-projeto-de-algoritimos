#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <queue>
#include "stdc++.h"  //Essa linha pode não ser necessaria
using namespace std;
#define DIM 100

void clearScreen(char* mensagem){
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";

    char ch;
    printf("%s", mensagem);
    scanf("%c",&ch);
    getchar();

    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

struct node{
    int num_trabalho;
    int num_pessoa;
    struct node* pai;
    int custo;
    int custo_caminho;
    bool atribuidos[DIM];
};

struct node* new_node(int pessoa, int trabalho, bool atribuidos[], struct node* pai){
    struct node* node = (struct node*)malloc(sizeof(struct node));
    for(int i = 0; i < DIM; i++){
        node -> atribuidos[i] = atribuidos[i];
    }
    node -> atribuidos[trabalho] = true;

    node -> pai = pai;
    node -> num_pessoa = pessoa;
    node -> num_trabalho = trabalho;

    return node;
}

//funcao que calcula o provavel menor custo depois que uma pessoa foi atribuida a um trabalho, ou seja, calcula todos depois dessa atribuição
int calcular_custo(int matrizCusto[DIM][DIM], int pessoa, int trabalho, bool atribuidos[], int tamanho){
    int custo = 0;

    //armazenar trabalhos indisponiveis e disponiveis
    bool disponiveis[DIM] = {true};

    //para cada pessoa
    for(int i = pessoa + 1; i < tamanho; i++){
        int min = INT_MAX; //custo minimo
        int min_indice = -1;
        //para cada trabalho
        for(int j = 0; j < tamanho; j++){
            //se o trabalho nao foi atribuidos ainda
            if(!atribuidos[j] && disponiveis[j] && matrizCusto[i][j] < min){
                //guarda o numero do trabalho
                min_indice = j;
                min = matrizCusto[i][j];
            }
        }
        //incrementa o custo
        custo += min;
        //torna o trabalho utilizado indisponivel
        disponiveis[min_indice] = false;
    }
    return custo;
}


struct comp{
    bool operator()(const struct node* left, const struct node* right)const{
        return left -> custo > right -> custo;
    }
};

void print_atribuicoes(struct node* minimo){
    if(minimo -> pai == NULL)
        return;
    print_atribuicoes(minimo -> pai);
    printf(" Trabalhador:  %d | Tarefa:  %d\n", minimo -> num_trabalho, minimo -> num_pessoa);
}

int encontrar_menor_custo(int matrizCusto[DIM][DIM], int tamanho){
    //criando fila de prioridade para armazenar os nós ativos da arvore de busca
    priority_queue<struct node*, std::vector<struct node*>, comp>fila_prioridade;

    //inicializando heap com raiz de custo 0
    bool atribuidos[DIM] = {false};
    struct node* raiz = new_node(-1, -1, atribuidos, NULL);
    raiz -> custo_caminho = raiz -> custo = 0;
    raiz -> num_pessoa = -1;

    fila_prioridade.push(raiz);

    while(!fila_prioridade.empty()){
        //Pega o nó ativo com menor custo estimado
        struct node* min = fila_prioridade.top();
        //O nó que foi pego é removido da lista de nós ativos
        fila_prioridade.pop();

        // i guarda a proxima pessoa
        int i = min -> num_pessoa + 1;
        //se todos as pessoas ja receberam trabalho
        if(i == tamanho){
            print_atribuicoes(min);
            return min -> custo;
        }
        //para cada trabalho
        for(int j = 0; j < tamanho; j++){
            //se nao foi atribuido
            if(!min->atribuidos[j]){
                //cria um nó novo
                struct node* filho = new_node(i, j, min -> atribuidos, min);
                //custo dos anteriores + o novo nó
                filho -> custo_caminho = min -> custo_caminho + matrizCusto[i][j];
                filho -> custo = filho -> custo_caminho + calcular_custo(matrizCusto, i, j, filho -> atribuidos, tamanho);
                //insere esse novo nó (filho) na lista de nós ativos
                fila_prioridade.push(filho);
            }
        }

    }
    return 0;
}
int main() {


    int matriz_custo[4][4] =
    {
        {82, 83, 69, 92},
        {77, 37, 49, 92},
        {11, 69,  5, 86},
        { 8,  9, 98, 23}
    };

    printf("\n\nEXEMPLO DE MATRIZ DE TAREFAS: \n\n");
    printf("             ");
    for(int k = 0; k < 4; k++){
        printf("Job %d   ", k);
    }
    printf("\n\n");
    for(int i = 0; i < 4; i++){
        printf("Pessoa %d ", i);
        for(int j = 0; j < 4; j++){
            printf("%8d", matriz_custo[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n\n");
    int tamanho;
    printf("\n\n");

    printf("Qual sera a dimensao da sua matriz de custos? ");
    scanf("%d", &tamanho);
    printf("\n\n");
    int matriz_usuario[DIM][DIM];
    //inicializa matriz
    for(int l = 0; l < tamanho; l++){
        for(int m = 0; m < tamanho; m++){
            matriz_usuario[l][m] = 0;
        }
    }
    int temp;
    for(int i = 0; i < tamanho; i++){
        for(int j = 0; j < tamanho; j++){
            //printa matriz atual
            printf("            ");
            for(int k = 0; k < tamanho; k++)
                printf("Job %d   ", k);
            printf("\n\n");
            for(int l = 0; l < tamanho; l++){
                printf("Pessoa %d ", l);
                for(int m = 0; m < tamanho; m++){
                    if(matriz_usuario[l][m] == 0){
                        printf("        ");
                    }
                    else
                        printf("%8d", matriz_usuario[l][m]);
                }
                printf("\n");
            }
            printf("\nSeguindo o exemplo, por favor insira um numero na matriz Pessoa X Trabalho\n");
            printf("Lembrando: os numeros serao inseridos na matriz sempre a direita, ate o fim da linha: ");
            scanf("%d", &temp);
            matriz_usuario[i][j] = temp;
            printf("\n\n%d %d \n\n", i, j);
            clearScreen("Aperte enter para ir para a inserção do prox numero...");
        }
    }



    printf("\n\nMELHOR CUSTO: %d\n", encontrar_menor_custo(matriz_usuario, tamanho));
    return 1;


}
