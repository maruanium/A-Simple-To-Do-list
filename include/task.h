#ifndef TASK_H
#define TASK_H

#include <stdbool.h>

#define MAX_TASKS 20

typedef struct {
  int id;
  char name[30];
  char description[256];
  bool isCompleted;
  bool exists;
} Task;

#endif
