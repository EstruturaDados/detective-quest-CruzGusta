#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//++++++++++ structs ++++++++++

//criação da struct sala, onde é apontado os filhos da esquerda e direita
typedef struct Sala{
    
    char nome[50];
    struct Sala* direita;
    struct Sala* esquerda;
} Sala;

//criação da struct do BST para as pistas
typedef struct Pista{

    char texto[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

//criação do nó da tabela hash
typedef struct HashNode{

    char pista[50];
    char suspeito[50];

    struct HashNode* proximo;
} HashNode;

//criação da struct de tabela hash
typedef struct Hash{

    HashNode* tabela[10];
} Hash;

//++++++++++ prototipos ++++++++++

int funcaoHash(char pista[]);
void inserirNaHash(Hash* hash, char pista[], char suspeito[]);

//++++++++++  funções +++++++++++

//função de criar sala definindo os filhos como NULL
Sala* criarSala(char nome[]){

    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}
//função de criar pista
Pista* criarPista(char texto[]){

    Pista* novaPista = (Pista*) malloc(sizeof(Pista));
    strcpy(novaPista->texto, texto);

    novaPista->direita = NULL;
    novaPista->esquerda = NULL;
    return novaPista;
}

//função para inserir pista 
Pista* inserirPista(Pista* raiz, char texto[]){

    //verifica se a arvore está vazia:
    if(raiz==NULL){
        
        return criarPista(texto);
    }
    //ordem alfabetica
    if(strcmp(texto, raiz->texto) < 0){

        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    }else if(strcmp(texto, raiz->texto) > 0){

        raiz->direita = inserirPista(raiz->direita, texto);
    }
    return raiz;
}

//função para deixar as pistas em ordem
void emOrdem(Pista* raiz){

    if(raiz != NULL){
        emOrdem(raiz->esquerda);// aponta para o filho da esquerda
        printf("- %s\n", raiz->texto);// aponta para o nó
        emOrdem(raiz->direita);// aponta para o filho da direita
    }
}

//função de explorar salas
//modificada para permitir que a função adicione pistas na BST
void explorarSalas(Sala* atual, Pista** pistas, Hash* hash){

    printf("\nVocê está em '%s'.", atual->nome);

    if(strcmp(atual->nome, "Biblioteca") == 0){

        *pistas = inserirPista(*pistas, "Livro antigo");
        inserirNaHash(hash, "Livro antigo", "João");
        printf("\nVocê encontrou uma pista: 'Livro antigo'\n");
    }

    if(strcmp(atual->nome, "Jardim") == 0){

        *pistas = inserirPista(*pistas, "Chave enferrujada");
        inserirNaHash(hash, "Chave enferrujada", "Maria");
        printf("\nVocê encontrou uma pista: 'Chave enferrujada'\n");
    }

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
            explorarSalas(atual->esquerda, pistas, hash);
        }else{
            printf("Não existe caminho para esquerda.\n");
        }
    }else if(opcao == 'd'){
        if(atual->direita != NULL){
            explorarSalas(atual->direita, pistas, hash);
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

//função para limpar a BST de pistas
void limparPistas(Pista* raiz){

    if(raiz == NULL){
        return;
    }

    limparPistas(raiz->esquerda);
    limparPistas(raiz->direita);

    free(raiz);
}

//função para limpar hash
void limparHash(Hash* hash){

    for(int i = 0; i < 10; i++){

        HashNode* atual = hash->tabela[i];
        while(atual != NULL){

            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    free(hash);
}

//criação da função para inicializar Hash
Hash* criarHash(){

    Hash* novaHash = (Hash*)malloc(sizeof(Hash));

    for(int i = 0; i < 10; i++){ //for para percorrer a tabela de 10 itens

        novaHash->tabela[i] = NULL;
    }
    return novaHash;
}

//função para transformar texto em indice da tabela
int funcaoHash(char pista[]){

    int soma = 0;

    for(int i = 0; pista[i] != '\0'; i++){
        soma+=pista[i];
    }
    return soma % 10;
}

//função para armazenar e evitar colisão com o encapsulamento
void inserirNaHash(Hash* hash, char pista[], char suspeito[]){

    int indice = funcaoHash(pista);

    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->proximo = hash->tabela[indice];
    hash->tabela[indice] = novo;
}

//função parar associar a pista ao suspeito e mostrar
void associar(Hash* hash){

    for(int i = 0; i < 10; i++){
        HashNode*atual = hash->tabela[i];

        while(atual != NULL){

            printf("Pista: %s -> Suspeito: %s\n", atual->pista, atual->suspeito);

            atual = atual-> proximo;
        }
    }
}

//função para encontrar o suspeito mais citado
void suspeitoMaisCitado(Hash* hash){

    char suspeitoMais[50] = "";
    int maiorQuantidade = 0;

    for(int i = 0; i < 10; i++){

        HashNode* atual = hash->tabela[i];

        while(atual != NULL){

            int contador = 0;

            for(int j = 0; j < 10; j++){

                HashNode* aux = hash->tabela[j];

                while(aux != NULL){

                    if(strcmp(atual->suspeito, aux->suspeito) == 0){
                        contador++;
                    }

                    aux = aux->proximo;
                }
            }

            if(contador > maiorQuantidade){

                maiorQuantidade = contador;
                strcpy(suspeitoMais, atual->suspeito);
            }

            atual = atual->proximo;
        }
    }

    printf("\nSuspeito mais citado: %s (%d pista(s))\n",
           suspeitoMais,
           maiorQuantidade);
}
//+++++ main +++++

int main() {

    Pista* pistas = NULL;

    Hash* hash = criarHash();

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
    explorarSalas(hall, &pistas, hash);

    printf("\nPistas encontradas: \n");
    emOrdem(pistas);

    printf("\nAssociacoes encontradas:\n");
    associar(hash);

    suspeitoMaisCitado(hash);

    liberarMapa(hall);
    limparPistas(pistas);
    limparHash(hash);
    return 0;
}