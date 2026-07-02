#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Menu ();
void ajudaJogador ();
void novoJogo ();

//perfil do jogador
typedef struct {
	char nome[16];
	char nivel;
} Jogador;


int main () {
	srand(time(NULL)); // semente de aleatoriedade definida
	Menu();

		
}




//explicação para o jogador de como o jogo funciona
void ajudaJogador () {
    printf("\n===== COMO JOGAR =====\n\n");

    printf("O programa gera uma sequencia secreta de cores.\n");
    printf("Descubra ela antes que as tentativas acabem.\n\n");

    printf("CORES: 1-Vermelho 2-Azul 3-Verde 4-Amarelo 5-Roxo 6-Laranja\n\n");

    printf("NIVEIS: 1-Facil(4c/10t)  2-Medio(5c/12t)  3-Dificil(6c/15t)\n\n");

    printf("Digite os codigos separados por espaco. Ex: 1 2 3 4\n\n");

    printf("RESULTADO:\n");
    printf("C = cor e posicao certas\n");
    printf("E = cor certa, posicao errada\n");
    printf("- = cor nao esta na sequencia\n\n");

    printf("Ex: Segredo = Vermelho Azul Verde Amarelo\n");
	printf("Tentativa = Vermelho Verde Azul Roxo -> C E E -\n\n");
    printf("Acerte tudo na posicao certa pra vencer.\n");
    
    printf("\n\n");
	printf("Selecione uma nova opção para o menu: \n");
	Menu(); 
}

void Menu () {
	char opcaoInicial; 
	Jogador player;

	// Introdução do jogo para o jogador e escolhas disponíveis
	
	printf("JOGO CODIGO SECRETO DE CORES\n\n");
	printf("Opcoes de jogo:\n\n");
	printf("N - Novo jogo\nE - Encerrar jogo\nC - Carregar jogo\nS - Salvar jogo\nA - Ajuda\n\n");
	printf("Digite a opcao: ");
	scanf(" %c", &opcaoInicial);

	switch (opcaoInicial) {
		case 'A':
			ajudaJogador();
			break;
		case 'N':
			printf("Digite o seu nome: ");
			scanf("%s", player.nome);
			printf("Niveis de dificuldade: \n");
			printf("F - 4 cores / 10 tentativas\n");
			printf("M - 5 cores / 12 tentativas\n");
			printf("D - 6 cores / 15 tentativas\n");
			printf("Qual nivel de dificuldade deseja? ");
			scanf("%c", &player.nivel);
			
			break;
		case 'E':
			printf("Encerrando o jogo...\n");
			exit(0);
		default:
			printf("Digite uma opcao valida.");
			break;
	}
}
