#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <iostream>
#include "procedimento.h"
#include "paciente.h"
#include "fila.h"
using namespace std;

class MinHeap {
    paciente_ptr *heap; // ponteiro para o array que armazena o heap
    int capacity; // capacidade máxima do heap
    int size; // tamanho atual do heap

    void heapify_up(int index); //ajusta o heap quando um novo paciente é inserido
    void heapify_down(int index); //ajusta o heap quando um paciente é retirado

    int parent(int i); // função para retornar o índice do pai de um nó
    int leftChild(int i); //função para retornar o índice do filho esquerdo de um nó
    int rightChild(int i); //função para retornar o índice do filho direito de um nó
    void Leoswap(int i, int j); //implementação da função swap

public:
    MinHeap(int cap); //construtor a partir da capacidade (sempre numPacientes)
    ~MinHeap(); //destrutor

    void insert(paciente_ptr &p); //inserir um paciente no heap escalonador
    paciente_ptr removeandreturnMin(paciente_ptr &p); //remove o paciente raiz e retorna para inserção na fila apropriada
    void displayHeap(); //impressão do heap

    paciente_ptr comparaPos(const paciente_ptr &p1, const paciente_ptr &p2); //compara baseado no tempo com desempate a favor do grau maior
    bool isEmpty(); //verifica se o heap está vazio
};

#endif