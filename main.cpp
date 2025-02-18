#include <iostream>
#include <fstream>
#include "procedimento.h"
#include "paciente.h"
#include "fila.h"
#include "escalonador.h"
#include "preescalonador.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "ERRO: NOME DO ARQUIVO NAO FORNECIDO" << std::endl;
        return 1;
    }

    char* nomeArquivo = argv[1];
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cout << "ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO" << std::endl;
        return 1;
    }
    //Carregar dados de procedimento do arquivo de entrada
    procedimento_t procedimentos; 
    PegaProcedimentos(procedimentos, (nomeArquivo));

    int numPacientes = procedimentos.numPacientes; //armazena o número total de pacientes na variável numPacientes
    paciente_ptr pacientes = Cria(numPacientes); //Cria vetor de pacientes (ponteiro pra paciente)
    carregaPacientes(pacientes, numPacientes, nomeArquivo, procedimentos); //preenche o vetor com os pacientes fornecidos no arquivo
    OrdenaPacientes(pacientes, numPacientes); //Ordena os pacientes por ordem de chegada usando o mergesort

    for (int i = 0; i < numPacientes; i++){
        pacientes[i].tempoTotal = pacientes[i].tmin;
        pacientes[i].tempoAtendimento = pacientes[i].tmin;
    }
    //Escalonadores para cada tipo de procedimento:
    MinheapT triagem(numPacientes); //organiza por hora com desempate pelo ID
    MinHeap atendimento(numPacientes); //os demais organizam pelo grau com desempate pela hora
    MinHeap medidas(numPacientes);
    MinHeap testes(numPacientes);
    MinHeap imagem(numPacientes);
    MinHeap inst_med(numPacientes);
    //Declarar um minheap bastaria para o funcionamento deste código, mas a utilização de vários e com nomes diferentes facilita a compreensão

    //criar as filas de prioridade para cada procedimento
    No *fila_triagem0 = nullptr;
    No *fila_atendimento0 = nullptr;
    No *fila_atendimento1 = nullptr;
    No *fila_atendimento2 = nullptr;
    No *fila_medidas0 = nullptr;
    No *fila_medidas1 = nullptr;
    No *fila_medidas2 = nullptr;
    No *fila_testes0 = nullptr;
    No *fila_testes1 = nullptr;
    No *fila_testes2 = nullptr;
    No *fila_imagem0 = nullptr;
    No *fila_imagem1 = nullptr;
    No *fila_imagem2 = nullptr;
    No *fila_inst_med0 = nullptr;
    No *fila_inst_med1 = nullptr;
    No *fila_inst_med2 = nullptr;

    //Declarar e inicializar as salas para cada procedimento
    Sala* salas_triagem = new Sala[procedimentos.num_triagem];
    Sala* salas_atendimento = new Sala[procedimentos.num_atendimento];
    Sala* salas_medidas = new Sala[procedimentos.num_medidas];
    Sala* salas_testes = new Sala[procedimentos.num_testes];
    Sala* salas_imagem = new Sala[procedimentos.num_imagem];
    Sala* salas_inst_med = new Sala[procedimentos.num_inst_med];

    
    for (int i = 0; i < numPacientes; i++){
        //Nesse momento não estamos inserindo os pacientes DEFINITIVAMENTE na fila de triagem
        //Essa é uma etapa anterior ao escalonador
        inserir_na_fila(&fila_triagem0, &pacientes[i]);
    }
    while(fila_triagem0){
        No* paciente_atual = remover_da_fila(&fila_triagem0); //Enquanto alguém estiver na fila provisória de triagem remove
        paciente_t* paciente = paciente_atual->paciente;
        triagem.insertT(paciente); //Insere o paciente removido no minheapT
        delete paciente_atual;
    }
    while(!triagem.isEmptyT()){
        //Esse loop esvazia o minheapT e insere na fila única de triagem
        paciente_ptr paciente = triagem.removeandreturnMinT(paciente);
        inserir_na_fila(&fila_triagem0, paciente);
    }
    
    No* paciente_atual = nullptr; //declara um ponteiro pra fila
    while((paciente_atual = remover_da_fila(&fila_triagem0)) != nullptr){ //Enquanto tiver pacientes na fila faça:
        //O paciente atual recebe o paciente que foi removido da fila
        paciente_t* paciente = paciente_atual->paciente; //Declara um ponteiro para paciente e aponta ele para o paciente atual
        ContabilizaTempo(1, procedimentos, *paciente); //Função que simula o atendimento
        atendimento.insert(paciente); //Insere no próximo minheap
        delete paciente_atual;
    }

    while(!atendimento.isEmpty()){
        //Esse loop esvazia o minheap do atendimento e insere na fila de acordo com o grau do paciente
        paciente_ptr paciente = atendimento.removeandreturnMin(paciente);
        if(paciente->grau == 2){
            inserir_na_fila(&fila_atendimento2, paciente);
        }
        else if(paciente->grau == 1){
            inserir_na_fila(&fila_atendimento1, paciente);
        }
        else{
            inserir_na_fila(&fila_atendimento0, paciente);
        }
    }
        
        while (fila_atendimento2 || fila_atendimento1 || fila_atendimento0) {
        //Esse loop busca o próximo paciente a ser atendido
        No* paciente_atual = nullptr;
            double minimo = salas_atendimento[0].relogio;
            int indice = 0;
            for (int i = 0; i < procedimentos.num_atendimento; i++){
                //Descobre a sala com menor tempo para a realização do exame
                if (minimo > salas_atendimento[i].relogio){
                    minimo = salas_atendimento[i].relogio;
                    indice = i;
                }
            }
        //primeiro vamos checar se existe um paciente com horafila menor ou igual ao relogio da sala na fila 2
        if (fila_atendimento2 && (fila_atendimento2->paciente->horafila <= salas_atendimento[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_atendimento2);
        } else if (fila_atendimento1 && (fila_atendimento1->paciente->horafila <= salas_atendimento[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_atendimento1);
        } else if (fila_atendimento0 && (fila_atendimento0->paciente->horafila <= salas_atendimento[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_atendimento0);
        } else {
            //se nenhuma das filas tiver um paciente com horafila menor ou igual ao relogio da sala, procurar o menor tempo dentre todas as filas
            No* menor_paciente = nullptr;
            if (fila_atendimento2) menor_paciente = fila_atendimento2;
            if (fila_atendimento1 && (!menor_paciente || fila_atendimento1->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_atendimento1;
            if (fila_atendimento0 && (!menor_paciente || fila_atendimento0->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_atendimento0;

            if (menor_paciente) {
                //Descobre de qual fila é o menor tempo global
                if (menor_paciente == fila_atendimento2) {
                    paciente_atual = remover_da_fila(&fila_atendimento2);
                } else if (menor_paciente == fila_atendimento1) {
                    paciente_atual = remover_da_fila(&fila_atendimento1);
                } else if (menor_paciente == fila_atendimento0) {
                    paciente_atual = remover_da_fila(&fila_atendimento0);
                }
            }
        }
        
        if (paciente_atual) {
            //Simula o atendimento do paciente atual
            paciente_t* paciente = paciente_atual->paciente;
            ContabilizaTempo(2, procedimentos, *paciente);
            //inserir no minheap de medidas após o atendimento, apenas se alta = 0
            if (paciente->alta == 0) {
                medidas.insert(paciente);
            }
            delete paciente_atual;
        }
    }

    while(!medidas.isEmpty()){
        //Esvazia o minheap de medidas e insere os pacientes na fila correta
        paciente_ptr paciente = medidas.removeandreturnMin(paciente);
        if(paciente->grau == 2){
            inserir_na_fila(&fila_medidas2, paciente);
        }
        else if(paciente->grau == 1){
            inserir_na_fila(&fila_medidas1, paciente);
        }
        else{
            inserir_na_fila(&fila_medidas0, paciente);
        }
    }



        while (fila_medidas2 || fila_medidas1 || fila_medidas0) {
        //Busca o próximo paciente a ser atendido
        No* paciente_atual = nullptr;
            double minimo = salas_medidas[0].relogio;
            int indice = 0;
            for (int i = 0; i < procedimentos.num_medidas; i++){
                //Descobre a sala com menor tempo para a realização do exame
                if (minimo > salas_medidas[i].relogio){
                    minimo = salas_medidas[i].relogio;
                    indice = i;
                }
            }
        //primeiro vamos checar se existe um paciente com horafila menor ou igual ao relogio da sala na fila 2
        if (fila_medidas2 && (fila_medidas2->paciente->horafila <= salas_medidas[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_medidas2);
        } else if (fila_medidas1 && (fila_medidas1->paciente->horafila <= salas_medidas[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_medidas1);
        } else if (fila_medidas0 && (fila_medidas0->paciente->horafila <= salas_medidas[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_medidas0);
        } else {
            //se nenhuma das filas tiver um paciente com horafila menor ou igual ao relogio da sala, procurar o menor tempo dentre todas as filas
            No* menor_paciente = nullptr;
            if (fila_medidas2) menor_paciente = fila_medidas2;
            if (fila_medidas1 && (!menor_paciente || fila_medidas1->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_medidas1;
            if (fila_medidas0 && (!menor_paciente || fila_medidas0->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_medidas0;

            if (menor_paciente) {
                //Descobre de qual fila é o menor paciente
                if (menor_paciente == fila_medidas2) {
                    paciente_atual = remover_da_fila(&fila_medidas2);
                } else if (menor_paciente == fila_medidas1) {
                    paciente_atual = remover_da_fila(&fila_medidas1);
                } else if (menor_paciente == fila_medidas0) {
                    paciente_atual = remover_da_fila(&fila_medidas0);
                }
            }
        }

        if (paciente_atual) {
            //Simula o atendimento do paciente
            paciente_t* paciente = paciente_atual->paciente;
            ContabilizaTempo(3, procedimentos, *paciente);
            //inserir no minheap de testes após o medidas
            testes.insert(paciente);
            
            delete paciente_atual;
        }
    }

    // Processar testes de laboratório
    while(!testes.isEmpty()){
        //Esvazia o minheap de teste e aloca os pacientes nas filas correspondentes
        paciente_ptr paciente = testes.removeandreturnMin(paciente);
        if(paciente->grau == 2){
            inserir_na_fila(&fila_testes2, paciente);
        }
        else if(paciente->grau == 1){
            inserir_na_fila(&fila_testes1, paciente);
        }
        else{
            inserir_na_fila(&fila_testes0, paciente);
        }
    }

    while (fila_testes2 || fila_testes1 || fila_testes0) {
        //Enquanto as filas não estiverem vazias, repete o processo
        No* paciente_atual = nullptr;
        double minimo = salas_testes[0].relogio;
        int indice = 0;
        for (int i = 0; i < procedimentos.num_testes; i++){
            //Descobre a sala com menor tempo para a realização do exame
            if (minimo > salas_testes[i].relogio){
                minimo = salas_testes[i].relogio;
                indice = i;
            }
        }
        //primeiro vamos checar se existe um paciente com horafila menor ou igual ao relogio da sala na fila 2
        if (fila_testes2 && (fila_testes2->paciente->horafila <= salas_testes[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_testes2);
        } else if (fila_testes1 && (fila_testes1->paciente->horafila <= salas_testes[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_testes1);
        } else if (fila_testes0 && (fila_testes0->paciente->horafila <= salas_testes[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_testes0);
        } else {
            //se nenhuma das filas tiver um paciente com horafila menor ou igual ao relogio da sala, procurar o menor tempo dentre todas as filas
            No* menor_paciente = nullptr;
            if (fila_testes2) menor_paciente = fila_testes2;
            if (fila_testes1 && (!menor_paciente || fila_testes1->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_testes1;
            if (fila_testes0 && (!menor_paciente || fila_testes0->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_testes0;

            if (menor_paciente) {
                //Descobre onde está o paciente com menor tempo
                if (menor_paciente == fila_testes2) {
                    paciente_atual = remover_da_fila(&fila_testes2);
                } else if (menor_paciente == fila_testes1) {
                    paciente_atual = remover_da_fila(&fila_testes1);
                } else if (menor_paciente == fila_testes0) {
                    paciente_atual = remover_da_fila(&fila_testes0);
                }
            }
        }

        if (paciente_atual) {
            //Simula o atendimento desse paciente
            paciente_t* paciente = paciente_atual->paciente;
            ContabilizaTempo(4, procedimentos, *paciente);
            //inserir no minheap de exames de imagem após os testes de laboratório
            imagem.insert(paciente);
            delete paciente_atual;
        }
    }

    // Processar exames de imagem
    while(!imagem.isEmpty()){
        //Enquanto o escalonador não está vazio, remove o paciente e insere na fila
        paciente_ptr paciente = imagem.removeandreturnMin(paciente);
        if(paciente->grau == 2){
            inserir_na_fila(&fila_imagem2, paciente);
        }
        else if(paciente->grau == 1){
            inserir_na_fila(&fila_imagem1, paciente);
        }
        else{
            inserir_na_fila(&fila_imagem0, paciente);
        }
    }
    
    while (fila_imagem2 || fila_imagem1 || fila_imagem0) {
        No* paciente_atual = nullptr;
        double minimo = salas_imagem[0].relogio;
        int indice = 0;
        for (int i = 0; i < procedimentos.num_imagem; i++){
            //Procura a sala com menor relógio
            if (minimo > salas_imagem[i].relogio){
                minimo = salas_imagem[i].relogio;
                indice = i;
            }
        }
        //primeiro vamos checar se existe um paciente com horafila menor ou igual ao relogio da sala na fila 2
        if (fila_imagem2 && (fila_imagem2->paciente->horafila <= salas_imagem[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_imagem2);
        } else if (fila_imagem1 && (fila_imagem1->paciente->horafila <= salas_imagem[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_imagem1);
        } else if (fila_imagem0 && (fila_imagem0->paciente->horafila <= salas_imagem[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_imagem0);
        } else {
            //se nenhuma das filas tiver um paciente com horafila menor ou igual ao relogio da sala, procurar o menor tempo dentre todas as filas
            No* menor_paciente = nullptr;
            if (fila_imagem2) menor_paciente = fila_imagem2;
            if (fila_imagem1 && (!menor_paciente || fila_imagem1->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_imagem1;
            if (fila_imagem0 && (!menor_paciente || fila_imagem0->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_imagem0;

            if (menor_paciente) {
                //Descobre em que fila está o menor tempo global
                if (menor_paciente == fila_imagem2) {
                    paciente_atual = remover_da_fila(&fila_imagem2);
                } else if (menor_paciente == fila_imagem1) {
                    paciente_atual = remover_da_fila(&fila_imagem1);
                } else if (menor_paciente == fila_imagem0) {
                    paciente_atual = remover_da_fila(&fila_imagem0);
                }
            }
        }

        if (paciente_atual) {
            //Simula o atendimento desse paciente
            paciente_t* paciente = paciente_atual->paciente;
            ContabilizaTempo(5, procedimentos, *paciente);
            //inserir no minheap de instrumentos após os exames de imagem
            inst_med.insert(paciente);
            delete paciente_atual;
        }
    }

    // Processar instrumentos médicos
    while(!inst_med.isEmpty()){
        //Enquanto há pacientes no escalonador, remove e insere na fila desejada
        paciente_ptr paciente = inst_med.removeandreturnMin(paciente);
        if(paciente->grau == 2){
            inserir_na_fila(&fila_inst_med2, paciente);
        }
        else if(paciente->grau == 1){
            inserir_na_fila(&fila_inst_med1, paciente);
        }
        else{
            inserir_na_fila(&fila_inst_med0, paciente);
        }
    }

    while (fila_inst_med2 || fila_inst_med1 || fila_inst_med0) {
        No* paciente_atual = nullptr;
        double minimo = salas_inst_med[0].relogio;
        int indice = 0;
        for (int i = 0; i < procedimentos.num_inst_med; i++){
            //Busca a sala com menor tempo para realizar o procedimento
            if (minimo > salas_inst_med[i].relogio){
                minimo = salas_inst_med[i].relogio;
                indice = i;
            }
        }
        //primeiro vamos checar se existe um paciente com horafila menor ou igual ao relogio da sala na fila 2
        if (fila_inst_med2 && (fila_inst_med2->paciente->horafila <= salas_inst_med[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_inst_med2);
        } else if (fila_inst_med1 && (fila_inst_med1->paciente->horafila <= salas_inst_med[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_inst_med1);
        } else if (fila_inst_med0 && (fila_inst_med0->paciente->horafila <= salas_inst_med[indice].relogio)) {
            paciente_atual = remover_da_fila(&fila_inst_med0);
        } else {
            //se nenhuma das filas tiver um paciente com horafila menor ou igual ao relogio da sala, procurar o menor tempo dentre todas as filas
            No* menor_paciente = nullptr;
            if (fila_inst_med2) menor_paciente = fila_inst_med2;
            if (fila_inst_med1 && (!menor_paciente || fila_inst_med1->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_inst_med1;
            if (fila_inst_med0 && (!menor_paciente || fila_inst_med0->paciente->horafila < menor_paciente->paciente->horafila)) menor_paciente = fila_inst_med0;

            if (menor_paciente) {
                //Descobre em qual fila está o menor tempo global
                if (menor_paciente == fila_inst_med2) {
                    paciente_atual = remover_da_fila(&fila_inst_med2);
                } else if (menor_paciente == fila_inst_med1) {
                    paciente_atual = remover_da_fila(&fila_inst_med1);
                } else if (menor_paciente == fila_inst_med0) {
                    paciente_atual = remover_da_fila(&fila_inst_med0);
                }
            }
        }

        if (paciente_atual) {
            //Simula o procedimento
            paciente_t* paciente = paciente_atual->paciente;
            ContabilizaTempo(6, procedimentos, *paciente);
            delete paciente_atual;
        }
    }


    for (int i = 0; i < numPacientes; i++){
        //Percorre todos os pacientes e acerta seu tempoTotal
        pacientes[i].tempoTotal = 0.0;
        pacientes[i].tempoTotal += pacientes[i].tempoEspera + pacientes[i].tmin;
    }

    OrdenaPacientesPorId(pacientes, numPacientes); //Utiliza a função de sort implementada para ordenar os pacientes pelo ID
    for (int i = 0; i < numPacientes; i++){
        //Esse 'for' prepara a saída do programa utilizando várias funções de paciente.cpp
        int id_inteiro = stoi(pacientes[i].id); //Converte o ID de string para inteiro
        string dia_semana = diaSemana(pacientes[i].anoin, pacientes[i].mesin, pacientes[i].diain); //Função que revela o dia da semana
        string mes_entrada = nomeDoMes(pacientes[i].mesin); //Dado o número do mês, retorna a abreviação do mês
        string horario_entrada = converterHoras(pacientes[i].horain); //Obtém as horas no formato esperado
        int dataAtual[3]; //Vetor que receberá informações de data
        double horario_atual; //variável que recebrá o horário atual

        //Função que preenche o vetor dataAtual e a variável horario_atual com a data de saída do paciente
        novaData(pacientes[i].anoin, pacientes[i].mesin, pacientes[i].diain, pacientes[i].horain, pacientes[i].tempoTotal, dataAtual, horario_atual); //
        int ano_saida = dataAtual[0];
        int mes_saida = dataAtual[1];
        int dia_saida = dataAtual[2];
        double horario_saida = horario_atual;
        //Mais formatações da saída
        string dia_semana_saida = diaSemana(ano_saida, mes_saida, dia_saida);
        string mes_saida_formatado = nomeDoMes(mes_saida);
        string horario_saida_formatado = converterHoras(horario_saida);
        
        //Impressão da saída desejada
        cout << fixed << setprecision(2)
        << id_inteiro << " "
        << dia_semana << " "
        << mes_entrada << " "
        << pacientes[i].diain << " "
        << horario_entrada << " "
        << pacientes[i].anoin << " "
        << dia_semana_saida << " "
        << mes_saida_formatado << " "
        << dia_saida << " "
        << horario_saida_formatado << " "
        << ano_saida << " "
        << pacientes[i].tempoTotal << " " 
        << pacientes[i].tmin << "  " 
        << pacientes[i].tempoEspera << endl;
    }
    
    //libera toda a memória alocada
    liberarProcedimentos(procedimentos);
    Destroi(pacientes);
    liberarFila(&fila_triagem0);
    liberarFila(&fila_atendimento0);
    liberarFila(&fila_atendimento1);
    liberarFila(&fila_atendimento2);
    liberarFila(&fila_medidas0);
    liberarFila(&fila_medidas1);
    liberarFila(&fila_medidas2);
    liberarFila(&fila_testes0);
    liberarFila(&fila_testes1);
    liberarFila(&fila_testes2);
    liberarFila(&fila_imagem0);
    liberarFila(&fila_imagem1);
    liberarFila(&fila_imagem2);
    liberarFila(&fila_inst_med0);
    liberarFila(&fila_inst_med1);
    liberarFila(&fila_inst_med2);
    delete[] salas_triagem;
    delete[] salas_atendimento;
    delete[] salas_medidas;
    delete[] salas_testes;
    delete[] salas_imagem;
    delete[] salas_inst_med;
    return 0;
    
    return 0;
}