#include "fila.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void inserir_na_fila(No **fila, paciente_t* paciente) {
    No *novo = new No{paciente, nullptr}; // Cria novo nó

    if (*fila == nullptr) { // Se a fila está vazia
        *fila = novo; // Novo nó se torna a frente
    } else {
        No *aux = *fila; // Percorre até o final
        while (aux->proximo) {
            aux = aux->proximo;
        }
        aux->proximo = novo; // Insere no final
    }
}

No *remover_da_fila(No **fila) {
    if (*fila == nullptr) return nullptr; //Caso em que a fila está vazia
    No *removido = *fila; //Armazena o nó a ser removido
    *fila = (*fila)->proximo; //Atualiza o ponteiro da fila
    return removido; //Retorna o removido
}

void liberarFila(No **fila) {
    while (*fila) {
        No *aux = *fila; //Armazena o nó atual
        *fila = (*fila)->proximo; //Atualiza o nó para o proximo
        delete aux; //Libera a memória do nó atual
    }
}

