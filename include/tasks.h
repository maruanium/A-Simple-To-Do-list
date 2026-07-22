#ifndef TASKS_H
#define TASKS_H

#include "task.h"

void add_task(Task *tasks, int index, int id);
void view_tasks(Task *tasks, int activeTasks);
void delete_task(Task *tasks, int capacity, int *activeTasks);
void edit_task(Task *tasks, int capacity, int activeTasks);
void mark_complete(Task *tasks, int capacity, int activeTasks);
void save_to_file(Task *tasks, int *activetasks);
void load_from_file(Task *tasks, int *activeTasks);

#endif
