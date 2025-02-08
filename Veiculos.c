/**
 * @file Veiculos.c
 * @author your name (you@domain.com)
 * @brief Funcoes para manipulacao de Veiculos e Registos.
 */

#include "Veiculos.h"

/* Hash table para armazenar os veiculos. */
Veiculo* lista_veiculos[NUM_VEICULOS] = {NULL};

/* COMANDO E: REGISTA ENTRADA DE UM VEICULO */

/**
    @brief Regista a entrada de um veiculo num parque. 
    @param data_mais_recente ponteiro para a data mais recente do sistema.
*/
void registaEntrada(long int *data_mais_recente){
    char nome_buffer[MAX_LEN], matricula_buffer[MAX_LEN], *matricula;
    DataHora entrada, *entrada_ptr;
    Parque *parque;
    int valida;

    leNome(nome_buffer); // ler o nome
    scanf("%s", matricula_buffer); // ler a matricula
    entrada_ptr = leData(); // ler a data
    entrada = *entrada_ptr;
    free(entrada_ptr);
    
    matricula = criaMatricula(matricula_buffer);
    parque = obtemParquePorNome(nome_buffer);
    valida = validaEntrada(nome_buffer, matricula, entrada, data_mais_recente);
    if (valida){
        // Se o input for valido, executar a entrada
        executaEntrada(entrada, matricula, parque, nome_buffer, 
        data_mais_recente);
    }
    else {
        free(matricula);
    }
}

/**
    @brief Executa a entrada de um veiculo num parque, isto eh, se o veiculo ja 
    existir, simplesmente adicionar um registo ah sua lista de registos e, caso 
    contrario, criar um novo veiculo.
    @param entrada data-hora de entrada do veiculo;
    @param matricula matricula do veiculo;
    @param parque parque onde o veiculo vai entrar;
    @param nome nome do parque onde o veiculo vai entrar;
    @param data_mais_recente ponteiro para a data mais recente do sistema.
*/
void executaEntrada(DataHora entrada, char *matricula, Parque *parque, char 
*nome, long int *data_mais_recente) {
    Veiculo *veiculo_existente; 
    Veiculo *veiculo_novo = (Veiculo*) malloc(sizeof(Veiculo));
    Registo registo;
    long int minutos = converteParaMinutos(entrada);

    // Adicionar a matricula ao veiculo.
    veiculo_novo->matricula = matricula;

    // Criar registo.
    registo.entrada = entrada;
    registo.nome_parque = criaNome(nome);
    registo.saida.ano = DATA_DEFAULT;

    // Atualizar a data mais recente do sistema.
    *data_mais_recente = minutos;

    // Se o veiculo ja existir simplesmente adicionar o registo.
    if ((veiculo_existente = obtemVeiculoPorMatricula(matricula)) != NULL){
        veiculo_existente->esta_em_parque = TRUE;
        pushRegisto(&veiculo_existente->lista_registos, &registo);
        pushRegistoOrdenada(&veiculo_existente->lista_registos_ordenada, 
            &registo);

        // Libertar memoria inutilizada.
        free(matricula);
        free(veiculo_novo);
    }

    // Criar um novo veiculo.
    else {
        veiculo_novo->esta_em_parque = TRUE;
        veiculo_novo->lista_registos = NULL;
        veiculo_novo->lista_registos_ordenada = NULL;
        pushRegisto(&veiculo_novo->lista_registos, &registo);
        pushRegistoOrdenada(&veiculo_novo->lista_registos_ordenada, &registo);
        adicionaVeiculo(veiculo_novo);
    }

    // Atualizar o numero de lugares vagos do parque.
    parque->lugares_vagos -= 1;

    printf("%s %d\n", parque->nome, parque->lugares_vagos);
}

/**
    @brief Valida o input do comando e. Se o parque nao existir ou nao tiver 
    lugares vagos, se a matricula for invalida, se o veiculo ja esta em um 
    parque ou a data eh invalida ou anterior ah ultima, retorna FALSE.
    Caso contrario, retorna TRUE.
    @param nome nome do parque;
    @param matricula matricula do veiculo;
    @param data_hora data-hora de entrada;
    @param data_mais_recente ponteiro para a data mais recente do sistema.
    @return TRUE se os argumentos forem validos; FALSE caso contrario.
*/
int validaEntrada(char *nome, char matricula[], DataHora data_hora, 
long int *data_mais_recente){
    Veiculo *veiculo = obtemVeiculoPorMatricula(matricula);

    // Verificar que o parque existe.
    if (!parqueExiste(nome)){
        printf("%s: %s\n", nome, PARQUE_NAO_EXISTE);
        return FALSE;
    }

    // Verificar que o parque tem lugares vazios.
    if (!temLugaresVagos(nome)) return FALSE;

    // Verificar se a matricula eh valida.
    if (!validaMatricula(matricula)){
        printf("%s: %s\n", matricula, MATRICULA_INVALIDA);
        return FALSE;
    }

    // Verificar se o veiculo nao esta em nenhum parque.
    if (veiculo != NULL){
        if (veiculo->esta_em_parque){
            printf("%s: %s\n", matricula, ENTRADA_INVALIDA);
            return FALSE;
        }
    }

    // Validar a data.
    if (!validaDataHora(data_hora, data_mais_recente)) return FALSE;
    
    return TRUE;
}

/* COMANDO S: REGISTA SAIDA DE UM VEICULO */

/**
    @brief Regista a saida de um veiculo de um parque. 
    @param data_mais_recente ponteiro para a data mais recente no sistema.
*/
void registaSaida(long int *data_mais_recente){
    char nome_parque[MAX_LEN], matricula_buffer[MAX_LEN];
    DataHora saida, *saida_ptr;
    char *nome, *matricula;
    Parque *parque;
    int valida = FALSE;

    // Ler o input.
    leNome(nome_parque);
    scanf("%s", matricula_buffer);
    saida_ptr = leData();
    saida = *saida_ptr;
    free(saida_ptr);
    
    // Obter o nome do parque, a matricula e o proprio parque.
    nome = criaNome(nome_parque);
    matricula = criaMatricula(matricula_buffer);
    parque = obtemParquePorNome(nome);

    // Realizar a saida apos a validacao.
    valida = validaSaida(nome, matricula, saida, data_mais_recente);
    if (valida){
        executaSaida(saida, matricula, parque, data_mais_recente);
    }
    // Libertar memoria usada.
    else {
        free(matricula);
    }
    free(nome);
}

/**
    @brief Executa a saida de um veiculo de um parque. Atualiza os registos do  
    veiculo, a lista de faturacoes do parque e a data mais recente do sistema.
    @param saida data-hora de saida do parque;
    @param matricula matricula do veiculo;
    @param parque ponteiro para o parque em que se vai realizar a saida;
    @param data_mais_recente ponteiro para a data mais recente no sistema.
*/
void executaSaida(DataHora saida, char *matricula, Parque *parque, long int
*data_mais_recente){
    long int minutos = converteParaMinutos(saida);
    Veiculo *veiculo = obtemVeiculoPorMatricula(matricula);
    Registo *registo = obtemUltimoRegisto(veiculo->lista_registos);
    Registo *registo_copia = obtemRegistoListaOrdenada(
        &veiculo->lista_registos_ordenada, *registo);
    float faturacao = calculaFaturacao(registo->entrada, saida, parque);
    Faturacao *nova_faturacao = criaFaturacao(faturacao, saida, matricula);

    // Atualizar os registos.
    registo->saida = saida;
    registo_copia->saida = saida;

    // Atualizar o estado do veiculo.
    veiculo->esta_em_parque = FALSE;

    // Atualizar o parque e a lista de faturacoes do mesmo.
    parque->lugares_vagos += 1;
    pushFaturacao(&parque->lista_faturacoes, &parque->tail_faturacoes,
    nova_faturacao);

    // Atualizar a data mais recente do sistema.
    *data_mais_recente = minutos;

    printf("%s ", matricula);
    printDatasEntradasSaidas(registo->entrada, registo->saida);
    printf(" %.2f\n", faturacao);
}

/**
    @brief Valida os argumentos do comando s. Verifica que o parque e veiculo 
    em questao existem, a matricula eh valida e a data tem formato valida e eh 
    posterior ah data mais recente do sistema.
    @param nome nome do parque;
    @param matricula matricula do veiculo;
    @param data_hora data-hora de saida
    @param data_mais_recente ponteiro para a data mais recente no sistema.
    @return TRUE se os argumentos forem validos; FALSE caso contrario.
 */
int validaSaida(char *nome, char matricula[], DataHora data_hora, long 
int *data_mais_recente){
    Veiculo *veiculo = obtemVeiculoPorMatricula(matricula);
    NodeRegisto *lista_registos;
    Registo *registo;

    // Verificar se o parque existe.
    if (!parqueExiste(nome)){
        printf("%s: %s\n", nome, PARQUE_NAO_EXISTE);
        return FALSE;
    }

    // Verificar se a matricula e valida.
    if (!validaMatricula(matricula)){
        printf("%s: %s\n", matricula, MATRICULA_INVALIDA);
        return FALSE;
    }

    // Verificar se o veiculo esta no parque indicado.
    if (veiculo == NULL || !veiculo->esta_em_parque) {
        printf("%s: %s\n", matricula, SAIDA_INVALIDA);
        return FALSE;
    }
    if (veiculo != NULL){
        lista_registos = veiculo->lista_registos;
        registo = obtemUltimoRegisto(lista_registos);
        if (registo == NULL || strcmp(registo->nome_parque, nome)){
            printf("%s: %s\n", matricula, SAIDA_INVALIDA);
            return FALSE;
        }
    }

    // Verificar se a data e hora sao validas.
    if (!validaDataHora(data_hora, data_mais_recente)) return FALSE;
    
    return TRUE;
}

/* COMANDO V: lISTA TODAS AS ENTRADAS E SAIDAS DE UM VEICULO */

/**
    @brief Lista todas as entradas e saidas de um certo veiculo em parques.
*/
void listaEntradasSaidas(){
    char *matricula, matricula_buffer[MAX_LEN];
    int valida;
    Veiculo *veiculo;
    NodeRegisto *lista_registos;

    // Ler a matricula e obter o veiculo em questao.
    scanf("%s", matricula_buffer);
    matricula = criaMatricula(matricula_buffer);
    veiculo = obtemVeiculoPorMatricula(matricula);

    // Validar o input e listar as entradas e saidas.
    valida = validaEntradasSaidas(matricula, veiculo);
    if (valida){
        lista_registos = veiculo->lista_registos_ordenada;
        listaRegistosEntradasSaidas(lista_registos);
    }
    // Libertar memoria.
    free(matricula);
}

/**
    @brief Lista as entradas e saidas de um veiculo por ordem alfabetica dos 
    parques.
    @param registos ponteiro para a cabeca da lista de registos.
*/
void listaRegistosEntradasSaidas(NodeRegisto *registos){
    Registo registo;
    NodeRegisto *tmp = registos;

    // Percorrer os registos e imprimir o pretendido.
    while (tmp != NULL) {
        registo = tmp->registo;

        // Se o registo apenas tiver entrada.
        if (registo.saida.ano == DATA_DEFAULT) {
            printf("%s ", registo.nome_parque);
            printData(registo.entrada);
            printf("\n");
        }
        // Se o registo tiver entrada e saida.
        else {
            printf("%s ", registo.nome_parque);
            printDatasEntradasSaidas(registo.entrada, registo.saida);
            printf("\n");
        }

        tmp = tmp->next;
    }
}

/**
    @brief Valida o input do comando v. Se a matricula for invalida ou se o 
    veiculo nao existir, retorna FALSE e imprime o erro correspondente. Caso 
    contrario, retorna TRUE.
    @param matricula matricula do veiculo em questao;
    @param veiculo ponteiro para o veiculo em questao.
    @return TRUE se o veiculo existir e tiver registos; FALSE caso contrario.
 */
int validaEntradasSaidas(char *matricula, Veiculo *veiculo) {
    // Verificar se a matricula e valida.
    if (!validaMatricula(matricula)) {
        printf("%s: %s\n", matricula, MATRICULA_INVALIDA);
        return FALSE;
    }
    // Verificar que o veiculo existe e contem registos.
    if (veiculo == NULL || veiculo->lista_registos == NULL){
        printf("%s: %s\n", matricula, NAO_EXISTEM_ENTRADAS);
        return FALSE;
    }

    return TRUE;
}

/* MANIPULACAO DE VEICULOS */

/**
    @brief Aloca memoria para guardar uma matricula e devolve um ponteiro para 
    essa memoria.
    @param matricula_buffer buffer que contem a string da matricula.
    @return matricula criada.
 */
char *criaMatricula(char *matricula_buffer){
    char *matricula = (char*) malloc(sizeof(char)*(strlen(matricula_buffer)+1));
    strcpy(matricula, matricula_buffer);
    return matricula;
}

/**
    @brief Verifica se uma matricula tem o formato certo.
    @param matricula matricula a validar.
    @return TRUE de a matricula for valida; FALSE caso contrario.
 */
int validaMatricula(char matricula[]){
    char c1, c2, c3 = matricula[0];
    int iterador = -1, pares = 0, pares_letras = 0, pares_nums = 0;
    int A = 65, Z = 90;

    while (c3 != '\0'){
        c1 = matricula[++iterador];
        c2 = matricula[++iterador];
        c3 = matricula[++iterador];

        // Verificar que os pares estao separados por '-'.
        if (c3 != '-' && c3 != '\0') return FALSE;

        // Verificar quantos pares diferentes existem.
        else if (isalpha(c1) && isalpha(c2)){
            // Verificar que sao letras validas.
            if (!((A <= c1 && c1 <= Z) && (A <= c2 && c2 <= Z))){
                return FALSE;
            }
            else {
                pares += 1;
                pares_letras += 1;
            }
        }
        else if (isdigit(c1) && isdigit(c2)){
            pares += 1;
            pares_nums += 1;
        }
        // Se houver um par com um num e uma letra ou outros caracteres.
        else return FALSE;
    }

    // Se o numero de pares, letras e numeros nao corresponder ao esperado.
    if (pares != 3 || pares_letras < 1 || pares_nums < 1) {
        return FALSE;
    }
    return TRUE;
}

/**
    @brief Atribui um valor de hash a uma matricula e retorna esse valor.
    @param matricula matricula de um veiculo.
    @return valor de hash correspondente ah matricula.
*/
int hash(char *matricula){
    int length = strlen(matricula);
    int hash_value = 0;

    for (int i = 0; i < length; i++){
        hash_value += matricula[i];
        hash_value = (hash_value * matricula[i]) % NUM_VEICULOS;
    }

    return hash_value;
}

/**
    @brief Adiciona um veiculo ah hash table de veiculos.
    @param veiculo ponteiro para o veiculo a ser adicionado.
 */
void adicionaVeiculo(Veiculo *veiculo){
    // Calcular o valor de hash da matricula correspondente.
    int index = hash(veiculo->matricula);
    /* Se houver colisoes, adicionar ao fim da lista ligada da posicao 
    correspondente. */
    veiculo->next = lista_veiculos[index];
    lista_veiculos[index] = veiculo;
}

/**
    @brief Obtem um veiculo da hash table de veiculos com base na matricula.
    @param matricula matricula do veiculo pretendido.
    @return ponteiro para o veiculo pretendido, caso exista; ponteiro nulo caso
    contrario.
 */
Veiculo* obtemVeiculoPorMatricula(char *matricula){
    int index = hash(matricula);
    Veiculo *tmp = lista_veiculos[index];

    // Percorrer a lista ate ao final ou ate encontrar a matricula pretendida.
    while (tmp != NULL && strcmp(tmp->matricula, matricula)){
        tmp = tmp->next;
    }
    
    return tmp;
}

/* MANIPULACAO DE REGISTOS */

/**
    @brief Adiciona um registo ah lista de registos de um certo veiculo. 
    @param head ponteiro para a cabeca da lista de registos;
    @param registo ponteiro para um registo a adicionar.
*/
void pushRegisto(NodeRegisto **head, Registo *registo){
    NodeRegisto *next_node = *head;

    // Criar registo.
    NodeRegisto *new_node = (NodeRegisto*) malloc(sizeof(NodeRegisto));
    new_node->registo = *registo;
    new_node->next = NULL;

    // Se a lista estiver vazia, a cabeca passa a ser igual ao novo no.
    if (*head == NULL) *head = new_node;
    // Se nao, adicionar registo ao final.
    else {
        while (next_node->next != NULL){
            next_node = next_node->next;
        }
        next_node->next = new_node;
    }
}

/**
    @brief Adiciona um registo ah lista de registos de um certo veiculo
    ordenada por nome do parque.
    @param head 
    @param registo 
*/
void pushRegistoOrdenada(NodeRegisto **head, Registo *registo){
    NodeRegisto *next_node = *head;
    char *nome_novo, *nome_antigo;
    // Estado que contem a comparaco alfabetica de dois nomes.
    int compare;

    // Criar registo.
    NodeRegisto *new_node = (NodeRegisto*) malloc(sizeof(NodeRegisto));
    new_node->registo = *registo;
    new_node->next = NULL;

    // Se a lista estiver vazia, o novo elemento sera a cabeca.
    if (*head == NULL) *head = new_node;
    else {        
        // Comparar o nome com o primeiro elemento.
        nome_antigo = next_node->registo.nome_parque;
        nome_novo = new_node->registo.nome_parque;
        compare = strcmp(nome_antigo, nome_novo);
        /* Se o nome do parque do novo registo vier alfabeticamente antes do 
        da cabeca, colocar o novo registo no inicio. */
        if (compare > 0) {
            new_node->next = *head;
            *head = new_node;
        }
        // Colocar registo no meio ou no fim.
        else { 
            pushRegistoOrdenadaAux(&new_node, head);
        }
    }
}

/**
    @brief Coloca ordenadamente um registo na lista de registos ordenada por 
    nome do parque de um certo veiculo.
    @param new_node novo registo a adicionar.
    @param head cabeca da lista de registos ordenada.
 */
void pushRegistoOrdenadaAux(NodeRegisto **new_node, NodeRegisto **head) {
    NodeRegisto *next_node = *head, *prev_node = NULL;
    char *nome_novo = (*new_node)->registo.nome_parque;
    char *nome_antigo = next_node->registo.nome_parque;
    int compare = strcmp(nome_antigo, nome_novo);

    // Iterar pelos registos.
    while (next_node->next != NULL) {
        // Comparar os nomes dos registos com o do registo novo.
        nome_antigo = next_node->registo.nome_parque;
        compare = strcmp(nome_antigo, nome_novo);
        /* Se encontrarmos um registo que vem alfabeticamente depois, sair do 
        loop. */
        if (compare > 0) break;

        // Passar para o proximo registo.
        prev_node = next_node;
        next_node = next_node->next;
    }

    // Se chegamos ao final da lista.
    if (next_node->next == NULL) {
        // Comparar os nomes.
        nome_antigo = next_node->registo.nome_parque;
        compare = strcmp(nome_antigo, nome_novo);
        // Colocar o novo registo no fim.
        if (compare <= 0) next_node->next = *new_node;
        // Colocar o novo registo antes do ultimo.
        else {
            (*new_node)->next = next_node;
            prev_node->next = *new_node;
        }
    }
    // Se nao chegamos ao final da lista.
    else {
        // Colocar registo antes do proximo.
        if (compare > 0) {
            (*new_node)->next = next_node;
            prev_node->next = *new_node;
        }
        // Colocar registo depois do proximo.
        else {
            (*new_node)->next = next_node->next;
            next_node->next = *new_node;
        }
    }
}

/**
    @brief Obtem o ultimo registo de uma lista de registos.
    @param head cabeca da lista de registos.
    @return ponteiro para o ultimo registo da lista.
 */
Registo* obtemUltimoRegisto(NodeRegisto *head){
    NodeRegisto *next_node = head;
    Registo *registo;

    // Se a lista esta vazia, retornar NULL.
    if (head == NULL) return NULL;

    // Caso contrario:
    else {
        // Iterar pela lista ate ao ultimo elemento.
        while (next_node->next != NULL){
            next_node = next_node->next;
        }
        // Retornar ultimo elemento.
        registo = &(next_node->registo);
        return registo;        
    }
    return NULL;
}

/**
    @brief Procura o registo pretendido na lista de registos ordenada de um 
    veiculo.
    @param head cabeca da lista de registos;
    @param registo registo a obter.
    @return ponteiro para o registo pretendido, caso exista; ponteiro nulo
    caso contrario.
*/
Registo* obtemRegistoListaOrdenada(NodeRegisto **head, Registo registo){
    NodeRegisto *next_node = *head;

    // Iterar pela lista de registos.
    while (next_node != NULL){
        // Se encontrarmos o registo, parar a procura.
        if (comparaRegistos(next_node->registo, registo)
            && next_node->registo.saida.ano == DATA_DEFAULT) break;
        next_node = next_node->next;
    }

    // Retornar o registo pretendido.
    if (next_node != NULL)
        return &(next_node->registo);
    else return NULL;
}

/**
    @brief Compara dois registos com base no nome do parque e data de entrada.
    @param registo1 primeiro registo a ser comparado;
    @param registo2 segundo registo a ser comparado.
    @return TRUE se os registos forem iguais; FALSE caso contrario.
*/
int comparaRegistos(Registo registo1, Registo registo2) {
    return (!strcmp(registo1.nome_parque, registo2.nome_parque)
    && comparaDatas(registo2.entrada.ano, registo1.entrada.ano, 
    registo2.entrada.mes, registo1.entrada.mes, registo2.entrada.dia, 
    registo1.entrada.dia));
}

/* REMOCAO DE REGISTOS */

/**
    @brief Permite remover os registos de uma lista de registos por nome do 
    parque.
    @param nome nome do parque do qual queremos remover os registos.
*/
void removeRegistos(char *nome) {
    Registo *registo;
    Veiculo *veiculo;

    // Iterar pela hash table de veiculos.
    for (int i = 0; i < NUM_VEICULOS; i++) {
        if (lista_veiculos[i] == NULL) continue;
        else {
            veiculo = lista_veiculos[i];
            /* Para cada veiculo numa posicao da hash table, apagar os registos 
            pretendidos. */
            while (veiculo != NULL) {
                registo = obtemUltimoRegisto(veiculo->lista_registos);
                if (registo != NULL){
                    /* Se o veiculo esta no parque a remover, alterar o seu 
                    estado. */
                    if (!strcmp(registo->nome_parque, nome)){
                        veiculo->esta_em_parque = FALSE;
                    }
                    // Remover os registos.
                    executaRemocaoRegistos(
                        &veiculo->lista_registos, nome, FALSE);
                    executaRemocaoRegistos(
                        &veiculo->lista_registos_ordenada, nome, TRUE);
                }
                veiculo = veiculo->next;
            }
        }
    }
}

/**
    @brief Remove os registos da lista de registos por nome do parque. 
    @param lista_registos lista de registos de onde os registos serao removidos;
    @param nome nome do parque;
    @param liberta_nome indica se a memoria associada ao nome do parque dentro
    do registo deve ser libertada.
*/
void executaRemocaoRegistos(NodeRegisto **lista_registos, char *nome,
int liberta_nome) {
    NodeRegisto *current = *lista_registos, *temp = NULL; 
    NodeRegisto *prev = NULL;

    // Iterar pela lista de registos.
    while (current != NULL) {
        // Se encontarmos um registo a remover.
        if (!strcmp(current->registo.nome_parque, nome)) {
            temp = current;
            // Se o registo estiver no inicio
            if (current == *lista_registos){
                *lista_registos = current->next;
            }
            // Caso contrario.
            else prev->next = current->next;

            // Apagar registo e libertar memoria.
            if (liberta_nome) free(temp->registo.nome_parque);
            current = current->next;
            free(temp);
        } 
        // Passar para o proximo.
        else {
            prev = current;
            current = current->next;
        }
    }
}

/**
    @brief Liberta a memoria associada ao nome de cada registo no sistema.
    @param lista_registos cabeca da lista de registos.
*/
void apagaNomesRegistos(NodeRegisto *lista_registos) {
    NodeRegisto *tmp = lista_registos;
    while (tmp != NULL) {
        free(tmp->registo.nome_parque);
        tmp = tmp->next;
    }
}

/**
    @brief Apaga o primeiro no de uma lista de registos, liberta a memoria a 
    ele associada e retorna a nova cabeca.
    @param head cabeca da lista de registos.
    @return nova cabeca da lista de registos.
*/
NodeRegisto *apagaNodeRegisto(NodeRegisto *head){
    NodeRegisto *new_head = head->next;
    free(head);
    return new_head;
}

/**
    @brief Apaga a lista de registos de um veiculo e liberta a memoria a ela
    associada.
    @param lista_registos lista de registos a apagar.
*/
void apagaListaRegistos(NodeRegisto *lista_registos){
    while (lista_registos != NULL) {
        lista_registos = apagaNodeRegisto(lista_registos);
    }
}

/* REMOCAO DE VEICULOS */

/**
    @brief Apaga o primeiro veiculo de uma lista de veiculos e liberta toda a 
    memoria a ele associada. Retorna a nova cabeca da lista.
    @param veiculo cabeca da lista de veiculos.
    @return nova cabeca da lista.
*/
Veiculo *apagaVeiculo(Veiculo *veiculo){
    Veiculo *head;
    // Apagar matricula.
    free(veiculo->matricula);
    // Apagar registos.
    apagaNomesRegistos(veiculo->lista_registos);
    apagaListaRegistos(veiculo->lista_registos);
    apagaListaRegistos(veiculo->lista_registos_ordenada);
    // Retornar nova cabeca.
    head = veiculo->next;
    free(veiculo);
    return head;
}

/**
    @brief Apaga todos os veiculos da hash table de veiculos. 
*/
void apagaVeiculos() {
    // Iterar pelos veiculos da hash table.
    for (int i = 0; i < NUM_VEICULOS; i++) {
        // Apagar todos os veiculos numa posicao.
        if (lista_veiculos[i] != NULL) {
            apagaVeiculo(lista_veiculos[i]);
        }
    }
}
