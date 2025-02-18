#include "paciente.h"
#include "procedimento.h"
#include <sstream>
using namespace std;
paciente_ptr Cria(int TAM) {
    return new paciente_t[TAM];
}

void Destroi(paciente_ptr pacientes) {
    delete[] pacientes;
}

void carregaPacientes(paciente_ptr pacientes, int TAM, const char* nomeentrada, procedimento_t pr) {
    ifstream arquivo(nomeentrada);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO DE PACIENTES" << std::endl;
        return;
    }

    string lixo1, lixo2;
    for (int i = 0; i < 6; i++) { // Pula as 6 primeiras linhas
        arquivo >> lixo1 >> lixo2; 
    }
    arquivo >> lixo1; //pula mais uma pra chegar em pacientes

    for (int i = 0; i < TAM; i++) { // Carrega pacientes
        arquivo >> pacientes[i].id >> pacientes[i].alta >> pacientes[i].anoin >> pacientes[i].mesin >> pacientes[i].diain >> pacientes[i].horain >> pacientes[i].grau >> pacientes[i].mh >> pacientes[i].tl >> pacientes[i].ei >> pacientes[i].im;
        pacientes[i].tempoAtendimento = 0.0;
        pacientes[i].tempoEspera = 0.0;
        pacientes[i].tempoTotal = 0.0;
        pacientes[i].horafila = pacientes[i].horain; //hora de entrada na fila é a mesma de entrada no hospital
        pacientes[i].estadoAtual = 1;
        pacientes[i].triado = 0;
        pacientes[i].atendido = 0;
        pacientes[i].horaentrada = calculaTempoContinuo(pacientes[i].anoin, pacientes[i].mesin, pacientes[i].diain, pacientes[i].horain, pacientes);
        if (pacientes[i].alta == 1){ //calculo do tempo minimo para quem teve alta (desconsidera filas)
            pacientes[i].tmin = pr.t_triagem + pr.t_atendimento; //se p ácoemte tem alta significa que ele só fez a triagem e o atendimento
        }
        else{//calculo do tempo minimo (desconsiderando fila) para quem não teve alta (todos os procedimentos são feitos)
            pacientes[i].tmin = 
                (
                pr.t_triagem + pr.t_atendimento + pacientes[i].mh * pr.t_medidas +
                pacientes[i].tl * pr.t_testes + pacientes[i].ei * pr.t_imagem + 
                pacientes[i].im * pr.t_inst_med
                );
        }
    } 
    arquivo.close();
}

// Função de tempo contínuo
double calculaTempoContinuo(int ano, int mes, int dia, int hora, paciente_ptr pacientes) {
    int anoBase = 2015;
    int mesBase = 1;
    int diaBase = 1;
    int horaBase = 0;
    return ((ano-anoBase) * 365 * 24) + ((mes - mesBase) * 30 * 24) + ((dia - diaBase) * 24) + (hora - horaBase);
    //retorna o tempo corrido do primeiro paciente até o paciente atual
}

// Função para calcular tempos
void ContabilizaTempo(int numproc, procedimento_t pr, paciente_t& paciente) {
    //vai atualizar as medições de tempo do paciente de acordo com o procedimento que ele acabou de passar e usando o relogio da sala para simulação de eventos discretos
    //numproc é o procedimento que o paciente acabou de passar, pr é a struct com os dados dos procedimentos e paciente é o paciente que acabou de passar pelo procedimento
    switch (numproc){
        case 1:
        {
            //procedimento de triagem realizado apenas pelo critério de hora de chegada (em caso de horas iguais o paciente com menor id é atendido primeiro)
            double minimo = pr.triagem[0].relogio; //minimo armazenará a sala que ficará livre primeiro
            int indice = 0; //será usado para armazenar o indice de uma sala
            for(int i = 0; i < pr.num_triagem; i++){
                //varre a sala buscando a com menor hora e salvando seu índice
                if(minimo > pr.triagem[i].relogio){
                    minimo = pr.triagem[i].relogio;
                    indice = i;
                }
            }

            if (pr.triagem[indice].relogio <= paciente.horafila){ //caso em que o paciente chega e entra
                //verifica se a sala está livre ou se o paciente chegou antes dela liberar
                paciente.horafila += pr.t_triagem; 
                pr.triagem[indice].relogio = paciente.horafila;
            }
            else{ //caso em que precisa esperar na fila (tempo ocioso)
                paciente.tempoEspera += pr.triagem[indice].relogio - paciente.horafila; //atualiza o tempo de espera do paciente somando a diferença entre a hora que ele entra na sala pela hora que ele chegou na fila (saiu do procedimento anterior)
                pr.triagem[indice].relogio += pr.t_triagem; //atualiza o relogio da sala para a hora que ela ficará livre
                paciente.horafila = pr.triagem[indice].relogio; //atualiza a horafila do paciente para a hora que a sala ficará livre, pois esse será o mesmo horário em que ele entrará na fila do próximo procedimento
            }
        }
        break;
        case 2:
        {
            double minimo = pr.atendimento[0].relogio;
            int indice = 0;
            //o processo de checagem da sala com menor tempo é igual para os demais cases
            for (int i = 0; i < pr.num_atendimento; i++){
                if (minimo > pr.atendimento[i].relogio){
                    minimo = pr.atendimento[i].relogio;
                    indice = i;
                }
            }
            if(pr.atendimento[indice].relogio <= paciente.horafila){
                //quando o paciente não fica esperando
                paciente.horafila += pr.t_atendimento;
                pr.atendimento[indice].relogio = paciente.horafila;
            }
            else{
                //quando o paciente tem que esperar
                paciente.tempoEspera += pr.atendimento[indice].relogio - paciente.horafila;
                pr.atendimento[indice].relogio += pr.t_atendimento;
                paciente.horafila = pr.atendimento[indice].relogio;
            }
        }
        break;
        //a partir do case 3 o tempo do procedimento deverá ser calculado multiplicando a duração da unidade do procedimento pela quantidade de procedimentos daquele tipo que o paciente realizará
        case 3:
        {
            double minimo = pr.medidas[0].relogio;
            int indice = 0;
            for (int i = 0; i < pr.num_medidas; i++){
                if (minimo > pr.medidas[i].relogio){
                    minimo = pr.medidas[i].relogio;
                    indice = i;
                }
            }
            if(pr.medidas[indice].relogio <= paciente.horafila){
                //quando o paciente não fica esperando
                paciente.horafila += paciente.mh * pr.t_medidas;
                pr.medidas[indice].relogio = paciente.horafila;
            }
            else{
                //quando o paciente tem que esperar
                paciente.tempoEspera += pr.medidas[indice].relogio - paciente.horafila;
                pr.medidas[indice].relogio += paciente.mh * pr.t_medidas;
                paciente.horafila = pr.medidas[indice].relogio; //verificar se isso causa erro quando o paciente n tem exame -> provavelmente nao causa
            }
        }
        break;
        case 4://implementar para testes
        {
            double minimo = pr.t_laboratorio[0].relogio;
            int indice = 0;
            for (int i = 0; i < pr.num_testes; i++){
                if (minimo > pr.t_laboratorio[i].relogio){
                    minimo = pr.t_laboratorio[i].relogio;
                    indice = i;
                }
            }
            if(pr.t_laboratorio[indice].relogio <= paciente.horafila){
                //quando o paciente não fica esperando
                paciente.horafila += paciente.tl * pr.t_testes;
                pr.t_laboratorio[indice].relogio = paciente.horafila;
            }
            else{
                //quando o paciente tem que esperar
                paciente.tempoEspera += pr.t_laboratorio[indice].relogio - paciente.horafila;
                pr.t_laboratorio[indice].relogio += paciente.tl * pr.t_testes;
                paciente.horafila = pr.t_laboratorio[indice].relogio; //verificar se isso causa erro quando o paciente n tem exame -> provavelmente nao causa
            }
        }
        break;
        case 5://implementar para imagem
        {
            double minimo = pr.e_imagem[0].relogio;
            int indice = 0;
            for (int i = 0; i < pr.num_imagem; i++){
                if (minimo > pr.e_imagem[i].relogio){
                    minimo = pr.e_imagem[i].relogio;
                    indice = i;
                }
            }
            if(pr.e_imagem[indice].relogio <= paciente.horafila){
                //quando o paciente não fica esperando
                paciente.horafila += paciente.ei * pr.t_imagem;
                pr.e_imagem[indice].relogio = paciente.horafila;
            }
            else{
                //quando o paciente tem que esperar
                paciente.tempoEspera += pr.e_imagem[indice].relogio - paciente.horafila;
                pr.e_imagem[indice].relogio += paciente.ei * pr.t_imagem;
                paciente.horafila = pr.e_imagem[indice].relogio; 
            }
        }
        break;
        case 6://implementar para instrumentos médicos
        {
            double minimo = pr.instrumentos[0].relogio;
            int indice = 0;
            for (int i = 0; i < pr.num_inst_med; i++){
                if (minimo > pr.instrumentos[i].relogio){
                    minimo = pr.instrumentos[i].relogio;
                    indice = i;
                }
            }
            if(pr.instrumentos[indice].relogio <= paciente.horafila){
                //quando o paciente não fica esperando
                paciente.horafila += paciente.im * pr.t_inst_med;
                pr.instrumentos[indice].relogio = paciente.horafila;
            }
            else{
                //quando o paciente tem que esperar
                paciente.tempoEspera += pr.instrumentos[indice].relogio - paciente.horafila;
                pr.instrumentos[indice].relogio += paciente.im * pr.t_inst_med;
                paciente.horafila = pr.instrumentos[indice].relogio; //verificar se isso causa erro quando o paciente n tem exame -> provavelmente nao causa
            } 
        }
            break;
    }

}

string diaSemana(int year, int month, int day) {
    //função para descobrir o dia da semana utilizando a ctime
    std::tm time_in = {};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;

    std::mktime(&time_in);

    const char* days_of_week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    return days_of_week[time_in.tm_wday];
}
string nomeDoMes(int numero) {
    //função para descobrir a sigla do mês
    switch (numero) {
        case JANEIRO:   return "Jan";
        case FEVEREIRO: return "Feb";
        case MARCO:     return "Mar";
        case ABRIL:     return "Apr";
        case MAIO:      return "May";
        case JUNHO:     return "Jun";
        case JULHO:     return "Jul";
        case AGOSTO:    return "Aug";
        case SETEMBRO:  return "Sep";
        case OUTUBRO:   return "Oct";
        case NOVEMBRO:  return "Nov";
        case DEZEMBRO:  return "Dec";
        default:        return "Mês inválido";
    }
}
std::string converterHoras(double horasDecimais)
{
    //converter horário em float pra hora formatada
    int horas = static_cast<int>(horasDecimais);                  
    int minutos = static_cast<int>((horasDecimais - horas) * 60); 
    int segundos = static_cast<int>(round((horasDecimais - horas - minutos / 60.0) * 3600));
    
    if (segundos == 60) { //para permitir a rodagem do tempo
        segundos = 0;
        minutos++;
        if (minutos == 60) {
            minutos = 0;
            horas++;
        }
    }
    if(segundos == 59){ //corrigir resíduos no horário
        segundos = 0;
        minutos++;
    }
    
    // Formatar como HH:MM
    std::ostringstream resultado;
    resultado << std::setfill('0') << std::setw(2) << horas << ":"
              << std::setfill('0') << std::setw(2) << minutos << ':'
              << std::setfill('0') << std::setw(2) << segundos;

    return resultado.str();
}
void novaData(int year, int month, int day, int hour, double elapsedHours, int result[3], double &resultHour) {
    //retorna as informações em um vetor e em uma variável para manipulação no main
    tm time_in = {};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;
    time_in.tm_hour = hour;
    time_in.tm_min = 0;
    time_in.tm_sec = 0;
    
    int totalSeconds = static_cast<int>(elapsedHours * 3600);
    time_t time_epoch = std::mktime(&time_in);
    time_epoch += totalSeconds;
    
    tm* new_time = std::localtime(&time_epoch);
    result[0] = new_time->tm_year + 1900;
    result[1] = new_time->tm_mon + 1;
    result[2] = new_time->tm_mday;
    
    resultHour = new_time->tm_hour + new_time->tm_min / 60.0 + new_time->tm_sec / 3600.0;
}

void merge(paciente_ptr pacientes, int left, int mid, int right) {
    //calcula o tamanho dos subvetores
    int n1 = mid - left + 1;
    int n2 = right - mid;

    //cria vetores temporários para armazenar as partes divididas
    paciente_t* L = new paciente_t[n1];
    paciente_t* R = new paciente_t[n2];

    //copia os dados para os arrays temporarios
    for (int i = 0; i < n1; i++)
        L[i] = pacientes[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = pacientes[mid + 1 + j];
    //mescla os arrays ordenados de volta ao array original
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].horaentrada <= R[j].horaentrada) {
            pacientes[k] = L[i];
            i++;
        } else {
            pacientes[k] = R[j];
            j++;
        }
        k++;
    }
    //copia os elementos restantes de L[], se houver
    while (i < n1) {
        pacientes[k] = L[i];
        i++;
        k++;
    }
    //copia os elementos restantes de L[], se houver
    while (j < n2) {
        pacientes[k] = R[j];
        j++;
        k++;
    }
    //libera a memória
    delete[] L;
    delete[] R;
}

void mergeSort(paciente_ptr pacientes, int left, int right) {
    if (left < right) {
        //calcula o ponto médio
        int mid = left + (right - left) / 2;
        //ordena a primeira e a segunda metade
        mergeSort(pacientes, left, mid);
        mergeSort(pacientes, mid + 1, right);
        //combina as duas metades ordenadas
        merge(pacientes, left, mid, right);
    }
}

void OrdenaPacientes(paciente_ptr pacientes, int numPacientes) {
    //chama a função merge sort para ordenar os pacientes
    mergeSort(pacientes, 0, numPacientes - 1);
}

void mergeById(paciente_ptr pacientes, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    //crias arrays temporários
    paciente_t* L = new paciente_t[n1];
    paciente_t* R = new paciente_t[n2];
    //copia os dados
    for (int i = 0; i < n1; i++)
        L[i] = pacientes[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = pacientes[mid + 1 + j];
    //mescla os arrays
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (std::stoi(L[i].id) <= std::stoi(R[j].id)) {
            pacientes[k] = L[i];
            i++;
        } else {
            pacientes[k] = R[j];
            j++;
        }
        k++;
    }
    //copia os elementos de L[], se houver
    while (i < n1) {
        pacientes[k] = L[i];
        i++;
        k++;
    }
    //copia os elementos de R[], se houver
    while (j < n2) {
        pacientes[k] = R[j];
        j++;
        k++;
    }
    //libera a memória alocada dinamicamente
    delete[] L;
    delete[] R;
}

void mergeSortById(paciente_ptr pacientes, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        //ordena a primeira e a segunda metade
        mergeSortById(pacientes, left, mid);
        mergeSortById(pacientes, mid + 1, right);
        //combina as duas metades ordenadas
        mergeById(pacientes, left, mid, right);
    }
}

void OrdenaPacientesPorId(paciente_ptr pacientes, int numPacientes) {
    //chama a função mergesort para ordenar os pacientes com base no horário de entrada
    mergeSortById(pacientes, 0, numPacientes - 1);
}