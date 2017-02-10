#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// Adds colour to the output.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct
{
    char board[3][3];
    bool validPos[10];
    char user, comp;
    bool userTurn, compTurn;

}Game;

void display(Game *game);
void assignXO(Game *game);
void userMove(Game *game);
void compMove(Game *game);
bool isComplete(Game *game);
bool victoryCheck(Game *game);
void startGame(Game *game);
int calcMove(Game *game, int position);
void gameSetup(Game *game);

int main(void)
{

    Game *game = malloc(sizeof( *game));
    srand((unsigned)time(NULL));

    gameSetup(game);
    assignXO(game);
    display(game);
    startGame(game);

    free(game);
}

void assignXO(Game *game)
{
    char choice;
    int errorCheck;

    // Asks user for a valid input.
    do
    {
        printf("X or O ?\n");
        errorCheck = scanf(" %c", &choice);

    }while((choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o') || errorCheck != 1);

    // assigns X or O to the player.
    if(choice == 'x' || choice == 'X')
    {
        game->user = 'X';
        game->comp = 'O';
    }

    else
    {
        game->user = 'O';
        game->comp = 'X';
    }

    game->userTurn = true;
    game->compTurn = false;
}

// displays the game.
void display(Game *game)
{
    int i, j;

    system("clear");
    printf("\n\t------------------\n");

    for( i = 0; i < 3; i++)
    {
        for( j = 0; j < 3; j++)
        {
            if(game->board[i][j] == 'X')
                printf(ANSI_COLOR_RED "\tX" ANSI_COLOR_RESET);

            else if(game->board[i][j] == 'O')
                printf(ANSI_COLOR_GREEN "\tO" ANSI_COLOR_RESET);

            else
                printf("\t%c", game->board[i][j]);
        }
        printf("\n\t------------------\n");
    }

    printf("\nYou - %c\t\t", game->user);
    printf("Computer - %c\n", game->comp);
}

void startGame(Game *game)
{
    while(!victoryCheck(game) && !isComplete(game))
    {
        if(game->userTurn)
            userMove(game);
        else
            compMove(game);

        if(victoryCheck(game))
        {
            if(game->userTurn)
                printf("You Win!\n");
            else
                printf("Computer Wins!\n");

            break;
        }

        if(isComplete(game))
        {
            printf("Game Over!\nNo more possible moves\n");
            break;
        }


        game->userTurn = !game->userTurn;
        game->compTurn = !game->compTurn;
    }
}

void userMove(Game *game)
{
    int i, j;
    int pos;
    int errorCheck;

    do
    {
        printf("Your turn : ");
        errorCheck = scanf("%d", &pos);

    }while((pos > 9 || pos < 1) || !game->validPos[pos] || errorCheck != 1);

    game->validPos[pos] = false;

    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game->board[i][j] - '0' == pos)
                game->board[i][j] = game->user;

    display(game);
}
// checks if the game is completed when there are 3 consecutive X or O
// along a row, coloumn or either of the diagonals.
bool victoryCheck(Game *game)
{
    int i, j;

    // checks along a row.
    for(i = 0, j = 0; i < 3; i++)
        if(game->board[i][0] == game->board[i][1] && game->board[i][j] == game->board[i][2])
            return true;

    // checks along a coloumn.
    for(j = 0, i = 0; j < 3; j++)
        if(game->board[0][j] == game->board[1][j] && game->board[0][j] == game->board[2][j])
            return true;

    // checks along right diagonal.
    if(game->board[0][0] == game->board[1][1] && game->board[0][0] == game->board[2][2])
            return true;

    // checks along left diagonal.
    if(game->board[0][2] == game->board[1][1] && game->board[0][2] == game->board[2][0])
            return true;

    return false;
}

void compMove(Game *game)
{
    int pos;
    int i, j;

    do
    {
        // calculates a good and valid move for the computer.
        pos = rand() / (RAND_MAX/10 + 1);
        printf("pos = %d\n", pos);

    }while((pos > 9 || pos < 1) || !game->validPos[pos]);

    pos = calcMove(game, pos);
    game->validPos[pos] = false;

    for( i = 0; i < 3; i++)
        for( j = 0; j < 3; j++)
            if(game->board[i][j] - '0' == pos)
                game->board[i][j] = game->comp;

    display(game);
}

int calcMove(Game *game, int pos)
{
    int i, j;
    int move = 0;

    // If there is any move along a row that is a winning move for the computer
    // then take it or block the winning move of the user(if any) along a row.
    for(i = 0; i < 3; i++)
    {
        if(game->board[i][0] == game->board[i][1] && isdigit(game->board[i][2]))
        {
            move = game->board[i][2] - '0';
            if(game->board[i][0] == game->comp)
                return move;
        }

        if(game->board[i][0] == game->board[i][2] && isdigit(game->board[i][1]))
        {
            move = game->board[i][1] - '0';
            if(game->board[i][0] == game->comp)
                return move;
        }

        if(game->board[i][1] == game->board[i][2] && isdigit(game->board[i][0]))
        {
            move = game->board[i][0] - '0';
            if(game->board[i][1] == game->comp)
                return move;
        }
    }

    // If there is any move along a coloumn that is a winning move for the computer
    // then take it or block the winning move of the user(if any) along a coloumn.
    for(j = 0; j < 3; j++)
    {
        if(game->board[0][j] == game->board[1][j] && isdigit(game->board[2][j]))
        {
            move = game->board[2][j] - '0';
            if(game->board[0][j] == game->comp)
                return move;
        }

        if(game->board[1][j] == game->board[2][j] && isdigit(game->board[0][j]))
        {
            move = game->board[0][j] - '0';
            if(game->board[1][j] == game->comp)
                return move;
        }

        if(game->board[0][j] == game->board[2][j] && isdigit(game->board[1][j]))
        {
            move = game->board[1][j] - '0';
            if(game->board[0][j] == game->comp)
                return move;
        }
    }

    // checks for a winning move along the right diagonal.
    // preferance is given to winning move for computer.
    if(game->board[0][0] == game->board[1][1] && isdigit(game->board[2][2]))
    {
        move = game->board[2][2] - '0';
        if(game->board[0][0] == game->comp)
            return move;
    }

    if(game->board[0][0] == game->board[2][2] && isdigit(game->board[1][1]))
    {
        move = game->board[1][1] - '0';
        if(game->board[0][0] == game->comp)
            return move;
    }

    if(game->board[1][1] == game->board[2][2] && isdigit(game->board[0][0]))
    {
        move = game->board[0][0] - '0';
        if(game->board[1][1] == game->comp)
            return move;
    }

    // checks for a winning move along the left diagonal.
    // preferance is given to winning move for computer.
    if(game->board[0][2] == game->board[1][1] && isdigit(game->board[2][0]))
    {
        move = game->board[2][0] - '0';
        if(game->board[0][2] == game->comp)
            return move;
    }

    if(game->board[0][2] == game->board[2][0] && isdigit(game->board[1][1]))
    {
        move = game->board[1][1] - '0';
        if(game->board[0][2] == game->comp)
            return move;
    }

    if(game->board[1][1] == game->board[2][0] && isdigit(game->board[0][2]))
    {
        move = game->board[0][2] - '0';
        if(game->board[1][1] == game->comp)
            return move;
    }

    // if there was any such move in which either the player or
    // the computer would win, then return it.
    if(move != 0)
        return move;

    // if there was no such move, then select either or the corners
    // or the center randomly.
    switch(rand() / (RAND_MAX/5))
    {

        case 1 : if(game->validPos[7] && game->validPos[3])
                    return 7;

        case 2 : if(game->validPos[3] && game->validPos[7])
                    return 3;

        case 3 : if(game->validPos[1] && game->validPos[9])
                    return 1;

        case 4 : if(game->validPos[5])
                    return 5;

        default : return pos;
    }
}

bool isComplete(Game *game)
{
    int i;

    for(i = 1; i < 10; i++)
    {
        if(game->validPos[i])
            return false;
    }

    return true;
}

// reset the valid positions and the board.
void gameSetup(Game *game)
{
    int i, j;
    char k = '1';

    for(i = 1; i < 10; i++)
        game->validPos[i] = true;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            game->board[i][j] = k;
            k++;
        }
    }
}
