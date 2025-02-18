#ifndef PREESCALONADOR_H
#define PREESCALONADOR_H

#include <iostream>
#include "procedimento.h"
#include "paciente.h"
#include "fila.h"

using namespace std;

class MinheapT {
    paciente_ptr *heap; // ponteiro para o array que armazena o heap
    int capacity; // capacidade máxima do heap
    int size; // tamanho atual do heap

    void heapify_upT(int index); //ajusta o heap quando um novo paciente é inserido
    void heapify_downT(int index); //ajusta o heap quando um paciente é retirado


    int parentT(int i); // função para retornar o índice do pai de um nó
    int leftChildT(int i); //função para retornar o índice do filho esquerdo de um nó
    int rightChildT(int i); //função para retornar o índice do filho direito de um nó
    void LeoswapT(int i, int j); //implementação da função swap

public:
    MinheapT(int cap); //construtor a partir da capacidade (sempre numPacientes)
    ~MinheapT(); //destrutor

    void insertT(paciente_ptr &p); //inserir um paciente no heap escalonador
    paciente_ptr removeandreturnMinT(paciente_ptr &p); //remove o paciente raiz e retorna para inserção na fila apropriada
    void displayHeapT(); //Impressão do heap

    paciente_ptr comparaPreT(const paciente_ptr &p1, const paciente_ptr &p2); //compara baseado no tempo com desempate a favor do ID menor
    bool isEmptyT(); //verifica se o heap está vazio
};

#endif