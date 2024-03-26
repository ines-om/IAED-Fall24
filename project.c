/* iaed24 - ist1107083 - project */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* DEFINIÇÕES REFERENTES AO INPUT - CMDLINE */
#define MAXCOMANDO 200
#define MAXPALAVRAS 20

/* DEFINIÇÕES REFERENTES A PARQUES */
#define MAXPARQUES 20

/* ----- INÍCIO PARQUES -----*/
struct Parque{
    char nome[50];

    int capacidadeMax;
    int lugaresDisponiveis;

    float regimeUmaHora;
    float regimeHoras;
    float regimeDiario;
};

struct Parque parques[MAXPARQUES];

/* Lista parques existentes */
void listaParques() {
    int i = 0;
   
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")) {
        printf("%s %d %d\n", (char *)(parques[i].nome), parques[i].capacidadeMax, parques[i].lugaresDisponiveis);
        i++;
    }
}

/* Função de Criação de Parque */
void criaParque(char *nome, char *capacidadePar, char *regimeUmaHoraPar, char *regimeHorasPar, char *regimeDiarioPar) {
    int i = 0;
    int capacidade=0;
    float regimeUmaHora=0;
    float regimeHoras=0;
    float regimeDiario=0;

    /* Pré-condições */

    /* Validar nome do parque */
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), nome )!=0) i++;
    if (strcmp((char *)(parques[i].nome), nome )==0){
        printf("%s: parking already exists. \n", nome);
        return;
    }

    /* Validar se a capacidade de parques nÃ£o foi excedida*/
    if (i >= MAXPARQUES) {
        printf("too many parks.\n");
        return;
    }
   
    /* Validar capacidade */
    capacidade = atoi( capacidadePar );
    if (capacidade == 0 && capacidadePar[0] != '0') // numero não é inteiro ! não é necessário testar
       
    if (capacidade <= 0) {
        printf("%d: invalid capacity. \n", capacidade);
        return;
    }

     /* Validar regimeUmaHora */
    regimeUmaHora=atof(regimeUmaHoraPar);
    /* Validar regimeHoras */
    regimeHoras=atof(regimeHorasPar);
    /* Validar regimeDiario */
    regimeDiario=atof(regimeDiarioPar);

    /* Validar tarifario */
    if (regimeUmaHora > regimeHoras || regimeHoras > regimeDiario || regimeUmaHora > regimeDiario || regimeUmaHora <= 0 || regimeHoras <= 0 || regimeDiario <=0) {
        printf("invalid cost. \n");
        return;
    }


    /* Execução */
    strcpy(parques[i].nome, nome);
    parques[i].regimeUmaHora = regimeUmaHora;
    parques[i].regimeHoras = regimeHoras;
    parques[i].regimeDiario = regimeDiario;
    parques[i].capacidadeMax = capacidade;
    parques[i].lugaresDisponiveis = capacidade;

    /* Pos-condicoes */

} 

/* Remove Parque da Lista */

void removeParque(char *nome){
    int i = 0;
    
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), nome )!=0) i++;

    if (strcmp((char *)(parques[i].nome), nome )!=0){
        printf ("%s: no such parking. \n",nome);
    } 
    else {
        /*Apaga regime do parque */
        strcpy(parques[i].nome, "");
        parques[i].regimeUmaHora = 0.0;
        parques[i].regimeHoras = 0.0;
        parques[i].regimeDiario = 0.0;
        parques[i].capacidadeMax = 0;
        parques[i].lugaresDisponiveis = 0; 
    

        /* Compacta Parques - evitar lugares indefinidos */
        struct Parque parqueEliminado = parques[i];
        i++;
        while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")){
            parques[i-1]=parques[i];
            i++;
        }
        parques[i-1] = parqueEliminado;               
    }    
} 
/* FIM PARQUES*/


/*--------------- Funções Base - CMD LINE --------------------*/

/* Função de leitura do comando */

char **lerComando (){

    #define MAXCOMANDO 200
    #define MAXPALAVRAS 20
    #define MAXPALAVRA 50

    const char separadores[10] = " \t\"";
  
    char comando[MAXCOMANDO];
    char token[MAXPALAVRA] ="";
    char *restoComando;
    int iToken =0;
    int inicioToken=0;
    int fimToken=0;
    int aspas = 0;   

    /*Aloca espaço e inicializa*/
    char **palavrasComando;
    palavrasComando=malloc(MAXPALAVRAS * sizeof(char*));
    for (int i = 0; i < MAXPALAVRAS; i++) palavrasComando[i] = NULL;
        
    /* Recebe comando */
    fgets(comando, sizeof(comando), stdin);
   
    /* Remoção de carater \n */
    if (comando[strlen(comando) - 1] == '\n') comando[strlen(comando) - 1] = '\0';

    /* Extração de palavras */
    while ( inicioToken < strlen(comando))
    {
       
       /* Procura separador */
        while (strchr( separadores, comando[fimToken]) == NULL ) 
            fimToken++;
       

        /* No caso de aspas */
        /* Procura segundas aspas */
        if (comando[fimToken] == '"' && aspas==1){
            aspas = 2;           
        }
        else if (comando[fimToken] == '"' && aspas==0){
            /* Identificar primeiras aspas */
            aspas = 1;
            inicioToken=++fimToken;
        }
        else if  (comando[fimToken] != '"' && aspas==1) {
            /* Ignorar separadores entre aspas*/
            fimToken++;
        } else if (inicioToken==fimToken){
            /* Ignora separadores */
            inicioToken++;
            ++fimToken;
        }
        /* Extrai a palavra */
        if (aspas!=1 && inicioToken<fimToken){
           for (int i=0;i<MAXPALAVRA; token[i++] = '\0');
           strncpy(token, comando+(inicioToken), fimToken-inicioToken);
           palavrasComando[iToken] = malloc((int)strlen(token) + 1); 
           strcpy(palavrasComando[iToken],token);
           iToken++; 
           inicioToken =fimToken;
        }
        /*Reset no processamento de aspas */
        if (aspas == 2) {
            inicioToken = ++fimToken;
            aspas = 0;
        }           
    }
    

    return palavrasComando;
}

/* Função para libertar memória alocada */
void limpaComando(char **lista){
    for (int i = 0; i < sizeof(lista); i++) free(lista[i]);
    free(lista);
    lista = NULL;
}

int main(){

    char **entrada;
    char operacao;
    

    do {
        entrada = lerComando();
        operacao = entrada[0][0];

        switch (operacao) {
            case 'q':
                break;
            case 'p':
                if (entrada[1] == NULL){
                    //listarParques(Parques);
                    listaParques();
                }
                else{
                    // criarParque(Parques, componentesComando[1], atoi(componentesComando[2]), atof(componentesComando[3]), atof(componentesComando[4]), atof(componentesComando[5]));
                    criaParque(entrada[1],entrada[2],entrada[3],entrada[4],entrada[5]);
                }
                break;
            case 'r':
                // remove 1 parque
                removeParque(entrada[1]);
                break;
            default:
                printf("erro");
                break;
        }
        limpaComando(entrada);
    }
    while (operacao != 'q');

    return 0;
}