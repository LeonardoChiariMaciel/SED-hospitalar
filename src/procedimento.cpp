#include "procedimento.h"
//construtor inicializa o relógio da sala com 0.0
Sala::Sala(){
    relogio = 0.0;
}

Sala::~Sala(){
    
}

void PegaProcedimentos(procedimento_t &proc, char* nomeentrada){
    procedimento_t pacientes;
    ifstream arquivo(nomeentrada);
    //lê o arquivo e preenche as informações
    arquivo >> proc.t_triagem >> proc.num_triagem;
    arquivo >> proc.t_atendimento >> proc.num_atendimento;
    arquivo >> proc.t_medidas >> proc.num_medidas;
    arquivo >> proc.t_testes >> proc.num_testes;
    arquivo >> proc.t_imagem >> proc.num_imagem;
    arquivo >> proc.t_inst_med >> proc.num_inst_med;
    arquivo >> proc.numPacientes;
    //aloca dinamicamente as salas dos procedimentos
    proc.triagem = new Sala[proc.num_triagem];
    proc.atendimento = new Sala[proc.num_atendimento];
    proc.medidas = new Sala[proc.num_medidas];
    proc.t_laboratorio = new Sala[proc.num_testes];
    proc.e_imagem = new Sala[proc.num_imagem];
    proc.instrumentos = new Sala[proc.num_inst_med];
    arquivo.close();
}
//libera a memória
void liberarProcedimentos(procedimento_t &proc) {
    delete[] proc.triagem;
    delete[] proc.atendimento;
    delete[] proc.medidas;
    delete[] proc.t_laboratorio;
    delete[] proc.e_imagem;
    delete[] proc.instrumentos;
}
