#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "util.h"

#define QUEUE 5

// ------------------------- GAME -------------------------------

static int nof_fruits[1] = { 83 };

static int entity_turns[3][5] = {
  { 4, 4, 4, 4, 2 },
  { 2, 2, 2, 2, 2 },
  { 2, 2, 2, 2, 4 }
};

static int map[1][WIDTH*HEIGHT] = {
  {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 2, 8, 8, 8, 2, 8, 8, 8, 2, 8, 8, 8, 2, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 8, 8, 8, 8, 2, 8, 8, 8, 2, 8, 8, 8, 8, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 2, 8, 8, 8, 2, 2, 0, 2, 2, 8, 8, 8, 2, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 2, 8, 8, 8, 2, 8, 8, 8, 2, 8, 8, 8, 2, 8,
    8, 2, 2, 2, 2, 2, 8, 8, 8, 2, 2, 2, 2, 2, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
  }
};

void set_turns(game_t *game) {
  game->turn[0] = entity_turns[game->level-1][0];
  game->turn[1] = entity_turns[game->level-1][1];
  game->turn[2] = entity_turns[game->level-1][2];
  game->turn[3] = entity_turns[game->level-1][3];
  game->turn[4] = entity_turns[game->level-1][4];
}

void reset(game_t *game) {
  game->map[game->pacman.y*WIDTH + game->pacman.x] = EMPTY;
  reset_entities(game);
  set_turns(game);
  game->map[game->pacman.y*WIDTH + game->pacman.x] = PACMAN;
}

int is_dead(game_t game) {
  int i;
  for (i = 0; i < 4; i++)
    if (game.ghost[i].x == game.pacman.x &&
        game.ghost[i].y == game.pacman.y)
      return 1;
  return 0;
}

int is_cleared(game_t game) {
  return game.nof_fruits == nof_fruits[0];
}

char *insert_bonus(game_t *game, char *s) {
  int prob = rand() % 100;
  if (prob < BONUS_PERCENT && game->nof_fruits > 7) {
    int qtt = nof_fruits[0] - game->nof_fruits;
    int place = rand() % qtt;
    int i;
    for (i = 0; i < WIDTH * HEIGHT; i++) {
      if (!place && game->map[i] == EMPTY) {
        game->map[i] = BONUS;
        sprintf(s, "B %d %d\n", i % WIDTH, i / WIDTH);
        s += strlen(s);
        break;
      } else if (game->map[i] == EMPTY)
        place--;
    }
  }
  return s;
}

int can_move(int *map, pos_t pos) {
  if (pos.x > 0 && pos.x < WIDTH && pos.y > 0 && pos.y < HEIGHT &&
      map[pos.y*WIDTH + pos.x] != BARRIER)
    return 1;
  return 0;
}

int moved_to(int *map, pos_t pos, pos_t *new_pos, int d) {
  new_pos->x = pos.x;
  new_pos->y = pos.y;
  switch (d) {
    case UP:
      new_pos->y--;
      if (!can_move(map, *new_pos))
        new_pos->y++;
      break;
    case DOWN:
      new_pos->y++;
      if (!can_move(map, *new_pos))
        new_pos->y--;
      break;
    case LEFT:
      new_pos->x--;
      if (!can_move(map, *new_pos))
        new_pos->x++;
      break;
    case RIGHT:
      new_pos->x++;
      if (!can_move(map, *new_pos))
        new_pos->x--;
      break;
  }
  return !(new_pos->x == pos.x && new_pos->y == pos.y);
}

int move(int *map, pos_t pos, pos_t *new_pos, int new_d, int d) {
  if (!moved_to(map, pos, new_pos, new_d)) {
    moved_to(map, pos, new_pos, d);
    return 0;
  }
  return 1;
}

void pacman_move(int *map, pos_t *pos, int *direction, int *new_direction) {
  pos_t new_pos;
  if (move(map, *pos, &new_pos, *new_direction, *direction))
    *direction = *new_direction;
  else
    *new_direction = *direction;
  pos->x = new_pos.x;
  pos->y = new_pos.y;
}

void ghost_move(int *map, pos_t *pos) {
  int d = rand() % 4;
  pos_t new_pos;

  while(!moved_to(map, *pos, &new_pos, d)) { 
    d = rand() % 4;
    moved_to(map, *pos, &new_pos, d);
  }

  pos->x = new_pos.x;
  pos->y = new_pos.y;
}

int get_moves(game_t *game, char *s) {
  int *turns = game->turn;
  int exit_loop = 0, i;
  while (!exit_loop) {
    for (i = 0; i < 5; i++) {
      if (turns[i] == 0) {
        if (i < 4) {
          ghost_move(game->map, &game->ghost[i]);
          sprintf(s, "F%d %d %d\n", i+1, game->ghost[i].x, game->ghost[i].y);
        } else {
          game->map[game->pacman.y*WIDTH + game->pacman.x] = EMPTY;

          pacman_move(game->map, &game->pacman, &game->direction, &game->new_direction);
          if (game->map[game->pacman.y*WIDTH + game->pacman.x] == FRUIT) {
            game->score += FRUIT_POINT;
            game->nof_fruits++;
          } else if (game->map[game->pacman.y*WIDTH + game->pacman.x] == BONUS)
            game->score += BONUS_POINT;
          game->map[game->pacman.y*WIDTH + game->pacman.x] = PACMAN;

          sprintf(s, "P %d %d\n", game->pacman.x, game->pacman.y);
        }
        turns[i] = entity_turns[game->level-1][i];
        s += strlen(s);
        exit_loop = 1;
      } else
        turns[i]--;
    }
  }
  s = insert_bonus(game, s);
  s--;
  *s = '\0';
  return is_dead(*game);
}

// -------------------------------------------------------------

void map_to_str(char *s, int *map) {
  int i;
  for (i = 0; i < WIDTH * HEIGHT; i++) {
    sprintf(s, "%d ", map[i]);
    s += 2;
  }
}

void start_game(int sock) {
  srand(time(0));

  char buffer[BUF_SIZE];
  char game_info[BUF_SIZE];
  char full_message[BUF_SIZE];
  char *lines[2];
  int end_connection = 0;
  game_t game;

  setup_game(&game);
  set_turns(&game);
  while (!end_connection) {

    receive(sock, buffer);
    split_lines(buffer, lines, 2);

    sprintf(game_info, "Score:%d\nGameover:%d\nLevel:%d\nLives:%d\nStartTime:%llu",
        game.score, game.game_over, game.level, game.lives, game.start_time);

    if (!strcmp(lines[0], "NEWGAME")) {
      char map_str[PAYLOAD];
      game.start_time = time(0);
      map_to_str(map_str, map[0]);
      sprintf(full_message, "MAP\n%s\n%s", game_info, map_str);
      set_game_map(&game, map[0]);

    } else if (!strcmp(lines[0], "ENDGAME")) {
      sprintf(full_message, "ENDCON\n%s", game_info);
      end_connection = 1;

    } else if (!strcmp(lines[0], "GETMAP")) {
      char map_str[PAYLOAD];
      map_to_str(map_str, map[0]);
      sprintf(full_message, "MAP\n%s\n%s", game_info, map_str);
      set_game_map(&game, map[0]);

    } else {
      int new_direction = get_int(lines[1]);
      if (new_direction != STILL) 
        game.new_direction = new_direction;

      char moves[PAYLOAD];
      if (get_moves(&game, moves)) {
        game.lives--;
        if (game.lives <= 0) {
          game.game_over = 1;
          sprintf(game_info, "Score:%d\nGameover:%d\nLevel:%d\nLives:%d\nStartTime:%llu",
              game.score, game.game_over, game.level, game.lives, game.start_time);
        }
        sprintf(full_message, "DEAD\n%s\nTotalTime:%llu", game_info, time(0) - game.start_time);
        reset(&game);

      } else if (!is_cleared(game)) {
        sprintf(game_info, "Score:%d\nGameover:%d\nLevel:%d\nLives:%d\nStartTime:%llu",
            game.score, game.game_over, game.level, game.lives, game.start_time); 
        sprintf(full_message,"POS\n%s\n%s", game_info, moves);
      } else if (game.level < 3) {
        sprintf(game_info, "Score:%d\nGameover:%d\nLevel:%d\nLives:%d\nStartTime:%llu",
            game.score, game.game_over, ++game.level, game.lives, game.start_time); 
        sprintf(full_message,"CLEAR\n%s", game_info);
        reset(&game);
        game.nof_fruits = 0;
      } else if (game.level == 3) {
        sprintf(full_message, "FIN\n%s\nTotalTime:%llu", game_info, time(0) - game.start_time);
      }
    }

    send_msg(sock, full_message);
    if (end_connection)
      close(sock);
  }

}

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("ERROR: could not open socket.");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr*) &server_addr,
        sizeof(struct sockaddr)) == -1) {
    perror("ERROR: could not bind socket.");
    exit(1);
  }

  listen(server_socket, QUEUE);
  while (1) {
    int client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*) &client_addr,
        &client_addr_len);

    if (client_socket == -1) {
      perror("ERROR: could not accept connection.");
      continue;
    }

    printf("Connection started.\n");
    start_game(client_socket);
    printf("Connection closed.\n");
  }
}
