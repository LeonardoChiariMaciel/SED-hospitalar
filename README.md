# Sistema de Escalonamento Hospitalar

## Descrição
Este projeto implementa um sistema de simulação de eventos discretos (SED) para escalonamento de atendimentos hospitalares. O sistema acompanha pacientes desde a triagem até a alta hospitalar, considerando diferentes tipos de procedimentos e tempos de atendimento.

Os pacientes possuem três níveis de prioridade:
- **Verde**: Sem risco de vida.
- **Amarelo**: Condição grave.
- **Vermelho**: Necessita de atendimento imediato.

A simulação deve considerar filas de espera para diferentes tipos de procedimentos, incluindo triagem, atendimento, exames e administração de medicamentos.

## Estrutura do Projeto
A estrutura do diretório segue o padrão exigido:
```
TP/
|-- src/        # Arquivos-fonte (*.cpp)
|-- include/    # Arquivos de cabeçalho (*.h)
|-- obj/        # Arquivos objeto (*.o) gerados pelo compilador
|-- bin/        # Diretório do executável gerado
|-- Makefile    # Arquivo de build para compilação
```

## Compilação e Execução

### Compilar o projeto:
```sh
make all
```

### Executar o programa:
```sh
bin/tp3.out <arquivo_entrada>
```

## Formato do Arquivo de Entrada
O arquivo de entrada contém informações sobre a arquitetura hospitalar e os pacientes. O formato é:
```
<tempotriagem> <numerotriagem>
<tempoatendimento> <numeroatendimento>
<tempomh> <numeromh>
<tempotl> <numerotl>
<tempoei> <numeroei>
<tempoim> <numeroim>
<numeropacientes>
<id_1> <alta_1> <ano_1> <mes_1> <dia_1> <hora_1> <grau_1> <mh_1> <tl_1> <ei_1> <im_1>
...
<id_n> <alta_n> <ano_n> <mes_n> <dia_n> <hora_n> <grau_n> <mh_n> <tl_n> <ei_n> <im_n>
```

## Formato do Arquivo de Saída
O programa deve gerar um arquivo com os tempos de permanência de cada paciente, seguindo o formato:
```
<id_paciente> <data_hora_admissao> <data_hora_alta> <tempo_total> <tempo_em_atendimento> <tempo_em_espera>
```
Exemplo:
```
9600008 Tue Mar 21 02:00:00 2017 Tue Mar 21 12:33:00 2017 10.55 9.85 0.70
```

## Regras e Restrições
- O código deve ser implementado em **C++11**.
- **Não** é permitido o uso de estruturas de dados da STL (como `vector`, `queue`, `stack`).
- O programa deve rodar corretamente em um ambiente Linux.
- O sistema de filas e escalonamento deve utilizar um **min-heap** para prioridade dos eventos.

## Avaliação
O projeto será avaliado considerando:
- Corretude na execução dos casos de teste.
- Qualidade do código (indentacão, organização, boas práticas).
- Documentação e análise experimental.
- Implementação correta das estruturas de dados e algoritmos.

## Autor
- Nome: Leonardo Chiari
