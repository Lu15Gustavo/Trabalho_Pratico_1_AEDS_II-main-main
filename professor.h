#ifndef PROFESSOR_H
#define PROFESSOR_H

typedef struct {
    char nome[100];
    int matricula;
    char disciplina[100];
    float salario;
} Professor;

void cadastrarProfessor(FILE *arq);
void listarProfessores(FILE *arq);
void salvarProfessores(FILE *arq);
void carregarProfessores(FILE *arq);
int buscaBinariaProfessor(FILE *arq, int matricula);
void editarProfessor(FILE *arq, int matricula);
int buscaSequencialProfessor(FILE *arq, int matricula);
void bubbleSortProfessores(FILE *arq);

#endif
