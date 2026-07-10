#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//perfil do jogador
typedef struct {
	char nome[16];
	int nivel;
} Jogador;
	
void Menu ();
void ajudaJogador ();
void novoJogo (Jogador player);
void conferirTentativas (int **jogoSecreto, int *Segredo, int nCores, int nTentativas);
void embaralhar(char resultado[6], int nCores);

int main () {
	srand(time(NULL)); // semente de aleatoriedade definida
	printf("\n\nJOGO CODIGO SECRETO DE CORES\n\n");
	Menu();

	return 0;	
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
}

void Menu () {
	char opcaoInicial; 
	Jogador player;

	// Introdução do jogo para o jogador e escolhas disponíveis
	// levando para o seguimento baseado no que o jogador escolher

	do {
		printf("Opcoes de jogo:\n\n");
		printf("N - Novo jogo\nX - Encerrar jogo\nC - Carregar jogo\nS - Salvar jogo\nA - Ajuda\n\n");
		printf("Digite a opcao: ");
		scanf(" %c", &opcaoInicial);
		while (getchar() != '\n');

		switch (opcaoInicial) {
			case 'A':
				ajudaJogador();
				printf("\nSelecione uma nova opcao: \n");
				break;
			case 'N':
				printf("Digite o seu nome: ");
				fgets (player.nome, sizeof(player.nome), stdin);
				printf("\n\nNiveis de dificuldade: \n");
				printf("1 - Facil - 4 cores / 10 tentativas\n");
				printf("2 - Medio - 5 cores / 12 tentativas\n");
				printf("3 - Dificil - 6 cores / 15 tentativas\n");
				printf("\nQual nivel de dificuldade deseja? (1, 2 ou 3) ");
				scanf("%d", &player.nivel);
				novoJogo(player);
				break;
			case 'X':
				printf("Encerrando o jogo...\n");
			default:
				printf("Digite uma opcao valida.\n");
				break;
		}
	} while (opcaoInicial != 'X'); // fechar programa caso seja X a opção escolhida
}

void novoJogo (Jogador player) {
	// Função para iniciar um novo jogo

	int nCores;
	int nTentativas;

	switch (player.nivel) {
		case 1:
			nCores = 4;
			nTentativas = 10;
			break;
		case 2:
			nCores = 5;
			nTentativas = 12;
			break;
		case 3:
			nCores = 6;
			nTentativas = 15;
			break;
		default:
			printf("Nivel de dificuldade invalido.\n"); //evitar comandos alternativos
			return; 
	}
	int *Segredo = malloc(nCores * sizeof(int));	// alocando dinamicamente o vetor
	for (int i=0; i < nCores; i++) {				// de cores baseado na dificuldade
		Segredo[i] = rand() % 6 + 1; 
	}

	printf("\nSequencia de cores gerada, boa sorte!\n\n");
	for(int i=0; i < nCores; i++) {
		printf("%d ", Segredo[i]); //imprime a sequencia de cores gerada RETIRAR DEPOIS DO TESTE
	}

	int **jogoSecreto = malloc(nTentativas * sizeof(int*)); 
	for (int i=0; i < nTentativas; i++) {
		jogoSecreto[i] = malloc(nCores * sizeof(int));
	}

	conferirTentativas(jogoSecreto, Segredo, nCores, nTentativas);

}

void conferirTentativas (int **jogoSecreto, int *Segredo, int nCores, int nTentativas) { 
	
	int tentativaAtual = 0;
	int Ganhou = 0;
	int sair = 0;

	do { 
		Ganhou = 0;
		char resultado[6]; // guarda os simbolos antes de imprimir para aleatorizar depois

		printf("Digite a tentativa %d: ", tentativaAtual + 1);
		for (int j=0; j < nCores; j++) {
			scanf("%d", &jogoSecreto[tentativaAtual][j]);
			if (jogoSecreto[tentativaAtual][j] == 0) {
				printf("Deseja sair do jogo? (S/N) ");
				char opcao;
				scanf(" %c", &opcao);
				if (opcao == 'S') {
					printf("Voltando ao menu...\n\n");
					sair = 1;
					break; // sai do laço
				} else {
					j--;
				}
			} else if (jogoSecreto[tentativaAtual][j] < 1 || jogoSecreto[tentativaAtual][j] > 6) {
				printf("Codigo de cor invalido. Digite novamente: ");
				j--;
			} 
		}

			
		
		if (sair) 
			return; // sai da funcao inteira sem chamar a funcao do menu

		printf("Resultado da tentativa %d: ", tentativaAtual + 1);
		for (int i=0; i < nCores; i++) {
			if (jogoSecreto[tentativaAtual][i] == Segredo[i]) {
				resultado[i] = 'C';
				Ganhou += 1;
			} 
			else 
			{
				int repetido = 0;
				for (int k=0; k < nCores; k++) {
					if (jogoSecreto[tentativaAtual][i] == Segredo[k]) {
						repetido = 1;
					}
				}
				if (repetido == 1) {
					resultado[i] = 'E';
				} else {
					resultado[i] = '-';
				}
			}
		}
		printf("\n");

		embaralhar(resultado, nCores); // randomiza a ordem dos simbolos das dicas

		for (int i =0; i < nCores; i++) {
			printf("%c ", resultado[i]);
		}
		printf("\n\n");
		tentativaAtual++;	
	} while (tentativaAtual < nTentativas && Ganhou != nCores);

	if (Ganhou == nCores) {
		printf("Parabens, voce acertou a sequencia secreta!\n\n");
	} else {
		printf("Que azar! Suas tentativas acabaram! A sequencia secreta era: ");
		for (int i=0; i < nCores; i++) {
			printf("%d ", Segredo[i]);
		}
		printf("\n\n");
	}
}

	void embaralhar(char *resultado, int nCores) {

		char substituicao;
		int P1, P2;
		for (int i = 0; i < nCores * 5; i++) {
			P1 = rand() % nCores;
			P2 = rand() % nCores;
			substituicao = resultado[P1];
			resultado[P1] = resultado[P2];
			resultado[P2] = substituicao;
	}
}







