#include <stdio.h>

int main(void) {
  int linha, coluna, equipas;
  printf("Introduza a quantidade de linhas do terreno: ");
  scanf("%i", &linha);
  printf("Introduza a quantidade de colunas do terreno: ");
  scanf("%i", &coluna);
  
  int tesouros[linha][coluna];
  int posicoes[linha][coluna];

  printf("Quantas equipas vão jogar?");
  scanf("%i", &equipas);
  return 0;
}