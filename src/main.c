#include "ANSI-color-codes.h"
#include "loading_screen.h"
#include "task.h"
#include "tasks.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void welcomeMessage() {
  printf("\t==========================\n");
  printf("\t     Welcome to To-Do\n");
  printf("\t==========================\n");
}

void loading() {
  ls_init();

  ls_config_t config = ls_get_default_config();
  config.label = "Loading user menu";
  config.type = LS_TYPE_BLOCKS;
  config.color = LS_COLOR_GREEN;
  ls_display(&config, 60);

  ls_cleanup();
}

void userMenu() {
  putchar('\n');
  printf(GREEN "==============To-Do List==============\n" RESET);
  putchar('\n');
  printf(BLUE "[1]." RESET " Add Task\n");
  printf(BLUE "[2]." RESET " View tasks\n");
  printf(BLUE "[3]." RESET " Delete Task\n");
  printf(BLUE "[4]." RESET " Edit Task\n");
  printf(BLUE "[5]." RESET " Mark Complete\n");
  // printf(BLUE "[6]." RESET " Mask Uncompete (under development)\n");
  printf(BLUE "[7]." RESET " Save Task(s)\n");
  printf(BLUE "[8]." RESET " Load Task(s)\n");
  // printf(RED "[8]" RESET " Restart Program (for devs)\n");
  printf("0. Exit\n");
}

void clear_input_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void menu_pause() {
  printf("\nPress Enter to continue...");
  getchar();
  system("clear");
}

void userInput(int *choice) {
  printf("Choice > ");

  if (scanf("%d", choice) != 1) {
    printf("Invalid input! Please try again.\n");
    *choice = -1;

    while (getchar() != '\n' && !feof(stdin))
      ;

    return;
  }
  clear_input_buffer();
}

int main() {
  // loading();
  system("clear");
  welcomeMessage();

  Task *tasks = malloc(sizeof(Task) * MAX_TASKS);
  if (tasks == NULL) {
    perror("malloc");
    return 1;
  }
  int activeTasks = 0;
  int nextId = 1;
  int choice; // user menu input choice

  // Initialize all slots as empty
  for (int i = 0; i < MAX_TASKS; i++) {
    tasks[i].exists = false;
  }

  do {
    userMenu();
    userInput(&choice);

    switch (choice) {
    case 1: {
      system("clear");
      int index = -1;

      // find empty slot
      for (int i = 0; i < MAX_TASKS; i++) {
        if (!tasks[i].exists) {
          index = i;
          break;
        }
      }

      if (index == -1) {
        printf("Task list is full!\n");
      } else {
        add_task(tasks, index, nextId);
        nextId++;
        activeTasks++;
      }
      menu_pause();
      break;
    }

    case 2:
      system("clear");
      view_tasks(tasks, activeTasks);
      menu_pause();
      break;

    case 3:
      system("clear");
      view_tasks(tasks, activeTasks);
      delete_task(tasks, MAX_TASKS, &activeTasks);
      // sleep(1.5);
      menu_pause();
      system("clear");
      break;

    case 4:
      system("clear");
      view_tasks(tasks, activeTasks);
      edit_task(tasks, MAX_TASKS, activeTasks);
      menu_pause();
      system("clear");
      break;

    case 5:
      system("clear");
      view_tasks(tasks, activeTasks);
      mark_complete(tasks, MAX_TASKS, activeTasks);
      menu_pause();
      system("clear");
      break;

    case 7:
      system("clear");
      save_to_file(tasks, &activeTasks);
      menu_pause();
      system("clear");
      break;

    case 8:
      system("clear");
      load_from_file(tasks, &activeTasks);
      menu_pause();
      system("clear");
      break;
    }
  } while (choice != 0);

  free(tasks);
  return 0;
}
