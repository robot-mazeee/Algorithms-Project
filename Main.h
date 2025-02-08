/**
 * @file Main.h
 * @author ist1110355
 * @brief Declaracao de estruturas, constantes e funcoes principais.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Definicao de Tipos de Dados. */
typedef struct Parque Parque;
typedef struct NodeParque NodeParque;
typedef struct Veiculo Veiculo;
typedef struct Registo Registo;
typedef struct NodeRegisto NodeRegisto;
typedef struct Faturacao Faturacao;
typedef struct NodeFaturacao NodeFaturacao;
typedef struct DataHora DataHora;

// Definicao de Constantes.
#define NUM_VEICULOS 1000
#define NUM_PARQUES 20
#define MAX_LEN BUFSIZ
#define TRUE 1
#define FALSE 0
#define DATA_DEFAULT -1

// Definicao de erros.
#define PARQUE_JA_EXISTE "parking already exists."
#define CAPACIDADE_INVALIDA "invalid capacity."
#define VALOR_INVALIDO "invalid cost."
#define MAXIMO_NUM_PARQUES_EXCEDIDO "too many parks."
#define PARQUE_NAO_EXISTE "no such parking."
#define PARQUE_CHEIO "parking is full."
#define MATRICULA_INVALIDA "invalid licence plate."
#define ENTRADA_INVALIDA "invalid vehicle entry."
#define SAIDA_INVALIDA "invalid vehicle exit."
#define DATA_INVALIDA "invalid date."
#define NAO_EXISTEM_ENTRADAS "no entries found in any parking."

/* Funcoes */
void leInput(long int *data_mais_recente, int *parques_no_sistema);
void apagaSistema();
