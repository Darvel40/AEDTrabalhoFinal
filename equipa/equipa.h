#ifndef H_EQUIPA
#define H_EQUIPA

//Struct Equipa
typedef struct _equipa{
  char* nome;
  int pontos;
  char** jogadores;
} equipa;

typedef struct _listaEquipas {
  equipa **equipa;
  int size;
  int capacity;
} listaEquipas;

listaEquipas *criaListaEquipas(int capacity);
equipa *criaEquipa(char *nome, char **jogadores, int numJogadores);
void addEquipa(listaEquipas *list, equipa *equipa);
void destroiEquipa(equipa *equipa);
void destroiListaEquipas(listaEquipas *list);
void showEquipas(listaEquipas *list);
void showClassificacoes(listaEquipas *list);
int tamanhoArrayString(char** array);
#endif