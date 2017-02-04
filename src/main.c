#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Adds colour to the output.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void display(char [][3]);
bool checkGame(char [][3]);
bool gameStatus(char [][3]);
void startGame(char [][3], const char, const char);

int main(void)
{
    // A two-dimensional array depicting the game.
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char player1 = 'O', player2 = 'X';
    char choice;

    do
    {
        printf("X or O ?\n");
        scanf(" %c", &choice);

    }while(choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o');

    // assigns X or O to the two players.
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

// displays the game's current status.
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
    char choice;

    do
    {
        printf("Player 1's turn : ");
        scanf(" %c", &choice);

    }while(!isdigit(choice));

    // Assigns either X or O to the spot where the player 1 wants to mark.
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(game[i][j] == choice)
                game[i][j] = player1;

    display(game);


    // checks if the player 1 has won the game or not.
    // also acts as the base case for the game to end.
    if(gameStatus(game))
    {
        printf("Player 1 Wins !\n");
        return;
    }

    // checks if there are any further moves possible.
    // also acts as the base case for the game to end.
    if(checkGame(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    do
    {
        printf("Player 2's turn : ");
        scanf(" %c", &choice);

    }while(!isdigit(choice));

    // Assigns either X or O to the spot where the player 2 wants to mark.
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(game[i][j] == choice)
                game[i][j] = player2;

    display(game);

    // checks if the player 2 has won the game or not.
    // also acts as the base case for the game to end.
    if(gameStatus(game))
    {
        printf("Player 2 Wins !\n");
        return;
    }

    // checks if the game has any possible moves that can be played.
    // also acts as the base case for the game to end.
    if(checkGame(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    // recursive call to continue the game.
    startGame(game, player1, player2);
}

// checks if the game is completed when there are 3 consecutive X or O
// along a row, coloumn or either of the diagonals.
bool gameStatus(char game[][3])
{
    int i, j;

    // checks along a row.
    for(i = 0, j = 0; i < 3; i++)
        if(game[i][j] == game[i][j + 1] && game[i][j] == game[i][j + 2])
            return true;

    // checks along a coloumn.
    for(j = 0, i = 0; j < 3; j++)
        if(game[i][j] == game[i + 1][j] && game[i][j] == game[i + 2][j])
            return true;

    // checks along right diagonal.
    for(i = 0, j = 0; i < 2; i++)
        if(game[i][j] == game[i + 1][j + 1] && game[i][j] == game[i + 2][j + 2])
            return true;

    // checks along left diagonal.
    for(i = 0, j = 2; i < 2; i++)
        if(game[i][j] == game[i + 1][j - 1] && game[i][j] == game[i + 2][j - 2])
            return true;

    return false;
}

// checks if there are any further moves possible.
bool checkGame(char game[][3])
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(game[i][j] != 'X' && game[i][j] != 'O')
                return false;

    return true;
}
