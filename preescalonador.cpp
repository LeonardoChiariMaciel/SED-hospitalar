#include <iostream>
#include "preescalonador.h"
using namespace std;
    //construtor e destrutor:
// Construtor e Destrutor:
MinheapT::MinheapT(int cap) : capacity(cap), size(0) {
    heap = new paciente_ptr[capacity];
}
//Destrutor
MinheapT::~MinheapT() {
    delete[] heap;
}

// Troca elementos no heap
void MinheapT::LeoswapT(int i, int j) {
    paciente_ptr temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}
//Função para checar se está vazia
bool MinheapT::isEmptyT() {
    return size == 0;
}
//As três funções abaixo etornam o índice do pai, do filho esquerdo e direito 
int MinheapT::parentT(int i) { return (i - 1) / 2; }
int MinheapT::leftChildT(int i) { return (2 * i + 1); }
int MinheapT::rightChildT(int i) { return (2 * i + 2); }

// Ajuste para cima usando comparaPre
void MinheapT::heapify_upT(int index) {
    while (index != 0 && comparaPreT(heap[index], heap[parentT(index)]) == heap[index]) {
        LeoswapT(index, parentT(index)); //Troca o nó com o pai
        index = parentT(index); //Atualiza o índice para o pai
    }
}

// Ajuste para baixo usando comparaPre
void MinheapT::heapify_downT(int index) {
    int smallest = index;
    int left = leftChildT(index);
    int right = rightChildT(index);
    //Verifica se o filho esquerdo é menor que o pai
    if (left < size && comparaPreT(heap[left], heap[smallest]) == heap[left]) {
        smallest = left;
    }
    //Checa agora para o filho direito
    if (right < size && comparaPreT(heap[right], heap[smallest]) == heap[right]) {
        smallest = right;
    }
    //Se o menor nó não é o atual troca e continua ajustando
    if (smallest != index) {
        LeoswapT(index, smallest);
        heapify_downT(smallest);
    }
}

// Inserção com ajuste de heap
void MinheapT::insertT(paciente_ptr &p) {
    if (size == capacity) {
        cout << "Heap cheio. Não é possível inserir mais elementos.\n";
        return;
    }
    heap[size] = p; //insere o novo elemento no final do heap
    heapify_upT(size); //ajusta o heap
    size++;
}

// Remoção com ajuste de heap
paciente_ptr MinheapT::removeandreturnMinT(paciente_ptr &p) {
    if (size <= 0) {
        cout << "Heap vazio. Não há elementos para remover.\n";
        return nullptr;
    }
    paciente_ptr root = heap[0]; //armazena o elemento raiz
    heap[0] = heap[size - 1]; //move o último elemento para a raiz
    size--; //decrementa o tamanho do heap
    heapify_downT(0); //ajusta
    return root; //retorna o elemento removido
}

void MinheapT::displayHeapT() {
    for (int i = 0; i < size; i++) {
        cout << "Paciente ID: " << heap[i]->id << ", HoraFila: " << heap[i]->horafila << ", Grau: " << heap[i]->grau << "\n";
    }
    cout << "\n";
}

// Comparação antes da triagem
paciente_ptr MinheapT::comparaPreT(const paciente_ptr &p1, const paciente_ptr &p2) {
    if (p1->horafila < p2->horafila) return p1;
    if (p1->horafila > p2->horafila) return p2;
    int id1 = std::stoi(p1->id);
    int id2 = std::stoi(p2->id);
    return (p1->id < p2->id) ? p1 : p2;
}

