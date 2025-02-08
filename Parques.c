#include "Parques.h"

/** Estrutura de dados para os parques. */
static NodeParque *lista_parques = NULL;

/** COMANDO P: CRIAR PARQUE OU LISTAR PARQUES */

/**
    @brief Permite criar um novo parque ou listar os parques existentes.
    @param parques_no_sistema ponteiro para o numero de parques no sistema.
*/
void criaParqueOuListaParques(int *parques_no_sistema){
    int capacidade;
    float valor, valor_apos_hora, valor_max_diario;
    char nome_buffer[MAX_LEN];
    int nome = leNome(nome_buffer), valido;
    char *nome_parque = criaNome(nome_buffer);

    // Se houver mais argumentos para alem do comando p, criar um parque.
    if (nome){
        scanf("%d%f%f%f", &capacidade, &valor, &valor_apos_hora, 
        &valor_max_diario);
        valido = validaParque(nome_parque, capacidade, valor, valor_apos_hora, 
        valor_max_diario, parques_no_sistema);
        if (valido) criaParque(nome_parque, capacidade, valor, valor_apos_hora, 
        valor_max_diario, parques_no_sistema);
    }
    // Caso contrario, listar os parques.
    else {
        free(nome_parque);
        listaParques(lista_parques);
    }
}

/**
    @brief Lista os nomes, capacidades e lugares disponiveis dos parques 
    existentes no sistema.
    @param parques ponteiro para a cabeca da lista de parques.
*/
void listaParques(NodeParque *parques){
    Parque parque;

    while (parques != NULL) {
        parque = parques->parque;
        printf("%s %d %d\n", parque.nome, parque.capacidade, parque.lugares_vagos);
        parques = parques->next;
    }
}

/**
    @brief Cria um novo parque.
    @param nome nome do parque;
    @param capacidade capacidade do parque;
    @param valor valor na primeira hora do parque;
    @param valor_apos_hora valor apos hora do parque;
    @param valor_max_diario valor maximo diario de um parque;
    @param parques_no_sistema ponteiro para o numero de parques no sistema.
*/
void criaParque(char *nome, int capacidade, float valor, float valor_apos_hora, 
float valor_max_diario, int *parques_no_sistema){
    Parque novo_parque;

    novo_parque.nome = nome;
    novo_parque.capacidade = capacidade;
    novo_parque.lugares_vagos = capacidade;
    novo_parque.valor = valor;
    novo_parque.valor_apos_hora = valor_apos_hora;
    novo_parque.valor_max_diario = valor_max_diario;
    novo_parque.lista_faturacoes = NULL;
    novo_parque.tail_faturacoes = NULL;
    *parques_no_sistema += 1;

    pushParque(&lista_parques, novo_parque);
}

/**
    @brief Verifica se um parque existe. Retorna TRUE se sim e FALSE caso 
    contrario.
    @param nome nome do parque.
    @return TRUE se o parque existe; FALSE caso contrario.
*/
int parqueExiste(char *nome) {
    Parque *parque = obtemParquePorNome(nome);
    if (parque == NULL) {
        return FALSE;
    }
    return TRUE;
}

/**
    @brief Verifica se um parque tem lugares vagos. Se sim retorna TRUE. Caso 
    contrario, retorna FALSE e imprime a mensagem de erro.
    @param nome nome do parque.
    @return: TRUE se o parque tem lugares vagos; FALSE caso contrario.
*/
int temLugaresVagos(char *nome) {
    Parque *parque = obtemParquePorNome(nome);
    if (parque->lugares_vagos-1 < 0){
        printf("%s: %s\n", nome, PARQUE_CHEIO);
        return FALSE;
    }
    return TRUE;
}

/**
    @brief Verifica a validade do input do comando p. Se todos os argumentos 
    forem validos, retorna TRUE. Caso contrario retorna FALSE e imprime o erro 
    correspondente.
    @param nome nome do parque;
    @param capacidade capacidade do parque;
    @param valor valor na primeira hora do parque;
    @param valor_apos_hora valor apos hora do parque;
    @param valor_max_diario valor maximo diario de um parque;
    @param parques_no_sistema ponteiro para o numero de parques no sistema.
    @return TRUE se os argumentos forem validos; FALSE caso contrario.
*/
int validaParque(char *nome, int capacidade, float valor, float 
valor_apos_hora, float valor_max_diario, int *parques_no_sistema){
    // Verificar que o parque ainda nao esta no sistema
    if (parqueExiste(nome)){
        printf("%s: %s\n", nome, PARQUE_JA_EXISTE);
        return FALSE;
    }

    // Validar capacidade
    if (capacidade <= 0){
        printf("%d: %s\n", capacidade, CAPACIDADE_INVALIDA);
        return FALSE;
    }

    // Validar valores do tarifario
    if (valor <= 0 || valor_apos_hora <= 0 || valor_max_diario <= 0){
        printf("%s\n", VALOR_INVALIDO);
        return FALSE;
    }
    if (!(valor < valor_apos_hora && valor_apos_hora < valor_max_diario)){
        printf("%s\n", VALOR_INVALIDO);
        return FALSE;
    }

    // Verificar se o numero de parques maximo nao e excedido
    if (*parques_no_sistema + 1 > NUM_PARQUES){
        printf("%s\n", MAXIMO_NUM_PARQUES_EXCEDIDO);
        return FALSE;
    }

    return TRUE;
}

/**
    @brief Permite ler um nome caracter a caracter. Se nao for providenciado um 
    nome como input, retorna FALSE. Caso contrario, retorna TRUE.
    @param nome_buffer string que armazena o nome a ser lido.
    @return TRUE se existir nome; FALSE caso contrario.
*/
int leNome(char nome_buffer[]){
    int iterador = -1, c;
    int fim = FALSE, nome = FALSE, aspas = FALSE;

    while (!fim) {
        c = getchar();

        if (c == EOF || c == '\n') {
            fim = TRUE;
            nome_buffer[++iterador] = '\0';
        }
        else if (isspace(c)){
            if (nome && !aspas) {
                nome_buffer[++iterador] = '\0';
                fim = TRUE;
            }
            else if (nome && aspas) nome_buffer[++iterador] = c;
        }
        else if (isalnum(c)){
            nome = TRUE;
            nome_buffer[++iterador] = c;
        }
        else if (c == '"') {
            aspas = !aspas;
            if (!aspas){
                nome_buffer[++iterador] = '\0';
                fim = TRUE;
            }
        }
    }
    return nome;
}

/**
    @brief Cria um nome (string) dinamicamente e devolve a memoria alocada.
    @param nome_buffer string com o nome a ser criado.
    @return nome criado.
*/
char* criaNome(char nome_buffer[]){
    char *nome = (char*) malloc(sizeof(char)*(strlen(nome_buffer)+1));
    strcpy(nome, nome_buffer);
    return nome;
}

/* MANIPULACAO DE PARQUES */

/**
    @brief Procura por um parque com base no nome. Se o encontrar, retorna um 
    ponteiro para o parque pretendido. Se nao, retorna o ponteiro nulo.
    @param nome nome do parque.
    @return ponteiro para o parque pretendido ou ponteiro nulo se o parque nao 
    for encontrado.
*/
Parque* obtemParquePorNome(char *nome){
    Parque *parque;
    NodeParque *current = lista_parques;
    char *nome_parque;

    while (current != NULL){
        parque = &(current->parque);
        nome_parque = parque->nome;

        if (!(strcmp(nome, nome_parque))){
            return parque;
        }
        current = current->next;
    } 

    return NULL;
}

/**
    @brief Adiciona um parque a lista dos parques e retorna a cabeca da lista.
    @param head cabeca da lista dos parques;
    @param parque parque a ser adicionado.
*/
void pushParque(NodeParque **head, Parque parque){
    NodeParque *next_node = *head;
    int run = TRUE;

    // Criar novo elemento
    NodeParque *new_node = (NodeParque*) malloc(sizeof(NodeParque));
    new_node->parque = parque;
    new_node->next = NULL;

    // Se a lista estiver vazia, o novo elemento sera o primeiro da mesma
    if (*head == NULL){
        *head = new_node;
    }

    else {
        while (run){
            // Se encontramos o ultimo elemento
            if (next_node->next == NULL){
                run = FALSE;
                next_node->next = new_node;
            }
            // Se nao, passar para o proximo
            else {
                next_node = next_node->next;
            }
        }
    }
}

/* COMANDO R: REMOVER PARQUE DO SISTEMA */

/**
    @brief Remove um parque do sistema, assim como todas as entradas e saidas 
    de veiculos nesse parque.
    @param parques_no_sistema ponteiro com o numero de parques no sistema
*/
void removeParque(int *parques_no_sistema){
    char nome_buffer[MAX_LEN], *nome;
    Parque *parque;
    char** parques = (char**) malloc(*parques_no_sistema * sizeof(char*));

    // Ler e criar o nome.
    leNome(nome_buffer);
    nome = criaNome(nome_buffer);

    // Erro.
    parque = obtemParquePorNome(nome);
    if (parque == NULL) {
        printf("%s: %s\n", nome, PARQUE_NAO_EXISTE);
    }
    else {
        // Executar a remocao do parque e dos registos correspondentes.
        removeRegistos(nome);
        executaRemocaoParque(&lista_parques, parque);
        *parques_no_sistema -= 1;
        // Lista os parques por ordem alfabetica
        if (*parques_no_sistema > 0) {
            criaArrayParquesOrdenados(parques, parques_no_sistema);
            listaNomesParques(parques, parques_no_sistema);
        }
    }
    free(parques);
    free(nome);
}

/**
    @brief Cria um array com os nomes dos parques ordenados alfabeticamente.
    @param parques array que vai armazenar strings com os nomes dos parques;
    @param parques_no_sistema ponteiro para o numero de parques no sistema.
*/
void criaArrayParquesOrdenados(char** parques, int *parques_no_sistema) {
    NodeParque *temp = lista_parques;
    int iterador = 0;

    // Colocar os nomes no array.
    while (temp != NULL && iterador < *parques_no_sistema) {
        parques[iterador] = temp->parque.nome;
        iterador++;
        temp = temp->next;
    }

    // Ordenar os parques.
    ordenaParques(parques, parques_no_sistema);
}

/**
    @brief Ordena alfabeticamente o array de nomes de parques.
    @param parques array que vai armazenar strings com os nomes dos parques,
    @param parques_no_sistema ponteiro para o numero de parques no sistema. 
*/
void ordenaParques(char **parques, int*parques_no_sistema) {
    int i, j;
    char *temp;
    
    for (i = 0; i < *parques_no_sistema-1; i++) {
        for (j = 0; j < *parques_no_sistema-i-1; j++) {
            if (strcmp(parques[j], parques[j+1]) > 0) {
                temp = parques[j];
                parques[j] = parques[j+1];
                parques[j+1] = temp;
            }
        }
    }
}

/**
    @brief Lista os nomes de todos os parques no sistema.
    @param parques array com os nomes dos parques no sistema.
    @param parques_no_sistema ponteiro para o numero de parques no sistema.
*/
void listaNomesParques(char **parques, int *parques_no_sistema) {
    for (int i = 0; i < *parques_no_sistema; i++) {
        printf("%s\n", parques[i]);
    }
}

/**
    @brief Liberta toda a memoria correspondente a um parque.
    @param parque parque a libertar.
*/
void libertaParque(Parque *parque) {
    while (parque->lista_faturacoes != NULL){
        parque->lista_faturacoes = 
        apagaNodeFaturacao(parque->lista_faturacoes);
    }
    free(parque->nome);
    free(parque);
}

/**
    @brief Remove o parque pretendido da lista dos parques e liberta a memoria 
    correspondente.
    @param lista_parques cabeca da lista dos parques;
    @param parque ponteiro para o parque a remover.
*/
void executaRemocaoParque(NodeParque **lista_parques, Parque *parque){
    NodeParque *temp = *lista_parques, *prev = *lista_parques;

    // Se o parque estiver no inicio da lista
    if (temp != NULL && !strcmp(temp->parque.nome, parque->nome)) {
        *lista_parques = temp->next;
        libertaParque(parque);
    }
    else {
        while (temp != NULL && strcmp(temp->parque.nome, parque->nome)){
            prev = temp;
            temp = temp->next;
        }
        if (temp != NULL && !strcmp(temp->parque.nome, parque->nome)) {
            prev->next = temp->next;  
            libertaParque(parque);
        }        
    }
}

/**
    @brief Apaga o primeiro no da lista dos parques, liberta a memoria 
    correspondente e retorna a nova cabeca.
    @param head cabeca da lista dos parques.
    @return nova cabeca da lista.
*/
NodeParque* apagaNodeParque(NodeParque *head){
    NodeParque* new_head = head->next;
    libertaParque(&head->parque);
    return new_head;
}

/**
    @brief Apaga todos os parques do sistema e liberta a memoria correspondente.
*/
void apagaParques(){
    while (lista_parques != NULL) {
        lista_parques = apagaNodeParque(lista_parques);
    }
}

/* COMANDO F: LISTAR A FATURACAO DE UM PARQUE */

/**
    @brief Mostra o resumo da faturacao diaria total de um parque ou as 
    faturacoes num certo dia.
    @param data_mais_recente ponteiro para a data mais recente do sistema. 
*/
void mostraFaturacao(long int *data_mais_recente){
    char *nome, nome_buffer[MAX_LEN], linha[MAX_LEN], c;
    DataHora data_hora;
    Parque *parque;
    int num_args = 0;

    fgets(linha, sizeof(linha), stdin);
    num_args = sscanf(linha, "%c \"%[^\"]\" %d-%d-%d", &c, nome_buffer, 
    &data_hora.dia, &data_hora.mes, &data_hora.ano);
    if (num_args == 1) {
        num_args = sscanf(linha, "%c %s %d-%d-%d", &c, nome_buffer, 
        &data_hora.dia, &data_hora.mes, &data_hora.ano);
    }

    nome = criaNome(nome_buffer);
    parque = obtemParquePorNome(nome);
    if (num_args == 5) {
        data_hora.hora = 0;
        data_hora.minuto = 0;
        int valida_data = validaFaturacoesPorData(parque, data_hora, nome, 
        data_mais_recente);
        if (valida_data) {
            listaFaturacoesPorData(parque->lista_faturacoes, data_hora);
        }
    } else if (num_args == 2) {
        int valida_parque = validaFaturacoesPorParque(parque, nome);
        if (valida_parque) {
            if (parque->lista_faturacoes != NULL)
                listaFaturacoesPorParque(parque);
        }
    } free(nome);
}

/**
    @brief Calcula a faturacao quando um veiculo sai de um certo parque, 
    baseada no seu tempo de estadia e dos valores de faturacao desse parque.
    @param entrada data-hora de entrada;
    @param saida data-hora de saida;
    @param parque ponteiro para o parque em questao.
    @return valor da faturacao.
*/
float calculaFaturacao(DataHora entrada, DataHora saida, Parque *parque){
    float valor = parque->valor;
    float valor_apos_hora = parque->valor_apos_hora;
    float valor_max_diario = parque->valor_max_diario;
    float faturacao = 0, tmp = 0;
    DataHora diferenca = calculaDiferencaEntreDatas(entrada, saida);
    int dias = diferenca.dia, horas = diferenca.hora, minutos = diferenca.minuto;
    int periodos_quinze_minutos = ((horas * 60) / 15) + (minutos / 15);
    minutos = minutos % 15;

    if (minutos > 0) {
        periodos_quinze_minutos += 1;
    }

    // Adicionar faturacao dos dias
    faturacao = valor_max_diario * dias;

    // Adicionar valor inicial
    if (periodos_quinze_minutos <= 4){
        tmp = periodos_quinze_minutos * valor;
    }
    else {
        tmp += 4 * valor;
        periodos_quinze_minutos -= 4;
        tmp += periodos_quinze_minutos * valor_apos_hora;
    }

    if (tmp > valor_max_diario){
        faturacao += valor_max_diario;
    }
    else {
        faturacao += tmp;
    }

    return faturacao;
}

/**
    @brief Valida o input do comando f com apenas um argumento. Retorna TRUE se 
    o nome do parque for valido e FALSE caso contrario.
    @param parque ponteiro para o parque em questao;
    @param nome nome do parque.
    @return TRUE se o parque for valido; FALSE caso contrario.
*/
int validaFaturacoesPorParque(Parque *parque, char *nome){
    if (parque == NULL) {
        printf("%s: %s\n", nome, PARQUE_NAO_EXISTE);
        return FALSE;
    }
    return TRUE;
}

/**
    @brief Valida o input do comando f com dois argumentos. Retorna TRUE se o 
    nome do parque for valido e a data for valida e anterior ao ultimo registo. 
    Caso contrario, retorna FALSE.
    @param parque ponteiro para o parque em questao;
    @param data_hora data-hora da faturacao pretendida;
    @param nome nome do parque;
    @param data_mais_recente ponteiro para a data mais recente no sistema.
    @return TRUE se os argumentos forem validos, FALSE caso contrario.
*/
int validaFaturacoesPorData(Parque *parque, DataHora data_hora, char *nome,
long int *data_mais_recente) {
    if (parque == NULL) {
        printf("%s: %s\n", nome, PARQUE_NAO_EXISTE);
        return FALSE;
    }

    if (!validaFormatoDataHora(data_hora)){
        printf("%s\n", DATA_INVALIDA);
        return FALSE;
    }

    // Verificar se a data e a hora sao anteriores ao ultimo registo
    long int minutos = converteParaMinutos(data_hora);
    if (*data_mais_recente <= minutos) {
        printf("%s\n", DATA_INVALIDA);
        return FALSE;
    }

    return TRUE;
}

/**
    @brief Mostra o resumo da faturacao diaria de um parque, ordenado por data.
    @param parque ponteiro para o parque em questao.
*/
void listaFaturacoesPorParque(Parque *parque) {
    NodeFaturacao *tmp = parque->lista_faturacoes;
    float faturacao = 0, valor = 0;
    DataHora data_atual = tmp->faturacao.saida;
    DataHora saida;

    if (tmp == NULL) return;
    while (tmp != NULL) {
        saida = tmp->faturacao.saida;
        valor = tmp->faturacao.valor;

        if (!comparaDatas(saida.ano, data_atual.ano, saida.mes, data_atual.mes, 
        saida.dia, data_atual.dia)) {
            printf("%02d-%02d-%02d %.2f\n", data_atual.dia, data_atual.mes,
            data_atual.ano, faturacao);

            data_atual = saida;
            faturacao = valor;
        }
        else faturacao += tmp->faturacao.valor;
        tmp = tmp->next;
    }
    printf("%02d-%02d-%02d %.2f\n", data_atual.dia, data_atual.mes,
    data_atual.ano, faturacao);
}

/**
    @brief Mostra todos os valores faturados por um parque numa certa data.
    @param faturacoes cabeca da lista de faturacoes do parque;
    @param data_hora data-hora pretendida.
*/
void listaFaturacoesPorData(NodeFaturacao *faturacaoes, DataHora data_hora){
    NodeFaturacao *tmp = faturacaoes;
    DataHora saida;

    while (tmp != NULL){
        saida = tmp->faturacao.saida;
        if (comparaDatas(data_hora.ano, saida.ano, data_hora.mes, 
        saida.mes, data_hora.dia, saida.dia)){
            printf("%s ", tmp->faturacao.matricula);
            printf("%02d:%02d", saida.hora, saida.minuto);
            printf(" %.2f\n", tmp->faturacao.valor);
        }
        tmp = tmp->next;
    }
}

/**
    @brief Aloca memoria necessaria para criar um ponteiro do tipo Faturacao e 
    retorna esse ponteiro.
    @param valor valor da faturacao;
    @param saida data-hora da faturacao;
    @param matricula matricula do veiculo em questao.
    @return ponteiro para uma estrutura do tipo Faturacao.
*/
Faturacao *criaFaturacao(float valor, DataHora saida, char *matricula){
    Faturacao *faturacao = (Faturacao*) malloc(sizeof(Faturacao));

    faturacao->valor = valor;
    faturacao->saida = saida;
    faturacao->matricula = matricula;

    return faturacao;
}

/**
    @brief Adiciona uma faturacao ao registo de faturacoes de um parque.
    @param head cabeca da lista de faturacoes;
    @param tail tail da lista de faturacoes;
    @param faturacao ponteiro para uma estrutura do tipo Faturacao.
*/
void pushFaturacao(NodeFaturacao **head, NodeFaturacao **tail, Faturacao 
*faturacao){
    NodeFaturacao *new_node = (NodeFaturacao*) malloc(sizeof(NodeFaturacao));
    new_node->faturacao = *faturacao;
    new_node->next = NULL;
    free(faturacao);

    if (*head == NULL){
        *head = new_node;
        *tail = *head;
    }

    else {
        (*tail)->next = new_node;
        (*tail) = (*tail)->next;
    }
}

/**
    @brief Apaga o primeiro no da lista de faturacoes de um parque, liberta a 
    memoria correspondente e retorna a nova cabeca.
    @param head cabeca da lista de faturacoes.
    @return nova cabeca da lista de faturacoes.
*/
NodeFaturacao* apagaNodeFaturacao(NodeFaturacao *head) {
    NodeFaturacao *new_head = head->next;
    free(head->faturacao.matricula);
    free(head);
    return new_head;
}
