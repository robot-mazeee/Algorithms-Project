/**
 * @file Main.c
 * @author ist1110355 Madalena Cardoso Mota
 * @brief Projeto IAED 23/24
 */

#include "Veiculos.h"

/**
    @brief Le os comandos inseridos pelo utilizador e executa a operacao 
    pretendida.
    @param data_mais_recente ponteiro para o valor em minutos da data mais
    recente registada;
    @param parques_no_sistema ponteiro para o numero de parques
    no sistema. 
*/
void leInput(long int *data_mais_recente, int *parques_no_sistema){
    int run = 1, input;

    while(run){
        input = getchar();

        switch (input){
            case 'q':
                // Libertar a memoria e terminar o programa.
                apagaSistema();
                run = 0;
                break;
            
            case 'p':
                /* Criar parque ou listar os parques existentes dependendo do
                numero de argumentos. */
                criaParqueOuListaParques(parques_no_sistema);
                break;

            case 'e':
                // Registar a entrada de um veiculo num parque.
                registaEntrada(data_mais_recente);
                break;

            case 's':
                // Registar a saida de um veiculo de um parque. 
                registaSaida(data_mais_recente);
                break;

            case 'v':
                // Listar todas as entradas e saidas de um veiculo em parques.
                listaEntradasSaidas();
                break;
            
            case 'f':
                /* Mostrar o resumo da faturacao diaria de um parque ou numa 
                data especifica.*/
                mostraFaturacao(data_mais_recente);
                break;

            case 'r':
                /* Remover um parque do sistema, assim como todas as entradas e
                saidas de veiculos desse parque. */
                removeParque(parques_no_sistema);
                break;
        }
    }
}

/**
    @brief Liberta toda a memoria alocada durante a execucao do programa.  
*/
void apagaSistema(){
    apagaParques();
    apagaVeiculos();
}

/**
    @brief Funcao main.
    @return sempre 0.
*/
int main(){  
    // Estados do sistema.
    long int DATA_MAIS_RECENTE = 0;
    int PARQUES_NO_SISTEMA = 0;

    leInput(&DATA_MAIS_RECENTE, &PARQUES_NO_SISTEMA);
    
    return 0;
}
