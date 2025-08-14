#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define DEVICE_PATH "/dev/Temperature_Sensor"

void init_color_pairs() {
    init_pair(1, COLOR_RED, COLOR_BLACK);      // Extreme Hot
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // Sunny
    init_pair(3, COLOR_WHITE, COLOR_BLACK);    // Warm
    init_pair(4, COLOR_CYAN, COLOR_BLACK);     // Cloudy
    init_pair(5, COLOR_GREEN, COLOR_BLACK);    // Cool
    init_pair(6, COLOR_BLUE, COLOR_BLACK);     // Cold
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);  // Freezing
}

int get_color_pair(const char* condition) {
    if (strcmp(condition, "Extreme") == 0 || strstr(condition, "Hot"))
        return 1;
    else if (strcmp(condition, "Sunny") == 0)
        return 2;
    else if (strcmp(condition, "Warm") == 0)
        return 3;
    else if (strcmp(condition, "Cloudy") == 0)
        return 4;
    else if (strcmp(condition, "Cool") == 0)
        return 5;
    else if (strcmp(condition, "Cold") == 0)
        return 6;
    else if (strcmp(condition, "Freezing") == 0)
        return 7;
    else
        return 4;
}

void print_centered(WINDOW *win, int y, const char *text, int color_pair) {
    int width = getmaxx(win);
    int x = (width - strlen(text)) / 2;
    attron(COLOR_PAIR(color_pair) | A_BOLD);
    mvprintw(y, x, "%s", text);
    attroff(COLOR_PAIR(color_pair) | A_BOLD);
}

int main() {
    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    initscr();
    start_color();
    init_color_pairs();
    cbreak();
    noecho();
    curs_set(0);

    while (1) {
        char buffer[256] = {0};
        int temperature;
        char condition[64];

        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, sizeof(buffer));
        sscanf(buffer, "%d %s", &temperature, condition);

        int color = get_color_pair(condition);
        clear();

        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        int start_y = rows / 2 - 4;

        print_centered(stdscr, start_y,     "===============================", color);
        print_centered(stdscr, start_y + 1, "     Temperature Monitor        ", color);
        print_centered(stdscr, start_y + 2, "===============================", color);

        char temp_line[64];
        snprintf(temp_line, sizeof(temp_line), "Temperature: %d°C", temperature);
        print_centered(stdscr, start_y + 4, temp_line, color);

        char cond_line[84];
        snprintf(cond_line, sizeof(cond_line), "Condition  : %s", condition);
        print_centered(stdscr, start_y + 5, cond_line, color);

        print_centered(stdscr, start_y + 7, "===============================", color);
        print_centered(stdscr, start_y + 9, "Press Ctrl+C to exit...", color);

        refresh();
        sleep(2);
    }

    endwin();
    close(fd);
    return 0;
}

