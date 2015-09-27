#ifndef UTIL_H
#define UTIL_H

#define BUF_SIZE 512
#define PAYLOAD 400

#define SERVER_IP "127.0.0.1"
#define PORT 8888

#define WIDTH 15
#define HEIGHT 13

#define PACMAN_X 7
#define PACMAN_Y 8
#define GHOST_1_X 6 
#define GHOST_1_Y 4 
#define GHOST_2_X 7 
#define GHOST_2_Y 4
#define GHOST_3_X 8
#define GHOST_3_Y 4
#define GHOST_4_X 7
#define GHOST_4_Y 5

#define EMPTY 0
#define PACMAN 1
#define GHOST_1 4
#define GHOST_2 5
#define GHOST_3 6
#define GHOST_4 7
#define FRUIT 2
#define BONUS 3
#define BARRIER 8

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define STILL 4

#define FRUIT_POINT 250
#define BONUS_POINT 500

#define BONUS_PERCENT 5

typedef struct {
  int x,y;
} pos_t;

typedef struct {
  int map[WIDTH * HEIGHT];
  int nof_fruits;
  int score;
  int level;
  int lives;
  int game_over;
  unsigned long long start_time;
  pos_t pacman;
  pos_t ghost[4];
  int turn[5];
  int direction;
  int new_direction;
} game_t;

void receive(int sock, char *buf) {
  int received = 0;
  char *p = buf;
  while (received < BUF_SIZE) {
    int ret_val = recv(sock, p, BUF_SIZE-received, 0); 

    if (ret_val == -1) {
      perror("ERROR: could not receive. Trying again...");
      continue;
    }   
    received += ret_val;
  }
  //printf("Message received: \"%s\"\n", buf);
}

void send_msg(int sock, char *buf) {
  int sent = 0;
  char *p = buf;
  while (sent < BUF_SIZE) {
    int ret_val = send(sock, p, BUF_SIZE, 0); 
    if (ret_val == -1) {
      perror("ERROR: could not send. Trying again...");
      continue;
    }   

    sent += ret_val;
  }
  //printf("Sending message: \"%s\"\n", buf);
}

int split_lines(char *s, char **lines, int n) {
  int i = 1;
  char *next = s;
  while (i < n && (next = strpbrk(next, "\n")) != NULL) {
    *next = '\0';
    lines[i++] = ++next;
  }
  lines[0] = s;
  return i;
}

void reset_entities(game_t *game) {
  game->pacman.x = PACMAN_X; 
  game->pacman.y = PACMAN_Y;
  game->ghost[0].x = GHOST_1_X;
  game->ghost[0].y = GHOST_1_Y;
  game->ghost[1].x = GHOST_2_X;
  game->ghost[1].y = GHOST_2_Y;
  game->ghost[2].x = GHOST_3_X;
  game->ghost[2].y = GHOST_3_Y;
  game->ghost[3].x = GHOST_4_X;
  game->ghost[3].y = GHOST_4_Y;
  game->direction = STILL;
  game->new_direction = STILL;
}

void setup_game(game_t *game) {
  reset_entities(game);
  game->score = 0;
  game->lives = 3;
  game->level = 1;
  game->game_over = 0;
  game->start_time = 0;
  game->nof_fruits = 0;
}                 

void set_game_map(game_t *game, int *map) {
  int i;
  for (i = 0; i < WIDTH * HEIGHT; i++)
    game->map[i] = map[i];
}

int get_int(char *s) {
  return atoi(strpbrk(s, ":") + 1);
}

#endif
