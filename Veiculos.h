/**
 * @file Veiculos.h
 * @author ist1110355
 * @brief Declaracao de estruturas e funcoes para manipulacao de Veiculos e 
 * Registos.
 */

#include "Parques.h"

/* Estruturas para os Veiculos e Registos. */

typedef struct Registo {
    char *nome_parque;
    DataHora entrada;
    DataHora saida;
} Registo;

typedef struct NodeRegisto {
    Registo registo;
    NodeRegisto *next;
} NodeRegisto;

typedef struct Veiculo {
    char *matricula;
    int esta_em_parque;
    NodeRegisto *lista_registos;
    NodeRegisto *lista_registos_ordenada;
    Veiculo *next;
} Veiculo;

/* Comando e. */
void registaEntrada(long int *data_mais_recente);
void executaEntrada(DataHora entrada, char *matricula, Parque *parque, char 
*nome, long int *data_mais_recente);
int validaEntrada(char *nome, char matricula[], DataHora entrada, long int *data_mais_recente);

/* Comando s. */
void registaSaida(long int *data_mais_recente);
void executaSaida(DataHora saida, char *matricula, Parque *parque, long int *data_mais_recente);
int validaSaida(char *nome, char matricula[], DataHora entrada, long int 
*data_mais_recente);

/* Comando v. */
void listaEntradasSaidas();
void listaRegistosEntradasSaidas(NodeRegisto *lista_registos);
int validaEntradasSaidas(char *matricula, Veiculo *veiculo);

/* Manipulacao de Veiculos. */
char *criaMatricula(char *matricula_buffer);
int validaMatricula(char matricula[]);
int hash(char *matricula);
void adicionaVeiculo(Veiculo *veiculo);
Veiculo *obtemVeiculoPorMatricula(char *matricula);

/* Manipulacao de Registos. */
void pushRegisto(NodeRegisto **head, Registo *registo);
void pushRegistoOrdenada(NodeRegisto **head, Registo *registo);
void pushRegistoOrdenadaAux(NodeRegisto **new_node, NodeRegisto **head);
Registo *obtemUltimoRegisto(NodeRegisto *lista_registos);
Registo* obtemRegistoListaOrdenada(NodeRegisto **head, Registo registo);
int comparaRegistos(Registo registo1, Registo registo2);

/* Remocao de Registos. */
void removeRegistos(char *nome);
void executaRemocaoRegistos(NodeRegisto **lista_registos, char *nome, int 
liberta_nome);
void apagaNomesRegistos(NodeRegisto *lista_registos);
NodeRegisto *apagaNodeRegisto(NodeRegisto *head);
void apagaListaRegistos(NodeRegisto *lista_registos);

/* Remocao de Veiculos. */
Veiculo *apagaVeiculo(Veiculo *veiculo);
void apagaVeiculos();
