#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "professor.h"
#include <time.h> // Necessário para a função time
#include "aluno.h"

Professor professores[1000000];
int qtdProfessores = 0;

void cadastrarProfessor(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    Professor professor;
    printf("\nCadastrar Professor\n");
    printf("Nome: ");
    scanf("%s", professor.nome);
    printf("Matricula: ");
    scanf("%d", &professor.matricula);
    printf("Disciplina: ");
    scanf("%s", professor.disciplina);
    printf("Salario: ");
    scanf("%f", &professor.salario);

    // Escreve o novo professor no arquivo
    fwrite(&professor, sizeof(Professor), 1, arq);

    printf("Professor cadastrado com sucesso!\n");
}



void listarProfessores(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    Professor professor;
    int count = 0;
    printf("\n+==================================================================================+\n");
    printf("|                              LISTA DE PROFESSORES                              |\n");
    printf("+==================================================================================+\n");
    printf("| %-4s | %-25s | %-10s | %-20s | %-10s |\n", "#", "Nome", "Matricula", "Disciplina", "Salario");
    printf("+-----+---------------------------+------------+----------------------+------------+\n");
    while (fread(&professor, sizeof(Professor), 1, arq)) {
        printf("| %-4d | %-25s | %-10d | %-20s | R$ %-8.2f |\n", ++count, professor.nome, professor.matricula, professor.disciplina, professor.salario);
    }
    printf("+==================================================================================+\n");
    printf("Total de professores listados: %d\n", count);
}



void salvarProfessores(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    // Grava todos os professores no arquivo de forma binária
    fwrite(&professores, sizeof(Professor), qtdProfessores, arq);

    printf("Dados dos professores salvos com sucesso!\n");
}


void carregarProfessores(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    // Calcula o número total de professores no arquivo
    fseek(arq, 0, SEEK_END);  // Move o ponteiro do arquivo para o final
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    qtdProfessores = tamanhoArquivo / sizeof(Professor);  // Calcula o número de professores no arquivo
    fseek(arq, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo

    // Lê todos os professores do arquivo
    fread(&professores, sizeof(Professor), qtdProfessores, arq);

    printf("Dados dos professores carregados com sucesso!\n");
}

int buscaSequencialProfessor(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return -1;
    }

    clock_t comeco, final;
    double tempoGasto;
    int comparacoes = 0;
    comeco = clock();

    Professor professor;
    int i = 0;

    // Lê os professores do arquivo e realiza a busca sequencial
    while (fread(&professor, sizeof(Professor), 1, arq)) {
        comparacoes++;  // Incrementa a contagem de comparações
        if (professor.matricula == matricula) {
            final = clock();
            tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
            char logMsg[256];
            snprintf(logMsg, sizeof(logMsg), "Busca Sequencial Professor: %d comparações, tempo: %.2f ms", comparacoes, tempoGasto * 1000);
            registrarLog(logMsg);  // Registra no arquivo log
            return i;  // Retorna o índice do professor encontrado
        }
        i++;  // Incrementa o índice do professor
    }

    final = clock();
    tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Busca Sequencial Professor: %d comparações, tempo: %.2f ms (não encontrado)", comparacoes, tempoGasto * 1000);
    registrarLog(logMsg);  // Registra no arquivo log

    return -1;  // Se não encontrar o professor, retorna -1
}




int buscaBinariaProfessor(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return -1;
    }

    clock_t comeco, final;
    double tempoGasto;
    int comparacoes = 0;
    comeco = clock();

    // Calcula o número total de professores no arquivo
    fseek(arq, 0, SEEK_END);  // Move o ponteiro para o final do arquivo
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    int qtdProfessores = tamanhoArquivo / sizeof(Professor);  // Calcula o número de professores no arquivo
    fseek(arq, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo

    int inicio = 0, fim = qtdProfessores - 1;
    Professor professor;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        // Posiciona o ponteiro do arquivo na posição do "meio" para ler o professor
        fseek(arq, meio * sizeof(Professor), SEEK_SET);
        fread(&professor, sizeof(Professor), 1, arq);  // Lê o professor do arquivo

        comparacoes++;  // Incrementa a contagem de comparações

        if (professor.matricula == matricula) {
            final = clock();
            tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
            char logMsg[256];
            snprintf(logMsg, sizeof(logMsg), "Busca Binária Professor: %d comparações, tempo: %.2f ms", comparacoes, tempoGasto * 1000);
            registrarLog(logMsg);  // Registra no arquivo log
            return meio;  // Retorna o índice do professor encontrado
        } else if (professor.matricula < matricula) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    final = clock();
    tempoGasto = (double)(final - comeco) / CLOCKS_PER_SEC;
    char logMsg[256];
    snprintf(logMsg, sizeof(logMsg), "Busca Binária Professor: %d comparações, tempo: %.2f ms (não encontrado)", comparacoes, tempoGasto * 1000);
    registrarLog(logMsg);  // Registra no arquivo log

    return -1;  // Se não encontrar o professor, retorna -1
}

void bubbleSortProfessores(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    // Calcula o número total de professores no arquivo
    fseek(arq, 0, SEEK_END);  // Move o ponteiro para o final do arquivo
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    int qtdProfessores = tamanhoArquivo / sizeof(Professor);  // Calcula o número de professores no arquivo
    fseek(arq, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo

    if (qtdProfessores == 0) {
        printf("Arquivo de professores vazio.\n");
        return;
    }

    // Aloca memória dinamicamente para o array de professores
    Professor *professores = (Professor*)malloc(qtdProfessores * sizeof(Professor));
    if (professores == NULL) {
        printf("Erro ao alocar memoria para ordenacao de professores.\n");
        return;
    }

    // Lê os professores do arquivo
    for (int i = 0; i < qtdProfessores; i++) {
        fread(&professores[i], sizeof(Professor), 1, arq);
    }

    // Ordenação Bubble Sort dos professores pela matrícula
    for (int i = 0; i < qtdProfessores - 1; i++) {
        for (int j = 0; j < qtdProfessores - 1 - i; j++) {
            if (professores[j].matricula > professores[j + 1].matricula) {
                // Troca os professores de posição no array
                Professor temp = professores[j];
                professores[j] = professores[j + 1];
                professores[j + 1] = temp;
            }
        }
    }

    // Volta o ponteiro para o início do arquivo e grava os professores ordenados de volta no arquivo
    fseek(arq, 0, SEEK_SET);
    for (int i = 0; i < qtdProfessores; i++) {
        fwrite(&professores[i], sizeof(Professor), 1, arq);
    }

    // Libera a memória alocada
    free(professores);

    printf("\nBase de dados de professores ordenada com sucesso!\n");
}


void editarProfessor(FILE *arq, int matricula) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de professores.\n");
        return;
    }

    Professor professor;
    int encontrado = 0;
    long posicao = 0;

    // Busca o professor no arquivo
    while (fread(&professor, sizeof(Professor), 1, arq)) {
        if (professor.matricula == matricula) {
            encontrado = 1;
            break;
        }
        posicao += sizeof(Professor);  // Calcula a posição do próximo professor no arquivo
    }

    if (!encontrado) {
        printf("Professor não encontrado.\n");
        return;
    }

    // Solicita a nova informação para o professor
    printf("Novo Nome: ");
    scanf("%s", professor.nome);
    printf("Nova Disciplina: ");
    scanf("%s", professor.disciplina);
    printf("Novo Salário: ");
    scanf("%f", &professor.salario);

    // Volta o ponteiro para a posição do professor no arquivo
    fseek(arq, posicao, SEEK_SET);  // Reposiciona o ponteiro para a posição correta
    fwrite(&professor, sizeof(Professor), 1, arq);  // Grava os dados do professor atualizados no arquivo

    printf("Dados do professor atualizados com sucesso!\n");
}


