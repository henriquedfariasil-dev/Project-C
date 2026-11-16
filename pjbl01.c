#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//criterio 1: Definir constantes para o numero de filas e colunas da plateia
#define NF 10
#define NC 12

//ORGANIZACAO: Prototipos das funcoes, como a main está no começo do código, as funcoes devem ser declaradas antes da main, para que o compilador saiba que elas existem.
void limpaEntradaTeclado();
int inputInt(char msg[]);
int inputFaixa(char msg[], int min, int max);
void mostrarPlateia(char plateia[NF][NC]);
void mostrarOcupacao(char plateia[NF][NC]);
void salvarArquivo(char nomeArq[], char plateia[NF][NC]);
void iniciarPlateia(char nomeArq[], char plateia[NF][NC]);
int plateiaLotada(char plateia[NF][NC]);
void venderIngresso(char plateia[NF][NC]);
void cancelarIngresso(char plateia[NF][NC]);



//A main contem o menu, cada opcao chama sua funçao.
int main() {
    char plateia[NF][NC];
    int opcao;

    iniciarPlateia("plateia.txt", plateia);

    do {
        printf("\n-------------------------------\n");
        printf("            MENU\n");
        printf("-------------------------------\n");
        printf("0-Sair do programa\n");
        printf("1-Mostrar plateia\n");
        printf("2-Mostrar ocupacao\n");
        printf("3-Vender ingresso\n");
        printf("4-Cancelar ingresso (devolucao)\n");
        printf("-------------------------------\n");

        opcao = inputFaixa("Escolha uma opcao: ", 0, 4);

        switch (opcao) {
            case 1:
                mostrarPlateia(plateia);
                break;
            case 2:
                mostrarOcupacao(plateia);
                break;
            case 3:
                venderIngresso(plateia);
                break;
            case 4:
                cancelarIngresso(plateia);
                break;
            case 0:
                printf("\nPrograma encerrado com sucesso...\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

//criterio 4: Funcoes para entrada de dados
void limpaEntradaTeclado() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // corpo vazio: apenas descarta o caractere lido
    }
}

int inputInt(char msg[]) {
    char str[50];
    int valido, i, digitos;

    printf("%s", msg);
    while (1) {
        // item a: le como string, no maximo tamanho 49, para evitar overflow, str 50 , 49 espaços para o '\0'
        if (scanf("%49s", str) != 1) {
            printf("\nEntrada encerrada.\n");
            exit(1);
        }
        limpaEntradaTeclado();   // descarta o restante da linha

        // item b: percorre a string conferindo os codigos ASCII
        valido = 1;
        digitos = 0;
        i = (str[0] == '-') ? 1 : 0;   // '-' so na primeira posicao
        while (str[i] != '\0') {
            if (str[i] < 48 || str[i] > 57) {
                valido = 0;
                break;
            }
            digitos++;
            i++;
        }

        // item c: vazia ou so "-" e invalida; mais de 9 digitos evita overflow no atoi
        if (digitos == 0 || digitos > 9) valido = 0;

        // item d: tudo numerico, converte
        if (valido) return atoi(str);
        printf("Nao é um inteiro! Digite um numero inteiro: ");
    }
}

//funcao que recebe valores e não permite que o usuario digite valores fora do intervalo definido
int inputFaixa(char msg[], int min, int max) {
    int n = inputInt(msg);
    while (n < min || n > max) {
        printf("Valor deve estar entre %d e %d.\n", min, max);
        n = inputInt(msg);
    }
    return n;
}

//funcao para salvar a plateia em um arquivo
void salvarArquivo(char nomeArq[], char plateia[NF][NC]) {
    FILE *arq = fopen(nomeArq, "w");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }
    for (int i = 0; i < NF; i++) {
        for (int j = 0; j < NC; j++) fprintf(arq, "%c", plateia[i][j]);
        fprintf(arq, "\n");
    }
    fclose(arq);
}

//funcao para iniciar a plateia, se o arquivo não existir, cria um novo arquivo com todas as poltronas livres
void iniciarPlateia(char nomeArq[], char plateia[NF][NC]) {
    FILE *arq = fopen(nomeArq, "r");
    if (arq == NULL) {
        for (int i = 0; i < NF; i++)
            for (int j = 0; j < NC; j++)
                plateia[i][j] = '-';
        salvarArquivo(nomeArq, plateia);
        printf("Primeiro acesso: arquivo da plateia criado.\n");
        return;
    }
    for (int i = 0; i < NF; i++) {
        for (int j = 0; j < NC; j++) fscanf(arq, "%c", &plateia[i][j]);
        fgetc(arq);
    }
    fclose(arq);
}
//funcao para mostrar a plateia, mostrando os numeros das poltronas livres e os ocupados com espaço em branco
void mostrarPlateia(char plateia[NF][NC]) {
    printf("\nPLATEIA\n----------------------------------------------------------\n");
    for (int i = 0; i < NF; i++) {
        for (int j = 0; j < NC; j++) {
            if (plateia[i][j] == '-') printf("%3d ", (i * NC) + j + 1);
            else printf("    ");
        }
        printf("\n");
    }
    printf("----------------------------------------------------------\nFIM PLATEIA\n");
}
//funcao para mostrar a ocupacao, modo de vista onde tem m, i e - para mostrar os locais.
void mostrarOcupacao(char plateia[NF][NC]) {
    int meia = 0, inteira = 0;
    printf("\nOCUPACAO\n----------------------------------\n");
    for (int i = 0; i < NF; i++) {
        for (int j = 0; j < NC; j++) {
            printf("%c ", plateia[i][j]);
            if (plateia[i][j] == 'm') meia++;
            else if (plateia[i][j] == 'i') inteira++;
        }
        printf("\n");
    }
    printf("----------------------------------\n");
    printf("Poltronas meias vendidas....: %d.\n", meia);
    printf("Poltronas inteiras vendidas.: %d.\n", inteira);
    printf("----------------------------------\nFIM OCUPACAO\n");
}

// Retorna 1 se nao ha nenhuma poltrona livre, 0 caso contrario
int plateiaLotada(char plateia[NF][NC]) {
    for (int i = 0; i < NF; i++)
        for (int j = 0; j < NC; j++)
            if (plateia[i][j] == '-') return 0;
    return 1;
}
//funcao de venda de ingresso, onde o usuario escolhe a poltrona e o tipo de ingresso, e salva no arquivo.
void venderIngresso(char plateia[NF][NC]) {
    int poltrona, i, j, outro;
    char tipo;

    if (plateiaLotada(plateia)) {
        printf("\nPlateia lotada!\n");
        return;
    }

    mostrarPlateia(plateia);
    printf("VENDA DE INGRESSOS\n--------------------------------------------------\n");

    do {
        poltrona = inputFaixa("Digite o numero da poltrona <1..120> (zero encerra): ", 0, NF * NC);
        // enquanto a poltrona estiver ocupada, pede outra
        while (poltrona != 0 && plateia[(poltrona - 1) / NC][(poltrona - 1) % NC] != '-')
            poltrona = inputFaixa("Poltrona ocupada. Digite outro numero de poltrona <1..120>: ", 0, NF * NC);
        if (poltrona == 0) break;

        i = (poltrona - 1) / NC;
        j = (poltrona - 1) % NC;

        // leitura e validacao do tipo (caractere)
        printf("Digite se a entrada é (m)eia ou (i)nteira: ");
        while (1) {
            if (scanf("%c", &tipo) != 1) {
                printf("\nEntrada encerrada.\n");
                exit(1);
            }
            if (tipo != '\n') limpaEntradaTeclado();   // evita esperar uma segunda linha se so digitou Enter
            tipo = toupper(tipo);
            if (tipo == 'M' || tipo == 'I') break;
            printf("Tipo invalido! Digite se a entrada é (m)eia ou (i)nteira: ");
        }

        plateia[i][j] = (tipo == 'M') ? 'm' : 'i';
        salvarArquivo("plateia.txt", plateia);

        if (tipo == 'M') printf("Poltrona vendida: %d (meia).\n", poltrona);
        else printf("Poltrona vendida: %d (inteira).\n", poltrona);
        printf("(Fileira %d, coluna %d).\n", i + 1, j + 1);

        if (plateiaLotada(plateia)) {
            printf("Plateia lotada!\n");
            break;
        }

        outro = inputFaixa("Vender outro ingresso (0:nao; 1:sim)?\n", 0, 1);
    } while (outro == 1);

    printf("--------------------------------------------------\nFIM VENDA DE INGRESSOS\n");
}

//funcao de cancelamento de ingresso, onde o usuario escolhe a poltrona e cancela o ingresso, e salva no arquivo.
void cancelarIngresso(char plateia[NF][NC]) {
    int poltrona, i, j, outro;

    printf("\nCANCELAMENTO DE INGRESSOS\n--------------------------------------------------\n");

    do {
        poltrona = inputFaixa("Digite a poltrona a cancelar (zero encerra): ", 0, NF * NC);
        // enquanto a poltrona estiver livre, pede outra
        while (poltrona != 0 && plateia[(poltrona - 1) / NC][(poltrona - 1) % NC] == '-')
            poltrona = inputFaixa("Poltrona vazia. Digite outra poltrona: ", 0, NF * NC);
        if (poltrona == 0) break;

        i = (poltrona - 1) / NC;
        j = (poltrona - 1) % NC;

        if (plateia[i][j] == 'm') printf("Poltrona com venda cancelada: %d (meia).\n", poltrona);
        else printf("Poltrona com venda cancelada: %d (inteira).\n", poltrona);

        plateia[i][j] = '-';
        salvarArquivo("plateia.txt", plateia);

        outro = inputFaixa("Cancelar outro ingresso (0:nao; 1:sim)?\n", 0, 1);
    } while (outro == 1);

    printf("--------------------------------------------------\nFIM CANCELAMENTO DE INGRESSOS\n");
}