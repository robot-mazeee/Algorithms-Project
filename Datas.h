/**
 * @file Datas.h
 * @author ist1110355
 * @brief Declaracao de estruturas e funcoes para manipulacao de datas.
 */

#include "Main.h"

/* Declaracao da estrutura de datas. */
typedef struct DataHora {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} DataHora;

/* Leituras e impressoes */
void printData(DataHora data_hora);
void printDatasEntradasSaidas(DataHora entrada, DataHora saida);
DataHora* leData();

/* Validacoes */
int validaFormatoDataHora(DataHora data_hora);
int validaDataHora(DataHora data_hora, long int *data_mais_recente);
int validaFormatoHora(int hora, int minuto);
int validaFormatoData(int ano, int mes, int dia);

/* Conversoes */
DataHora converteParaDataHora(long int minutos);
long int converteParaMinutos(DataHora data_hora);
DataHora calculaDiferencaEntreDatas(DataHora entrada, DataHora saida);

/* Comparacoes */
int comparaDatas(int ano1, int ano2, int mes1, int mes2, int dia1, int dia2);
int comparaDatasHoras(DataHora data_hora1, DataHora data_hora2);
int comparaHoras(int hora1, int hora2, int minuto1, int minuto2);
