#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_SCORE 21

void render(int paddle1_x, int paddle1_y, int paddle2_x, int paddle2_y, int ball_x, int ball_y, int score1,
            int score2);

int handle_input(void);

int main(void) {
    int paddle1_x = 2, paddle1_y = 11;
    int paddle2_x = 77, paddle2_y = 11;
    int ball_x = 40, ball_y = 12;
    int dx = -1, dy = 0;
    int score1 = 0, score2 = 0;
    int game_over = 0;
    int winner = 0;

    while (!game_over) {
        render(paddle1_x, paddle1_y, paddle2_x, paddle2_y, ball_x, ball_y, score1, score2);

        int action = handle_input();
        if (action == -1) {
            printf("\nThe game ended early.\n");
            return 0;
        } else if (action == 0) {
            continue;
        }

        switch (action) {
            case 'a':
                if (paddle1_y > 1) paddle1_y--;
                break;
            case 'z':
                if (paddle1_y < HEIGHT - 4) paddle1_y++;
                break;
            case 'k':
                if (paddle2_y > 1) paddle2_y--;
                break;
            case 'm':
                if (paddle2_y < HEIGHT - 4) paddle2_y++;
                break;
            default:
                break;
        }

        ball_x += dx;
        ball_y += dy;

        if (ball_y - 1 < 0) {
            ball_y = 1;
            dy = 1;
        }
        if (ball_y - 1 > HEIGHT - 2) {
            ball_y = HEIGHT - 2;
            dy = -1;
        }

        if (ball_x - 1 == paddle1_x - 1) {
            if (ball_y - 1 >= paddle1_y - 1 && ball_y - 1 <= paddle1_y + 1) {
                dx = -dx;
                int hit = (ball_y - 1) - (paddle1_y - 1);
                if (hit == 0)
                    dy = -1;
                else if (hit == 1)
                    dy = 0;
                else if (hit == 2)
                    dy = 1;
                ball_x = paddle1_x + 1;
            }
        }
        if (ball_x - 1 == paddle2_x - 1) {
            if (ball_y - 1 >= paddle2_y - 1 && ball_y - 1 <= paddle2_y + 1) {
                dx = -dx;
                int hit = (ball_y - 1) - (paddle2_y - 1);
                if (hit == 0)
                    dy = -1;
                else if (hit == 1)
                    dy = 0;
                else if (hit == 2)
                    dy = 1;
                ball_x = paddle2_x - 1;
            }
        }

        if (ball_x - 1 < 0) {
            score2++;
            ball_x = 40;
            ball_y = 12;
            dx = 1;
            dy = 0;
        }
        if (ball_x - 1 > WIDTH - 3) {
            score1++;
            ball_x = 40;
            ball_y = 12;
            dx = -1;
            dy = 0;
        }

        if (score1 >= MAX_SCORE) {
            game_over = 1;
            winner = 1;
        }
        if (score2 >= MAX_SCORE) {
            game_over = 1;
            winner = 2;
        }
    }

    render(paddle1_x, paddle1_y, paddle2_x, paddle2_y, ball_x, ball_y, score1, score2);
    printf("\nWinner: Player %d! Congratulations!\n", winner);

    return 0;
}

void render(int paddle1_x, int paddle1_y, int paddle2_x, int paddle2_y, int ball_x, int ball_y, int score1,
            int score2) {
    printf("\033[2J\033[H");

    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    for (int y = 0; y < HEIGHT - 2; y++) {
        printf("#");
        for (int x = 0; x < WIDTH - 2; x++) {
            char ch = ' ';
            if (x == paddle1_x - 1 && y >= paddle1_y - 1 && y <= paddle1_y + 1)
                ch = '|';
            else if (x == paddle2_x - 1 && y >= paddle2_y - 1 && y <= paddle2_y + 1)
                ch = '|';
            else if (x == ball_x - 1 && y == ball_y - 1)
                ch = 'o';
            printf("%c", ch);
        }
        printf("#\n");
    }

    for (int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    printf("Player1: %d   Player2: %d\n", score1, score2);
    printf("Controls: A/Z (Player 1), K/M (Player 2), Q - Exit\n");
    printf("ENTER required\n");
}

int handle_input(void) {
    int ch = getchar();

    while (getchar() != '\n');

    if (ch >= 'A' && ch <= 'Z') ch += 32;

    if (ch == 'q') return -1;

    if (ch == 'a' || ch == 'z' || ch == 'k' || ch == 'm' || ch == ' ')
        return ch;
    else
        return 0;
}