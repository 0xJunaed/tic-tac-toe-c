#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

// For Regestor :
#define CREDENTIAL_LENGTH 30
// For Game :
#define BOARD_SIZE 3
#define X 'X'
#define O 'O'
// For Progress_bar
#define BAR_LENGTH 30

// Regestor Structures :

typedef struct
{
    int games_played;
    int win;
    int lose;
    int draw;
    int solo;
    int duo;
} GameRecord;
typedef struct
{
    char full_name[CREDENTIAL_LENGTH];
    char user_name[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
    int user_no;
    GameRecord record;
} User;
// Game Structures :
typedef struct
{
    char user_name[CREDENTIAL_LENGTH];
    int player;
    int computer;
    int draw;
} score_solo;
typedef struct
{
    char username_X[CREDENTIAL_LENGTH];
    char username_O[CREDENTIAL_LENGTH];
    int player_O;
    int player_X;
    int draw;
} score_duo;
// Progress_bar Structures :

typedef struct
{
    int progress;
    int step;
} Task;

// Regestor Function :
void print_welcame_banner();
void print_main_menu();
void user_register();
int user_login();
void input_password_masking(char *password);
void fix_fgets(char *string);
void clear_input_buffer();
void screen_clear();
void save_game_credential();
void login_menu();
void view_status();

// Game Function :
void tic_teo_game();
void play_game();
void input_difficulty_details();
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int play_turn(char board[BOARD_SIZE][BOARD_SIZE], char player, int is_computer);
void player_move(char board[BOARD_SIZE][BOARD_SIZE], char player);
void computer_move(char board[BOARD_SIZE][BOARD_SIZE]);
int check_win(char board[BOARD_SIZE][BOARD_SIZE], char player);
int check_draw(char board[BOARD_SIZE][BOARD_SIZE]);
int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
void input_guest_user_name();

// Progress_bar Function :
void show_progress();
void progress_bar(Task task);
void wait_for_click();

// --------- Global Variables ---------
score_solo solo_score = {.player = 0, .computer = 0, .draw = 0};
score_duo duo_score = {.player_O = 0, .player_X = 0, .draw = 0};
int stop = 1;
int login;
char temp_username[CREDENTIAL_LENGTH] = {0};
char temp_password[CREDENTIAL_LENGTH] = {0};

int level_choice;
int game_mode;
char username_X[CREDENTIAL_LENGTH] = {0}; // for guest
char username_O[CREDENTIAL_LENGTH] = {0}; // for guest
const char *USERINFO = "Tic_Teo_User-info.dat";

// --------------- Code Start ---------------

int main()
{

    while (stop)
    {
        screen_clear();
        print_welcame_banner();
        print_main_menu();
    }

    return 0;
}
void print_main_menu()
{
    int option;
    printf("---- Main Menu ----\n");
    printf("1. Regester \n");
    printf("2. Login \n");
    printf("3. Play as Guest \n");
    printf("4. Exit \n");
    printf("\nSelect Your Choice : ");
    scanf("%d", &option);
    clear_input_buffer();
    login = 0;
    switch (option)
    {
    case 1:
        user_register();
        break;
    case 2:
        if (user_login())
        {
            login_menu();
        }
        else
        {
            printf("\nSomething Goes Wrong!! Please Try Again ...\n");
            wait_for_click();
        }
        break;
    case 3:
        tic_teo_game();
        break;
    case 4:
        printf("Exiting Program. Thakns For Playing -- 6XDot. \n");
        stop = 0;
        break;

    default:
        printf("Invalid Option, Try again..\n");
        wait_for_click();
        break;
    }
}

void user_register()
{
    char temp_fullname[CREDENTIAL_LENGTH];
    char temp_username[CREDENTIAL_LENGTH];
    char temp_password[CREDENTIAL_LENGTH];
    User user;

    FILE *file = fopen(USERINFO, "rb+");
    if (file == NULL)
    {
        file = fopen(USERINFO, "wb+");
        if (file == NULL)
        {
            printf("Unable to open the User-information file :( !! \n");
            return;
        }
    }

    printf("Regester New User...\n");
    printf("Enter Your Full Name : ");
    fgets(temp_fullname, CREDENTIAL_LENGTH, stdin);
    fix_fgets(temp_fullname);
    printf("Enter User-Name : ");
    fgets(temp_username, CREDENTIAL_LENGTH, stdin);
    fix_fgets(temp_username);

    rewind(file);
    while (fread(&user, sizeof(user), 1, file))
    {
        if (strcmp(user.user_name, temp_username) == 0)
        {
            printf("Username Already Exit! Please use differnt Username...\n");
            fclose(file);
            wait_for_click();
            return;
        }
    }
    printf("Enter Your Password : ");
    input_password_masking(temp_password);

    memset(&user, 0, sizeof(user)); // Clears any garbage/random values that might be in memory
    strcpy(user.full_name, temp_fullname);
    strcpy(user.user_name, temp_username);
    strcpy(user.password, temp_password);

    fseek(file, 0, SEEK_END); // Ensure the curser at end (safety)
    fwrite(&user, sizeof(user), 1, file);
    fclose(file);

    show_progress(); // show progress bar
    printf("\nRegestration Is Successful ...\n");
    wait_for_click();
}
int user_login()
{
    int found = 0;
    User user;

    FILE *file = fopen(USERINFO, "rb");
    if (file == NULL)
    {
        printf("Unable to open the user-information file :( !!\n");
        return 0;
    }
    printf("User Loggin Section ...\n");

    printf("Enter Your User-Name : ");
    fgets(temp_username, CREDENTIAL_LENGTH, stdin);
    fix_fgets(temp_username);

    printf("Enter Your Password : ");
    input_password_masking(temp_password);

    show_progress();

    rewind(file); // move the file curser at the start point
    while (fread(&user, sizeof(user), 1, file))
    {
        if ((strcmp(user.user_name, temp_username) == 0 && (strcmp(user.password, temp_password)) == 0))
        {
            found = 1;
            break;
        }
    }

    fclose(file);
    if (found)
    {
        printf("\nLogin Successful, Welcome %s !! \n", user.full_name);
        wait_for_click();
        login = 1;
        return 1;
    }
    else
    {
        printf("Login Failed, Incorrect User Name or Password!! \n");
        login = 0;
    }
    return 0;
}
void input_password_masking(char *password)
{
    char ch;
    int index = 0;
    memset(password, 0, CREDENTIAL_LENGTH);
    while (1)
    {
        ch = getch();
        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (index > 0)
            {
                index--;
                printf("\b \b");
            }
        }
        else if (index < CREDENTIAL_LENGTH - 1)
        {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
}
void login_menu()
{
    int stop = 1;
    while (stop)
    {
        screen_clear();
        printf("---- Login Menu ----\n");
        printf("1. Play Game \n");
        printf("2. View My Stats \n");
        printf("3. Exit \n");
        int choice = 0;
        printf("Enter Your Choice : ");
        scanf("%d", &choice);
        clear_input_buffer();
        switch (choice)
        {
        case 1:
            tic_teo_game();
            break;
        case 2:
            view_status();
            break;
        case 3:
            printf("Exiting Login Section . Thakns For Playing -- 6XDot. \n");
            stop = 0;
            wait_for_click();
            break;

        default:
            printf("Invalid Option, Try again..\n");
            wait_for_click();
            break;
        }
    }
}
void view_status()
{
    int found = 0;
    FILE *file = fopen(USERINFO, "rb+");
    if (file == NULL)
    {
        printf("\n");
        fclose(file);
        return;
    }
    User user;
    rewind(file);

    while (fread(&user, sizeof(user), 1, file))
    {
        if ((strcmp(user.user_name, temp_username) == 0 && (strcmp(user.password, temp_password)) == 0))
        {
            found = 1;
            break;
        }
    }


    fclose(file);
    if (found)
    {
        printf("Player Name : %s | Win : %d | Lose : %d | Draw : %d | Solo : %d | Dua : %d \n", user.user_name, user.record.win, user.record.lose,
               user.record.draw, user.record.solo, user.record.duo);
        wait_for_click();
    }
    else
    {
        printf("Sorry There is No Information About Your Accounts !! \n");
        wait_for_click();
    }
    



}

// ------------------ Game code -----------------

void tic_teo_game()
{
    printf("\nWant To Play Solo ? Or With Frineds ?\n");

    do
    {
        printf("Solo For 1 || Friends For 2 : ");
        scanf("%d", &game_mode);
        clear_input_buffer();
    } while (game_mode != 1 && game_mode != 2);

    input_guest_user_name();

    int choice;
    while (1)
    {
        play_game();
        if (login == 1)
        {
            save_game_credential();
        }
        printf("\nDo You Want To Play Again ? ( Yes For 1 || No For 0  ) : ");
        scanf("%d", &choice);
        clear_input_buffer();
        if (choice == 0)
        {
            break;
        }
    }
    if (login == 0)
    {
        printf("Thanks For Playing The Game -- 6XDot -- \n");
    }

    return;
}
void play_game()
{
    srand(time(NULL));
    char board[BOARD_SIZE][BOARD_SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    char current_player = rand() % 2 == 0 ? X : O;

    print_board(board);
    while (1)
    {
        if (play_turn(board, current_player, (game_mode == 1 && current_player == O)))
        {
            break;
        }
        current_player = (current_player == X) ? O : X;
    }
}
int play_turn(char board[BOARD_SIZE][BOARD_SIZE], char player, int is_computer)
{
    if (is_computer)
    {
        computer_move(board);
    }
    else
    {
        player_move(board, player);
    }
    if (check_win(board, player))
    {
        if (game_mode == 2)
        {
            if (player == X)
            {
                duo_score.player_X++;
                print_board(board);
                if (login == 1) // login user
                {
                    printf("\nCongratulations Player-%c %s Won The Game :) \n", player, temp_username);
                }
                else // or guest usre
                {
                    printf("\nCongratulations Player-%c %s Won The Game :) \n", player, username_X);
                }
            }
            else
            {
                duo_score.player_O++;
                print_board(board);
                printf("\nCongratulations Player-%c %s Won The Game :) \n", player, username_O);
            }
        }
        else
        {
            if (player == X)
            {
                solo_score.player++;
                print_board(board);
                printf("\nCongratulations You Won The Game :O \n");
            }
            else
            {
                solo_score.computer++;
                print_board(board);
                printf("\nComputer Won The Game :) \n");
            }
        }
        return 1; // Game is over
    }
    if (check_draw(board))
    {
        (game_mode == 2) ? duo_score.draw++ : solo_score.draw++;
        print_board(board);
        printf("\nIt's A Draw :-| !!\n");
        return 1; // Game Over
    }
    print_board(board);
    return 0; // continue game
}
void player_move(char board[BOARD_SIZE][BOARD_SIZE], char move)
{
    int row, col, valid_input;
    do
    {
        printf("\nNow Turn of %c \n", move);
        printf("Enter Your Move (1-3) For %c : ", move);
        valid_input = scanf("%d %d", &row, &col);
        clear_input_buffer();
        if (valid_input != 2)
        {
            printf("Invalid input. Please enter two numbers. \n");
            continue;
        }
        row--;
        col--;
    } while (!is_valid_move(board, row, col));
    board[row][col] = move;
}

void computer_move(char board[BOARD_SIZE][BOARD_SIZE])
{
    // Combine 2 win & block into one stage :
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col] == ' ')
            {
                // check for immediate win :
                board[row][col] = O;
                if (check_win(board, O))
                {
                    return;
                }
                board[row][col] = ' ';

                // check for immediate block :
                board[row][col] = X;
                if (check_win(board, X))
                {
                    board[row][col] = O;
                    return;
                }
                board[row][col] = ' ';
            }
        }
    }

    // for god level
    if (level_choice == 2)
    {
        // check for center :
        if (board[1][1] == ' ')
        {
            board[1][1] = O;
            return;
        }
        // check for the corner :
        int corner[4][2] = {
            {0, 0},
            {0, 2},
            {2, 0},
            {2, 2}};
        for (int i = 0; i < 4; i++)
        {
            if (board[corner[i][0]][corner[i][1]] == ' ')
            {
                board[corner[i][0]][corner[i][1]] = O;
                return;
            }
        }
    }

    // play random move  without center :
    int row, col;
    do
    {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
    } while (
        board[row][col] != ' ' ||                   // Skip if not empty
        (level_choice == 1 && row == 1 && col == 1) // Skip center in normal mode
    );
    board[row][col] = O;
}
int check_win(char board[BOARD_SIZE][BOARD_SIZE], char player)
{
    // check row and colum
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if ((board[i][0] == player) && (board[i][1] == player) && (board[i][2] == player))
        {
            return 1;
        }
        if ((board[0][i] == player) && (board[1][i] == player) && (board[2][i] == player))
        {
            return 1;
        }
    }
    // check diagonals
    if (((board[0][0] == player) && (board[1][1] == player) && (board[2][2] == player)) ||
        ((board[0][2] == player) && (board[1][1] == player) && (board[2][0] == player)))
    {
        return 1;
    }

    return 0;
}
int check_draw(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col] == ' ')
            {
                return 0; // there space still on the baord mean still match isn't finished
            }
        }
    }
    return 1; // match is draw !
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
    screen_clear();
    if (login == 1) // user login check
    {
        if (game_mode != 2)
        {
            // 1 solo mode
            printf("\nScore -  %s-%c : %d, Computer-%c : %d, Draw : %d\n", temp_username, X, solo_score.player, O, solo_score.computer, solo_score.draw);
        }
        else
        {
            // 2 Friends mode
            printf("Score -  %s-%c : %d,  %s-%c : %d, Draw : %d \n", temp_username, X, duo_score.player_X, username_O, O, duo_score.player_O, duo_score.draw);
        }
    }
    else // for guest
    {
        if (game_mode != 2)
        {
            // 1 solo mode
            printf("\nScore -  %s-%c : %d, Computer-%c : %d, Draw : %d\n", username_X, X, solo_score.player, O, solo_score.computer, solo_score.draw);
        }
        else
        {
            // 2 Friends mode
            printf("Score -  %s-%c : %d,  %s-%c : %d, Draw : %d \n", username_X, X, duo_score.player_X, username_O, O, duo_score.player_O, duo_score.draw);
        }
    }

    printf("Tic-Tac-Toe : \n");

    printf("    1   2   3  \n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("  +---+---+---+\n");
        printf("%d | %c | %c | %c |\n", i + 1, board[i][0], board[i][1], board[i][2]);
    }
    printf("  +---+---+---+\n");
}
void save_game_credential()
{
    FILE *file = fopen(USERINFO, "rb+");
    if (file == NULL)
    {
        printf("Unable to open the User-information file. Can't save the user. :( !! \n");
        return;
    }
    User user;
    while (fread(&user, sizeof(user), 1, file))
    {
        if ((strcmp(user.user_name, temp_username)) == 0 && (strcmp(user.password, temp_password)) == 0)
        {
            user.record.games_played++;
            if (game_mode == 1)
            {
                user.record.solo++;
                user.record.win += solo_score.player;
                user.record.lose += solo_score.computer;
                user.record.draw += solo_score.draw;
            }
            else if (game_mode == 2)
            {
                user.record.duo++;
                if (strcmp(user.user_name, username_X) == 0)
                {
                    user.record.win += duo_score.player_X;
                    user.record.lose += duo_score.player_O;
                    user.record.draw += duo_score.draw;
                }
                else if (strcmp(user.user_name, username_O))
                {
                    user.record.win += duo_score.player_O;
                    user.record.lose += duo_score.player_X;
                    user.record.draw += duo_score.draw;
                }
            }
            fseek(file, -sizeof(user), SEEK_CUR);
            fwrite(&user, sizeof(user), 1, file);
            fclose(file);
        }
    }
}
void input_difficulty_details()
{
    while (1)
    {
        printf("\nSelect Difficulty level -- \n");
        printf("1 - normal (Standard) \n");
        printf("2 - God (Impossible To Win)\n");
        printf("Enter Your Choice : ");
        scanf("%d", &level_choice);
        clear_input_buffer();
        if ((level_choice < 1) || (level_choice > 2))
        {
            printf("Please Enter Valid user (1-2)\n");
        }
        else
        {
            break;
        }
    }
}
void input_guest_user_name()
{
    if (login == 1)
    {
        if (game_mode == 2)
        {
            printf("\nPlease enter a user-name : ");
            fgets(username_O, CREDENTIAL_LENGTH, stdin);
            fix_fgets(username_O);
        }
    }
    else
    {
        if (game_mode == 1)
        {
            printf("\nPlease enter a user-name : ");
            fgets(username_X, CREDENTIAL_LENGTH, stdin);
            fix_fgets(username_X);
        }
        else
        {
            printf("\nPlease enter user-name for X : ");
            fgets(username_X, CREDENTIAL_LENGTH, stdin);
            fix_fgets(username_X);
            printf("Please enter user-name for O : ");
            fgets(username_O, CREDENTIAL_LENGTH, stdin);
            fix_fgets(username_O);
        }
    }
}
int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    if (row < 0 || col < 0 || row >= BOARD_SIZE || col >= BOARD_SIZE)
    {
        printf("Position out of bounds! Please enter values between 1-3.\n");
        return 0;
    }
    if (board[row][col] != ' ')
    {
        printf("That position is already taken! Choose another.\n");
        return 0;
    }
    return 1;
}

// Global use :
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
void print_welcame_banner()
{
    printf("\n*********************************************************\n");
    printf("|                                                       |\n");
    printf("|                      TIC TAC TOE                      |\n");
    printf("|                   WITH USER ACCOUNTS                  |\n");
    printf("|                                                       |\n");
    printf("*********************************************************\n");
}
void fix_fgets(char *string)
{
    if (string == NULL)
    {
        return;
    }
    size_t lenth = strcspn(string, "\n");
    string[lenth] = '\0';
}
void screen_clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_progress()
{
    Task task;
    srand(time(NULL));
    task.progress = 0;

    while (task.progress < 100)
    {
        screen_clear();
        task.step = rand() % 18 + 1;
        task.progress += task.step;

        if (task.progress > 100)
        {
            task.progress = 100;
        }

        progress_bar(task);
        sleep(1);
    }
}
void progress_bar(Task task)
{
    int bar_show = (task.progress * BAR_LENGTH) / 100;
    printf("Loading");

    for (int i = 0; i < BAR_LENGTH; i++)
    {
        if (i < bar_show)
        {
            printf(".");
        }
        else
        {
            printf(" ");
        }
    }
    printf("| %d%%\n", task.progress);
}

void wait_for_click()
{
    char ch;
    printf("Press Enter To Continue - ");
    ch = getch();
}
