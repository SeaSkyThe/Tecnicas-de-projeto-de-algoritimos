#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
using namespace std;
#define DIM 4



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

int calcular_custo(int matrizCusto[DIM][DIM], int pessoa, int trabalho, bool atribuidos[]){
    int custo = 0;

    //armazenar trabalhos indisponiveis e disponiveis
    bool disponiveis[DIM] = {true};

    //para cada pessoa
    for(int i = pessoa + 1; i < DIM; i++){
        int min = INT_MAX; //custo minimo
        int min_indice = -1;
        //para cada trabalho
        for(int j = 0; j < DIM; j++){
            //se o trabalho nao foi atribuidos ainda
            if(!atribuidos[j] && disponiveis[j] && matrizCusto[i][j] < min){
                //guarda o numero do trabalho
                min_indice = j;
                min = matrizCusto[i][j];
            }
        }
        //incrementa o custo
        custo += min;
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
    printf("Trabalhador:  %d | Tarefa:  %d\n", minimo -> num_trabalho, minimo -> num_pessoa);
}

int encontrar_menor_custo(int matrizCusto[DIM][DIM]){
    //criando fila de prioridade
    priority_queue<struct node*, std::vector<struct node*>, comp>fila_prioridade;

    bool atribuidos[DIM] = {false};
    struct node* raiz = new_node(-1, -1, atribuidos, NULL);
    raiz -> custo_caminho = raiz -> custo = 0;
    raiz -> num_pessoa = -1;

    fila_prioridade.push(raiz);

    while(!fila_prioridade.empty()){
        struct node* min = fila_prioridade.top();
        fila_prioridade.pop();

        int i = min -> num_pessoa + 1;
        //se todos as pessoas ja receberam trabalho
        if(i == DIM){
            print_atribuicoes(min);
            return min -> custo;
        }
        //para cada trabalho
        for(int j = 0; j < DIM; j++){
            //se nao foi atribuido
            if(!min->atribuidos[j]){
                struct node* filho = new_node(i, j, min -> atribuidos, min);
                filho -> custo_caminho = min -> custo_caminho + matrizCusto[i][j];
                filho -> custo = filho -> custo_caminho + calcular_custo(matrizCusto, i, j, filho -> atribuidos);

                fila_prioridade.push(filho);
            }
        }

    }
    return 0;
}
int main() {

    int matriz_custo[DIM][DIM] =
    {
        {82, 83, 69, 92},
        {77, 37, 49, 92},
        {11, 69,  5, 86},
        { 8,  9, 98, 23}
    };

    printf("O MELHOR CUSTO E: %d\n",encontrar_menor_custo(matriz_custo));
    return 1;


}
