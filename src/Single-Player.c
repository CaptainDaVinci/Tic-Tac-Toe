#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// Adds colour to the output.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void display(char [][3]);
bool checkGame(char [][3]);
bool gameStatus(char [][3]);
void startGame(char [][3], const char, const char);
int goodMove(char [][3], int);

bool validPos[10] = {false, true, true, true, true, true, true, true, true, true};
int flag = 0;

int main(void)
{
    // A two-dimensional array depicting the game.
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char player1 = 'O', computer = 'X';
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
        computer = 'O';
    }

    display(game);

    printf("Player 1 - %c\n", player1);
    printf("Computer - %c\n", computer);


    startGame(game, player1, computer);
}

// displays the game's current status.
void display(char game[][3])
{
    system("clear");

    int i, j;
    printf("\n\t------------------\n");
    for( i = 0; i < 3; i++)
    {
        for( j = 0; j < 3; j++)
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

void startGame(char game[3][3], const char player1, const char computer)
{
    int i, j;
    int userChoice, compChoice;

    do
    {
        printf("Player 1's turn : ");
        scanf("%d", &userChoice);

    }while((userChoice > 9 || userChoice < 1) || !validPos[userChoice]);

    validPos[userChoice] = false;
    // Assigns either X or O to the spot where the player 1 wants to mark.
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] - '0' == userChoice)
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

    srand((unsigned)time(NULL));
    do
    {
        compChoice = rand() % 11;

    }while((compChoice > 9 || compChoice < 1) || !validPos[compChoice]);

    printf("%d\n", compChoice);
    compChoice = goodMove(game, compChoice);
    printf("%d\n", compChoice);
    validPos[compChoice] = false;

    // Assigns either X or O to the spot where the player 2 wants to mark.
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] - '0' == compChoice)
                game[i][j] = computer;

    display(game);

    // checks if the player 2 has won the game or not.
    // also acts as the base case for the game to end.
    if(gameStatus(game))
    {
        printf("computer Wins !\n");
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
    startGame(game, player1, computer);
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
    if(game[0][0] == game[1][1] && game[0][0] == game[2][2])
            return true;

    // checks along left diagonal.
    if(game[0][2] == game[1][1] && game[0][2] == game[2][0])
            return true;

    return false;
}

// checks if there are any further moves possible.
bool checkGame(char game[][3])
{
    int i, j;
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] != 'X' && game[i][j] != 'O')
                return false;

    return true;
}

int goodMove(char game[][3], int choice)
{
    int i, j;

    for(i = 0; i < 3; i++)
    {
        if(game[i][0] == game[i][1] && validPos[game[i][2] - '0'])
            return game[i][2] - '0';

        if(game[i][0] == game[i][2] && validPos[game[i][1] - '0'])
            return game[i][1] - '0';

        if(game[i][1] == game[i][2] && validPos[game[i][0] - '0'])
            return game[i][0] - '0';
    }

    for(j = 0; j < 3; j++)
    {
        if(game[0][j] == game[1][j] && validPos[game[2][j] - '0'])
            return game[2][j] - '0';

        if(game[1][j] == game[2][j] && validPos[game[0][j] - '0'])
            return game[0][j] - '0';

        if(game[0][j] == game[2][j] && validPos[game[1][j] - '0'])
            return game[1][j] - '0';
    }

    if(game[0][0] == game[1][1] && validPos[game[2][2] - '0'])
        return game[2][2] - '0';

    if(game[0][0] == game[2][2] && validPos[game[1][1] - '0'])
        return game[1][1] - '0';

    if(game[1][1] == game[2][2] && validPos[game[0][0] - '0'])
        return game[0][0] - '0';

    if(game[0][2] == game[1][1] && validPos[game[2][0] - '0'])
        return game[2][0] - '0';

    if(game[0][2] == game[2][0] && validPos[game[1][1] - '0'])
        return game[1][1] - '0';

    if(game[1][1] == game[2][0] && validPos[game[0][2] - '0'])
        return game[0][2] - '0';

        if(validPos[5])
        {
            flag = 1;
            return 5;
        }
        
        if(validPos[7])
        {
            flag = 1;
            return 7;
        }

        if(validPos[3])
        {
            flag = 1;
            return 3;
        }

        if(validPos[1])
        {
            flag = 1;
            return 1;
        }


        if(validPos[9])
        {
            flag = 1;
            return 9;
        }

    return choice;
}
