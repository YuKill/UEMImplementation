#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <semaphore.h>

#include <curses.h>

#include "util.h"

#define MAX_LINE 12

sem_t input_sem;
int input = STILL;
int end_connection = 0;

void get_map(char *s, int *map) {
  int i;
  for (i = 0; i < HEIGHT * WIDTH; i++) {
    sscanf(s, "%d", &map[i]);
    s += 2;
  }
}

void draw_win() {
  printw("Congratz!\n");
}

void draw_game_over(int tm) {
  printw("Game Over!\n");
  printw("You played for: %ds\n", tm);
}

void insert_entities(game_t game, int *map, int *mem) {
  map[game.pacman.y * WIDTH + game.pacman.x] = PACMAN;
  mem[0] = map[game.ghost[0].y * WIDTH + game.ghost[0].x];
  map[game.ghost[0].y * WIDTH + game.ghost[0].x] = GHOST_1;
  mem[1] = map[game.ghost[1].y * WIDTH + game.ghost[1].x];
  map[game.ghost[1].y * WIDTH + game.ghost[1].x] = GHOST_2;
  mem[2] = map[game.ghost[2].y * WIDTH + game.ghost[2].x];
  map[game.ghost[2].y * WIDTH + game.ghost[2].x] = GHOST_3;
  mem[3] = map[game.ghost[3].y * WIDTH + game.ghost[3].x];
  map[game.ghost[3].y * WIDTH + game.ghost[3].x] = GHOST_4;
}

void remove_entities(game_t game, int *map, int *mem) {
  map[game.pacman.y * WIDTH + game.pacman.x] = EMPTY;
  map[game.ghost[0].y * WIDTH + game.ghost[0].x] = mem[0];
  map[game.ghost[1].y * WIDTH + game.ghost[1].x] = mem[1];
  map[game.ghost[2].y * WIDTH + game.ghost[2].x] = mem[2];
  map[game.ghost[3].y * WIDTH + game.ghost[3].x] = mem[3];
}

void draw_map(game_t game) {
  int i, *map = game.map, mem[4];
  insert_entities(game, map, mem);
  for (i = 0; i < WIDTH * HEIGHT; i++) {
    if (i % WIDTH == 0) printw("\n");
    if (map[i] == 0) {
      printw(" ");
    } else if (map[i] == 1) {
      printw("c");
    } else if (map[i] == 2) {
      printw(".");
    } else if (map[i] == 3) {
      printw("&");
    } else if ((map[i] >= 4) && (map[i] <= 7)) {
      printw("@");
    } else {
      printw("#");
    }
  }
  printw("\n");
  remove_entities(game, map, mem);
}

void draw_hud(game_t game) {
  printw("Score: %d\n", game.score);
  printw("Level: %d\n", game.level);
  printw("Lives: %d\n", game.lives);
}

void *user_input() {
  while (!end_connection) {
    /*
       char command[2];
       scanf("%s", command);
       */

    char command;
    command = getch();

    sem_wait(&input_sem);
    input = (int) (command - 'i');
    sem_post(&input_sem);
  }
  pthread_exit(NULL);
}

void move_positions(game_t *game, char **lines, int nlines) {
  int i;
  for (i = 0; i < nlines; i++) {
    char entity[3];
    int x, y;
    pos_t *ent_pos;

    sscanf(lines[i], "%s %d %d", entity, &x, &y);
    if (!strcmp(entity, "B")) {
      game->map[y*WIDTH + x] = BONUS;
      continue;
    }

    if (!strcmp(entity, "F1")) 
      ent_pos = &game->ghost[0];
    else if (!strcmp(entity, "F2")) 
      ent_pos = &game->ghost[1];
    else if (!strcmp(entity, "F3")) 
      ent_pos = &game->ghost[2];
    else if (!strcmp(entity, "F4")) 
      ent_pos = &game->ghost[3];
    else {//PACMAN
      game->map[game->pacman.y*WIDTH + game->pacman.x] = EMPTY;
      ent_pos = &game->pacman; 
    }

    ent_pos->x = x;
    ent_pos->y = y;
  }
}

void *server_communication(void *pack) {
  int sock = *((int*) pack);
  char full_message[BUF_SIZE];
  char buffer[BUF_SIZE];
  char *lines[MAX_LINE];
  int map[WIDTH*HEIGHT];
  int cleared = 0;
  int i;
  game_t game;

  setup_game(&game);

  sprintf(full_message, "NEWGAME\nInput:5");
  send_msg(sock, full_message);

  while (!end_connection) {

    receive(sock, buffer);
    int nlines = split_lines(buffer, lines, MAX_LINE) - 6;

    game.score = get_int(lines[1]);
    game.game_over = get_int(lines[2]);
    game.level = get_int(lines[3]);
    game.lives = get_int(lines[4]);
    game.start_time = get_int(lines[5]);

    if (!strcmp(lines[0], "MAP")) {
      sem_wait(&input_sem);
      sprintf(full_message, "GIMME\nInput:%d", input);
      sem_post(&input_sem);

      printf("Map: %s\n", lines[6]);
      get_map(lines[6], map);
      set_game_map(&game, map);

    } else if (!strcmp(lines[0], "POS")) {
      sem_wait(&input_sem);
      sprintf(full_message, "GIMME\nInput:%d", input);
      sem_post(&input_sem);

      move_positions(&game, &lines[6], nlines);

    } else if (!strcmp(lines[0], "CLEAR")) {
      strcpy(full_message, "GETMAP\nInput:5");
      cleared = 1;
    } else if (!strcmp(lines[0], "DEAD")) { 
      if (game.game_over) {
        strcpy(full_message, "ENDGAME\nInput:5");
        end_connection = 1;
      } else {
        reset_entities(&game);
        sem_wait(&input_sem);
        input = STILL;
        sprintf(full_message, "GIMME\nInput:%d", input);
        sem_post(&input_sem);
      }
    } else if (!strcmp(lines[0], "FIN")) {
      strcpy(full_message, "ENDGAME\nInput:5");
      end_connection = 1;
    }

    clear();
    if (cleared) {
      draw_win(game);
      cleared = 0;
    } else if (end_connection) {
      draw_game_over(get_int(lines[6]));
    } else {
      draw_hud(game);
      draw_map(game);
    }
    refresh();

    send_msg(sock, full_message);
    sem_wait(&input_sem);
    input = STILL;
    sem_post(&input_sem);

    usleep(500000);
    //scanf("%d", &end_connection);

  }

  strcpy(full_message, "ENDGAME\nInput:5");
  send_msg(sock, full_message);
  pthread_exit(NULL);
}

void start_game(int sock) {
  sem_init(&input_sem, 0, 1);  
  pthread_t input_thread, server_thread;


  pthread_create(&input_thread, NULL, user_input, NULL);
  pthread_create(&server_thread, NULL, server_communication, (void*)&sock);


  pthread_join(input_thread, NULL);
  pthread_join(server_thread, NULL);
}

void sendHelp() {

  clear();
  printw("# # # # # # # # # # # # # # #\n");
  printw("#                           #\n");
  printw("#       --HOW TO PLAY --    #\n");
  printw("#                           #\n");
  printw("#   Use j, k, l and i to    #\n");
  printw("# move Pacman, and eat all  #\n");
  printw("# the candy!!!              #\n");
  printw("#                           #\n");
  printw("#   BEWARE THE GHOSTS!!!!   #\n");
  printw("#                           #\n");
  printw("# # # # # # # # # # # # # # #\n");
  printw("\nPress 0 to return \n");
  refresh();

  char choice = 'a';
  while (choice != '0')
    choice = getch();

}

void mainMenu(int server_socket) {

  char choice;

  do {
    clear();
    printw("# # # # # # # # # # # # # # #\n");
    printw("#                           #\n");
    printw("# Welcome to PACMAN ! ! !   #\n");
    printw("#                           #\n");
    printw("# Choose an option:         #\n");
    printw("#                           #\n");
    printw("# 1. Start Game             #\n");
    printw("# 2. Help                   #\n");
    printw("# 3. Exit                   #\n");
    printw("#                           #\n");
    printw("# # # # # # # # # # # # # # #\n");
    refresh();

    choice = getch();

    switch (choice) {
      case '1':
        start_game(server_socket);
        break;

      case '2':
        sendHelp();
        break;

      case '3':
        printw("Goodbye.....\n\n");
        refresh();
        break;

      default:
        printw("Wrong option. Try again\n");
        refresh();
        break;  

    }

  } while (choice != '3');

}

int main() {
  int server_socket;
  struct sockaddr_in server_addr;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("ERROR: could not open socket.");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  if (connect(server_socket, (struct sockaddr*) &server_addr,
        sizeof(struct sockaddr)) == -1) {
    perror("ERROR: could not connect to server.");
    exit(1);
  }

  initscr();
  mainMenu(server_socket);
  endwin();

}
