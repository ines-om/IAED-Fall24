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

/* Parques - Variáveis Globais */
struct Parque {
    char nome[8192];

    int capacidadeMax;
    int lugaresDisponiveis;

    float regimeUmaHora;
    float regimeHoras;
    float regimeDiario;

    struct Entrada* primeiraEntrada;
    struct EntradaSaida* primeiraSaida;
};


struct Parque parques[MAXPARQUES];


// Data/Hora - Variáveis Globais 
int ultimoRegisto = 0;

/* Entrada/ Saída - Variáveis Globais */
struct Entrada {
    char veiculo[9];
    
    char data[11];
    char hora[6];

    int unifiedMinutos;

    struct Entrada* proxEntrada;
};

struct EntradaSaida {
    char veiculo[9];
    
    char dataEntrada[11];
    char horaEntrada[6];
    int unifiedMinutosEntrada;

    char dataSaida[11];
    char horaSaida[6];
    int unifiedMinutosSaida;

    float Custo;

    struct EntradaSaida* proxEntradaSaida;
};

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

/* Função que lista parques existentes */
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

    /* número não é inteiro, não é necessário testar*/
    if (capacidade == 0 && capacidadePar[0] != '0')
       
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
/* ----------- FIM PARQUES ----------- */

/* ---------- INÍCIO ENTRADAS E SAÍDAS -----------*/

/* Entrada de um veiculo no Parque                                  
|      Parametros:
|        - parque, veiculo, data, hora     
|      Retorno: 
|        - 0: erro | 1: Ok                  */

int entradaVeiculo ( char *parque, char * matricula, char *data, char *hora){

    int retorno =1;

    int unifiedMin = 0;
    
    struct Entrada *novaEntrada, *entPtr;
    
    /* Pré-condições*/
    int i = 0, j=0;
    
    /* Verifica se parque existe */  
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "") && strcmp((char *)(parques[i].nome), parque) ) i++;
    if (strcmp((char *)(parques[i].nome), parque) !=0) {
        printf ("%s: No such parking.\n", parque);
        retorno =0;

    /* Verifica se ainda existem lugares disponíveis */
    } else if (parques[i].lugaresDisponiveis == 0){
            printf("%s: parking is full.\n", parque);
            retorno =0;
            }
    /* Verifica se a matrícula é válida */
    if (retorno !=0 && ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate.\n", matricula);
        retorno =0;
    }
    
    /* Converte data e verifica se é válida */
    unifiedMin = converteDataHora(data, hora) ;

    /* Verifica se data/hora e superior ao ultimo movimento */
    if (retorno !=0 && (unifiedMin == 0 || (unifiedMin < ultimoRegisto))) {
        printf("invalid date.\n");
        retorno =0;
    }
    
    /* Verifica se veículo está noutro parque */
    j=0;
    while(j< MAXPARQUES && strcmp((char *)(parques[j].nome), "") && (retorno !=0)){
        entPtr= parques[j].primeiraEntrada;
            while ((entPtr != NULL)&& (retorno !=0)) {
                if (strcmp((char *)(entPtr->veiculo), matricula)==0){
                    printf("%s: invalid vehicle entry.\n", matricula);
                    retorno=0;
                }
                else {
                    entPtr = entPtr->proxEntrada;
                }
            }
            j++;
    }

    /* Execução */

    /* Cria novo registo de entrada */
    if (retorno == 1) {
        novaEntrada = malloc(sizeof(struct Entrada));
        strcpy(novaEntrada->veiculo, matricula);
    
        strcpy(novaEntrada->data,data);
        strcpy(novaEntrada->hora, hora);

        novaEntrada->unifiedMinutos=unifiedMin;

        novaEntrada->proxEntrada= NULL;

        /* insere novo registo na lista ligada e no parque respectivo */
        if (parques[i].primeiraEntrada == NULL) {
            parques[i].primeiraEntrada = novaEntrada;
        }
        else {
            entPtr = parques[i].primeiraEntrada;
            while (entPtr->proxEntrada != NULL) 
                entPtr = entPtr->proxEntrada;
            entPtr->proxEntrada =  novaEntrada; 
        }

        /* actualizar Data/Hora do ultimo movimento */
        ultimoRegisto = unifiedMin;
        
        /* Atualiza oupação do parque */
        parques[i].lugaresDisponiveis--;
        printf("%s %d\n",parques[i].nome, parques[i].lugaresDisponiveis  );
    }

    return retorno;
}

/* Saída de veículo no parque 
|       Parametros:
|       - parque, matricula, data e hora
|        Retorno:
|       0: erro | 1: ok                    */

int saidaVeiculo ( char *parque, char * matricula, char *data, char *hora){

    int retorno =1;

    int unifiedMin = 0;
    
    struct Entrada *entPtr=NULL, *previoPtr=NULL;
    struct EntradaSaida *entSaiPtr=NULL, *novaEntSaida=NULL;
    
    /* Pré-condições*/
    int i = 0, encontrei=0;

    /* Verifica se parque existe */ 
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "") && strcmp((char *)(parques[i].nome), parque) ) i++;
    if (strcmp((char *)(parques[i].nome), parque) !=0) {
        printf ("%s: No such parking. \n", parque);
        retorno =0;
    } 
    
    /* Verifica se a matrícula é válida */
    if (retorno !=0 && ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate. \n", matricula);
        retorno =0;
    }
    
    /* Converte e verifica validade das horas */
    unifiedMin = converteDataHora(data, hora) ;
    
    /*Verifica data e hora até ao último momento */
    if (retorno !=0 && unifiedMin == 0 || (unifiedMin < ultimoRegisto)) {
        printf("invalid date.\n");
        retorno =0;
    }

    /* Verifica se veículo está no parque*/
    entPtr= parques[i].primeiraEntrada;
    while (retorno !=0 && (entPtr != NULL)&& (encontrei ==0)) {
        if (strcmp((char *)(entPtr->veiculo), matricula)==0){
            encontrei=1;
        }
        else {
            previoPtr = entPtr;
            entPtr = entPtr->proxEntrada;
            }
    } 
    if (retorno !=0 && encontrei ==0) {
        printf("%s: invalid vehicle exit.\n", matricula);
        retorno = 0;
    }  


    /* Execução */

    /* Novo registo */
    if (retorno == 1) {
        novaEntSaida = malloc(sizeof(struct EntradaSaida));
        strcpy(novaEntSaida->veiculo, matricula);

        strcpy(novaEntSaida->dataEntrada,entPtr->data);
        strcpy(novaEntSaida->horaEntrada,entPtr->hora);

        novaEntSaida->unifiedMinutosEntrada=entPtr->unifiedMinutos;
    
        strcpy(novaEntSaida->dataSaida ,data);
        strcpy(novaEntSaida->horaSaida, hora);
        novaEntSaida->unifiedMinutosSaida=unifiedMin;

        novaEntSaida->Custo = calculaTarifa(novaEntSaida->unifiedMinutosEntrada,novaEntSaida->unifiedMinutosSaida, parques[i].regimeUmaHora,parques[i].regimeHoras, parques[i].regimeDiario);

        novaEntSaida->proxEntradaSaida= NULL;

        /* Imprime resultado */
        printf("%s %s %s %s %s %.2f\n", matricula, novaEntSaida->dataEntrada, novaEntSaida->horaEntrada, novaEntSaida->dataSaida, novaEntSaida->horaSaida, novaEntSaida->Custo );
  
        /* Insere novo registo */
        if (parques[i].primeiraSaida == NULL) {
            parques[i].primeiraSaida = novaEntSaida;
        }
        else {
            entSaiPtr = parques[i].primeiraSaida;
            while (entSaiPtr->proxEntradaSaida != NULL) 
                entSaiPtr = entSaiPtr->proxEntradaSaida;
            entSaiPtr->proxEntradaSaida =  novaEntSaida; 
        }

        /* Elimina registo correspondente */
        if (previoPtr == NULL){
            parques[i].primeiraEntrada = entPtr->proxEntrada;
        }
        else {
            previoPtr->proxEntrada = entPtr->proxEntrada;
        }
   
        /* Atualização de Data/Hora */
        ultimoRegisto = unifiedMin;
        
        /* Atualização de lugares disponíveis */
        parques[i].lugaresDisponiveis++;
        
        /* Liberta memória alocada */
        free(entPtr);
    }

    return retorno;

}


/* Imprime veiculos que entraram e sairam  
|    Retorno:
|    - 0: erro | 1: ok                        */

int imprimeLinhaVeiculoES (char * parque, char * matricula, struct EntradaSaida *entSaiPtr, int linhas){
    int ret=linhas;

    if (entSaiPtr != NULL){
       if (strcmp(entSaiPtr->veiculo, matricula)== 0){
            ret++;
            printf("%s %s %s %s %s\n", parque, entSaiPtr->dataEntrada, entSaiPtr->horaEntrada, entSaiPtr->dataSaida, entSaiPtr->horaSaida);
       }
       ret = imprimeLinhaVeiculoES(parque, matricula, entSaiPtr->proxEntradaSaida, ret);
    }
    return ret;
}


/* Imprime veiculos que entraram no       
|       Retorno: 
|        - 0-erro | 1-Ok                 */

int imprimeLinhaVeiculoE (char * parque, char * matricula, struct Entrada *entPtr, int linhas){
    int ret=linhas;
    if (entPtr != NULL){
       if (strcmp(entPtr->veiculo, matricula)== 0){
            ret++;
            printf("%s %s %s\n", parque, entPtr->data, entPtr->hora);
       }
       ret = imprimeLinhaVeiculoE(parque, matricula, entPtr->proxEntrada, ret);
    }
    return ret;
}


/* | Historico de veiculo nos Parques       
|       Retorno: 
|        - 0-erro | 1-Ok                 */


int historicoVeiculo (  char * matricula){

    int retorno =1;
    
    struct Parque *parquesOrdenados[MAXPARQUES], *temParq;
    int totalParques=0, i=0, j=0;
    int linhas=0;

    /* pre-condicoes*/

    /* Vê se matrícula é válida */
    if (ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate. \n", matricula);
        retorno =0;
    } else {

        /* Ordenação dos parques */

        /* Inicialização de estrutura temporária */
        while (totalParques< MAXPARQUES && strcmp((char *)(parques[totalParques].nome), "")){
            parquesOrdenados[totalParques] = &parques[totalParques];
            totalParques++;
        }

        /* Ordena estrutura temporaria */
        for (i = 0; i < totalParques; ++i)  {
            for (j = i + 1; j < totalParques; ++j) {
                if (strcmp(parquesOrdenados[i]->nome,parquesOrdenados[j]->nome)>0) {
                    temParq =  parquesOrdenados[i];
                    parquesOrdenados[i] = parquesOrdenados[j];
                    parquesOrdenados[j] = temParq;
                }
            }
        }    

        for (i = 0; i < totalParques; ++i)  {
            linhas = imprimeLinhaVeiculoES(parquesOrdenados[i]->nome, matricula, parquesOrdenados[i]->primeiraSaida,linhas);
            linhas = imprimeLinhaVeiculoE(parquesOrdenados[i]->nome, matricula, parquesOrdenados[i]->primeiraEntrada,linhas);
        } 

        /* não ha registos do veiculo */
        if (linhas ==0) {
            printf("%s: no entries found in any parking.\n", matricula);
            retorno =0;
        }       
    }
    return retorno;
}

/* -------------- FIM ENTRADAS/SAÍDAS --------------------------------- */



/* ----------- Inicio Facturação ----------------------------------- */

/*  Facturação acumulada do Parque  */
void imprimeLinhaAcum( char* data, float* acumulado, struct EntradaSaida *ptr){
    
    if (ptr == NULL) {
        /* fim da lista imprimir remanescente */
        if (*acumulado > 0) printf ("%s %.2f\n", data, *acumulado);
    } else {
        if(strcmp(data,"")==0){
            /* inicio da lista inicializacao */
            strcpy(data, ptr->dataSaida);
            *acumulado = ptr->Custo;
        } else if (strcmp(data, ptr->dataSaida)<0){
                /*  Troca data */
                printf ("%s %.2f\n", data, *acumulado);
                strcpy(data, ptr->dataSaida);
                *acumulado = ptr->Custo;
            } else {
            *acumulado += ptr->Custo;
             }
       imprimeLinhaAcum( data, acumulado, ptr->proxEntradaSaida);
    }
}


/* Facturação Detalhada do Parque */
void imprimeLinhaDet( char* data, struct EntradaSaida *ptr){
    
    if (ptr != NULL) {
        if (strcmp(data, ptr->dataSaida)==0){
            printf ("%s %s %.2f\n", ptr->veiculo, ptr->horaSaida, ptr->Custo);
        }
        imprimeLinhaDet(data, ptr->proxEntradaSaida);
    }    
}


/* Facturação acumulada do Parque */

void listaFactParAc(char* parque){

    int i = 0;
    float acumulado=0.0;
    char data[11] = ""; 
        
    
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), parque )!=0) i++;

    if (strcmp((char *)(parques[i].nome), parque )!=0){
        printf ("%s: no such parking. \n",parque);
    } 
    else  imprimeLinhaAcum( data, &acumulado, parques[i].primeiraSaida);
}


/* Facturação detalhada do Parque  */                            

void listaFactParDet(char* parque, char* data ){

    int i=0;
    int unifiedMin=0;
    char hora[5]="00:00";

    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), parque )!=0) i++;

    if (strcmp((char *)(parques[i].nome), parque )!=0){
        printf ("%s: no such parking. \n",parque);
    } else {
        
        /* Converte data e verifica se é válida */
        unifiedMin = converteDataHora(data, hora);

        /* verifica se data/hora e superior ao ultimo movimento */
        if (unifiedMin == 0 || (unifiedMin > ultimoRegisto)) {
            printf("invalid date.");
        } else imprimeLinhaDet(data, parques[i].primeiraSaida);
    } 
}

/* ---------- FIM FATURAÇÃO -----------------*/


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
    for (int i = 0; i < sizeof(lista); i++) 
        free(lista[i]);
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
                    listaParques();
                }
                else{
                    criaParque(entrada[1],entrada[2],entrada[3],entrada[4],entrada[5]);
                }
                break;
            case 'r':
                removeParque(entrada[1]);
                break;

            case 'e':
                entradaVeiculo( entrada[1],entrada[2],entrada[3],entrada[4]);
                break;
            case 's':
                saidaVeiculo( entrada[1],entrada[2],entrada[3],entrada[4]);
                break;
            case 'v':
                historicoVeiculo( entrada[1]);
                break;
            case 'f':
                if (entrada[2] == NULL){
                    /* Lista faturação do dia */
                    listaFactParAc(entrada[1]);
                }
                else{
                    /* Lista faturação detalhada */
                    listaFactParDet(entrada[1],entrada[2]);
                }
                break;
            default:
                printf("Error.");
                break;
        }
        limpaComando(entrada);
    }
    while (operacao != 'q');

    return 0;
}