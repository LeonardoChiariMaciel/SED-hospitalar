#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//função para simular as salas de procedimentos
class Sala
{

public:
    double relogio;
    Sala();
    ~Sala();
};

//estrutura que armazena as informações de procedimento
typedef struct Procedimento{
    double t_triagem; //tempo de duração da triagem
    double t_atendimento; //tempo de duração do atendimento
    double t_medidas; //tempo de duração de uma medida hospitalar
    double t_testes; //tempo de duração de um teste de laboratório
    double t_imagem; //tempo de duração de um exame de imagem
    double t_inst_med; //tempo de duração de um procedimento do tipo instrumentos e medicamentos
    int num_triagem; //número de salas que realizam a triagem
    int num_atendimento; //número de salas que realizam o atendimento
    int num_medidas; //número de salas que realizam medidas hospitalares
    int num_testes; //número de salas que realizam teste de laboratório
    int num_imagem; //número de salas que realizam exames de imagem
    int num_inst_med; //número de salas que realizam o procedimento do tipo instrumentos e medicamentos
    int numPacientes; //número de pacientes no arquivo
    //salas para cada tipo de procedimento:
    Sala *triagem; 
    Sala *atendimento;
    Sala *medidas;
    Sala *t_laboratorio;
    Sala *e_imagem;
    Sala *instrumentos;
}procedimento_t, *procedimento_ptr;

void PegaProcedimentos(procedimento_t &proc, char* nomeentrada); //lê o arquivo e preenche a estrutura com as informações

void liberarProcedimentos(procedimento_t &proc); //libera memória


#endif