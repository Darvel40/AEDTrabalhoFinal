#include "equipa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Cria uma nova lista de equipas dada uma capacidade inicial
 * Retorna um apontador para a nova lista se tudo correr bem, ou NULL se a
 * alocação de memoria falhar
 */
equipa *criaEquipa(char *nome, char **jogadores, int numJogadores) {
  equipa *novaEquipa = (equipa *)malloc(sizeof(equipa));
  if (novaEquipa != NULL) {
    novaEquipa->nome = strdup(nome);
    novaEquipa->pontos = 0;
    novaEquipa->jogadores = (char **)malloc((numJogadores + 1) * sizeof(char *));
    if (novaEquipa->jogadores != NULL) {
      for (int i = 0; i < numJogadores; i++) {
        novaEquipa->jogadores[i] = strdup(jogadores[i]);
      }
      novaEquipa->jogadores[numJogadores] = NULL; // Terminador nulo para o array de jogadores
    }
  }
  return novaEquipa;
}

listaEquipas *criaListaEquipas(int capacity) {
  listaEquipas *list = (listaEquipas *)malloc(sizeof(listaEquipas));
  if (list == NULL) {
    return NULL;
  }
  list->equipa = (equipa **)malloc(capacity * sizeof(equipa *));
  if (list->equipa != NULL) {
    list->size = 0;
    list->capacity = capacity;
  } else {
    free(list);
    return NULL;
  }
  return list;
}

    void addEquipa(listaEquipas *list, equipa *novaEquipa) {
  if (list->size < list->capacity) {
    list->equipa[list->size] = novaEquipa;
    list->size++;
  } else {
    printf("A lista de equipas está cheia. Não é possível adicionar mais equipas.\n");
  }
}

void destroiEquipa(equipa *equipa) {
  free(equipa->nome);
  for (int i = 0; equipa->jogadores[i] != NULL; i++) {
    free(equipa->jogadores[i]);
  }
  free(equipa->jogadores);
  free(equipa);
}

/**
 * Destroi a lista de equipas libertando a memoria reservada
 * bem como a memoria de cada elemento da lista
 */
void destroiListaEquipas(listaEquipas *list) {
  for (int i = 0; i < list->size; i++) {
    destroiEquipa(list->equipa[i]);
  }
  free(list->equipa);
  free(list);
}

void showEquipas(listaEquipas *list){
  printf("/***************************************************/\n");
  if(list->size == 0){
    printf("Nao existem equipas registadas!\n");
  } else{
    for (int i = 0; i < list->size; i++) {
      printf("  Equipa:%s\n", list->equipa[i]->nome, tamanhoArrayString(list->equipa[i]->jogadores));
      for (int j = 0; j < tamanhoArrayString(list->equipa[i]->jogadores); j++) {
      printf("    - Nome do Jogador:%s\n", list->equipa[i]->jogadores[j]);
      }
    }
  }
  printf("/***************************************************/\n");
}

void showClassificacoes(listaEquipas *list){
  printf("\n\n/***************************************************/\n");
  equipa *novaEquipa = (equipa *)malloc(sizeof(equipa));
  printf("Classificações:\n");
  for (int i=0; i<list->size; ++i){
    for (int j=i+1; j<list->size; ++j){
      if (list->equipa[i]->pontos < list->equipa[j]->pontos){
         novaEquipa = list->equipa[i];
         list->equipa[i] = list->equipa[j];
         list->equipa[j] = novaEquipa;
      }
    }
  }

  for (int i = 0; i < list->size; i++) {
    printf("%d - %s: %d (pontos); \n", i, list->equipa[i]->nome, list->equipa[i]->pontos);
  }
  printf("/***************************************************/\n");
}

int tamanhoArrayString(char** array) {
    int tamanho = 0;
    
    while (array[tamanho] != NULL) {
        tamanho++;
    }
    
    return tamanho;
}

