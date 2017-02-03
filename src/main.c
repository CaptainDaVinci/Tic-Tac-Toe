#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void display(char [][3]);
bool checkGame(char [][3]);
bool gameStatus(char [][3]);
void startGame(char [][3], const char, const char);

int main(void)
{
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char player1 = 'O', player2 = 'X';
    char choice;
    int option;

    printf("X or O ?\n");
    scanf("%c", &choice);

    if(choice == 'x' || choice == 'X')
    {
        player1 = 'X';
        player2 = 'O';
    }

    display(game);

    printf("Player 1 - %c\n", player1);
    printf("Player 2 - %c\n", player2);

    startGame(game, player1, player2);
}

void display(char game[][3])
{
    system("clear");

    printf("\n\t------------------\n");
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(game[i][j] == 'X')
                printf(ANSI_COLOR_RED "\tX" ANSI_COLOR_RESET);

            else if(game[i][j] == 'O')
                printf(ANSI_COLOR_GREEN "\tO" ANSI_COLOR_RESET);

            else
                printf("\t%c", game[i][j]);
        }
        printf("\n\t------------------\n");
    }
}

void startGame(char game[3][3], const char player1, const char player2)
{
    int option;

    if(checkGame(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    printf("Player 1's turn - ");
    scanf("%d", &option);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(game[i][j] - '0' == option)
            {
                game[i][j] = player1;
            }
        }
    }

    if(checkGame(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    display(game);
    if(gameStatus(game))
    {
        printf("Player 1 Wins !\n");
        return;
    }

    printf("Player 2's turn - ");
    scanf("%d", &option);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(game[i][j] - '0' == option)
            {
                game[i][j] = player2;
            }
        }
    }

    display(game);
    if(gameStatus(game))
    {
        printf("Player 2 Wins !\n");
        return;
    }

    startGame(game, player1, player2);
}

bool gameStatus(char game[][3])
{
    int i, j;

    for(i = 0, j = 0; i < 3; i++)
        if(game[i][j] == game[i][j + 1] && game[i][j] == game[i][j + 2])
            return true;

    for(j = 0, i = 0; j < 3; j++)
        if(game[i][j] == game[i + 1][j] && game[i][j] == game[i + 2][j])
            return true;


    for(i = 0, j = 0; i < 2; i++)
        if(game[i][j] == game[i + 1][j + 1] && game[i][j] == game[i + 2][j + 2])
            return true;

    for(i = 0, j = 2; i < 2; i++)
        if(game[i][j] == game[i + 1][j - 1] && game[i][j] == game[i + 2][j - 2])
            return true;

    return false;
}

bool checkGame(char game[][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(game[i][j] != 'X' && game[i][j] != 'O')
                return false;
        }
    }
    return true;
}
