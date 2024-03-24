/* iaed24 - ist1107083 - project */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Definição de limites para comprimento do input e de cada componente do mesmo */
#define MAXCOMANDO 200
#define MAXPALAVRAS 20

/* função para interpretação de comando */

char **lerComando (){

    const char separadores[10] = " \t";
  
    char comando[MAXCOMANDO];
    char *token;
    char *restoComando;
    int iToken =0;   

    /*Aloca espaço e inicializa*/
    char **palavrasComando;
    palavrasComando = malloc(MAXPALAVRAS * sizeof(char*));
    for (int i = 0; i < MAXPALAVRAS; i++) palavrasComando[i] = NULL;
        
    /* Reebe e lê comando */
    fgets(comando, sizeof(comando), stdin);
    printf("Comando: %s\n", comando);

    /* Remocao de carater \n e \0 */
    if (comando[strlen(comando) - 1] == '\n')
        comando[strlen(comando) - 1] = '\0';

    restoComando = comando;

    while ((token = strtok_r(restoComando, separadores, &restoComando))){
        palavrasComando[iToken] = malloc((int)strlen(token) + 1); 
        strcpy(palavrasComando[iToken],token);
        printf( "Token[%i] - %s\n", iToken, palavrasComando[iToken] );
        iToken++;
    }

    return palavrasComando;

}

int main(){

    char **entrada;
    char operacao;

    do {
        entrada = lerComando();
        
        int i=0;
        while (entrada[i] != NULL){
        printf( "Token[%i] - %s\n", i, entrada[i++] );
        }

        operacao = entrada[0][0];

    }
    while (operacao != 'q');

    return 0;
}