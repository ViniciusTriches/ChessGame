#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cctype>

// Configura��es de logs
#define MAX_LOGS 200
#define LOG_LENGTH 50
char logs[MAX_LOGS][LOG_LENGTH];
int logCount = 0;

int whiteKingCaptured = 0;  // Indica se o rei branco foi capturado
int blackKingCaptured = 0;  // Indica se o rei preto foi capturado

int whiteWins = 0;
int blackWins = 0;

// Estado do tabuleiro e pe�es
char board[8][8] = {
    { 't', 'c', 'b', 'r', 'd', 'b', 'c', 't' },
    { 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p' },
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' },
    { 'T', 'C', 'B', 'R', 'D', 'B', 'C', 'T' }
};
int pwstatus[8] = { 0 };
int pbstatus[8] = { 0 };

// Fun��es principais
void displayBoard();
void logMovement(char piece, int startX, int startY, int endX, int endY);
void logGameEnd();
void changePosition(int r1, int c1, int r2, int c2);
void playerTurn(int player);
void movePiece(char piece, int r1, int c1, int r2, int c2);
int isOpponentPiece(char piece, int r, int c);
void checkForKingCapture();
int isValidPosition(int x, int y);
void menu();
void mainGame();
void displayLogs();
void displayVictories();

// Fun��es de pe�as
void peao(int r1, int c1, int isWhite);
void torre(int r1, int c1);
void cavalo(int r1, int c1);
void bispo(int r1, int c1);
void rei(int r1, int c1);
void dama(int r1, int c1);

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
        printf("3 - Logs da Ultima Partida\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            mainGame();
            break;
        case 2:
            displayVictories();
            break;
        case 3:
            displayLogs();
            break;
        case 4:
            printf("Saindo...\n");
            exit(0);
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 4);
}

void mainGame() {
    int round = 0;
    logCount = 0; // Limpar logs para a nova partida
    while (1) {
        system("cls");
        displayBoard();
        playerTurn((round % 2) + 1);
        round++;
    }
}

void displayBoard() {
    // T�tulo do tabuleiro
    printf("\n---------Xadrez----------\n");

    // Exibi��o do tabuleiro
    printf("\n"); // Linha inicial para separa��o visual
    for (int r = 7; r >= 0; r--) { // Exibe as linhas de cima para baixo
        printf(" %d ", r + 1); // Numera��o das linhas � esquerda
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
                printf("%c", piece);  // Espa�os vazios
            }
        }
        printf("|\n"); // Fecha a linha
    }

    // Cabe�alho das colunas
    printf("    "); // Espa�o antes do cabe�alho das colunas
    for (int i = 0; i < 8; i++) {
        printf("%d ", i + 1); // Numera��o das colunas na parte inferior
    }
    printf("\n");

    // Legenda explicativa
    printf("\nLegenda:\n");
    printf("  Pecas \033[33mAmarelas\033[0m: Pretas (maiusculas)\n");
    printf("  Pecas \033[37mBrancas\033[0m: Brancas (minusculas)\n");

    // Divis�o final
    printf("\n--------------------------\n");
}




void logMovement(char piece, int startX, int startY, int endX, int endY) {
    if (logCount < MAX_LOGS) {
        sprintf(logs[logCount], "Peca: %c | De: (%d, %d) -> Para: (%d, %d)",
            piece, startX + 1, startY + 1, endX + 1, endY + 1);
        logCount++;
    }
    else {
        printf("Limite de logs atingido!\n");
    }
}


void logGameEnd() {
    system("cls");  // Limpa a tela

    // Exibe o tabuleiro e os logs
    printf("======== Jogo Finalizado ========\n");
    displayBoard();  // Exibe o tabuleiro atual

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

    printf("\nPressione ENTER para voltar ao menu...\n");
    while (_kbhit()) _getch();  // Limpa qualquer tecla pressionada anteriormente
    _getch();  // Espera pelo Enter
}


void displayLogs() {
    system("cls");
    logGameEnd();
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    _getch();
}

void displayVictories() {
    system("cls");  // Limpa a tela para exibir apenas as informa��es necess�rias

    printf("======== Numero de Vitorias ========\n\n");
    printf("Jogador 1 (Pecas Brancas): %d vitoria(s)\n", whiteWins);
    printf("Jogador 2 (Pecas Pretas): %d vitoria(s)\n", blackWins);

    printf("\nPressione qualquer tecla para voltar ao menu...\n");
    _getch();  // Aguarda o usu�rio pressionar uma tecla antes de retornar
}


void playerTurn(int player) {
    int r1, c1, r2, c2;
    char piece;

    // Determina o jogador atual
    if (player == 1) {
        printf("\nJogador 1 (Pecas Brancas), eh sua vez.\n");
    }
    else {
        printf("\nJogador 2 (Pecas Pretas), eh sua vez.\n");
    }

    // Solicita a posi��o inicial
    printf("Digite a posicao inicial (linha coluna): ");
    scanf("%d %d", &r1, &c1);
    r1 -= 1; // Ajuste para �ndice do array
    c1 -= 1;

    // Verifica validade da posi��o inicial
    if (!isValidPosition(r1, c1) || board[r1][c1] == ' ') {
        printf("Posicao invalida ou sem peca. Tente novamente.\n");
        _getch();
        return;
    }

    // Identifica a pe�a na posi��o inicial
    piece = board[r1][c1];

    // Verifica se a pe�a pertence ao jogador atual
    if ((player == 1 && (piece < 'a' || piece > 'z')) ||
        (player == 2 && (piece < 'A' || piece > 'Z'))) {
        printf("Voce nao pode mover uma peca do adversario. Tente novamente.\n");
        _getch();
        return;
    }

    // Chama a fun��o de movimento espec�fica com base na pe�a
    switch (piece) {
    case 'P': case 'p': // Pe�o
        peao(r1, c1, (piece == 'p'));
        break;
    case 'T': case 't': // Torre
        torre(r1, c1);
        break;
    case 'C': case 'c': // Cavalo
        cavalo(r1, c1);
        break;
    case 'B': case 'b': // Bispo
        bispo(r1, c1);
        break;
    case 'D': case 'd': // Dama
        dama(r1, c1);
        break;
    case 'R': case 'r': // Rei
        rei(r1, c1);
        break;
    default:
        printf("Peca invalida. Tente novamente.\n");
        _getch();
        return;
    }
    checkForKingCapture();
}


int isValidPosition(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}


void movePiece(char piece, int r1, int c1, int r2, int c2) {
    if (board[r2][c2] == ' ') {  // Caso o destino esteja vazio, apenas move a pe�a
        changePosition(r1, c1, r2, c2);
        logMovement(piece, r1, c1, r2, c2);
    }
    else if (isOpponentPiece(piece, r2, c2)) {  // Se houver pe�a advers�ria no destino, captura a pe�a
        printf("Captura de pe�a: %c\n", board[r2][c2]);
        changePosition(r1, c1, r2, c2);
        logMovement(piece, r1, c1, r2, c2);
    }
    else {  // Caso contr�rio, o movimento � inv�lido
        printf("Movimento inv�lido, posi��o ocupada pela pr�pria pe�a.\n");
        _getch();
        return;  // N�o chama checkForKingCapture se o movimento for inv�lido
    }

    // Chama checkForKingCapture somente ap�s um movimento/captura v�lido
    checkForKingCapture();
}


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

    // Condi��es para verificar a captura de reis
    if (whiteKingCaptured) {
        printf("Jogador 2 (Pecas Pretas) venceu! O Rei Branco foi capturado!\n");
        blackWins += 1;
        logGameEnd();
        menu();
        exit(0);
    }
    else if (blackKingCaptured) {
        printf("Jogador 1 (Pecas Brancas) venceu! O Rei Preto foi capturado!\n");
        whiteWins += 1;
        logGameEnd();
        menu();
        exit(0);
    }
}


int isOpponentPiece(char piece, int r, int c) {
    // Verifica se a pe�a � do jogador advers�rio
    if (piece >= 'a' && piece <= 'z') {  // Pe�as brancas s�o min�sculas
        return board[r][c] >= 'A' && board[r][c] <= 'Z';  // Se for mai�scula, � uma pe�a preta
    }
    else if (piece >= 'A' && piece <= 'Z') {  // Pe�as pretas s�o mai�sculas
        return board[r][c] >= 'a' && board[r][c] <= 'z';  // Se for min�scula, � uma pe�a branca
    }
    return 0;  // N�o � uma pe�a advers�ria
}

void changePosition(int r1, int c1, int r2, int c2) {
    board[r2][c2] = board[r1][c1];
    board[r1][c1] = ' ';
}


void peao(int r1, int c1, int isWhite) {
    int direction = isWhite ? 1 : -1; // Branco sobe (-1), preto desce (+1)
    int startRow = isWhite ? 1 : 6;  // Linha inicial dos pe�es brancos e pretos

    int r2, c2;
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)
    c2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
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
    // Promo��o do pe�o (quando chega na �ltima linha do tabuleiro)
    else if (r2 == (isWhite ? 0 : 7)) {
        char escolha;
        printf("Parab�ns! Seu pe�o foi promovido. Escolha uma pe�a (D: Dama, T: Torre, B: Bispo, C: Cavalo): ");
        scanf(" %c", &escolha);
        escolha = isWhite ? tolower(escolha) : toupper(escolha); // Ajusta a pe�a para mai�scula ou min�scula
        board[r1][c1] = ' '; // Remove o pe�o da posi��o original
        board[r2][c2] = escolha; // Coloca a pe�a promovida no lugar do pe�o
    }
    // Movimento inv�lido
    else {
        printf("Movimento invalido para o peao.\n");
        _getch();
    }
}


void torre(int r1, int c1) {
    int r2, c2;
    int validMove = 0;

    // Solicita ao jogador as novas coordenadas (r2, c2)
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para indice do array (entrada do usuario e 1-indexed)
    c2 -= 1; // Ajusta para indice do array (entrada do usuario e 1-indexed)

    // Verifica se o movimento e dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
    }

    // Verifica se a torre esta se movendo na mesma linha ou coluna
    if (r1 == r2) {
        // Movimento horizontal: percorre as colunas entre c1 e c2
        if (c2 > c1) {
            for (int i = c1 + 1; i < c2; i++) {
                if (board[r1][i] != ' ') {  // Encontra um obstaculo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", r1 + 1, i + 1);
                    _getch();
                    return;
                }
            }
        }
        else if (c2 < c1) {
            for (int i = c1 - 1; i > c2; i--) {
                if (board[r1][i] != ' ') {  // Encontra um obstaculo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", r1 + 1, i + 1);
                    _getch();
                    return;
                }
            }
        }

        // Verifica a posicao de destino
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }
    else if (c1 == c2) {
        // Movimento vertical: percorre as linhas entre r1 e r2
        if (r2 > r1) {
            for (int i = r1 + 1; i < r2; i++) {
                if (board[i][c1] != ' ') {  // Encontra um obstaculo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", i + 1, c1 + 1);
                    _getch();
                    return;
                }
            }
        }
        else if (r2 < r1) {
            for (int i = r1 - 1; i > r2; i--) {
                if (board[i][c1] != ' ') {  // Encontra um obstaculo
                    printf("Movimento invalido. Ha um obstaculo na posicao (%d, %d).\n", i + 1, c1 + 1);
                    _getch();
                    return;
                }
            }
        }

        // Verifica a posicao de destino
        if (board[r2][c2] == ' ' || isOpponentPiece(board[r1][c1], r2, c2)) {
            changePosition(r1, c1, r2, c2);
            validMove = 1;
        }
    }

    // Se o movimento nao for valido
    if (!validMove) {
        printf("Movimento invalido para a torre.\n");
        _getch();
    }
}



void cavalo(int r1, int c1) {
    // O cavalo se move em forma de "L", 2 casas em uma dire��o e 1 casa perpendicular a essa
    // Exemplo: (r1+2, c1+1) ou (r1-2, c1-1), etc.

    int r2, c2;
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)
    c2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
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
        if (newR == r2 && newC == c2 && isValidPosition(r2, c2)) {
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
}


void bispo(int r1, int c1) {
    // O bispo se move quantas casas quiser, mas somente nas diagonais
    int possibleMoves[4][2] = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    int validMove = 0;
    int r2, c2;
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)
    c2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
    }

    // Verificar se o movimento � v�lido para o bispo, ou seja, apenas nas diagonais
    for (int i = 0; i < 4; i++) {
        int newR = r1;
        int newC = c1;

        // Move a pe�a ao longo da diagonal
        while (isValidPosition(newR, newC)) {
            newR += possibleMoves[i][0];
            newC += possibleMoves[i][1];

            // Verifica se atingiu a posi��o final (r2, c2)
            if (newR == r2 && newC == c2) {
                if (board[newR][newC] == ' ' || isOpponentPiece(board[r1][c1], newR, newC)) {
                    changePosition(r1, c1, r2, c2);
                    validMove = 1;
                }
                break;
            }

            // Se houver uma pe�a no caminho, o movimento � bloqueado
            if (board[newR][newC] != ' ') {
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
}


void rei(int r1, int c1) {
    // O rei pode mover-se uma casa em qualquer dire��o (vertical, horizontal ou diagonal)
    int possibleMoves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    int r2, c2;
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)
    c2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
    }

    int validMove = 0;
    for (int i = 0; i < 8; i++) {
        int newR = r1 + possibleMoves[i][0];
        int newC = c1 + possibleMoves[i][1];

        if (newR == r2 && newC == c2) {
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
}


void dama(int r1, int c1) {
    // A rainha se move em qualquer dire��o, vertical, horizontal e diagonal
    int validMove = 0;
    int r2, c2;
    printf("Digite a posicao de destino (linha coluna): ");
    scanf("%d %d", &r2, &c2);
    r2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)
    c2 -= 1; // Ajusta para �ndice do array (entrada do usu�rio � 1-indexed)

    // Verifica se o movimento � dentro do tabuleiro
    if (!isValidPosition(r2, c2)) {
        printf("Movimento fora do tabuleiro. Tente novamente.\n");
        _getch();
        return;
    }

    // Verifica movimento horizontal
    if (r1 == r2) {
        if (c1 < c2) { // Movimento para a direita
            for (int i = c1 + 1; i < c2; i++) {
                if (board[r1][i] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return;
                }
            }
        }
        else { // Movimento para a esquerda
            for (int i = c1 - 1; i > c2; i--) {
                if (board[r1][i] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return;
                }
            }
        }
        // Verifica se o destino � vazio ou uma pe�a inimiga
        if (board[r1][c2] == ' ' || isOpponentPiece(board[r1][c1], r1, c2)) {
            changePosition(r1, c1, r1, c2);
            validMove = 1;
        }
    }
    // Verifica movimento vertical
    else if (c1 == c2) {
        if (r1 < r2) { // Movimento para baixo
            for (int i = r1 + 1; i < r2; i++) {
                if (board[i][c1] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return;
                }
            }
        }
        else { // Movimento para cima
            for (int i = r1 - 1; i > r2; i--) {
                if (board[i][c1] != ' ') { // Verifica se h� pe�as no caminho
                    printf("Ha uma pe�a no caminho. Movimente-se para outra casa.\n");
                    _getch();
                    return;
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
                return;
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
}