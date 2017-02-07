#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define GAMES 3
#define WIN 2
#define LOSS -1
#define DRAW 0
// Adds colour to the output.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"


typedef struct
{
    int score[GAMES];
    char choice;
    char name[10];
    bool turn;

}Player;

typedef struct
{
    char board[3][3];
    bool validPos[10];
    int games;

}Game;

void assignXO(Player *, Player *);
void startGame(Game *, Player *, Player *);
void display(Game *, Player *, Player *);
bool victoryCheck(Game *);
bool isComplete(Game *);
void playerMove(Game *, Player *);
void declareWinner(Game *, Player *, Player *);
void gameReset(Game *);
void scoreBoard(Player *, Player *, int);

int main(void)
{
    char choice;
    Game *game = malloc(sizeof(Game));
    Player *player_1 = malloc(sizeof(Player));
    Player *player_2 = malloc(sizeof(Player));

    gameReset(game);
    game->games = 0;

    assignXO(player_1, player_2);
    startGame(game, player_1, player_2);
    scoreBoard(player_1, player_2, game->games);

    while(game->games < GAMES)
    {
        printf("Play again ? (Y/N)\n");
        if(scanf(" %c", &choice) == 1);

        if(choice == 'y' || choice == 'Y')
        {
            gameReset(game);
            startGame(game, player_1, player_2);
            scoreBoard(player_1, player_2, game->games);
        }

        else
            break;
    }

    free(game);
    free(player_1);
    free(player_2);
}

void assignXO(Player *player_1, Player *player_2)
{
    char choice;
    int errorCheck;

    do
    {
        printf("Enter Player 1's name : ");
        errorCheck = scanf("%s", player_1->name);

    } while(errorCheck != 1);

    do
    {
        printf("Enter Player 1's name : ");
        errorCheck = scanf("%s", player_2->name);

    } while(errorCheck != 1);

    printf("\n%s, ", player_1->name);

    do
    {
        printf("X or O ?\n");

        errorCheck = scanf(" %c", &choice);

    }while((choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o') || errorCheck != 1);

    // assigns X or O to the two players.
    player_1->choice = toupper(choice);

    if(player_1->choice == 'X')
    {
        player_1->choice = 'X';
        player_2->choice = 'O';
    }
    
    else
    {
        player_2->choice = 'X';
        player_1->choice = 'O';
    }

    player_1->turn = true;
    player_2->turn = false;
}

void startGame(Game *game, Player *player_1, Player *player_2)
{
    display(game, player_1, player_2);

    while(!victoryCheck(game) && !isComplete(game))
    {
        if(player_1->turn)
            playerMove(game, player_1);

        else
            playerMove(game, player_2);

        player_1->turn = !player_1->turn;
        player_2->turn = !player_2->turn;

        display(game, player_1, player_2);
    }

    declareWinner(game, player_1, player_2);
}

void playerMove(Game *game, Player *player)
{
    int i, j;
    int errorCheck;
    char pos;

    do
    {
        printf("%s's turn : ", player->name);
        errorCheck = scanf(" %c", &pos);

    }while(!isdigit(pos) || !game->validPos[pos - '0'] || errorCheck != 1);

    game->validPos[pos - '0'] = false;

    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(game->board[i][j] == pos)
                    game->board[i][j] = player->choice;
}

// checks if the game is completed when there are 3 consecutive X or O
// along a row, coloumn or either of the diagonals.
bool victoryCheck(Game *game)
{
    int i, j;

    // checks along a row.
    for(i = 0, j = 0; i < 3; i++)
        if(game->board[i][j] == game->board[i][j + 1] && game->board[i][j] == game->board[i][j + 2])
            return true;

    // checks along a coloumn.
    for(j = 0, i = 0; j < 3; j++)
        if(game->board[i][j] == game->board[i + 1][j] && game->board[i][j] == game->board[i + 2][j])
            return true;

    // checks along right diagonal.
    if(game->board[0][0] == game->board[1][1] && game->board[0][0] == game->board[2][2])
        return true;

    // checks along left diagonal.
    if(game->board[0][2] == game->board[1][1] && game->board[0][2] == game->board[2][0])
        return true;

    return false;
}

bool isComplete(Game *game)
{
    int i, j;

    for(i = 1; i < 10; i++)
    {
        if(game->validPos[i])
            return false;
    }

    return true;
}

void declareWinner(Game *game, Player *player_1, Player *player_2)
{
    int i, j;

    // checks along a row.
    for(i = 0, j = 0; i < 3; i++)
    {
        if(game->board[i][0] == game->board[i][1] && game->board[i][0] == game->board[i][2])
        {
            if(game->board[i][j] == player_1->choice)
            {
                printf("%s Wins!\n", player_1->name);
                player_1->score[game->games] = WIN;
                player_2->score[game->games] = LOSS;
            }

            else
            {
                printf("%s Wins!\n", player_2->name);
                player_2->score[game->games] = WIN;
                player_1->score[game->games] = LOSS;
            }

            game->games++;
            return;
        }
    }

    // checks along a coloumn.
    for(j = 0, i = 0; j < 3; j++)
    {
        if(game->board[0][j] == game->board[1][j] && game->board[0][j] == game->board[2][j])
        {
            if(game->board[0][j] == player_1->choice)
            {
                printf("%s Wins!\n", player_1->name);
                player_1->score[game->games] = WIN;
                player_2->score[game->games] = LOSS;
            }

            else
            {
                printf("%s Wins!\n", player_2->name);
                player_2->score[game->games] = WIN;
                player_1->score[game->games] = LOSS;
            }

            game->games++;
            return;
        }
    }

    // checks along right diagonal.
    if(game->board[0][0] == game->board[1][1] && game->board[0][0] == game->board[2][2])
    {
        if(game->board[0][0] == player_1->choice)
        {
            printf("%s Wins!\n", player_1->name);
            player_1->score[game->games] = WIN;
            player_2->score[game->games] = LOSS;
        }

        else
        {
            printf("%s Wins!\n", player_2->name);
            player_2->score[game->games] = WIN;
            player_1->score[game->games] = LOSS;
        }
        game->games++;
        return;
    }

    // checks along left diagonal.
    if(game->board[0][2] == game->board[1][1] && game->board[0][2] == game->board[2][0])
    {
        if(game->board[0][2] == player_1->choice)
        {
            printf("%s Wins!\n", player_1->name);
            player_1->score[game->games] = WIN;
            player_2->score[game->games] = LOSS;
        }

        else
        {
            printf("%s Wins!\n", player_2->name);
            player_2->score[game->games] = WIN;
            player_1->score[game->games] = LOSS;
        }
        game->games++;
        return;
    }

    if(isComplete(game))
    {
        printf("Game Over !\n");
        printf("No more possible moves\n");

        player_1->score[game->games] = DRAW;
        player_2->score[game->games] = DRAW;

        game->games++;
        return ;
    }
}

// displays the game's current status.
void display(Game *game, Player *player_1, Player * player_2)
{
    int i, j;

    // use system("cls"); if using windows.
    system("clear");

    printf("\n\t------------------\n");
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
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
    printf("\n%s - %c\t", player_1->name, player_1->choice);
    printf("%s - %c\n", player_2->name, player_2->choice);
}

void gameReset(Game *game)
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

void scoreBoard(Player *player_1, Player *player_2, int games)
{
    int i;
    int totalScore1 = 0;
    int totalScore2 = 0;

    printf("\n==================\n");
    printf("   LEADERBOARD\n\n");
    // printf("==================\n");
    printf("   Game (%d / %d)\n", games, GAMES);
    printf("==================\n");

    for(i = 0; i < games; i++)
    {
        totalScore1 += player_1->score[i];
        totalScore2 += player_2->score[i];

    }

    printf("%s[%d]  %s[%d]\n\n", player_1->name, totalScore1, player_2->name, totalScore2);

    for(i = 0; i < games; i++)
    {
        if(player_1->score[i] == WIN)
            printf(ANSI_COLOR_GREEN "  %2d", WIN);

        else
            printf(ANSI_COLOR_RED "  %2d", LOSS);

        printf(ANSI_COLOR_RESET "\t");

        if(player_2->score[i] == WIN)
            printf(ANSI_COLOR_GREEN "   %2d", WIN);


        else
            printf(ANSI_COLOR_RED "   %2d", LOSS);

        printf(ANSI_COLOR_RESET "\n");
    }

    printf("==================\n");
    printf("\n");
    if(games + 1 > GAMES)
    {
        if(totalScore1 > totalScore2)
            printf("%s WINS THE SERIES !\n", player_1->name);

        else if(totalScore2 > totalScore1)
            printf("%s WINS THE SERIES !\n", player_2->name);

        else
            printf("NO RESULT!\n");
    }
}
