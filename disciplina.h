#ifndef DISCIPLINA_H
#define DISCIPLINA_H

typedef struct {
    int codigo;
    char nome[100];
    char professor[100];
} Disciplina;

void cadastrarDisciplina(FILE *arq);
void listarDisciplinas(FILE *arq);
void salvarDisciplinas(FILE *arq);
void carregarDisciplinas(FILE *arq);
FILE *abrirArquivoDisciplina(char *nomeArquivo, char *modo);
void trocarProfessorDisciplina(int codigoDisciplina, const char *novoProfessor, FILE *arq);
void bubbleSortDisciplina(FILE *arq);

#endif
