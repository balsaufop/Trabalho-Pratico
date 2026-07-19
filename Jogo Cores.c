// Trabalho Prático de Introdução a Programação
// Matheus Balsamão Guarda - 26.1.4154

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
	char nome[51];
	int nivel;
} Jogador;

typedef struct {
	char nome[51];
	int nivel;
	int tentativas;
} Ranking;

typedef struct {
	Jogador player;
	int nCores;
	int nTentativas;
	int tentativaAtual;
	int *Segredo;
	int **jogoSecreto;
	int emAndamento; 
} JogoAtual;

void Menu ();
void ajudaJogador ();
void novoJogo (Jogador player, JogoAtual *jogoAtual);
void conferirTentativas (int **jogoSecreto, int *Segredo, int nCores, int nTentativas, Jogador Player, JogoAtual *jogoAtual, int tentativaInicial);
void embaralhar(char *resultado, int nCores);
void RankingJogo (Jogador player, int *tentativaAtual);
void reescreverRanking (Ranking ranking[], int quantidade);
void exibirRanking (Ranking ranking[], int quantidade);
void salvarJogo (int Segredo[], Jogador player, int nCores, int nTentativas, int tentativaAtual, int **jogoSecreto);
void carregarJogo (char dificuldade, Jogador *player, int *tentativaAtual, JogoAtual *jogoAtual);

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
	JogoAtual jogoAtual; 
	jogoAtual.emAndamento = 0; 

	// Introdução do jogo para o jogador e escolhas disponíveis
	// levando para o seguimento baseado no que o jogador escolher

	do {
		printf("Opcoes de jogo:\n\n");
		printf("N - Novo jogo\nX - Encerrar jogo\nC - Carregar jogo\nR - Ranking\nS - Salvar jogo\nA - Ajuda\n\n");
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
				player.nome[strcspn(player.nome, "\n")] = '\0';
				printf("\n\nNiveis de dificuldade: \n");
				printf("1 - Facil - 4 cores / 10 tentativas\n");
				printf("2 - Medio - 5 cores / 12 tentativas\n");
				printf("3 - Dificil - 6 cores / 15 tentativas\n");
				printf("\nQual nivel de dificuldade deseja? (1, 2 ou 3) ");
				do {
					scanf("%d", &player.nivel);
					if (player.nivel <1 || player.nivel > 3)
						printf("\nDificuldade inválida. Digite novamente: ");
				} while (player.nivel < 1 || player.nivel > 3);
				novoJogo(player, &jogoAtual); // NOVO: passa o ponteiro da struct
				break;
			case 'X':
				if (jogoAtual.emAndamento == 1){
					printf("\n\nDeseja salvar o jogo atual antes de sair? (S/N)");
					char SalvarOuNao;
					scanf(" %c", &SalvarOuNao);
					switch (SalvarOuNao) {
						case 'S': 
						salvarJogo(jogoAtual.Segredo, jogoAtual.player, jogoAtual.nCores, jogoAtual.nTentativas, jogoAtual.tentativaAtual, jogoAtual.jogoSecreto);
						printf("Jogo atual salvo.\n");
						break;
						case 'N': 
						break;
					}
				}
				printf("Encerrando o jogo...\n");
				break;
			case 'R': {
				FILE *arquivoRanking = fopen("ranking.rnk", "rb"); 
				Ranking ranking[10];                                
				int quantidade = fread(ranking, sizeof(Ranking), 10, arquivoRanking); 
				fclose(arquivoRanking);
				
				exibirRanking(ranking, quantidade);
				break;
			}
			case 'S': 
				if (jogoAtual.emAndamento == 1) {
					salvarJogo(jogoAtual.Segredo, jogoAtual.player, jogoAtual.nCores, jogoAtual.nTentativas, jogoAtual.tentativaAtual, jogoAtual.jogoSecreto);
				} else {
					printf("\nNao ha jogo em andamento para salvar.\n\n");
				}
				break;
			case 'C':  {
				char dificuldade;
				int tentativaAtual;
				carregarJogo(dificuldade, &player, &tentativaAtual, &jogoAtual);
				break;
			}
			default:
				printf("\n\nDigite uma opcao valida.\n\n");
				break;
		}
	} while (opcaoInicial != 'X'); // fechar programa caso seja X a opção escolhida
}

void novoJogo (Jogador player, JogoAtual *jogoAtual) {
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

	conferirTentativas(jogoSecreto, Segredo, nCores, nTentativas, player, jogoAtual, 0); 
}

void conferirTentativas (int **jogoSecreto, int *Segredo, int nCores, int nTentativas, Jogador player, JogoAtual *jogoAtual, int tentativaInicial) { 
	
	int tentativaAtual = tentativaInicial;
	int Ganhou = 0;
	int sair = 0;
	jogoAtual->emAndamento = 0; 
	printf("Escolha numeros entre 1 e 6. Caso deseje sair, digite 0.\n\n");
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
				printf("\n\nCodigo de cor invalido. \nDigite novamente a tentativa %d: ", tentativaAtual + 1);
				while (getchar() != '\n');
				j--;
			} 
		}
		while (getchar() != '\n');
		
		if (sair == 1) {
			// NOVO: guarda o estado atual do jogo na struct antes de sair, para poder salvar depois
			jogoAtual->player = player;
			jogoAtual->nCores = nCores;
			jogoAtual->nTentativas = nTentativas;
			jogoAtual->tentativaAtual = tentativaAtual;
			jogoAtual->Segredo = Segredo;
			jogoAtual->jogoSecreto = jogoSecreto;
			jogoAtual->emAndamento = 1;
			return; // sai da funcao inteira sem chamar a funcao do menu
		}

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
		RankingJogo(player, &tentativaAtual);
	} else {
		printf("Que azar! Suas tentativas acabaram! A sequencia secreta era: ");
		for (int i=0; i < nCores; i++) {
			printf("%d ", Segredo[i]);
		}
		printf("\n\n");

		
	}
	free(Segredo);
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

void RankingJogo (Jogador player, int *tentativaAtual) {

    FILE *arquivo = fopen("ranking.rnk", "rb");

    Ranking ranking[11]; //colocar posição a mais para o novo jogador;
    int quantidade = 0;
    
    if (arquivo != NULL) {
        quantidade = fread(ranking, sizeof(Ranking), 10, arquivo);
        fclose (arquivo);
    }

    strcpy (ranking[quantidade].nome, player.nome);
    ranking[quantidade].nivel = player.nivel;
    ranking[quantidade].tentativas = *tentativaAtual;
    quantidade++;

    reescreverRanking(ranking,quantidade);

    if (quantidade > 10)
    quantidade--;

    arquivo = fopen("ranking.rnk", "wb");
    fwrite(ranking, sizeof(Ranking), quantidade, arquivo);
    fclose (arquivo);
}

void reescreverRanking (Ranking ranking[], int quantidade) {

	for (int i=0;i < quantidade - 1; i++) {
		for (int j=0;j < quantidade - 1; j++) {
			int trocar = 0;
			if (ranking[j].tentativas > ranking[j + 1].tentativas )
				trocar = 1;
			else if (ranking[j].tentativas == ranking[j + 1].tentativas && ranking[j].nivel < ranking[j + 1].nivel)
				trocar = 1;

			if (trocar == 1) {
				Ranking substituicao = ranking[j];
				ranking[j] = ranking[j + 1];
				ranking[j + 1] = substituicao;
			}
		}
	}

}

void exibirRanking (Ranking ranking[], int quantidade) {

	for (int i=0; i < quantidade; i++) 
		printf("Ranking: \n\nPosicao %d:\n%sDificuldade %d - %d tentativas \n\n", i+1, ranking[i].nome, ranking[i].nivel, ranking[i].tentativas);
}

void salvarJogo (int Segredo[], Jogador player, int nCores, int nTentativas, int tentativaAtual, int **jogoSecreto) {
	printf("Digite o nome do arquivo que deseja salvar seu jogo: ");
	char nomeJogo[30];
	scanf(" %s", nomeJogo);
	FILE *arquivo = fopen (strcat(nomeJogo, ".cor"), "w");

	char dificuldade;

	if (Segredo != 0) {
		switch (player.nivel) {
		case 1:
			dificuldade = 'F';
			break;
		case 2:
			dificuldade = 'M';
			break;
		case 3:
			dificuldade = 'D';
			break;
		default: 
			break;
	}
		fprintf(arquivo, "%s\n%c\n",player.nome,dificuldade);
		for (int i=0; i < nCores; i++) 
			fprintf(arquivo,"%d ", Segredo[i]);
		fprintf(arquivo,"\n%d\n", tentativaAtual);
		if (tentativaAtual > 0 ) {
			for (int i=0; i < tentativaAtual; i++) {
				fprintf(arquivo, "\n");
				for (int j=0; j < nCores; j++)
					fprintf(arquivo,"%d ", jogoSecreto[i][j]);
			}
		}
	}
	fclose(arquivo);
}

void carregarJogo (char dificuldade, Jogador *player, int *tentativaAtual, JogoAtual *jogoAtual) {
	printf("Digite o nome do jogo que deseja abrir: ");
	char abrirJogo[30];
	scanf(" %s", abrirJogo);


	FILE *arquivo = fopen(strcat(abrirJogo, ".cor"), "r");
	if (arquivo == NULL) {
		printf("Jogo não encontrado!");
		return;
	}
	fscanf(arquivo, " %s", player->nome);
	fscanf(arquivo, " %c", &dificuldade);
	int nTentativas, nCores;
		switch (dificuldade) {
		case 'F':
			player->nivel = 1;
			nCores = 4;
			nTentativas = 10;
			break;
		case 'M':
			player->nivel = 2;
			nCores = 5;
			nTentativas = 12;
			break;
		case 'D':
			player->nivel = 3;
			nCores = 6;
			nTentativas = 15;
			break;
		default: 
			break;
		}

		int *Segredo = malloc (nCores * sizeof(int));

		int **jogoSecreto = malloc (nTentativas * sizeof(int*));
		for (int i = 0; i < nTentativas; i++) 
			jogoSecreto[i] = malloc(nCores * sizeof(int));
		
	for ( int i=0; i < nCores; i++) 
		fscanf(arquivo, "%d", &Segredo[i]);
	fscanf(arquivo,"%d", tentativaAtual);
	for (int i=0; i < *tentativaAtual; i++) {
		fscanf(arquivo, "\n");
		for (int j=0; j < nCores; j++)
			fscanf(arquivo,"%d ", &jogoSecreto[i][j]);
	}
	fclose (arquivo);

	conferirTentativas(jogoSecreto, Segredo, nCores, nTentativas, *player, jogoAtual, *tentativaAtual);
}