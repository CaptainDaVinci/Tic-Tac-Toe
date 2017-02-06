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
void startGame(char [][3], const char);
int calcMove(char [][3], int, char);

// keeps track of all the valid positions.
bool validPos[10] = {false, true, true, true, true, true, true, true, true, true};
char user = 'O', computer = 'X';

int main(void)
{
    // A two-dimensional array depicting the game.
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char choice;

    // Asks user for a valid input.
    do
    {
        printf("X or O ?\n");
        scanf(" %c", &choice);

    }while(choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o');

    // assigns X or O to the player.
    if(choice == 'x' || choice == 'X')
    {
        user = 'X';
        computer = 'O';
    }

    display(game);

    printf("You - %c\n", user);
    printf("Computer - %c\n", computer);

    startGame(game, user);
}

// displays the game.
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

void startGame(char game[3][3], const char turn)
{
    int i, j;
    int pos;

    srand((unsigned)time(NULL));
    if(turn == user)
    {
        // asks user for a valid input.
        do
        {
            printf("Your turn : ");
            scanf("%d", &pos);

        }while((pos > 9 || pos < 1) || !validPos[pos]);
    }

    else
    {
        do
        {
            // calculates a good and valid move for the computer.
            pos = rand() % 10;
        }while((pos > 9 || pos < 1) || !validPos[pos]);
        pos = calcMove(game, pos, computer);
    }

    // makes the position chosen invalid.
    validPos[pos] = false;

    // Assigns 'X' or 'O' to the chosen position.
    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game[i][j] - '0' == pos)
                game[i][j] = turn;

    // prints the game onto the screen.
    display(game);

    // checks if the player or computer has won or not.
    // Also checks if there are any further moves possible.
    if(gameStatus(game))
    {
        if(turn == user)
            printf("You Win !\n");

        else
            printf("Computer Wins !\n");

        return;
    }

    // recursive call to continue the game.
    if(turn == user)
        startGame(game, computer);

    if(turn == computer)
        startGame(game, user);
}

// checks if the game is completed when there are 3 consecutive X or O
// along a row, coloumn or either of the diagonals.
bool gameStatus(char game[][3])
{
    int i, j;
    int flag = 0;

    // checks if there are any further possible moves.
    for(i = 1; i < 10; i++)
    {
        if(validPos[i])
        {
            flag = 1;
            break;
        }
    }

    // if there are no more possible moves then the game is over.
    if(flag == 0)
    {
        printf("Game Over!\nNo more possible moves\n");
        exit(0);
    }
    
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

int calcMove(char game[][3], int pos, char computer)
{
    int i, j;
    int move = 0;

    // If there is any move along a row that is a winning move for the computer
    // then take it or block the winning move of the user(if any) along a row.
    for(i = 0; i < 3; i++)
    {
        if(game[i][0] == game[i][1] && validPos[game[i][2] - '0'])
        {
            move = game[i][2] - '0';
            if(game[i][0] == computer)
                return move;
        }

        if(game[i][0] == game[i][2] && validPos[game[i][1] - '0'])
        {
            move = game[i][1] - '0';
            if(game[i][0] == computer)
                return move;
        }

        if(game[i][1] == game[i][2] && validPos[game[i][0] - '0'])
        {
            move = game[i][0] - '0';
            if(game[i][1] == computer)
                return move;
        }
    }

    // If there is any move along a coloumn that is a winning move for the computer
    // then take it or block the winning move of the user(if any) along a coloumn.
    for(j = 0; j < 3; j++)
    {
        if(game[0][j] == game[1][j] && validPos[game[2][j] - '0'])
        {
            move = game[2][j] - '0';
            if(game[0][j] == computer)
                return move;
        }

        if(game[1][j] == game[2][j] && validPos[game[0][j] - '0'])
        {
            move = game[0][j] - '0';
            if(game[1][j] == computer)
                return move;
        }

        if(game[0][j] == game[2][j] && validPos[game[1][j] - '0'])
        {
            move = game[1][j] - '0';
            if(game[0][j] == computer)
                return move;
        }
    }

    // checks for a winning move along the right diagonal.
    // preferance is given to winning move for computer.
    if(game[0][0] == game[1][1] && validPos[game[2][2] - '0'])
    {
        move = game[2][2] - '0';
        if(game[0][0] == computer)
            return move;
    }

    if(game[0][0] == game[2][2] && validPos[game[1][1] - '0'])
    {
        move = game[1][1] - '0';
        if(game[0][0] == computer)
            return move;
    }

    if(game[1][1] == game[2][2] && validPos[game[0][0] - '0'])
    {
        move = game[0][0] - '0';
        if(game[1][1] == computer)
            return move;
    }

    // checks for a winning move along the left diagonal.
    // preferance is given to winning move for computer.
    if(game[0][2] == game[1][1] && validPos[game[2][0] - '0'])
    {
        move = game[2][0] - '0';
        if(game[0][2] == computer)
            return move;
    }

    if(game[0][2] == game[2][0] && validPos[game[1][1] - '0'])
    {
        move = game[1][1] - '0';
        if(game[0][2] == computer)
            return move;
    }

    if(game[1][1] == game[2][0] && validPos[game[0][2] - '0'])
    {
        move = game[0][2] - '0';
        if(game[1][1] == computer)
            return move;
    }

    // if there was any such move in which either the player or
    // the computer would win, then return it.
    if(move != 0)
        return move;

    // if there was no such move, then select either or the corners
    // or the center randomly.
    switch(rand() % 4)
    {

        case 1 : if(validPos[7] && validPos[3])
                    return 7;

        case 2 : if(validPos[3] && validPos[7])
                    return 3;

        case 3 : if(validPos[1] && validPos[9])
                    return 1;

        case 4 : if(validPos[5])
                    return 5;

        default : return pos;
    }
}
