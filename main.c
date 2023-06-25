#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "equipa/equipa.h"

int main(void) {
  //Variavel Ficheiro TXT
  FILE *equipasFile;
  //Variaveis auxiliares
  int linha, coluna, equipas = 0;
  //Pedir ao utilizador para introduzir as linha do terreno
  printf("Introduza a quantidade de linhas do terreno: ");
  scanf(" %d", &linha);
  //Pedir ao utilizador para introduzir as colunas do terreno
  printf("Introduza a quantidade de colunas do terreno: ");
  scanf(" %d", &coluna);
  //Remover 1 para colocar no array
  linha--;
  coluna--;
  //Mostrar o Terreno
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
  
  //For para introduzir os tesouros nas posições
  for(int i = 0; i <= linha; i++){
    for(int j = 0; j <= coluna; j++){
      printf("Introduza o tesouro da posição [%i, %i]: ", i, j);
      int tesouro = 0;
      
      scanf(" %d", &tesouro);
      fflush(stdin);
      tesouros[i][j] = tesouro;
    }
  }

  //Mostrar o Terreno
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

  //Abrir o ficheiro e permitir ler e escrever
  equipasFile = fopen("equipas.txt", "r+");
  fseek(equipasFile, 0, 0);
  //Verificar se conseguiu abrir os ficheiros
  if(!equipasFile){
    printf("Não conseguio abrir o ficheiro");
    return 0;
  }

  //Criar lista de quipas
  listaEquipas *listaTeams = criaListaEquipas(100);
  //Guardar as linhas
  char buffer[100];
  //Guardar o nome da equipa
  char nomeEquipa[100];
  //Alocar memoria para o array de jogadores
  char **jogadores = (char **)malloc(100 * sizeof(char *));
  for (int i = 0; i < 100; i++) {
    jogadores[i] = (char *)malloc(100 * sizeof(char));
  }
  //Variavel auxiliar
  int numJogadores = 0;
  //Variavel auxiliar
  int linhaLop = 0;
  //While para ler o ficheiro txt todo
  while (fgets(buffer, sizeof(buffer), equipasFile) != NULL) {
    if (linhaLop % 3 == 0) { //Verificar se é o nome da equipa
      strncpy(nomeEquipa, buffer, sizeof(nomeEquipa));
      nomeEquipa[strcspn(nomeEquipa, "\n")] = '\0';
    } else if (linhaLop % 3 == 1) {  //Caso seja nome do primero jogador
      jogadores[numJogadores] = strdup(buffer);
      jogadores[numJogadores][strcspn(jogadores[numJogadores], "\n")] = '\0';
      numJogadores++;
    } else if (linhaLop % 3 == 2) {  //Caso seja nome do segundo jogador
      jogadores[numJogadores] = strdup(buffer);
      jogadores[numJogadores][strcspn(jogadores[numJogadores], "\n")] = '\0';

      //Cria a equipa
      equipa *novaEquipa = criaEquipa(nomeEquipa, jogadores, numJogadores + 1);
      addEquipa(listaTeams, novaEquipa);
  
      numJogadores = 0;
    }

    linhaLop++;
  }

  //Pedir ao utilizador quantas equipas vao participar
  printf("\n");
  do{
    printf("Quantas equipas vão jogar? (Minimo 2ª Equipas) ");
    scanf(" %d", &equipas);
  }while(equipas < 2);

  //Ver equipas
  showEquipas(listaTeams);
  //Criar lista de equipas
  listaEquipas *listaEquipasJogar = criaListaEquipas(100);
  //For para selecionar as equipas que vão jogar
  for(int i = 1; i < equipas+1; i++){
    int equipaSelecionada = 0;
    printf("Selecione a %dª Equipa: ", i);
    scanf(" %d", &equipaSelecionada);
    //Adicionar equipa á lista
    addEquipa(listaEquipasJogar, listaTeams->equipa[equipaSelecionada-1]);
  }
  //Ver equipas
  showEquipas(listaEquipasJogar);

  //----------- Começar o jogo até acabar

  bool continua = true;
  
  do {
    //For para rodar por cada equipa
    for (int i = 0; i < listaEquipasJogar->size; i++) {
      printf("\n\n");
      printf("  Equipa: %s\n", listaEquipasJogar->equipa[i]->nome);
      
      //For para rodar por cada jogador de cada equipa
      for (int j = 0; j < tamanhoArrayString(listaEquipasJogar->equipa[i]->jogadores); j++) {
        
        // flag para indicar se NULL foi encontrado
        bool encontradoNull = false; 
        // Verificar se há algum valor NULL na matriz posicoes
        for (int l = 0; l <= linha; l++) {
          for (int c = 0; c <= coluna; c++) {
            if (posicoes[l][c] == NULL) {
              encontradoNull = true;
              break; // interrompe os loops se NULL for encontrado
            }
          }
        }
        
        //Sair do jogo se todos os tesouros fossem descobertos
        if(!encontradoNull){
          printf("\nTodos os tesouros foram descobertos!\n");
          //Ver classificações
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
        printf("    - %d° Jogador a jogar é: %s\n\n", j+1,listaEquipasJogar->equipa[i]->jogadores[j]);
        //Variavel para guardar o comando
        char *comando = malloc(sizeof(char) * 100);
        do{
          //Pedir ao utilizador 
          printf("Introduza o comando ('escavacao', 'riqueza', 'estrela', 'sair'): ");
          scanf(" %s", comando);
          fflush(stdin);
  
          //Verificar se o comando é 'riqueza'
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
          //Verificar se o comando é 'estrela'
          if(!strcmp(comando, "estrela")){
            //Ver classificações
            showClassificacoes(listaEquipasJogar);
  
            int equipaSelecionada = 0;
            printf("\nSelecione a Equipa: ");
            scanf(" %d", &equipaSelecionada);
  
            // Variavel para contar a frequência dos nomes de jogador
            int frequencia[100] = {0};
            
            for (int y = 0; y < tamanhoArrayString(listaEquipasJogar->equipa[equipaSelecionada]->jogadores); y++) {
              char* jogador = listaEquipasJogar->equipa[equipaSelecionada]->jogadores[y];
              for (int l = 0; l <= linha; l++) {
                for (int c = 0; c <= coluna; c++) {
                  if (posicoes[l][c] != NULL && strcmp(posicoes[l][c], jogador) == 0) {
                    frequencia[y]++;
                  }
                }
              }
            }
  
            int maxFrequencia = 0;
            int jogadorMaisFrequenteIndex = 0;
        
            // Encontrar o jogador mais frequente
            for (int y = 0; y < tamanhoArrayString(listaEquipasJogar->equipa[equipaSelecionada]->jogadores); y++) {
              if (frequencia[y] > maxFrequencia) {
                maxFrequencia = frequencia[y];
                jogadorMaisFrequenteIndex = y;
              }
            }
        
            char* jogadorMaisFrequente = listaEquipasJogar->equipa[equipaSelecionada]->jogadores[jogadorMaisFrequenteIndex];
        
            printf("\nO arqueólogo com o maior mérito é: %s\n", jogadorMaisFrequente);
          }
          
          //Verificar se o comando é 'sair'
          if(!strcmp(comando, "sair")){
            printf("\nAinda havia tesouros por descobrir...\n");
            //Ver classificações
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
        
        //Verificar se o comando é 'escavacao'
        if(!strcmp(comando, "escavacao")){
          //Variaveis auxiliares
          int linhaJogar = 0, colunaJogar = 0;
          do{
            //Pedir ao uilizador a linha e coluna que deseja escavar
            printf("Introduza a linha e a coluna onde deseja escavar (Ex: 1 2): ");
            scanf(" %d", &linhaJogar);
            scanf(" %d", &colunaJogar);
            fflush(stdin);
            //Retirar 1 a linha e coluna para o array
            linhaJogar -= 1;
            colunaJogar -= 1;
            printf("\n");
          }while(posicoes[linhaJogar][colunaJogar] != NULL);
          //Colocar o nome do jogador no array posições
          posicoes[linhaJogar][colunaJogar] = listaEquipasJogar->equipa[i]->jogadores[j];
          //Colocar os pontos escavados na equipa
          listaEquipasJogar->equipa[i]->pontos += tesouros[linhaJogar][colunaJogar];
          //Mostrar o terreno escavado
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

    //Não continuar o jogo
    continua = false;
    
    // Verificar se há algum valor NULL na matriz posicoes
    for (int l = 0; l <= linha; l++) {
      for (int c = 0; c <= coluna; c++) {
        if (posicoes[l][c] == NULL) {
          continua = true;
          break; // interrompe os loops se NULL não for encontrado
        }
      }
    }
    
  } while(continua);
  
  
  printf("\nTodos os tesouros foram descobertos!\n");
  //Ver classificações
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