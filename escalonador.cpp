#include <iostream>
#include "procedimento.h"
#include "paciente.h"
#include "fila.h"
#include "escalonador.h"

using namespace std;
    //construtor e destrutor:
// Construtor e Destrutor:
MinHeap::MinHeap(int cap) : capacity(cap), size(0) {
    heap = new paciente_ptr[capacity];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

// Troca elementos no heap
void MinHeap::Leoswap(int i, int j) {
    paciente_ptr temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}
//Função para checar se está vazio
bool MinHeap::isEmpty() {
    return size == 0;
}
//Funções para encontrar os índices do pai, filho esquerdo e direito:
int MinHeap::parent(int i) { return (i - 1) / 2; }
int MinHeap::leftChild(int i) { return (2 * i + 1); }
int MinHeap::rightChild(int i) { return (2 * i + 2); }

// Ajuste para cima usando comparaPos
void MinHeap::heapify_up(int index) {
    while (index != 0 && comparaPos(heap[index], heap[parent(index)]) == heap[index]) {
        Leoswap(index, parent(index)); //troca com o pai
        index = parent(index); //atualiza o índice para o pai
    }
}

// Ajuste para baixo usando comparaPos
void MinHeap::heapify_down(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    //Verifica se o filho esquerdo é menor que o atual
    if (left < size && comparaPos(heap[left], heap[smallest]) == heap[left]) {
        smallest = left;
    }
    //Faz o mesmo para o filho direito
    if (right < size && comparaPos(heap[right], heap[smallest]) == heap[right]) {
        smallest = right;
    }
    //Se o menor não é o atual troca e chama recursivamente
    if (smallest != index) {
        Leoswap(index, smallest);
        heapify_down(smallest);
    }
}

// Inserção com ajuste de heap
void MinHeap::insert(paciente_ptr &p) {
    if (size == capacity) {
        cout << "Heap cheio. Não é possível inserir mais elementos.\n";
        return;
    }
    heap[size] = p; //Insere o novo elemento no final do heap
    heapify_up(size); //Ajusta o heap
    size++;
}

// Remoção com ajuste de heap
paciente_ptr MinHeap::removeandreturnMin(paciente_ptr &p) {
    //Remove o paciente raiz do minheap e retira ele da árvore
    if (size <= 0) {
        cout << "Heap vazio. Não há elementos para remover.\n";
        return nullptr;
    }
    paciente_ptr root = heap[0]; //Armazena o elemento raiz
    heap[0] = heap[size - 1]; //Move o último elemento para a raiz
    size--; //Decrementa o tamanho
    heapify_down(0); //Ajusta o heap
    return root; //Retorna o elemento removido
}

void MinHeap::displayHeap() {
    //Função para imprimir o heap
    for (int i = 0; i < size; i++) {
        cout << "Paciente ID: " << heap[i]->id << ", HoraFila: " << heap[i]->horafila << ", Grau: " << heap[i]->grau << "\n";
    }
    cout << "\n";
}

// Comparação após a triagem com prioridade
paciente_ptr MinHeap::comparaPos(const paciente_ptr &p1, const paciente_ptr &p2) {
    if (p1->grau > p2->grau) return p1;
    if (p1->grau < p2->grau) return p2;
    if (p1->horafila < p2->horafila) return p1;
    if (p1->horafila > p2->horafila) return p2;
    int id1 = std::stoi(p1->id);
    int id2 = std::stoi(p2->id);
    return (id1 < id2) ? p1 : p2;
}
