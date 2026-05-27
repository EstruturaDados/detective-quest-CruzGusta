#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//criação da struct sala, onde é apontado os filhos da esquerda e direita
typedef struct Sala{
    
    char nome[50];
    struct Sala* direita;
    struct Sala* esquerda;
} Sala;

//função de criar sala definindo os filhos como NULL
Sala* criarSala(char nome[]){

    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

//função de explorar salas
void explorarSalas(Sala* atual){

    printf("\nVocê está em '%s'.", atual->nome);

    //verifica se o usuario está em um nó folha
    if(atual->esquerda == NULL && atual->direita == NULL){
        printf("\nVocê chegou no fim do caminho.\n");
        return;
    }

    //menu de opção 
    char opcao;
    printf("\nEscolha um caminho:\n");
    printf("e - Esquerda\n");
    printf("d - Direita\n");
    printf("s - Sair\n");
    scanf(" %c", &opcao);

    //if para escolhas
    if(opcao == 'e'){
        if(atual->esquerda != NULL){
            explorarSalas(atual->esquerda);
        }else{
            printf("Não existe caminho para esquerda.\n");
        }
    }else if(opcao == 'd'){
        if(atual->direita != NULL){
            explorarSalas(atual->direita);
        }else{
            printf("Não tem caminho para direita.\n");
        }
    }else if(opcao == 's'){
        printf("Exploração encerrada.\n");
    }
}
//função para liberar memoria com free
void liberarMapa(Sala* atual){

    if(atual == NULL){
        return;
    }
    liberarMapa(atual->esquerda);
    liberarMapa(atual->direita);

    free(atual);
}

int main() {

    //criação de 5 salas 
    Sala* hall = criarSala("Hall da entrada");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* quarto = criarSala("Quarto");
    Sala* jardim = criarSala("Jardim");

    //conexão das salas na arvore
    hall->esquerda = cozinha;
    hall->direita = biblioteca;
    cozinha->esquerda = quarto;
    cozinha->direita = jardim;

    //chamando função para explorar o lugar
    explorarSalas(hall);
    liberarMapa(hall);
    return 0;
}

