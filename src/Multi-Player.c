#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

// points for each result.
enum Result {WIN = 2, LOSS = -1, DRAW = 0, GAMES = 3};

// Adds colour to the output.
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

// replace "clear" by "cls" on windows.
#define CLEAR "clear"

bool useColour = 1;

typedef struct
{
    int score[GAMES];
    char choice;
    char name[15];
    bool turn;

}Player;

typedef struct
{
    char board[3][3];
    bool validPos[10];
    int rounds;

}Game;

void assignXO(Player *player_1, Player *player_2);
void startGame(Game *game, Player *player_1, Player *player_2);
void display(Game *game, Player *player_1, Player *player_2);
bool victoryCheck(Game *game);
bool isComplete(Game *game);
void move(Game *game, Player *player);
void reset(Game *game);
void scoreBoard(Player *player_1, Player *player_2, int rounds);
void loadingScreen(void);

int main(void)
{
    char choice;
    Game *game = malloc(sizeof( *game));
    Player *player_1 = malloc(sizeof( *player_1));
    Player *player_2 = malloc(sizeof( *player_2));

    // sets the number of games played to zero.
    game->rounds = 0;

    printf("Do you want to use colour in the output? (y/n): ");
    if((choice = getchar()) == 'n' || choice == 'N')
        useColour = false;

    loadingScreen();
    assignXO(player_1, player_2);
    startGame(game, player_1, player_2);

    // asks user if he/she wants to play another game
    // until the maximum game count is reached. This act as a series of game
    // so that final winner is decided based on the number of matche won out of the
    // number of  GAMES played.
    while(game->rounds < GAMES)
    {
        printf("Play again ? (Y/N)\n");
        if(scanf(" %c", &choice) == 1);

        // if yes then start the game again with a fresh board.
        if(choice == 'y' || choice == 'Y')
        {
            player_1->turn = !player_1->turn;
            player_2->turn = !player_2->turn;
            startGame(game, player_1, player_2);
        }

        else
            break;
    }

    free(game);
    free(player_1);
    free(player_2);
}

// assigns either 'X' or 'O' to the players.
void assignXO(Player *player_1, Player *player_2)
{
    char choice;
    int errorCheck;

    // checks for invalid inputs.
    do
    {
        printf("Enter Player 1's name : ");
        errorCheck = scanf("%s", player_1->name);
    } while(errorCheck != 1);

    // checks for invalid inputs.
    do
    {
        printf("Enter Player 2's name : ");
        errorCheck = scanf("%s", player_2->name);
    } while(errorCheck != 1);


    printf("\n%s, ", player_1->name);
    // checks for invalid inputs.
    do
    {
        printf("X or O ?\n");
        errorCheck = scanf(" %c", &choice);
    }while((choice != 'X' && choice != 'O' && choice != 'x' && choice != 'o') || errorCheck != 1);

    player_1->choice = toupper(choice);

    // assign 'X' to player_1
    if(player_1->choice == 'X')
    {
        player_1->choice = 'X';
        player_2->choice = 'O';
    }

    // assign 'O' to player_1
    else
    {
        player_1->choice = 'O';
        player_2->choice = 'X';
    }

    // first player starts the game.
    player_1->turn = true;
    player_2->turn = false;
}

// the main game loop where the moves are made and winner is decided.
void startGame(Game *game, Player *player_1, Player *player_2)
{
    // resets the game board and displays it.
    reset(game);
    display(game, player_1, player_2);

    // continue the game till either player wins or no more moves are possible.
    while(true)
    {
        // players make a move depending on whose turn it is.

        if(player_1->turn)
            move(game, player_1);
        else
            move(game, player_2);

        display(game, player_1, player_2);

        // check if the move played resulted in a win.
        // declare winner according to the previous turn.
        if(victoryCheck(game))
        {
            if(player_1->turn)
            {
                printf("\n%s Wins!!!", player_1->name);
                player_1->score[game->rounds] = WIN;
                player_2->score[game->rounds] = LOSS;
            }

            else
            {
                printf("\n%s Wins!!!", player_2->name);
                player_1->score[game->rounds] = LOSS;
                player_2->score[game->rounds] = WIN;
            }

            break;
        }

        // check if there are any further possible moves.
        if(isComplete(game))
        {
            printf("Game Over!\nNo more possible moves\n");
            player_1->score[game->rounds] = DRAW;
            player_2->score[game->rounds] = DRAW;
            break;
        }

        // swap the turns.
        player_1->turn = !player_1->turn;
        player_2->turn = !player_2->turn;
    }

    // display the scoreboard after every game.
    game->rounds++;
    scoreBoard(player_1, player_2, game->rounds);
}

// stored and checks the move of each player.
void move(Game *game, Player *player)
{
    int i, j;
    int errorCheck;
    char pos;

    // checks for a valid input.
    do
    {
        printf("%s's turn : ", player->name);
        errorCheck = scanf(" %c", &pos);
    }while(!isdigit(pos) || !game->validPos[pos - '0'] || errorCheck != 1);

    game->validPos[pos - '0'] = false;

    // if input is valid, then store it in the board.
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

// checks if there are any futher possible moves.
bool isComplete(Game *game)
{
    int i;

    for(i = 1; i < 10; i++)
        if(game->validPos[i])
            return false;

    return true;
}

// displays the game's current status.
void display(Game *game, Player *player_1, Player * player_2)
{
    int i, j;

    system(CLEAR);
    printf("\n");

    for(i = 0; i < 3; i++)
    {
        printf("\t");
        for(j = 0; j < 3; j++)
        {
            if(game->board[i][j] == 'X')
            {
                if(useColour)
                    printf(RED);

                printf(" X ");
            }

            else if(game->board[i][j] == 'O')
            {
                if(useColour)
                    printf(GREEN);

                printf(" O ");
            }

            else
            {
                if(useColour)
                    printf(RESET);
                printf(" %c ", game->board[i][j]);
            }

            if(useColour)
                printf(RESET);

            if(j != 2)
                printf("|");
        }

        if(i != 2)
            printf("\n\t------------\n");
    }

    printf("\n\n%s - %c\t", player_1->name, player_1->choice);
    printf("%s - %c\n", player_2->name, player_2->choice);
}

// reset the valid positions and the board.
void reset(Game *game)
{
    int i, j;
    char k = '1';

    for(i = 1; i < 10; i++)
        game->validPos[i] = true;

    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            game->board[i][j] = k++;
}

// display and calculate the score board.
void scoreBoard(Player *player_1, Player *player_2, int rounds)
{
    int i;
    int totalScore1 = 0;
    int totalScore2 = 0;

    printf("\n==================\n");
    printf("   LEADERBOARD\n\n");
    printf("   Game (%d / %d)\n", rounds, GAMES);
    printf("==================\n");

    // caculate the score for each player.
    for(i = 0; i < rounds; i++)
    {
        totalScore1 += player_1->score[i];
        totalScore2 += player_2->score[i];

    }

    printf("%s[%d]  %s[%d]\n\n", player_1->name, totalScore1, player_2->name, totalScore2);

    // display the score on the screen with colour.
    for(i = 0; i < rounds; i++)
    {
        if(player_1->score[i] == WIN)
        {
            if(useColour)
                printf(GREEN);
            printf("  %2d", WIN);

            if(useColour)
                printf(RESET);
            printf("\t");

            if(useColour)
                printf(RED);
            printf("  %2d", LOSS);
        }

        else if (player_1->score[i] == LOSS)
        {
            if(useColour)
                printf(RED);
            printf("  %2d", LOSS);

            if(useColour)
                printf(RESET);
            printf("\t");

            if(useColour)
                printf(GREEN);
            printf("  %2d", WIN);
        }

        else
            printf("  %2d\t  %2d", DRAW, DRAW);

        printf(RESET "\n");
    }

    //  display the series result.
    printf("==================\n");
    printf("\n");
    if(rounds == GAMES)
    {
        if(totalScore1 > totalScore2)
            printf("%s WINS THE SERIES !\n", player_1->name);

        else if(totalScore2 > totalScore1)
            printf("%s WINS THE SERIES !\n", player_2->name);

        else
            printf("NO RESULT!\n");
    }
}

void loadingScreen(void)
{
    int i;
    char load[26];

    for(i = 0; i < 25;)
    {
        system(CLEAR);
        load[i++] = '#';
        load[i] = '\0';

        if(i == 25)
        {
            if(useColour)
                printf(GREEN);
            printf("\n\nLOADING [%-25s]\n", load);

            if(useColour)
                printf(RESET);
            break;
        }

        else
        {
            printf("\n\nLOADING ");
            if(useColour)
                printf(RED);

            printf("[%-25s]", load);

        }

        if(useColour)
            printf(RESET);

        printf("\n");

        usleep(199900);
    }

    sleep(1);
    system(CLEAR);
    printf("Game Loaded!\n");
}
