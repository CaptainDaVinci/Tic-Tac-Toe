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
bool isComplete(char [][3]);
bool gameStatus(char [][3]);
void startGame(char [][3], const char, const char);
int goodMove(char [][3], int, char);

bool validPos[10] = {false, true, true, true, true, true, true, true, true, true};

int main(void)
{
    // A two-dimensional array depicting the game.
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char user = 'O', computer = 'X';
    char choice;

    do
    {
        printf("X or O ?\n");
        scanf(" %c", &choice);

    }while(choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o');

    // assigns X or O to the two players.
    if(choice == 'x' || choice == 'X')
    {
        user = 'X';
        computer = 'O';
    }

    display(game);

    printf("You - %c\n", user);
    printf("Computer - %c\n", computer);

    startGame(game, user, computer);
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

void startGame(char game[3][3], const char user, const char computer)
{
    int i, j;
    int choice;

    do
    {
        printf("Your turn : ");
        scanf("%d", &choice);

    }while((choice > 9 || choice < 1) || !validPos[choice]);

    validPos[choice] = false;

    // Assigns either X or O to the spot where the Your turn wants to mark.
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] - '0' == choice)
                game[i][j] = user;

    display(game);

    // checks if the Your turn has won the game or not.
    // also acts as the base case for the game to end.
    if(gameStatus(game))
    {
        printf("You Win !\n");
        return;
    }

    // checks if there are any further moves possible.
    // also acts as the base case for the game to end.
    if(isComplete(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    srand((unsigned)time(NULL));
    do
    {
        choice = rand() % 11;

    }while((choice > 9 || choice < 1) || !validPos[choice]);

    choice = goodMove(game, choice, computer);
    validPos[choice] = false;

    // Assigns either X or O to the spot where the player 2 wants to mark.
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] - '0' == choice)
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
    if(isComplete(game))
    {
        printf("Game Over!\nNo More Possible Moves\n");
        return;
    }

    // recursive call to continue the game.
    startGame(game, user, computer);
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
bool isComplete(char game[][3])
{
    int i, j;
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] != 'X' && game[i][j] != 'O')
                return false;

    return true;
}

int goodMove(char game[][3], int choice, char computer)
{
    int i, j;

    for(i = 0; i < 3; i++)
    {
        if(game[i][0] == game[i][1] && game[i][0] == computer && validPos[game[i][2] - '0'])
            return game[i][2] - '0';

        if(game[i][0] == game[i][2] && game[i][0] == computer && validPos[game[i][1] - '0'])
            return game[i][1] - '0';

        if(game[i][1] == game[i][2] && game[i][1] == computer && validPos[game[i][0] - '0'])
            return game[i][0] - '0';
    }

    for(j = 0; j < 3; j++)
    {
        if(game[0][j] == game[1][j] && game[0][j] == computer && validPos[game[2][j] - '0'])
            return game[2][j] - '0';

        if(game[1][j] == game[2][j] && game[1][j] == computer && validPos[game[0][j] - '0'])
            return game[0][j] - '0';

        if(game[0][j] == game[2][j] && game[0][j] == computer && validPos[game[1][j] - '0'])
            return game[1][j] - '0';
    }

    if(game[0][0] == game[1][1] && game[0][0] == computer && validPos[game[2][2] - '0'])
        return game[2][2] - '0';

    if(game[0][0] == game[2][2] && game[0][0] == computer && validPos[game[1][1] - '0'])
        return game[1][1] - '0';

    if(game[1][1] == game[2][2] && game[1][1] == computer && validPos[game[0][0] - '0'])
        return game[0][0] - '0';

    if(game[0][2] == game[1][1] && game[0][2] == computer && validPos[game[2][0] - '0'])
        return game[2][0] - '0';

    if(game[0][2] == game[2][0] && game[0][2] == computer && validPos[game[1][1] - '0'])
        return game[1][1] - '0';

    if(game[1][1] == game[2][0] && game[1][1] == computer && validPos[game[0][2] - '0'])
        return game[0][2] - '0';

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

    switch(rand() % 6)
    {
        case 0 : if(validPos[5])
                    return 5;

        case 1 : if(validPos[7] && validPos[3])
                    return 7;

        case 2 : if(validPos[3] && validPos[7])
                    return 3;

        case 3 : if(validPos[1] && validPos[9])
                    return 1;

        case 4 : if(validPos[5])
                    return 5;

        default : return choice;
    }
}
