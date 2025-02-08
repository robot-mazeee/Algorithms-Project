#include "Datas.h"

typedef struct Parque {
    char *nome;
    int capacidade;
    int lugares_vagos;
    float valor;
    float valor_apos_hora;
    float valor_max_diario;
    NodeFaturacao *lista_faturacoes;
    NodeFaturacao *tail_faturacoes;
} Parque;

typedef struct NodeParque{
    Parque parque;
    NodeParque *next;
} NodeParque;

typedef struct Faturacao {
    float valor;
    char *matricula;
    DataHora saida;
} Faturacao;

typedef struct NodeFaturacao{
    Faturacao faturacao;
    NodeFaturacao *next;
} NodeFaturacao;

void criaParqueOuListaParques(int *parques_no_sistema);
int leNome(char nome_buffer[]);
char* criaNome(char nome_buffer[]);
void listaNomesParques(char** parques, int *parques_no_sistema);
void executaRemocaoParque(NodeParque **lista_parques, Parque *parque);
void ordenaParques(char **parques, int *parques_no_sistema);
void criaArrayParquesOrdenados(char** parques, int *parques_no_sistema);
int validaParque(char *nome, int capacidade, float valor, float valor_apos_hora, 
float valor_max_diario, int *parques_no_sistema);
void criaParque(char *nome, int capacidade, float valor, 
float valor_apos_hora, float valor_max_diario, int *parques_no_sistema);
Parque* obtemParquePorNome(char *nome);
void removeRegistos(char *nome);
void pushParque(NodeParque **head, Parque parque);
void listaParques(NodeParque *parques);
NodeParque* apagaNodeParque(NodeParque *head);
void libertaParque(Parque *parque);
void removeParque(int *parques_no_sistema);
void apagaParques();
int parqueExiste(char *nome);
int temLugaresVagos(char *nome);

float calculaFaturacao(DataHora entrada, DataHora saida, Parque *parque);
void pushFaturacao(NodeFaturacao **head, NodeFaturacao **tail, Faturacao *faturacao);
void mostraFaturacao(long int *data_mais_recente);
Faturacao *criaFaturacao(float valor, DataHora saida, char *matricula);
void listaFaturacoesPorData(NodeFaturacao *lista_faturacaoes, DataHora data_hora);
void listaFaturacoesPorParque(Parque *parque);
NodeFaturacao* apagaNodeFaturacao(NodeFaturacao *head);
int validaFaturacoesPorData(Parque *parque, DataHora data_hora, char *nome,
long int *data_mais_recente);
int validaFaturacoesPorParque(Parque *parque, char *nome);