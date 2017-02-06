#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define GAMES 3
#define WIN 2
#define LOSS -1

// Adds colour to the output.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

bool gameStatus(char [][3]);
void display(char [][3]);
void startGame(char [][3]);
void gameReset(char [][3]);
void scoreBoard(void);

struct Player
{
    int score[GAMES];
    char choice;
    char name[10];
    bool turn;

}player1 = {{0}, 'O', "xyz", false}, player2 = {{0}, 'X', "abc", false};

// keeps track of all the valid positions left in the game.
int games = 0;

bool validPos[10] = {false, true, true, true, true, true, true, true, true, true};

int main(void)
{
    // A two-dimensional array depicting the game.
    char game[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char choice;

    printf("Enter Player 1's name : ");
    scanf("%s", player1.name);
    printf("Enter Player 2's name : ");
    scanf("%s", player2.name);

    printf("\n%s, ", player1.name);
    do
    {
        printf("X or O ?\n");
        scanf(" %c", &player1.choice);

    }while(player1.choice != 'X' && player1.choice != 'O' && player1.choice != 'x' && player1.choice != 'o');

    // assigns X or O to the two players.
    player1.choice = toupper(player1.choice);

    if(player1.choice == 'x' || player1.choice == 'X')
    {
        player1.choice = 'X';
        player2.choice = 'O';
    }

    display(game);

    player1.turn = true;
    player2.turn = false;
    startGame(game);
}

// displays the game's current status.
void display(char game[][3])
{
    char choice;
    int i, j;

    // use system("cls"); if using windows.
    system("clear");

    printf("\n\t------------------\n");
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
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

void startGame(char game[3][3])
{
    int i, j;
    char pos;
    char choice;
    printf("\n%s - %c\t", player1.name, player1.choice);
    printf("%s - %c\n", player2.name, player2.choice);

    // if it is player 1's turn, check for valid input.
    if(player1.turn)
    {
        do
        {
            printf("%s's turn : ", player1.name);
            scanf(" %c", &pos);

        }while(!isdigit(pos) || !validPos[pos - '0']);
    }

    // if it is player 2's turn, check for valid input.
    if(player2.turn)
    {
        do
        {
            printf("%s's turn : ", player2.name);
            scanf(" %c", &pos);

        }while(!isdigit(pos) || !validPos[pos - '0']);

    }
    // update the valid positions availabe.
    validPos[pos - '0'] = false;

    // Assigns either X or O to the spot chosen.
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(game[i][j] == pos)
            {
                if(player1.turn)
                    game[i][j] = player1.choice;

                else
                    game[i][j] = player2.choice;
            }
        }
    }

    display(game);

    // checks if the any has won the game or not also checks if there are any
    // further possible moves.
    // also acts as the base case for the game to end.
    if(gameStatus(game))
    {
        if(player1.turn)
        {
            printf("\n%s WINS !\n", player1.name);
            player1.score[games] = WIN;
            player2.score[games] = LOSS;
        }

        else
        {
            printf("\n%s WINS !\n", player2.name);
            player2.score[games] = WIN;
            player1.score[games] = LOSS;
        }

        scoreBoard();

        printf("Play again ? (Y/N)\n");
        scanf(" %c", &choice);

        if(choice == 'y' || choice == 'Y')
            gameReset(game);

        return;
    }

    // swap the turns
    player1.turn = !player1.turn;
    player2.turn = !player2.turn;

    // recursive call to continue the game.
    startGame(game);
}

// checks if the game is completed when there are 3 consecutive X or O
// along a row, coloumn or either of the diagonals.
bool gameStatus(char game[][3])
{
    char choice;
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
        scoreBoard();

        printf("Game Over!\nNo more possible moves\n");
        printf("Play again ? (Y/N)\n");
        scanf(" %c", &choice);

        if(choice == 'y' || choice == 'Y')
            gameReset(game);

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

void gameReset(char game[][3])
{
    int i, j;
    char k = '1';

    for(i = 1; i < 10; i++)
        validPos[i] = true;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            game[i][j] = k;
            k++;
        }
    }

    display(game);

    player1.turn = !player1.turn;
    player2.turn = !player2.turn;

    startGame(game);
}

void scoreBoard(void)
{
    games++;

    int i;
    int totalScore1 = 0;
    int totalScore2 = 0;

    printf("\n================\n");
    printf("  LEADERBOARD\n");
    printf("================\n");

    for(i = 0; i < GAMES; i++)
    {
        totalScore1 += player1.score[i];
        totalScore2 += player2.score[i];

    }

    printf("%s[%d]  %s[%d]\n\n", player1.name, totalScore1, player2.name, totalScore2);

    for(i = 0; i < games; i++)
    {
        if(player1.score[i] == WIN)
            printf(ANSI_COLOR_GREEN "  %d", WIN);

        else
            printf(ANSI_COLOR_RED "  %d", LOSS);

        printf(ANSI_COLOR_RESET "\t");

        if(player2.score[i] == WIN)
            printf(ANSI_COLOR_GREEN "   %d", WIN);


        else
            printf(ANSI_COLOR_RED "   %d", LOSS);

        printf(ANSI_COLOR_RESET "\n");
    }

    printf("\n");
    if(games + 1 > GAMES)
    {
        if(totalScore1 > totalScore2)
            printf("%s WINS THE SERIES !\n", player1.name);

        else if(totalScore2 > totalScore1)
            printf("%s WINS THE SERIES !\n", player2.name);

        else
            printf("NO RESULT!\n");

        exit(0);
    }
}
