/*
===================================================================================
                PROJETO FINAL — REGISTRO DE NOTAS (C PURO)
-----------------------------------------------------------------------------------
VERSÃO DIDÁTICA 2025 — SEGUINDO TODAS AS EXIGÊNCIAS DA ATIVIDADE
===================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -------------------------------------------------------------------------------
   ESTRUTURA DO REGISTRO
   nome (char[])
   matricula (int)
   nota (float)
   status (' ' ativo / '*' excluído)
-------------------------------------------------------------------------------- */
typedef struct {
    char nome[50];
    int matricula;
    float nota;
    char status;  // ' ' ativo / '*' excluído
} reg;

/* Protótipos */
void limpaBuffer(void);
void lerString(char *s, int tam);
int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);

/* =========================================================================== */
void limpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* =========================================================================== */
void lerString(char *s, int tam) {
    fgets(s, tam, stdin);
    s[strcspn(s, "\n")] = '\0';
}

/* =========================================================================== */
int tamanho(FILE *arq) {
    long pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    long fim = ftell(arq);
    fseek(arq, pos, SEEK_SET);

    return (int)(fim / sizeof(reg));
}

/* =========================================================================== */
void cadastrar(FILE *arq) {
    reg aluno;
    char conf;
    aluno.status = ' ';

    printf("\n=== CADASTRAR ALUNO ===\n");
    printf("Registro número: %d\n", tamanho(arq) + 1);

    printf("Nome: ");
    lerString(aluno.nome, sizeof(aluno.nome));

    printf("Matrícula: ");
    scanf("%d", &aluno.matricula);
    limpaBuffer();

    printf("Nota: ");
    scanf("%f", &aluno.nota);
    limpaBuffer();

    printf("Confirmar cadastro (s/n)? ");
    scanf("%c", &conf);
    limpaBuffer();

    if (toupper(conf) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&aluno, sizeof(reg), 1, arq);
        fflush(arq);
        printf("Registro salvo com sucesso!\n");
    } else {
        printf("Cadastro cancelado.\n");
    }
}

/* =========================================================================== */
void consultar(FILE *arq) {
    int cod;
    reg aluno;

    printf("\nInforme o código do aluno: ");
    if (scanf("%d", &cod) != 1) {
        printf("Entrada inválida!\n");
        limpaBuffer();
        return;
    }
    limpaBuffer();

    int total = tamanho(arq);
    if (cod <= 0 || cod > total) {
        printf("Código inválido! Total de registros: %d\n", total);
        return;
    }

    long pos = (long)(cod - 1) * sizeof(reg);
    fseek(arq, pos, SEEK_SET);
    fread(&aluno, sizeof(reg), 1, arq);

    printf("\n=== ALUNO %d ===\n", cod);

    if (aluno.status == '*') {
        printf("Status: EXCLUÍDO LOGICAMENTE\n");
    }

    printf("Nome......: %s\n", aluno.nome);
    printf("Matrícula.: %d\n", aluno.matricula);
    printf("Nota......: %.2f\n", aluno.nota);
}

/* =========================================================================== */
int main(void) {

    FILE *arq = fopen("notas.dat", "r+b");
    if (!arq) {
        arq = fopen("notas.dat", "w+b");
        if (!arq) {
            printf("Erro crítico ao abrir/criar arquivo.\n");
            return 1;
        }
    }

    int op;

    do {
        printf("\n========== REGISTRO DE NOTAS ==========\n");
        printf("1. Cadastrar aluno\n");
        printf("2. Consultar aluno\n");
        printf("3. Sair\n");
        printf("---------------------------------------\n");
        printf("Total de registros: %d\n", tamanho(arq));
        printf("Opção: ");

        if (scanf("%d", &op) != 1) {
            printf("Digite um número válido.\n");
            limpaBuffer();
            continue;
        }
        limpaBuffer();

        switch (op) {
            case 1: cadastrar(arq); break;
            case 2: consultar(arq); break;
            case 3: printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (op != 3);

    fclose(arq);
    return 0;
}