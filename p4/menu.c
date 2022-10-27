/**
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "menu.h"


Menu *makeMenu() {
  Menu *menu = malloc(sizeof(Menu));
  menu->capacity = 5;
  menu->count = 0;
  menu->list = (MenuItem **)malloc(menu->capacity * sizeof(MenuItem *)); 
  return menu;
}

void freeMenu(Menu *menu) {
  for (int i = 0; i < menu->count; i++) {
    free(menu->list[i]);
  }
  free(menu->list);
  free(menu);
}

void readMenuItems(char const *filename, Menu *menu) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s%s\n","Can't open file: ", filename);
    exit(EXIT_FAILURE);
  }
 
  FILE *fp1 = fopen(filename, "r"); 
  int lines = 0;
  while (readLine(fp1) != NULL) {
    lines++;
  } 

//  char *str = readLine(fp);
  char category[16];
  char id[5];
  char name[21];
  int cost;
  lines = lines + menu->count;
  int count = menu->count;
  while (count < lines) {
    if (count >= menu->capacity) {
      menu->capacity = (menu->capacity) * 2;
      menu->list = (MenuItem **)realloc(menu->list, (menu->capacity) * sizeof(MenuItem *));
    }
    char *str = readLine(fp);
    int matches = sscanf(str, "%s%s%d %[ -z]", id, category, &cost, name);
    if (matches != 4) {
      fprintf(stderr, "%s%s\n","Invalid menu file: ", filename);
      exit(EXIT_FAILURE);
    }
   
    if (strlen(id) != 4) {
      fprintf(stderr, "%s%s\n","Invalid menu file: ", filename);
      exit(EXIT_FAILURE);
    }
    if (strlen(category) > sizeof(menu->list[count]->category)) {
      fprintf(stderr, "%s%s\n","Invalid menu file: ", filename);
      exit(EXIT_FAILURE);
    } 
    if ((strlen(name)) > (sizeof(menu->list[count]->name))) {
      fprintf(stderr, "%s%s\n", "Invalid menu file: ", filename);
      exit(EXIT_FAILURE);
    }
    menu->list[count] = (MenuItem *)malloc(sizeof(MenuItem));
    strcpy(menu->list[count]->id, id);
    strcpy(menu->list[count]->name, name);
    strcpy(menu->list[count]->category, category);
    menu->list[count]->cost = cost;

 //   free(str);        //Causes errors
 //   str = readLine(fp);
    menu->count = menu->count + 1; 
    count++;
  }
  fclose(fp);

  for (int i = 0; i < menu->count; i++) {  //Dupe ID check
    for (int j = i + 1; j < menu->count - 1; j++) {
      if (strcmp(menu->list[i]->id, menu->list[j]->id) == 0) {
        fprintf(stderr, "%s%s\n","Invalid menu file: ", filename);
        exit(EXIT_FAILURE);
      }
    }
  }
}


void listMenuItems(Menu *menu, int (*compare)( void const *va, void const *vb ), bool (*test)( MenuItem const *item, char const *str ), char const *str) {
  qsort(menu->list, menu->count, sizeof(menu->list[0]), compare);
  printf("%-5s%-21s%-8s%12s\n", "ID", "Name", "Category", "Cost");
  for (int i = 0; i < menu->count; i++) {
    if (test(menu->list[i], str)) {
      double price = (double)(menu->list[i]->cost) / 100;
      printf("%-4s %-20s %-15s %c %5.2f\n", menu->list[i]->id, menu->list[i]->name, menu->list[i]->category, '$' , price);
    }
  }
}

