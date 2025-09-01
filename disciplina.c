#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disciplina.h"

Disciplina disciplinas[1000000];
int qtdDisciplinas = 0;

void cadastrarDisciplina(FILE *arq) {

    Disciplina disciplina;
    printf("\nCadastrar Disciplina\n");
    printf("Codigo: ");
    scanf("%d", &disciplina.codigo);
    printf("Nome: ");
    scanf("%s", disciplina.nome);
    printf("Professor Responsável: ");
    scanf("%s", disciplina.professor);

    // Escreve a nova disciplina no arquivo
    fwrite(&disciplina, sizeof(Disciplina), 1, arq);

    printf("Disciplina cadastrada com sucesso!\n");
}



void listarDisciplinas(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de disciplinas.\n");
        return;
    }

    Disciplina disciplina;
    int count = 0;
    printf("\n+==========================================================================+\n");
    printf("|                         LISTA DE DISCIPLINAS                            |\n");
    printf("+==========================================================================+\n");
    printf("| %-4s | %-25s | %-25s |\n", "Cod", "Nome", "Professor");
    printf("+------+---------------------------+---------------------------+\n");
    while (fread(&disciplina, sizeof(Disciplina), 1, arq)) {
        printf("| %-4d | %-25s | %-25s |\n", disciplina.codigo, disciplina.nome, disciplina.professor);
        count++;
    }
    printf("+==========================================================================+\n");
    printf("Total de disciplinas listadas: %d\n", count);
}



void salvarDisciplinas(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de disciplinas.\n");
        return;
    }

    // Grava todas as disciplinas no arquivo de forma binária
    fwrite(&disciplinas, sizeof(Disciplina), qtdDisciplinas, arq);

    printf("Dados das disciplinas salvos com sucesso!\n");
}


FILE *abrirArquivoDisciplina(char *nomeArquivo, char *modo) {
    FILE *arq = fopen(nomeArquivo, modo);
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    return arq;
}

// Função para trocar o professor responsável por uma disciplina
void trocarProfessorDisciplina(int codigoDisciplina, const char *novoProfessor, FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de disciplinas.\n");
        return;
    }

    Disciplina disciplina;
    long pos = 0;

    // Percorre todas as disciplinas no arquivo
    while (fread(&disciplina, sizeof(Disciplina), 1, arq)) {
        if (disciplina.codigo == codigoDisciplina) {
            // Atualiza no array em memória
            for (int i = 0; i < qtdDisciplinas; i++) {
                if (disciplinas[i].codigo == codigoDisciplina) {
                    strncpy(disciplinas[i].professor, novoProfessor, sizeof(disciplinas[i].professor) - 1);
                    disciplinas[i].professor[sizeof(disciplinas[i].professor) - 1] = '\0';  // Garantir que a string termine corretamente
                    break;
                }
            }

            // Volta o ponteiro para a posição da disciplina e atualiza no arquivo
            fseek(arq, pos * sizeof(Disciplina), SEEK_SET);
            snprintf(disciplina.professor, sizeof(disciplina.professor), "%s", novoProfessor);
            fwrite(&disciplina, sizeof(Disciplina), 1, arq);

            printf("Professor responsável pela disciplina %d alterado com sucesso para %s.\n", codigoDisciplina, novoProfessor);
            return;
        }
        pos++;
    }

    printf("Disciplina com código %d não encontrada.\n", codigoDisciplina);
}


void carregarDisciplinas(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de disciplinas.\n");
        return;
    }

    // Calcula o número total de disciplinas no arquivo
    fseek(arq, 0, SEEK_END);  // Move o ponteiro para o final do arquivo
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    qtdDisciplinas = tamanhoArquivo / sizeof(Disciplina);  // Calcula o número de disciplinas no arquivo
    fseek(arq, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo

    // Lê todas as disciplinas do arquivo
    fread(&disciplinas, sizeof(Disciplina), qtdDisciplinas, arq);

    printf("Dados das disciplinas carregados com sucesso!\n");
}


void bubbleSortDisciplina(FILE *arq) {
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de disciplinas.\n");
        return;
    }

    // Calcula o número total de disciplinas no arquivo
    fseek(arq, 0, SEEK_END);  // Move o ponteiro para o final do arquivo
    long tamanhoArquivo = ftell(arq);  // Obtém o tamanho do arquivo
    int qtdDisciplinas = tamanhoArquivo / sizeof(Disciplina);  // Calcula o número de disciplinas no arquivo
    fseek(arq, 0, SEEK_SET);  // Volta o ponteiro para o início do arquivo

    Disciplina *disciplinas = (Disciplina *)malloc(qtdDisciplinas * sizeof(Disciplina));  // Aloca memória para armazenar as disciplinas

    if (disciplinas == NULL) {
        printf("Erro ao alocar memória para as disciplinas.\n");
        return;
    }

    // Lê as disciplinas do arquivo
    for (int i = 0; i < qtdDisciplinas; i++) {
        fread(&disciplinas[i], sizeof(Disciplina), 1, arq);
    }

    // Ordenação Bubble Sort das disciplinas pelo código
    for (int i = 0; i < qtdDisciplinas - 1; i++) {
        for (int j = 0; j < qtdDisciplinas - 1 - i; j++) {
            if (disciplinas[j].codigo > disciplinas[j + 1].codigo) {
                // Troca as disciplinas de posição no array
                Disciplina temp = disciplinas[j];
                disciplinas[j] = disciplinas[j + 1];
                disciplinas[j + 1] = temp;
            }
        }
    }

    // Volta o ponteiro para o início do arquivo e grava as disciplinas ordenadas de volta no arquivo
    fseek(arq, 0, SEEK_SET);
    for (int i = 0; i < qtdDisciplinas; i++) {
        fwrite(&disciplinas[i], sizeof(Disciplina), 1, arq);
    }

    free(disciplinas);  // Libera a memória alocada para as disciplinas
    printf("\nBase de dados de disciplinas ordenada com sucesso!\n");
}


