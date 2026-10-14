#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_SCORE 21

void init_ncurses(void);

void end_ncurses(void);

void render_ncurses(int p1_y, int p2_y, int ball_x, int ball_y, int score1, int score2, int game_over,
                    int winner);

int handle_input_ncurses(void);

int main(void) {
    init_ncurses();

    int paddle1_x = 2, paddle1_y = 11;
    int paddle2_x = 77, paddle2_y = 11;
    int ball_x = 40, ball_y = 12;
    int dx = -1, dy = 0;
    int score1 = 0, score2 = 0;
    int game_over = 0;
    int winner = 0;

    render_ncurses(paddle1_y, paddle2_y, ball_x, ball_y, score1, score2, game_over, winner);

    while (!game_over) {
        int action = handle_input_ncurses();
        if (action == -1) {
            break;
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

        render_ncurses(paddle1_y, paddle2_y, ball_x, ball_y, score1, score2, game_over, winner);

        napms(50);
    }

    if (!game_over) {
        clear();
        mvprintw(HEIGHT / 2, (WIDTH - 20) / 2, "Game ended early.");
        refresh();
        napms(1500);
    } else {
        napms(2000);
    }

    end_ncurses();

    if (game_over) {
        printf("Winner: Player %d! Congratulations!\n", winner);
    } else {
        printf("The game ended early.\n");
    }

    return 0;
}

void init_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    clear();
}

void end_ncurses(void) { endwin(); }

void render_ncurses(int p1_y, int p2_y, int ball_x, int ball_y, int score1, int score2, int game_over,
                    int winner) {
    clear();

    for (int i = 0; i < WIDTH; i++) {
        mvaddch(0, i, '#');
    }

    for (int y = 0; y < HEIGHT - 2; y++) {
        mvaddch(y + 1, 0, '#');
        for (int x = 0; x < WIDTH - 2; x++) {
            char ch = ' ';
            if (x == 1 && y >= p1_y - 1 && y <= p1_y + 1)
                ch = '|';
            else if (x == 76 && y >= p2_y - 1 && y <= p2_y + 1)
                ch = '|';
            else if (x == ball_x - 1 && y == ball_y - 1)
                ch = 'o';
            mvaddch(y + 1, x + 1, ch);
        }
        mvaddch(y + 1, WIDTH - 1, '#');
    }

    for (int i = 0; i < WIDTH; i++) {
        mvaddch(HEIGHT - 1, i, '#');
    }

    mvprintw(HEIGHT, 0, "Player1: %d   Player2: %d", score1, score2);
    mvprintw(HEIGHT + 1, 0, "Controls: A/Z (P1), K/M (P2), Q - Exit");

    if (game_over) {
        mvprintw(HEIGHT + 2, 0, "Winner: Player %d! Congratulations!", winner);
    }

    refresh();
}

int handle_input_ncurses(void) {
    int ch = getch();
    if (ch == ERR) return 0;
    if (ch >= 'A' && ch <= 'Z') ch += 32;
    if (ch == 'q') return -1;
    if (ch == 'a' || ch == 'z' || ch == 'k' || ch == 'm' || ch == ' ') return ch;
    return 0;
}