#include "tasks.h"
#include "ANSI-color-codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_task(Task *tasks, int index, int id) {

  tasks[index].id = id;

  printf("\n============================\n");

  printf("Task name => ");

  if (fgets(tasks[index].name, sizeof(tasks[index].name), stdin) == NULL)
    return;

  tasks[index].name[strcspn(tasks[index].name, "\n")] = '\0';

  printf("Task description => ");

  if (fgets(tasks[index].description, sizeof(tasks[index].description),
            stdin) == NULL)
    return;

  tasks[index].description[strcspn(tasks[index].description, "\n")] = '\0';

  tasks[index].isCompleted = false;
  tasks[index].exists = true;

  printf("Task id " GREEN "%d" RESET " successfully added!\n", tasks[index].id);
}

void view_tasks(Task *tasks, int activeTasks) {

  if (activeTasks == 0) {
    printf("\n============================\n");
    printf("No tasks Available.\n");
    return;
  }

  printf("\n============================\n");
  printf("Fetching tasks ...\n");

  sleep(2);

  if (activeTasks == 1)
    printf("--- Current Todo Tasks (%d item) ---\n\n", activeTasks);
  else
    printf("--- Current Todo Tasks (%d items) ---\n\n", activeTasks);

  for (int i = 0; i < MAX_TASKS; i++) {

    if (tasks[i].exists) {
      if (tasks[i].isCompleted) {
        printf(BGRN "Task ID" RESET ": %d | " BGRN "Name: " RESET "%s | " BGRN
                    "Description: " RESET "%s | " BGRN "Status: " RESET BGRN
                    "Completed!" RESET "\n",
               tasks[i].id, tasks[i].name, tasks[i].description);

      } else {
        printf(BGRN "Task ID: " RESET "%d | " BGRN "Name: " RESET "%s | " BGRN
                    "Description: " RESET "%s | " BGRN "Status: " RESET BRED
                    "Pending!" RESET "\n",
               tasks[i].id, tasks[i].name, tasks[i].description);
      }
    }
  }

  printf("--- --- --- --- --- --- --- --- --- --- ---\n");
}

void delete_task(Task *tasks, int capacity, int *activeTasks) {
  if (*activeTasks == 0) {
    printf("============================\n");
    printf("No tasks available to delete.\n");
    return;
  }

  int id;
  char input[32];

  while (true) {

    printf("Enter task ID to delete (or q to quit): ");

    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    if (input[0] == 'q' || input[0] == 'Q') {
      printf("Returning to menu...\n");
      return;
    }

    char *end;
    long value = strtol(input, &end, 10);

    // check if input was not a valid number
    if (*end != '\0' || input[0] == '\0') {
      printf("Invalid input!\n");
      continue;
    }

    id = (int)value;

    for (int i = 0; i < capacity; i++) {

      if (tasks[i].exists && tasks[i].id == id) {

        tasks[i].exists = false;
        (*activeTasks)--;

        printf("Deleting task ID %d...\n", id);
        sleep(2);

        printf("Task deleted successfully!\n");
        return;
      }
    }

    printf("Task ID %d not found.\n", id);
  }
}

void edit_task(Task *tasks, int capacity, int activeTasks) {
  if (activeTasks == 0) {
    printf("============================\n");
    printf("No tasks available to edit.\n");
    return;
  }

  int id;
  char input[32];

  while (true) {
    printf("Enter task ID to edit (or q to quit): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (input[0] == 'q' || input[0] == 'Q') {
      printf("Returning to menu...\n");
      return;
    }

    char *end;
    long value = strtol(input, &end, 10);

    // check if input was valid number
    if (*end != '\0' || input[0] == '\0') {
      printf("Invalid input!\n");
      continue;
    }

    id = (int)value;

    for (int i = 0; i < capacity; i++) {
      if (tasks[i].exists && tasks[i].id == id) {
        printf("Editing task ID %d...\n", id);

        printf("New task name => ");
        fgets(tasks[i].name, sizeof(tasks[i].name), stdin);
        tasks[i].name[strcspn(tasks[i].name, "\n")] = '\0';

        printf("New task description => ");
        fgets(tasks[i].description, sizeof(tasks[i].description), stdin);
        tasks[i].description[strcspn(tasks[i].description, "\n")] = '\0';

        printf("Task updated successfully!\n");
        return;
      }
    }
    printf("Task ID %d not found.\n", id);
  }
}

void mark_complete(Task *tasks, int capacity, int activeTasks) {
  if (activeTasks == 0) {
    printf("============================\n");
    printf("No tasks available to mark as complete.\n");
    return;
  }

  int id;
  char input[32];

  while (true) {
    printf("Enter task ID to mark as complete (or q to quit): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (input[0] == 'q' || input[0] == 'Q') {
      printf("Returing to menu...\n");
      return;
    }

    char *end;
    long value = strtol(input, &end, 10);

    if (*end != '\0' || input[0] == '\0') {
      printf("Invalid input!\n");
      continue;
    }

    id = (int)value;

    for (int i = 0; i < capacity; i++) {
      if (tasks[i].exists && tasks[i].id == id) {
        if (tasks[i].isCompleted == true) {
          printf("Task ID %d is already completed!\n", id);
          return;
        }
        sleep(1);
        printf("Tasks ID %d marked as complete!\n", id);
        tasks[i].isCompleted = true;
        return;
      }
    }
    printf("Task ID %d not found.\n", id);
  }
}

void save_to_file(Task *tasks, int *activetasks) {
  FILE *file = fopen("tasks.dat", "wb");

  if (file == NULL) {
    printf("Couldn't open the file!\n");
    return;
  }

  fwrite(activetasks, sizeof(*activetasks), 1, file);

  for (int i = 0; i < MAX_TASKS; i++) {
    if (tasks[i].exists) {
      fwrite(&tasks[i], sizeof(tasks[i]), *activetasks, file);
    }
  }

  printf("Task(s) saved successfully!\n");

  fclose(file);
}

void load_from_file(Task *tasks, int *activeTasks) {
  FILE *file = fopen("tasks.dat", "rb");

  if (file == NULL) {
    printf("No saved file found!\n");
    return;
  }

  if (fread(activeTasks, sizeof(*activeTasks), 1, file) != 1) {
    printf("Corrupted saved file.\n");
    fclose(file);
    return;
  }

  for (int i = 0; i < *activeTasks; i++) {
    fread(&tasks[i], sizeof(tasks[i]), 1, file);
  }

  fclose(file);
}
