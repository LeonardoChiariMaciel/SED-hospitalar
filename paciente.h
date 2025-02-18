#ifndef PACIENTE_H
#define PACIENTE_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "procedimento.h"

using namespace std;

// TAD Paciente
typedef struct Paciente {
    string id; // Identificação do paciente
    int alta; // 0 = não recebeu alta, 1 = recebeu alta
    int anoin, mesin, diain, horain; // Data de entrada no hospital
    int anoout, mesout, diaout, horaout; // Data de saída do hospital
    int grau; // Grau de prioridade
    int mh, tl, ei, im; // Número de medidas, testes, imagens e instrumentos médicos
    double tempoAtendimento = 0.0; // Tempo total nos procedimentos
    double tempoEspera = 0.0; // Tempo de espera em filas
    double tempoTotal = 0.0; // Soma dos dois tempos anteriores
    double horafila = 0.0; // Hora em que o paciente entra/volta pra fila
    int estadoAtual; // Estado atual do paciente no processo
    int triado; // 0 = não triado, 1 = triado
    int atendido; // 0 = não atendido, 1 = atendido
    int horaentrada; // Hora de entrada no hospital medida a partir do tempo 0 determinado pela chegada do primeiro paciente
    double tmin; // Tempo mínimo de permanência no hospital considerando 0 tempo de espera
} paciente_t, *paciente_ptr;

paciente_ptr Cria(int TAM); // Cria vetor de pacientes
void Destroi(paciente_ptr pacientes); // Destroi vetor de pacientes
void carregaPacientes(paciente_ptr pacientes, int TAM, const char* nomeentrada, procedimento_t pr); // Carrega pacientes do arquivo
double calculaTempoContinuo(int ano, int mes, int dia, int hora, paciente_ptr pacientes); // Calcula tempo contínuo
void ContabilizaTempo(int numproc, procedimento_t pr, paciente_t& paciente); // Contabiliza tempos de espera e atualiza 'horafila' utilizando o relogio da sala

// Funções e ferramentas para a formatação da saída:
enum Mes {
    JANEIRO = 1, FEVEREIRO, MARCO, ABRIL, MAIO, JUNHO,
    JULHO, AGOSTO, SETEMBRO, OUTUBRO, NOVEMBRO, DEZEMBRO
};
string diaSemana(int year, int month, int day); // Descobre o dia da semana de entrada do paciente
string nomeDoMes(int numero); // Retorna as três primeiras letras do mês de entrada
string converterHoras(double horasDecimais); // Converte o horário de double para o formato desejado
void novaData(int year, int month, int day, int hour, double elapsedHours, int result[3], double &resultHour); // Função para obter a data de saída do hospital a partir da data de entrada e do tempo decorrido

// Função de ordenação de pacientes usando o algoritmo mergesort
void merge(paciente_ptr pacientes, int left, int mid, int right);
void mergeSort(paciente_ptr pacientes, int left, int right);
void OrdenaPacientes(paciente_ptr pacientes, int numPacientes);

// Função para ordenar a saída pelo ID
void mergeById(paciente_ptr pacientes, int left, int mid, int right);
void mergeSortById(paciente_ptr pacientes, int left, int right);
void OrdenaPacientesPorId(paciente_ptr pacientes, int numPacientes);

#endif