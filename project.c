/* iaed24 - ist1107083 - project */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/* Parque */
#define MAXPARQUES 20

/* Data/Hora */
#define ANO_GENESIS 2000
#define MINUTOS_DIA 1440

/* Variáveis Globais - Entrada/Saída */
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

/* Parques - Variáveis Globais */ 
struct Parque{
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


/* Data/Hora - Variáveis Globais */
int ultimoRegisto = 0;

/* ----------- INICIO PARQUES ------------------ */

/* +----------------------------------------+ */
/* | imprime o conteudo da lista de         | */
/* |  parques                               | */
/* | param:                                 | */
/* | retorno:                               | */
/* +----------------------------------------+ */
void listaParques() {
    int i = 0;
   
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")) {
        printf("%s %d %d\n", (char *)(parques[i].nome), parques[i].capacidadeMax, parques[i].lugaresDisponiveis);
        i++;
    }
}

/* +----------------------------------------+ */
/* | insere um parque na lista de           | */
/* |  parques                               | */
/* | param: nome do parque                  | */
/* |      : capacidade                      | */
/* |      : regime primeira hora            | */
/* |      : regime seguintes horas          | */
/* |      : regime diario                   | */
/* | retorno:                               | */
/* +----------------------------------------+ */

void criaParque(char *nome, char *capacidadePar, char *regimeUmaHoraPar, char *regimeHorasPar, char *regimeDiarioPar) {
    int i = 0;
    int capacidade=0;
    float regimeUmaHora=0;
    float regimeHoras=0;
    float regimeDiario=0;

    /* Pre-condições */

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
    if (capacidade == 0 && capacidadePar[0] != '0') {}// numero não é inteiro ! não é necessário testar
       
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
    /* Execucao */

    strcpy(parques[i].nome, nome);
    parques[i].regimeUmaHora = regimeUmaHora;
    parques[i].regimeHoras = regimeHoras;
    parques[i].regimeDiario = regimeDiario;
    parques[i].capacidadeMax = capacidade;
    parques[i].lugaresDisponiveis = capacidade;
    parques[i].primeiraEntrada =NULL;
    parques[i].primeiraSaida = NULL;

} 

/* +----------------------------------------+ */
/* | remove lista de entradas de            | */
/* |  parque                                | */
/* | param: apontador                       | */
/* |      : nome do parque                  | */
/* | retorno:                               | */
/* +----------------------------------------+ */
 
 void apagaListaE (struct Entrada* nodo){

    if (nodo !=NULL) apagaListaE(nodo->proxEntrada);
    free(nodo);
 }

/* +----------------------------------------+ */
/* | remove lista de entradas/saidas de     | */
/* |  parque                                | */
/* | param: apontador                       | */
/* |      : nome do parque                  | */
/* | retorno:                               | */
/* +----------------------------------------+ */
 void apagaListaES (struct EntradaSaida* nodo){

    if (nodo !=NULL) apagaListaES(nodo->proxEntradaSaida);
    free(nodo);
 }

/* +----------------------------------------+ */
/* | remove um parque na lista de           | */
/* |  parques                               | */
/* | param: lista de parques                | */
/* |      : nome do parque                  | */
/* | retorno:                               | */
/* +----------------------------------------+ */
void removeParque(char *nome){

    struct Parque *parquesOrdenados[MAXPARQUES], *temParq;
    int totalParques=0, i=0, j=0;

    
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), nome )!=0) i++;

    if (strcmp((char *)(parques[i].nome), nome )!=0){
        printf ("%s: no such parking. \n",nome);
    } 
    else {
        // apaga registo de parque
        strcpy(parques[i].nome, "");
        parques[i].regimeUmaHora = 0.0;
        parques[i].regimeHoras = 0.0;
        parques[i].regimeDiario = 0.0;
        parques[i].capacidadeMax = 0;
        parques[i].lugaresDisponiveis = 0; 
    
        apagaListaE(parques[i].primeiraEntrada);
        apagaListaES(parques[i].primeiraSaida);
        
        // compacta Parques evita posições indefinidas entre registos de parques 
        struct Parque parqueEliminado = parques[i];
        i++;
        while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")){
            parques[i-1]=parques[i];
            i++;
        }
        parques[i-1] = parqueEliminado;   

        /* Ordenação dos Parques */
        /* inicialização de estrutura temporária */
        while (totalParques< MAXPARQUES && strcmp((char *)(parques[totalParques].nome), "")){
            parquesOrdenados[totalParques] = &parques[totalParques];
            totalParques++;
        }

        /* Ordena estrutura temporária */
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
            printf("%s\n", parquesOrdenados[i]->nome);
        }
    }    
} 

/* ------- FIM PARQUES ---------------*/


/* -------------------- INICIO ENTRADAS/SAIDAS -------------------- */

/* +----------------------------------------+ */
/* | Valida matricula                       | */
/* | param: string matricula                | */
/* | retorno: bool  0-Ok, 1-NOk             | */
/* +----------------------------------------+ */

int ValidaMatricula ( char *matricula){
    int retorno=1;
    char alfint[8];

    if (matricula[2] =='-' && matricula[5]=='-') {
        matricula[2] = matricula[5]='0';
        if (strlen(matricula)==8){
            for (int i=0; i < 8; i++){
                if (matricula[i] >='A' &&  matricula[i] <='Z') alfint[i] = 'a';
                else if (isdigit(matricula[i])) alfint[i] = 'd';
                else retorno = 0;
            }
            if (retorno==1 && alfint[0] == alfint[1] && alfint[3] == alfint[4] && alfint[6] == alfint[7] && (alfint[0] != alfint[3] || alfint[4] != alfint[6]) ) {
                retorno = 1;
            }
            else retorno=0;
        }
         else retorno =0;
   }
    else retorno=  0;
    matricula[2] = matricula[5]='-';
    return retorno;
}


const int DiasMes[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31  };
/* +----------------------------------------+ */
/* | Converte data e hora para nr de minutos| */
/* |    desde inicio dos tempos - GENESIS   | */
/* | param: data, hora                      | */
/* | retorno: 0-erro                        | */
/* +----------------------------------------+ */

int converteDataHora (char *data, char *hora){

    int retorno = 1;
    int dd, mm, aaaa, hh, mi, i;
    char horaReact[5]="0\0" ;
    char temp[5] = "\0";

    /* Garante hora no formato HH:MM */
    horaReact[0]='0';
    if (strlen(hora)<5) {
        strcat (horaReact,hora);
        strcpy (hora,horaReact);
    } 

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


/* +----------------------------------------+ */
/* | Calcula valor a pagar                  | */
/* | param: inicio - entrada no parque      | */
/* |        fim - saida do parque           | */
/* |        x - tarifa 1ª hora              | */
/* |        y - tarifa horas seguintes      | */
/* |        x - tarifa diaria               | */
/* | retorno: valor a pagar                 | */
/* +----------------------------------------+ */
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

/* +----------------------------------------+ */
/* | Entrada de um veiculo no Parque        | */
/* |                                        | */
/* | param: parque, veiculo, data, hora     | */
/* | retorno: 0-erro | 1-Ok                 | */
/* +----------------------------------------+ */
int entradaVeiculo ( char *parque, char * matricula, char *data, char *hora){

    int retorno =1;

    int unifiedMin = 0;
    
    struct Entrada *novaEntrada, *entPtr;
    // pre condicoes 
    int i = 0, j=0;
    // Verifica se parque esta definido   
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "") && strcmp((char *)(parques[i].nome), parque) ) i++;
    if (strcmp((char *)(parques[i].nome), parque) !=0) {
        printf ("%s: No such parking.\n", parque);
        retorno =0;
    } else if (parques[i].lugaresDisponiveis == 0){
            // verifica se ainda existem lugares disponiveis no parque
            printf("%s: parking is full.\n", parque);
            retorno =0;
            }
    // verifica se matricula é aceitavel
    if (retorno !=0 && ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate.\n", matricula);
        retorno =0;
    }
    // converte data verificando se e valida
    unifiedMin = converteDataHora(data, hora) ;
    // verifica se data/hora e superior ao ulktimo movimento
    if (retorno !=0 && (unifiedMin == 0 || (unifiedMin < ultimoRegisto))) {
        printf("invalid date.\n");
        retorno =0;
    }
    // verifica se veiculo está em algum parque
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

    // Cria novo registo de entrada
    if (retorno == 1) {
        novaEntrada = malloc(sizeof(struct Entrada));
        strcpy(novaEntrada->veiculo, matricula);
    
        strcpy(novaEntrada->data,data);
        strcpy(novaEntrada->hora, hora);

        novaEntrada->unifiedMinutos=unifiedMin;

        novaEntrada->proxEntrada= NULL;

        // insere novo registo na lista ligada e no parque respectivo
        if (parques[i].primeiraEntrada == NULL) {
            parques[i].primeiraEntrada = novaEntrada;
        }
        else {
            entPtr = parques[i].primeiraEntrada;
            while (entPtr->proxEntrada != NULL) 
                entPtr = entPtr->proxEntrada;
            entPtr->proxEntrada =  novaEntrada; 
        }

        // actualizar Data/Hora do ultimo movimento
        ultimoRegisto = unifiedMin;
        // actualizar ocupação do parque
        parques[i].lugaresDisponiveis--;
        printf("%s %d\n",parques[i].nome, parques[i].lugaresDisponiveis  );
    }

    return retorno;
}

/* +----------------------------------------+ */
/* | Saida de um veiculo no Parque        | */
/* |                                        | */
/* | param: parque, veiculo, data, hora     | */
/* | retorno: 0-erro | 1-Ok                 | */
/* +----------------------------------------+ */

int saidaVeiculo ( char *parque, char * matricula, char *data, char *hora){

    int retorno =1;

    int unifiedMin = 0;
    
    struct Entrada *entPtr=NULL, *previoPtr=NULL;
    struct EntradaSaida *entSaiPtr=NULL, *novaEntSaida=NULL;
    // pre condicoes 
    int i = 0, encontrei=0;
    // Verifica se parque esta definido   
    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "") && strcmp((char *)(parques[i].nome), parque) ) i++;
    if (strcmp((char *)(parques[i].nome), parque) !=0) {
        printf ("%s: No such parking. \n", parque);
        retorno =0;
    } 
    // verifica se matricula é aceitavel
    if (retorno !=0 && ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate. \n", matricula);
        retorno =0;
    }
    // converte data verificando se e valida
    unifiedMin = converteDataHora(data, hora) ;
    // verifica se data/hora e superior ao ulktimo movimento
    if (retorno !=0 && unifiedMin == 0 || (unifiedMin < ultimoRegisto)) {
        printf("invalid date.\n");
        retorno =0;
    }
    // verifica se veiculo está no parque
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

    // Cria novo registo de entrada/saida
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

        // imprime resultado
        printf("%s %s %s %s %s %.2f\n", matricula, novaEntSaida->dataEntrada, novaEntSaida->horaEntrada, novaEntSaida->dataSaida, novaEntSaida->horaSaida, novaEntSaida->Custo );
  
        // insere novo registo de saida na lista ligada e no parque respectivo
        if (parques[i].primeiraSaida == NULL) {
            parques[i].primeiraSaida = novaEntSaida;
        }
        else {
            entSaiPtr = parques[i].primeiraSaida;
            while (entSaiPtr->proxEntradaSaida != NULL) 
                entSaiPtr = entSaiPtr->proxEntradaSaida;
            entSaiPtr->proxEntradaSaida =  novaEntSaida; 
        }

        // elimina registo de entrada correspondente
        if (previoPtr == NULL){
            parques[i].primeiraEntrada = entPtr->proxEntrada;
        }
        else {
            previoPtr->proxEntrada = entPtr->proxEntrada;
        }
   
          // actualizar Data/Hora do ultimo movimento
        ultimoRegisto = unifiedMin;
        // actualizar ocupação do parque
        parques[i].lugaresDisponiveis++;
        // liberta memoria do registo de entrada
        free(entPtr);
    }

    return retorno;

}

/* +----------------------------------------+ */
/* |Imprime veiculos que entraram e sairam  | */
/* |    do Parque                           | */
/* | param: Parque                          | */
/* |        veiculo                         | */
/* |        registo entrada/saida           | */
/* |        contador de linhas impressas    | */
/* | retorno: 0-erro | 1-Ok                 | */
/* +----------------------------------------+ */

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

/* +----------------------------------------+ */
/* |Imprime veiculos que entraram no        | */
/* |    Parque                              | */
/* | param: Parque                          | */
/* |        veiculo                         | */
/* |        registo entrada                 | */
/* |        contador de linhas impressas    | */
/* | retorno: 0-erro | 1-Ok                 | */
/* +----------------------------------------+ */

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

/* +----------------------------------------+ */
/* | Historico de veiculo nos Parques       | */
/* |                                        | */
/* | param: veiculo                         | */
/* | retorno: 0-erro | 1-Ok                 | */
/* +----------------------------------------+ */

int historicoVeiculo (  char * matricula){

    int retorno =1;
    
    struct Parque *parquesOrdenados[MAXPARQUES], *temParq;
    int totalParques=0, i=0, j=0;
    int linhas=0;

    /* pre-condicoes*/

    // verifica se matricula é aceitavel
    if (ValidaMatricula(matricula) != 1) {
        printf("%s: invalid licence plate. \n", matricula);
        retorno =0;
    } else {

        // Ordena Parques

        // inicializa estrutura temporaria
        while (totalParques< MAXPARQUES && strcmp((char *)(parques[totalParques].nome), "")){
            parquesOrdenados[totalParques] = &parques[totalParques];
            totalParques++;
        }
        // ordena estrutura temporaria
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
        // não ha registos do veiculo
        if (linhas ==0) {
            printf("%s: no entries found in any parking.\n", matricula);
            retorno =0;
        }       
    }
    return retorno;
}

// ----------- Fim Entradas Saidas ---------------------------------

////////////////////////////////////////////////////////////////////

// ----------- Inicio Facturação -----------------------------------

/* +----------------------------------------+ */
/* | Facturação acumulada do Parque         | */
/* |   Calcula acumulado e imprime linha    | */
/* | param: data      (ref)                 | */
/* |      : acumulado (ref)                 | */
/* |      : proximo registo                 | */
/* | retorno:                               | */
/* +----------------------------------------+ */
void imprimeLinhaAcum( char* data, float* acumulado, struct EntradaSaida *ptr){
    
    if (ptr == NULL) {
        // fim da lista imprimir remanescente 
        if (*acumulado > 0) printf ("%s %.2f\n", data, *acumulado);
    } else {
        if(strcmp(data,"")==0){
            /* inicio da lista inicializacao */
            strcpy(data, ptr->dataSaida);
            *acumulado = ptr->Custo;
        } else if (strcmp(data, ptr->dataSaida)<0){
                // troca de data
                printf ("%s %.2f\n", data, *acumulado);
                strcpy(data, ptr->dataSaida);
                *acumulado = ptr->Custo;
            } else {
            *acumulado += ptr->Custo;
             }
       imprimeLinhaAcum( data, acumulado, ptr->proxEntradaSaida);
    }
}

/* +----------------------------------------+ */
/* | Facturação Detalhada do Parque         | */
/* |   Calcula acumulado e imprime linha    | */
/* | param: data      (ref)                 | */
/* |      : proximo registo                 | */
/* | retorno:                               | */
/* +----------------------------------------+ */
void imprimeLinhaDet( char* data, struct EntradaSaida *ptr){
    
    if (ptr != NULL) {
        if (strcmp(data, ptr->dataSaida)==0){
            printf ("%s %s %.2f\n", ptr->veiculo, ptr->horaSaida, ptr->Custo);
        }
        imprimeLinhaDet(data, ptr->proxEntradaSaida);
    }    
}

/* +----------------------------------------+ */
/* | Facturação acumulada do Parque         | */
/* |                                        | */
/* | param: Parque                          | */
/* | retorno:                               | */
/* +----------------------------------------+ */
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

/* +----------------------------------------+ */
/* | Facturação acumulada do Parque         | */
/* |                                        | */
/* | param: Parque, Data                    | */
/* | retorno:                               | */
/* +----------------------------------------+ */

void listaFactParDet(char* parque, char* data ){

    int i=0;
    int unifiedMin=0;
    char hora[5]="00:00";

    while(i< MAXPARQUES && strcmp((char *)(parques[i].nome), "")!=0 && strcmp((char *)(parques[i].nome), parque )!=0) i++;

    if (strcmp((char *)(parques[i].nome), parque )!=0){
        printf ("%s: no such parking. \n",parque);
    } else {
        
        // converte data verificando se e valida
        unifiedMin = converteDataHora(data, hora) ;
        // verifica se data/hora e superior ao ultimo movimento
        if (unifiedMin == 0 || (unifiedMin > ultimoRegisto)) {
            printf("invalid date.");
        } else imprimeLinhaDet(data, parques[i].primeiraSaida);
    } 
}

// ----------- Fim facturação --------------------------------------


/* +----------------------------------------+ */
/* | Lê comando do stdio e                  | */
/* |  devolve uma lista com cada elemento   | */
/* | param:                                 | */
/* | retorno: lista com palavras do comando | */
/* +----------------------------------------+ */

char **lerComando (){

    #define MAXCOMANDO 9000
    #define MAXPALAVRAS 20
    #define MAXPALAVRA 8192

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
        
    /* Leitura de comando */
    fgets(comando, sizeof(comando), stdin);
   
    /* Remocao de carater \n */
    if (comando[strlen(comando) - 1] == '\n') comando[strlen(comando) - 1] = '\0';
    // Extrai palavras
    while ( inicioToken < strlen(comando))
    {
        // encontra separador
        while (strchr( separadores, comando[fimToken]) == NULL ) fimToken++;
       
        if (comando[fimToken] == '"' && aspas==1){
            // identificadas segundas aspas
             aspas = 2;           
        }
        else if (comando[fimToken] == '"' && aspas==0){
            //identificadas primeiras aspas
            aspas = 1;
            inicioToken=++fimToken;
        }
        else if  (comando[fimToken] != '"' && aspas==1) {
            // ignorar separador entre aspas
            fimToken++;
        } else if (inicioToken==fimToken){
            // ignora separadores 
            inicioToken++;
            ++fimToken;
        }
        // extrai palavra
        if (aspas!=1 && inicioToken<fimToken){
           for (int i=0;i<MAXPALAVRA; token[i++] = '\0');
           strncpy(token, comando+(inicioToken), fimToken-inicioToken);
           palavrasComando[iToken] = malloc((int)strlen(token) + 1); 
           strcpy(palavrasComando[iToken],token);
           iToken++; 
           inicioToken =fimToken;
        }
        // reset de processamento entre aspas
        if (aspas == 2) {
            inicioToken = ++fimToken;
            aspas = 0;
        }           
    }
    
    return palavrasComando;
}

/* +----------------------------------------+ */
/* | Liberta memoria alocada para o comando | */
/* |  apos processamento                    | */
/* | param: apontador para comando          | */
/* | retorno:                               | */
/* +----------------------------------------+ */
void limpaComando(char **lista){    for (int i = 0; i < sizeof(lista); i++) free(lista[i]);
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
