#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipa/equipa.h"

int main(void) {
  FILE *equipasFile;
  int linha, coluna, equipas = 0;
  printf("Introduza a quantidade de linhas do terreno: ");
  scanf(" %d", &linha);
  printf("Introduza a quantidade de colunas do terreno: ");
  scanf(" %d", &coluna);
  linha--;
  coluna--;
  printf("\n\nTerreno:\n");
  for(int i = 0; i <= linha; i++){
    printf("|");
    for(int j = 0; j <= coluna; j++){
      
      printf("   -");
    }
    printf("   |\n");
  }
  printf("\n\n");
  
  /*-----------------------------------------------*/

  //Alocar memoria para os tesouros e posições
  int** tesouros = (int**)malloc(linha * sizeof(int*));
  char*** posicoes = (char***)malloc(linha * sizeof(char**));
  for (int i = 0; i <= linha; i++) {
    tesouros[i] = (int*)malloc(coluna * sizeof(int));
    posicoes[i] = (char**)malloc(coluna * sizeof(char*));
  }

  for(int i = 0; i <= linha; i++){
    for(int j = 0; j <= coluna; j++){
      printf("Introduza o tesouro da posição [%i, %i]: ", i, j);
      int tesouro = 0;
      
      scanf(" %d", &tesouro);
      fflush(stdin);
      tesouros[i][j] = tesouro;
    }
  }

  printf("\nTerreno:\n");
  for(int l = 0; l <= linha; l++){
    printf("|");
    for(int c = 0; c <= coluna; c++){
      printf("   %d", tesouros[l][c]);
    }
    printf("   |\n");
  }
  printf("\n");
  
  /*-----------------------------------------------*/

  equipasFile = fopen("equipas.txt", "r+");
  fseek(equipasFile, 0, 0);
  //Verificar se conseguiu abrir os ficheiros
  if(!equipasFile){
    printf("Não conseguio abrir o ficheiro");
    return 0;
  }

  listaEquipas *listaTeams = criaListaEquipas(100);
  char buffer[100];
  char nomeEquipa[100];
  char **jogadores = (char **)malloc(100 * sizeof(char *));
  for (int i = 0; i < 100; i++) {
    jogadores[i] = (char *)malloc(100 * sizeof(char));
  }
  int numJogadores = 0;
  int linhaLop = 0;
  while (fgets(buffer, sizeof(buffer), equipasFile) != NULL) {
    if (linhaLop % 3 == 0) {
      strncpy(nomeEquipa, buffer, sizeof(nomeEquipa));
      nomeEquipa[strcspn(nomeEquipa, "\n")] = '\0';
    } else if (linhaLop % 3 == 1) {
      jogadores[numJogadores] = strdup(buffer);
      jogadores[numJogadores][strcspn(jogadores[numJogadores], "\n")] = '\0';
      numJogadores++;
    } else if (linhaLop % 3 == 2) {
      jogadores[numJogadores] = strdup(buffer);
      jogadores[numJogadores][strcspn(jogadores[numJogadores], "\n")] = '\0';
  
      equipa *novaEquipa = criaEquipa(nomeEquipa, jogadores, numJogadores + 1);
      addEquipa(listaTeams, novaEquipa);
  
      numJogadores = 0;
    }

    linhaLop++;
  }

  printf("\n");
  do{
    printf("Quantas equipas vão jogar? (Minimo 2ª Equipas) ");
    scanf(" %d", &equipas);
  }while(equipas < 2);

  showEquipas(listaTeams);
  listaEquipas *listaEquipasJogar = criaListaEquipas(100);
  for(int i = 1; i < equipas+1; i++){
    int equipaSelecionada = 0;
    printf("Selecione a %dª Equipa: ", i);
    scanf(" %d", &equipaSelecionada);
    addEquipa(listaEquipasJogar, listaTeams->equipa[equipaSelecionada-1]);
  }
  showEquipas(listaEquipasJogar);

  for (int i = 0; i < listaEquipasJogar->size; i++) {
    printf("\n\n");
    printf("  Equipa: %s\n", listaEquipasJogar->equipa[i]->nome);
    for (int j = 0; j < tamanhoArrayString(listaEquipasJogar->equipa[i]->jogadores); j++) {
      printf("    - %d° Jogador a jogar é: %s\n\n", j+1,listaEquipasJogar->equipa[i]->jogadores[j]);
      char *comando = malloc(sizeof(char) * 100);
      do{
        printf("Introduza o comando ('escavacao', 'riqueza', 'estrela', 'sair'): ");
        scanf(" %s", comando);
        fflush(stdin);

        if(!strcmp(comando, "riqueza")){
        int riqueza = 0;
        for(int l = 0; l <= linha; l++){
          for(int c = 0; c <= coluna; c++){
            if(posicoes[l][c] == NULL){
              riqueza += tesouros[l][c];
            }
          }
        }
        printf("\nRiqueza enterrada: %d\n", riqueza);
        }
        if(!strcmp(comando, "estrela")){
          
        }
        if(!strcmp(comando, "sair")){
          printf("\nAinda havia tesouros por descobrir...\n");
          showClassificacoes(listaEquipasJogar);
          
          //Sair do ficheiro
          fclose(equipasFile);
        
          //Libertar memoria
          for (int i = 0; i <= linha; i++) {
            free(tesouros[i]);
            free(posicoes[i]);
          }
          free(tesouros);
          free(posicoes);
          return 0;
        }
      } while(strcmp(comando, "escavacao"));
      if(!strcmp(comando, "escavacao")){
        int linhaJogar = 0, colunaJogar = 0;
        printf("Introduza a linha e a coluna onde deseja escavar (Ex: 1 2): ");
        scanf(" %d", &linhaJogar);
        scanf(" %d", &colunaJogar);
        fflush(stdin);
        linhaJogar -= 1;
        colunaJogar -= 1;
        printf("\n");
        posicoes[linhaJogar][colunaJogar] = listaEquipasJogar->equipa[i]->jogadores[j];
        listaEquipasJogar->equipa[i]->pontos += tesouros[linhaJogar][colunaJogar];
        printf("\nTerreno Escavado:\n");
        for(int l = 0; l <= linha; l++){
          printf("|");
          for(int c = 0; c <= coluna; c++){
            printf("   %s", posicoes[l][c] ? "*" : "-");
          }
          printf("   |\n");
        }
        printf("\n");
      }
    }
  }
  
  printf("\nTodos os tesouros foram descobertos!\n");
  showClassificacoes(listaEquipasJogar);
        
  //Sair do ficheiro
  fclose(equipasFile);

  //Libertar memoria
  for (int i = 0; i <= linha; i++) {
    free(tesouros[i]);
    free(posicoes[i]);
  }
  free(tesouros);
  free(posicoes);
  return 0;
}