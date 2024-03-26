/* iaed24 - ist1107083 - project */

/* BIBLIOTECAS UTILIZADAS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/* DEFINIÇÕES REFERENTES AO INPUT - CMDLINE */
#define MAXCOMANDO 200
#define MAXPALAVRAS 20

/* DEFINIÇÕES REFERENTES A PARQUES */
#define MAXPARQUES 20

/* DEFINIÇÕES REFERENTES A DATA/HORA*/
#define ANO_GENESIS 2000
#define MINUTOS_DIA 1440


/*----- INÍCIO DATA/HORA -----*/

/* Quantidade de dias em cada mês do ano (sem anos bissextos) */
const int DiasMes[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31  };

/* Converte Data/Hora a um único inteiro (número de minutos desde ANO_GENESIS)*/
int converteDataHora (char *data, char *hora){

    int retorno = 1;
    int dd, mm, aaaa, hh, mi, i;

    /*Variável auxiliar*/
    char temp[4] = "\0";

    /* Conversão de valores para inteiros */
    /* - dia */
    temp[0] = data[0]; temp[1] = data[1]; temp[2] = '\0';
    dd = atoi(temp);

    /* - mês */
    temp[0] = data[3]; temp[1] = data[4]; temp[2] = '\0';
    mm = atoi(temp);

    /* - ano */
    temp[0] = data[6]; temp[1] = data[7]; temp[2] = data[8]; temp[3] = data[9];temp[4] = '\0';
    aaaa = atoi(temp);


    /* - hora */
    temp[0] = hora[0]; temp[1] = hora[1]; temp[2] = '\0';
    hh = atoi(temp);

    /* - minuto */
    temp[0] = hora[3]; temp[1] = hora[4]; temp[4] = '\0';
    mi = atoi(temp);


    /* Avaliação do ano e sua conversão em dias */
    if(aaaa>=ANO_GENESIS && aaaa<=9999){ 
        retorno = (aaaa-ANO_GENESIS)*365; /* numero de dias desde ano genesis ate inicio deste ano */
        
        /* Avaliação do mês e sua conversão em dias */
        if(mm>=1 && mm<=12){            
            i = 0;

            /* Adição da quantidade de dias entre ano corrente até início do mês atual; através da lista DiasMes */
            while (i < mm) 
                retorno += DiasMes[i++]; 
            
            /* Cálculo de dias completos dentro do mês atual*/
            if (dd>=1 && dd <= DiasMes[i]) {
                retorno += (dd-1);

                /* Conversão de número de dias total (desde ANO_GENESIS) para minutos*/  
                retorno *= MINUTOS_DIA;
                
                /* Conversão da hora do dia em minutos */
                if (hh>= 0 && hh<24){
                    retorno += hh*60;

                    /* Adição do número de minutos */
                    if (mi>= 0 && mi<60){
                        retorno +=mi;   
                    }
                    else retorno = 0;  /* Minuto Inválido */
                }
                else retorno =0;  /* Hora Inválida */
           } 
           else retorno =0;  /* Dia Inválido */                
        }
        else retorno =0; /* Mês Inválido */
    }
    else  retorno =0; /* Ano Inválido */
   
    /* Devolução de data e hora num único inteiro (minutos desde ANO_GENESIS)*/
    return retorno;   
}


/*  Função de Cálculo de tarifa
|    Parâmetros:
|        X1h - Valor a pagar a cada 15 min na 1ª hora
|        y - Valor a pagar nas horas seguintes
|        z - Valor a pagar 
|    Retorna:
|        Tarifa a ser paga                               */

float calculaTarifa(int inicio, int fim, float X1h, float y, float z){
    
    int totalMinutos= 0, periodos15min=0, dias=0;
    float tarifa = 0.0, tarifaUltimoDia=0.0;

    /* Calcula número de minutos passados no parque */
    totalMinutos = fim - inicio;

    /* ERRO: Quantidade de tempo inválida */
    if (totalMinutos< 0) printf(" invalid date. /n");

    /* Conversão de minutos para períodos tarifados - Dias, horas após primeira, quartos de hora na primeira hora*/
    else {
        /* Cálculo de Número de dias*/
        dias = totalMinutos/MINUTOS_DIA;

        /*  Minutos restantes */
        totalMinutos %=MINUTOS_DIA;

        /*Adição à tarifa de quantidade a tarifar já existente - TARIFA DIÁRIA*/
        tarifa += (dias*z); 

        /* Cálculo de resto da tarifa - No caso de quantidade de minutos restantes não ser nulo*/
        if (totalMinutos> 0){
            /* Cálculo de períodos de 15 min*/
            periodos15min = totalMinutos/15;

            /*Minutos restantes contam como um período adicional de 15 min*/
            if ((totalMinutos%15) !=0) 
                periodos15min++;
            
            /* Tarifa se veículo passar mais de uma hora no parque no seu último dia*/
            if (periodos15min>4) 
                tarifaUltimoDia = (4 * X1h)+ (periodos15min-4) * y;
            
            /* Tarifa se passar apenas 1h ou menos */
            else tarifaUltimoDia = periodos15min*X1h;

            /* Se a tarifa do dia for superior à diária, paga a tarifa diária*/
            if (tarifaUltimoDia>z) 
                tarifa +=z;
            else 
                tarifa += tarifaUltimoDia;

        }
    }
    return tarifa;
 
}
/* ------ FIM DATA/HORA ------*/



/* ----- INÍCIO VEÍCULO ----- */

/* Função de validação de Matrícula 
|  retorno = 0 -> matrícula válida
|   retorno = 1 -> matrícula inválida  */

int ValidaMatricula ( char *matricula){

    int retorno = 0;

    /* Comprimento da matrícula: */
    char alfint[8];

    /* Confirmar presença de 3 pares de valores e sua separação por '-' */
    if (matricula[2] =='-' && matricula[5]=='-') {
        matricula[2] = matricula[5]='0';

        /*Confirmar que tem 8 caráteres */
        if (strlen(matricula)==8){
            for (int i=0; i < 8; i++){

                /* Confirmar que letras presentes são maiusculas e identifica-las com 'a'*/
                if (matricula[i] >='A' &&  matricula[i] <='Z') alfint[i] = 'a';

                /*Identificação de dígitos com 'd' */
                else if (isdigit(matricula[i])) alfint[i] = 'd';

                /* ERRO: Letra é minúscula */
                else retorno = 1;
            }

            /*Confirmar que cada par ou é dd e aa e que há pelo menos um de cada tipo*/
            if (retorno==0 && alfint[0] == alfint[1] && alfint[3] == alfint[4] && alfint[6] == alfint[7] && (alfint[0] != alfint[3] || alfint[4] != alfint[6]) ) {
                retorno = 0;
            }
            else 
                retorno = 1; /*ERRO: Pares são todos do mesmo tipo/ Há pares da ou ad*/
        }
         else retorno = 1; /* ERRO: Comprimento errado */
   }
    else retorno =  1; /* ERRO: Pares não são separados por '-' */

    return retorno;
}

/* ----- FIM VEÍCULO ------*/


/* ----- INÍCIO PARQUES -----*/

/* Estrutura Parque */
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

    /* Validar se a capacidade de parques não foi excedida*/
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
/* ------ FIM PARQUES ------ */


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