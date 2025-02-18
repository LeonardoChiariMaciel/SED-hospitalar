#ifndef FILA_H
#define FILA_H

#include "paciente.h"
#include "procedimento.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//TAD Fila para pacientes
typedef struct no {
    paciente_t* paciente;
    struct no *proximo;

} No;

void inserir_na_fila(No **fila, paciente_t* paciente); //insere um paciente na fila
No *remover_da_fila(No **fila); //remove o primeiro da fila e retorna o removido
void liberarFila(No **fila); //libera a fila

#endif