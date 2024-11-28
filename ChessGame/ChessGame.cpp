#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cctype>


// Variaveis Globais para log
#define MAX_LOGS 200 // Tamanho maximo de logs (Linhas)
#define LOG_LENGTH 50 // Tamanho maximo de caracteres por linha 
char logs[MAX_LOGS][LOG_LENGTH];
int logCount = 0;


// Variaveis de controle
int whiteKingCaptured = 0;  // Indica se o rei branco foi capturado
int blackKingCaptured = 0;  // Indica se o rei preto foi capturado


int whiteWins = 0; // Contador de vitoria pe�as Brancas
int blackWins = 0; // Contador de vitoria pe�as Pretas


// Variavel do controle
char board[8][8];


//Fun��es Principais
void menu();
void mainGame();
void playerTurn(int player);


// Fun��es de Display e Board
void displayBoard();
void displayLogs();
void displayVictories();
void resetBoard();


// Fun��es de Mecanicas de jogo
void checkForKingCapture();
int isOpponentPiece(char piece, int r, int c);
int isValidPosition(int x, int y);
void changePosition(int r1, int c1, int r2, int c2);


// Fun��es de controle de log
void logMovement(char piece, int startX, int startY, int endX, int endY);
void logEndGame();


// Fun��es de pe�as
int peao(int r1, int c1, int r2, int c2, int isWhite);
int torre(int r1, int c1, int r2, int c2);
int cavalo(int r1, int c1, int r2, int c2);
int bispo(int r1, int c1, int r2, int c2);
int rei(int r1, int c1, int r2, int c2);
int dama(int r1, int c1, int r2, int c2);


int main() {
    menu();
    return 0;
}


void menu() {
    int choice;
    do {
        system("cls");
        printf("======== Jogo de Xadrez ========\n");
        printf("1 - Iniciar Jogo\n");
        printf("2 - Numero de Vitorias\n");
        printf("3 - Logs da Partida\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            resetBoard(); // Reset/Montagem do Tabuleiro
            mainGame(); // Fun��o principal do Jogo
            break;
        case 2:
            displayVictories(); // Tela de contagem das vitorias
            break;
        case 3:
            displayLogs(); // Tela de Log Eng game (Tabuleiro da Partida + Vencedor + Movimentos)
            break;
        case 4:
            printf("Saindo...\n");
            exit(0);
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 4);
}


// Fun��o Principal da Jogo
void mainGame() {
    int round = 0;
    while (1) {
        system("cls");
        displayBoard();
        playerTurn((round % 2) + 1);
        round++;
    }
}


// Fun��o de Reset/Montagem do tabuleiro
void resetBoard() {
    // Inicializa o tabuleiro com espa�os vazios
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = ' ';
        }
    }

    // Configura as pe�as brancas (min�sculas) no topo do tabuleiro
    board[0][0] = board[0][7] = 't'; // Torres
    board[0][1] = board[0][6] = 'c'; // Cavalos
    board[0][2] = board[0][5] = 'b'; // Bispos
    board[0][3] = 'd';               // Dama
    board[0][4] = 'r';               // Rei
    for (int j = 0; j < 8; j++) {
        board[1][j] = 'p';           // Pe�es
    }

    // Configura as pe�as pretas (mai�sculas e Amarelas) na base do tabuleiro
    board[7][0] = board[7][7] = 'T'; // Torres
    board[7][1] = board[7][6] = 'C'; // Cavalos
    board[7][2] = board[7][5] = 'B'; // Bispos
    board[7][3] = 'D';               // Dama
    board[7][4] = 'R';               // Rei
    for (int j = 0; j < 8; j++) {
        board[6][j] = 'P';           // Pe�es
    }

    // Reseta o estado dos reis capturados
    whiteKingCaptured = 0;
    blackKingCaptured = 0;

    // Reseta os logs
    logCount = 0;
}


// Fun��o da tela de jogo
void displayBoard() {
    printf("\n---------Xadrez----------\n");

    // Exibi��o do tabuleiro
    printf("\n");
    for (int r = 7; r >= 0; r--) {
        printf(" %d ", r + 1);
        for (int c = 0; c < 8; c++) {
            printf("|");

            char piece = board[r][c];
            if (piece >= 'A' && piece <= 'Z') {  // Pe�as pretas (mai�sculas)
                printf("\033[33m%c\033[0m", piece);  // Amarelo
            }
            else if (piece >= 'a' && piece <= 'z') {  // Pe�as brancas (min�sculas)
                printf("\033[37m%c\033[0m", piece);  // Branco
            }
            else {
                printf("%c", piece);
            }
        }
        printf("|\n");
    }

    // Cabe�alho das colunas
    printf("    ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", i + 1);
    }
    printf("\n");

    // Legenda explicativa
    printf("\nLegenda:\n");
    printf("  Pecas \033[33mAmarelas\033[0m: Pretas (maiusculas)\n");
    printf("  Pecas \033[37mBrancas\033[0m: Brancas (minusculas)\n");

    printf("\n--------------------------\n");
}


// Fun��o para montar e armazenar log de movimentos
void logMovement(char piece, int startX, int startY, int endX, int endY) {
    if (logCount < MAX_LOGS) {
        // Formata e armazena o movimento no vetor de logs
        snprintf(logs[logCount], LOG_LENGTH,
            "Peca: %c | De: (%d, %d) -> Para: (%d, %d)",
            piece, startX + 1, startY + 1, endX + 1, endY + 1);
        logCount++;
    }
    else {
        printf("Limite de logs atingido!\n");
    }
}


// Monta e imprime tela de final de end game (Tabuleiro da Partida + Vencedor + Movimentos)
void logEndGame() {
    system("cls");  // Limpa a tela

    // Tabuleiro da partida
    printf("======== Jogo Finalizado ========\n");
    displayBoard();

    // Verifica quem � o vencedor e exibe a mensagem
    if (whiteKingCaptured) {
        printf("\nVencedor: Jogador 2 (Pecas Pretas) - O Rei Branco foi capturado!\n");
    }
    else if (blackKingCaptured) {
        printf("\nVencedor: Jogador 1 (Pecas Brancas) - O Rei Preto foi capturado!\n");
    }

    printf("\nLogs da Partida:\n");
    if (logCount == 0) {
        printf("Nenhum movimento foi realizado.\n");
    }
    else {
        for (int i = 0; i < logCount; i++) {
            printf("%d. %s\n", i + 1, logs[i]);
        }
    }

    // Solicita ao usu�rio se deseja salvar os logs em um arquivo
    char choice;
    printf("\nDeseja exportar os logs em um arquivo txt? (S/N): ");
    scanf(" %c", &choice);

    if (choice == 'S' || choice == 's') {
        FILE* file = fopen("jogo_logs.txt", "w");
        if (file == NULL) {
            printf("Erro ao criar o arquivo.\n");
        }
        else {
            fprintf(file, "======== Jogo Finalizado ========\n");

            // Escreve o tabuleiro no arquivo
            fprintf(file, "Tabuleiro Final:\n");
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    fprintf(file, "%c ", board[i][j]);
                }
                fprintf(file, "\n");
            }

            // Escreve o vencedor no arquivo
            if (whiteKingCaptured) {
                fprintf(file, "\nVencedor: Jogador 2 (Pecas Pretas) - O Rei Branco foi capturado!\n");
            }
            else if (blackKingCaptured) {
                fprintf(file, "\nVencedor: Jogador 1 (Pecas Brancas) - O Rei Preto foi capturado!\n");
            }

            // Escreve os logs no arquivo
            fprintf(file, "\nLogs da Partida:\n");
            if (logCount == 0) {
                fprintf(file, "Nenhum movimento foi realizado.\n");
            }
            else {
                for (int i = 0; i < logCount; i++) {
                    fprintf(file, "%d. %s\n", i + 1, logs[i]);
                }
            }

            fclose(file);
            printf("Logs salvos no arquivo 'jogo_logs.txt'.\n");
        }
    }

    printf("\nPressione ENTER para voltar ao menu...\n");
    while (_kbhit()) _getch();  // Limpa qualquer tecla pressionada anteriormente
    _getch();  // Espera pelo Enter
}


// Fun��o para imprimir o log de final de jogo
void displayLogs() {
    system("cls");
    logEndGame();
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    _getch();
}


//Fun��o que imprimi o numero de vitorias de cada jogador
void displayVictories() {
    system("cls");

    printf("======== Numero de Vitorias ========\n\n");
    printf("Jogador 1 (Pecas Brancas): %d vitoria(s)\n", whiteWins);
    printf("Jogador 2 (Pecas Pretas): %d vitoria(s)\n", blackWins);

    printf("\nPressione qualquer tecla para voltar ao menu...\n");
    _getch();
}


// Fun��o que organiza turnos e captura posi��o inicial e final das jogadas
void playerTurn(int player) {
    int r1, c1, r2, c2;
    char piece;
    int validMove = 0;

    // Determina o jogador atual
    if (player == 1) {
        printf("\nJogador 1 (Pecas Brancas), eh sua vez.\n");
    }
    else {
        printf("\nJogador 2 (Pecas Pretas), eh sua vez.\n");
    }

    do {
        // Solicita a posi��o inicial
        printf("Digite a posicao inicial (linha coluna): ");
        scanf("%d %d", &r1, &c1);
        r1 -= 1; // Ajuste para �ndice do vetor
        c1 -= 1; // Ajuste para �ndice do vetor

        // Verifica se posi��o inicial � valida
        if (!isValidPosition(r1, c1) || board[r1][c1] == ' ') {
            printf("Posicao invalida ou sem peca. Tente novamente.\n");
            continue; // Solicita novamente at� ser v�lida
        }

        // Identifica qual pe�a na posi��o inicial
        piece = board[r1][c1];

        // Verifica se a pe�a pertence ao jogador atual
        if ((player == 1 && (piece < 'a' || piece > 'z')) ||
            (player == 2 && (piece < 'A' || piece > 'Z'))) {
            printf("Voce nao pode mover uma peca do adversario. Tente novamente.\n");
            continue;
        }

        // Solicita a posi��o final
        printf("Digite a posicao final (linha coluna): ");
        scanf("%d %d", &r2, &c2);
        r2 -= 1; // Ajuste para �ndice do vetor
        c2 -= 1; // Ajuste para �ndice do vetor

        validMove = 0;

        // Chama a fun��o de movimento espec�fica com base na pe�a
        switch (piece) {
        case 'P': case 'p': // Pe�o
            validMove = peao(r1, c1, r2, c2, (piece == 'p'));
            break;
        case 'T': case 't': // Torre
            validMove = torre(r1, c1, r2, c2);
            break;
        case 'C': case 'c': // Cavalo
            validMove = cavalo(r1, c1, r2, c2);
            break;
        case 'B': case 'b': // Bispo
            validMove = bispo(r1, c1, r2, c2);
            break;
        case 'D': case 'd': // Dama
            validMove = dama(r1, c1, r2, c2);
            break;
        case 'R': case 'r': // Rei
            validMove = rei(r1, c1, r2, c2);
            break;
        default:
            printf("Peca invalida. Tente novamente.\n");
            continue;
        }

        // Se o movimento for v�lido, sai do loop e registra o movimento
        if (validMove) {
            logMovement(piece, r1, c1, r2, c2);
            checkForKingCapture(); // Verifica se houve captura do rei
            break;
        }
    } while (!validMove); // Repete at� o movimento ser v�lido
}


// Fun��o para validar se movimento est� dentro do tabuleiro
int isValidPosition(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}


// Fun��o para validar captura do rei e final da partida
void checkForKingCapture() {
    // Inicializa como n�o capturados
    whiteKingCaptured = 1; // Assume-se que o rei branco est� capturado at� ser encontrado
    blackKingCaptured = 1; // Assume-se que o rei preto est� capturado at� ser encontrado

    // Percorre o tabuleiro procurando os reis
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 'R') {  // Rei preto
                blackKingCaptured = 0; // O Rei Preto est� no tabuleiro
            }
            if (board[i][j] == 'r') {  // Rei branco
                whiteKingCaptured = 0; // O Rei Branco est� no tabuleiro
            }
        }
    }

    // Imprime vencedor e finaliza partida caso um dos reis tenha sido capturado
    if (whiteKingCaptured) {
        printf("Jogador 2 (Pecas Pretas) venceu! O Rei Branco foi capturado!\n");
        blackWins += 1;
        logEndGame();
        menu();
        exit(0);
    }
    else if (blackKingCaptured) {
        printf("Jogador 1 (Pecas Brancas) venceu! O Rei Preto foi capturado!\n");
        whiteWins += 1;
        logEndGame();
        menu();
        exit(0);
    }
}


// Fun��o verifica se � pe�a � do jogador correspondente
int isOpponentPiece(char piece, int r, int c) {
    // Verifica se a pe�a � do jogador advers�rio
    if (piece >= 'a' && piece <= 'z') {  // Pe�as brancas s�o min�sculas
        return board[r][c] >= 'A' && board[r][c] <= 'Z';
    }
    else if (piece >= 'A' && piece <= 'Z') {  // Pe�as pretas s�o mai�sculas
        return board[r][c] >= 'a' && board[r][c] <= 'z';
    }
    return 0;  // N�o � uma pe�a advers�ria
}


// Fun�a� de troca de posi��o
void changePosition(int r1, int c1, int r2, int c2) {
    board[r2][c2] = board[r1][c1];
    board[r1][c1] = ' ';
}


// Fun��o do peao e suas especificidades
int peao(int r1, int c1, int r2, int c2, int isWhite) {
    int direction = isWhite ? 1 : -1; // Branco sobe (+1), preto desce (-1)
    int startRow = isWhite ? 1 : 6;   // Linha inicial dos pe�es brancos e pretos
    int finalRow = isWhite ? 7 : 0;   // Linha final para promo��o

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;
    }

    // Movimento simples para frente (apenas uma casa)
    if (c2 == c1 && r2 == r1 + direction && board[r2][c2] == ' ') {
        changePosition(r1, c1, r2, c2);
    }
    // Movimento inicial de duas casas (se o pe�o estiver na linha inicial)
    else if (c2 == c1 && r1 == startRow && r2 == r1 + 2 * direction &&
        board[r2][c2] == ' ' && board[r1 + direction][c1] == ' ') {
        changePosition(r1, c1, r2, c2);
    }
    // Captura em diagonal (movimento para as casas adjacentes na diagonal)
    else if ((c2 == c1 + 1 || c2 == c1 - 1) && r2 == r1 + direction &&
        isOpponentPiece(board[r1][c1], r2, c2)) {
        changePosition(r1, c1, r2, c2);
    }
    // Movimento inv�lido
    else {
        printf("Movimento invalido para o peao.\n");
        _getch();
        return 0;
    }

    // Verifica promo��o do pe�o ao atingiu a �ltima linha (finalRow)
    if (r2 == finalRow) {
        char escolha;
        printf("Parabens! Seu peao foi promovido. Escolha uma peca (D: Dama, T: Torre, B: Bispo, C: Cavalo): ");
        scanf(" %c", &escolha);
        escolha = isWhite ? tolower(escolha) : toupper(escolha); // Ajusta a pe�a para mai�scula ou min�scula
        board[r2][c2] = escolha; // Substitui o pe�o pela pe�a promovida
    }

    return 1;
}


// Fun��o do torre e suas especificidades
int torre(int r1, int c1, int r2, int c2) {
    int validMove = 0;

    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;
    }

    // Verifica se a torre est� se movendo na mesma linha ou coluna
    if (r1 == r2) {
        // Movimento horizontal: percorre as colunas entre c1 e c2
        if (c2 > c1) {
            for (int i = c1 + 1; i < c2; i++) {
                if (board[r1][i] != ' ') { // Encontra um obst�culo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", r1 + 1, i + 1);
                    _getch();
                    return 0;
                }
            }
        }
        else if (c2 < c1) {
            for (int i = c1 - 1; i > c2; i--) {
                if (board[r1][i] != ' ') { // Encontra um obst�culo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", r1 + 1, i + 1);
                    _getch();
                    return 0;
                }
            }
        }

        // Verifica a posi��o de destino
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }
    else if (c1 == c2) {
        // Movimento vertical: percorre as linhas entre r1 e r2
        if (r2 > r1) {
            for (int i = r1 + 1; i < r2; i++) {
                if (board[i][c1] != ' ') { // Encontra um obst�culo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", i + 1, c1 + 1);
                    _getch();
                    return 0;
                }
            }
        }
        else if (r2 < r1) {
            for (int i = r1 - 1; i > r2; i--) {
                if (board[i][c1] != ' ') { // Encontra um obst�culo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", i + 1, c1 + 1);
                    _getch();
                    return 0;
                }
            }
        }

        // Verifica a posi��o de destino
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }

    // Se o movimento n�o for v�lido
    if (!validMove) {
        printf("Movimento invalido para a torre.\n");
        _getch();
    }

    return validMove;
}


// Fun��o do cavalo e suas especificidades
int cavalo(int r1, int c1, int r2, int c2) {

    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;
    }

    // Verifica se o movimento do cavalo � v�lido (movimento em forma de "L")
    int validMove = 0;
    int possibleMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (int i = 0; i < 8; i++) {
        int newR = r1 + possibleMoves[i][0];
        int newC = c1 + possibleMoves[i][1];

        // Verifica se a posi��o destino � v�lida e se o cavalo pode se mover para l�
        if (newR == r2 && newC == c2) {
            // Permite mover para uma casa vazia ou capturar uma pe�a inimiga
            if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
                changePosition(r1, c1, r2, c2);
                validMove = 1;
            }
            break;
        }
    }

    if (!validMove) {
        printf("Movimento invalido para o cavalo.\n");
        _getch();
    }

    return validMove;
}


// Fun��o do bispo e suas especificidades
int bispo(int r1, int c1, int r2, int c2) {
    int possibleMoves[4][2] = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;
    }

    int validMove = 0;

    // Verifica se o movimento � v�lido para o bispo (somente nas diagonais)
    for (int i = 0; i < 4; i++) {
        int newR = r1;
        int newC = c1;

        // Move a pe�a ao longo da diagonal
        while (isValidPosition(newR, newC)) {
            newR += possibleMoves[i][0];
            newC += possibleMoves[i][1];

            // Verifica se atingiu a posi��o final (r2, c2)
            if (newR == r2 && newC == c2) {
                // Permite mover para uma casa vazia ou capturar uma pe�a inimiga
                if (board[newR][newC] == ' ' || isOpponentPiece(board[r1][c1], newR, newC)) {
                    changePosition(r1, c1, r2, c2);
                    validMove = 1;
                }
                break;
            }

            // Se houver uma pe�a no caminho, o movimento � bloqueado
            if (board[newR][newC] != ' ') {
                printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                break;
            }
        }

        if (validMove) {
            break;
        }
    }

    if (!validMove) {
        printf("Movimento invalido para o bispo.\n");
        _getch();
    }

    return validMove;
}


// Fun��o do Dama e suas especificidades
int dama(int r1, int c1, int r2, int c2) {
    int validMove = 0;

    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;  // Retorna 0 se o movimento for inv�lido
    }

    // Verifica movimento horizontal (mesma linha)
    if (r1 == r2) {
        if (c1 < c2) { // Movimento para a direita
            for (int i = c1 + 1; i < c2; i++) {
                if (board[r1][i] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return 0;
                }
            }
        }
        else { // Movimento para a esquerda
            for (int i = c1 - 1; i > c2; i--) {
                if (board[r1][i] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return 0;
                }
            }
        }
        // Verifica se o destino � vazio ou uma pe�a inimiga
        if (board[r1][c2] == ' ' || isOpponentPiece(board[r1][c1], r1, c2)) {
            changePosition(r1, c1, r1, c2);
            validMove = 1;
        }
    }
    // Verifica movimento vertical (mesma coluna)
    else if (c1 == c2) {
        if (r1 < r2) { // Movimento para baixo
            for (int i = r1 + 1; i < r2; i++) {
                if (board[i][c1] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return 0;
                }
            }
        }
        else { // Movimento para cima
            for (int i = r1 - 1; i > r2; i--) {
                if (board[i][c1] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return 0;
                }
            }
        }
        // Verifica se o destino � vazio ou uma pe�a inimiga
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }
    // Verifica movimento diagonal
    else {
        int rowDirection = (r2 > r1) ? 1 : -1;
        int colDirection = (c2 > c1) ? 1 : -1;
        int i = r1 + rowDirection, j = c1 + colDirection;

        while (i != r2 && j != c2) {
            if (board[i][j] != ' ') { // Verifica se h� pe�as no caminho
                printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                _getch();
                return 0;
            }
            i += rowDirection;
            j += colDirection;
        }
        // Verifica se o destino � vazio ou uma pe�a inimiga
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }

    if (!validMove) {
        printf("Movimento invalido para a rainha.\n");
        _getch();
    }

    return validMove;
}


// Fun��o do Rei e suas especificidades
int rei(int r1, int c1, int r2, int c2) {
    int possibleMoves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return 0;
    }

    int validMove = 0;
    for (int i = 0; i < 8; i++) {
        int newR = r1 + possibleMoves[i][0];
        int newC = c1 + possibleMoves[i][1];

        // Verifica se a posi��o de destino � v�lida para o movimento do rei
        if (newR == r2 && newC == c2) {
            // Permite mover para uma casa vazia ou capturar uma pe�a inimiga
            if (board[newR][newC] == ' ' || isOpponentPiece(board[r1][c1], newR, newC)) {
                changePosition(r1, c1, newR, newC);
                validMove = 1;
                break;
            }
        }
    }

    if (!validMove) {
        printf("Movimento invalido para o rei.\n");
        _getch();
    }

    return validMove;
}