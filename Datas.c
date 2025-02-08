/**
 * @file Datas.c
 * @author ist1110355
 * @brief Funcoes para manipulacao de datas.
 */

#include "Datas.h"

/**
    @brief Imprime uma data.
    @param data_hora data-hora a imprimir.
*/
void printData(DataHora data_hora){
    printf("%02d-%02d-%02d %02d:%02d", data_hora.dia, data_hora.mes, 
    data_hora.ano, data_hora.hora, data_hora.minuto);
}

/**
    @brief Imprime as datas de entrada e saida de um veiculo.
    @param entrada data-hora de entrada;
    @param saida data-hora de saida.
*/
void printDatasEntradasSaidas(DataHora entrada, DataHora saida){
    printData(entrada);
    printf(" ");
    printData(saida);
}

/**
    @brief Le uma data, aloca memoria para guardar data correspondente e 
    retorna um ponteiro para a memoria alocada.
    @return ponteiro para uma estrutura DataHora.
*/
DataHora* leData(){
    DataHora *data_hora = (DataHora*) malloc(sizeof(DataHora));
    int dia, mes, ano, hora, minuto;

    // Ler a data.
    scanf("%d-%d-%d", &dia, &mes, &ano);
    scanf("%d:%d", &hora, &minuto);

    data_hora->ano = ano;
    data_hora->mes = mes;
    data_hora->dia = dia;
    data_hora->hora = hora;
    data_hora->minuto = minuto;

    return data_hora;
}

/**
    @brief Valida o formato de uma data. Retorna TRUE de a data for valida, ou 
    seja, o ano eh nao negativo, o mes esta entre 1 e 12 e o dia corresponde a 
    um dia valido no mes em questao. Caso contrario, retorna FALSE.
    @param ano ano da data a ser verificada;
    @param mes mes da data a ser verificada;
    @param dia dia da data a ser verificada.
    @return TRUE se a data for valida; FALSE caso contrario.
*/
int validaFormatoData(int ano, int mes, int dia) {
    // Valida o ano e o mes.
    if (ano <= 0 || (mes <= 0 || mes > 12)) return FALSE;

    // Valida os dias consoante o mes.
    if (mes == 2 && (dia <= 0 || dia > 28)) {
        return FALSE; // Fevereiro.
    }
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia <= 0 || dia > 30) return FALSE; // Meses de 30 dias.
    }
    else {
        if (dia <= 0 || dia > 31) return FALSE; // Meses de 31 dias.
    }
    return TRUE;
}

/**
    @brief Valida o formato de uma hora, isto eh, se o minuto esta entre 1 e 59 
    e a hora entre 0 e 23. Retorna TRUE se isto de verificar e FALSE em caso 
    contrario.
    @param hora hora a verificar;
    @param minuto minuto a verificar.
    @return TRUE se a hora for valida e FALSE caso contrario.
*/
int validaFormatoHora(int hora, int minuto) {
    if ((hora < 0 || hora > 23) || (minuto < 0 || minuto > 59)) 
        return FALSE;

    return TRUE;
}

/**
    @brief Valida o formato de uma estrutura do tipo DataHora.
    @param data_hora data-hora a validar.
    @return TRUE se a data-hora for valida; FALSE caso contrario.
*/
int validaFormatoDataHora(DataHora data_hora){
    int ano, mes, dia, hora, minuto;

    ano = data_hora.ano;
    mes = data_hora.mes;
    dia = data_hora.dia;
    hora = data_hora.hora;
    minuto = data_hora.minuto;

    // Validar a data e a hora.
    return 
    (validaFormatoData(ano, mes, dia) && validaFormatoHora(hora, minuto));
}

/**
    @brief Verifica se uma data tem formato valido e e posterior ao ultimo 
    registo.
    @param data_hora data-hora a validar;
    @param data_mais_recente ponteiro para a data mais recente do sistema.
    @return TRUE se a data-hora for valida; FALSE caso contrario.
*/
int validaDataHora(DataHora data_hora, long int *data_mais_recente) {
    // Verificar se a data e hora estão no formato certo.
    if (!validaFormatoDataHora(data_hora)){
        printf("%s\n", DATA_INVALIDA);
        return FALSE;
    }

    // Verificar se a data e a hora sao posteriores ao ultimo registo.
    long int minutos = converteParaMinutos(data_hora);
    if (*data_mais_recente > minutos) {
        printf("%s\n", DATA_INVALIDA);
        return FALSE;
    }

    return TRUE;
}

/**
    @brief Converte um inteiro com o numero de minutos para uma estrutura do 
    tipo DataHora.
    @param minutos inteiro com o numero de minutos a converter.
    @return data-hora criada.
*/
DataHora converteParaDataHora(long int minutos){
    long int total_dias = 0, horas = 0, minutos_restantes = 0;
    DataHora data_hora;

    // Calculo dos dias, minutos e horas.
    total_dias = minutos / (24 * 60);
    minutos_restantes = minutos % (24 * 60);
    horas = minutos_restantes / 60;
    minutos_restantes = minutos_restantes % 60;

    data_hora.ano = 0;
    data_hora.mes = 0;
    data_hora.dia = total_dias;
    data_hora.hora = horas;
    data_hora.minuto = minutos_restantes;

    return data_hora;
}

/**
    @brief Converte uma data para minutos. Retorna um inteiro que contem o 
    numero de minutos a que corresponde a data.
    @param data_hora data-hora a ser convertida para minutos.
    @return numero de minutos.
*/
long int converteParaMinutos(DataHora data_hora){
    int ano, mes, dia, hora, minuto;
    long int minutos = 0;
    // Array com o numero de dias de cada mes.
    int dias_do_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    ano = data_hora.ano;
    mes = data_hora.mes;
    dia = data_hora.dia;
    hora = data_hora.hora;
    minuto = data_hora.minuto;

    // Calcular minutos, horas, dias e anos.
    minutos = minuto + hora*60 + (dia-1)*24*60 + (ano-1)*365*60*24;
    // Calcular dias consoante o mes.
    for (int i = 0; i < mes-1; i++){
        minutos += dias_do_mes[i]*24*60;
    }

    return minutos;
}

/**
    @brief Calcula a diferenca entre duas datas em dias, horas e minutos. 
    Devolve uma estrutura do tipo DataHora com a diferenca correspondente.
    @param entrada data-hora de entrada;
    @param saida data-hora de saida.
    @return data-hora com a diferenca em dias, horas e minutos entre as datas 
    de entrada e saida.
*/
DataHora calculaDiferencaEntreDatas(DataHora entrada, DataHora saida){
    // Converter as datas para minutos e calcular a diferenca.
    long int minutos_entrada = converteParaMinutos(entrada);
    long int minutos_saida = converteParaMinutos(saida);
    long int minutos_diferenca = minutos_saida - minutos_entrada;

    // Converter a diferenca em data-hora e retornar.
    DataHora data_hora = converteParaDataHora(minutos_diferenca);
    return data_hora;
}

/**
    @brief Verifica se duas datas sao iguais.
    @param ano1 ano da primeira data;
    @param ano2 ano da segunda data;
    @param mes1 mes da primeira data;
    @param mes2 mes da segunda data;
    @param dia1 dia da primeira data;
    @param dia2 dia da segunda data;
    @return TRUE se as datas forem iguais; FALSE caso contrario.
*/
int comparaDatas(int ano1, int ano2, int mes1, int mes2, int dia1, int dia2){
    return (ano1 == ano2 && mes1 == mes2 && dia1 == dia2);
}

/**
    @brief Verifica se duas horas sao iguais.
    @param hora1 primeira hora;
    @param hora2 segunda hora;
    @param minuto1 minuto da primeira hora;
    @param minuto2 minuto da segunda hora.
    @return TRUE se as horas forem iguais; FALSE caso contrario.
*/
int comparaHoras(int hora1, int hora2, int minuto1, int minuto2) {
    return (hora1 == hora2 && minuto1 == minuto2);
}

/**
    @brief Verifica se duas estruturas DataHora sao iguais.
    @param data_hora1 primeira data-hora;
    @param data_hora2 segunda data-hora.
    @return TRUE se as data-horas forem iguais; FALSE caso contrario.
 */
int comparaDatasHoras(DataHora data_hora1, DataHora data_hora2){
    int ano1 = data_hora1.ano, mes1 = data_hora1.mes, dia1 = data_hora1.dia;
    int ano2 = data_hora2.ano, mes2 = data_hora2.mes, dia2 = data_hora2.dia;
    int hora1 = data_hora1.hora, minuto1 = data_hora1.minuto;
    int hora2 = data_hora2.hora, minuto2 = data_hora2.minuto;

    return (comparaDatas(ano1, mes1, dia1, ano2, mes2, dia2)
        && comparaHoras(hora1, hora2, minuto1, minuto2));
}
