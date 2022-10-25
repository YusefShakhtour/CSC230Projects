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

}

void readMenuItems(char const *filename, Menu *menu) {
  FILE *fp = fopen(filename, "r");
  FILE *fp1 = fopen(filename, "r");
  int lines = 0;
  char ch = fgetc(fp1);
  while (ch != EOF) {
    if (ch == '\n') {
      lines++;
    }
    ch = fgetc(fp1);
    if (ch == EOF) {
 //     lines++;
    }
  }
  fclose(fp1);

  char *str = readLine(fp);
  char category[16];
  char id[5];
  char name[21];
  int cost;
  int count = menu->count;
  while (count < lines) {
    if (count >= menu->capacity) {
      menu->capacity = (menu->capacity) * 2;
      menu->list = (MenuItem **)realloc(menu->list, (menu->capacity) * sizeof(MenuItem *));
    }
    
    sscanf(str, "%s%s%d%s", id, category, &cost, name);
    
    menu->list[count] = (MenuItem *)malloc(sizeof(MenuItem));
    strcpy(menu->list[count]->id, id);
    strcpy(menu->list[count]->name, name);
    strcpy(menu->list[count]->category, category);
    menu->list[count]->cost = cost;

    str = readLine(fp);
    menu->count = menu->count + 1; 
    count++;
  }
}


void listMenuItems(Menu *menu, int (*compare)( void const *va, void const *vb ), bool (*test)( MenuItem const *item, char const *str ), char const *str) {
  qsort(menu->list, menu->count, sizeof(menu->list[0]), compare);
  printf("%-7s%-21s%-12s%12s\n", "ID", "Name", "Category", "Cost");
  for (int i = 0; i < menu->count; i++) {
    if (test(menu->list[i], str)) {
      double price = (double)(menu->list[i]->cost) / 100;
      printf("%-7s%-21s%-12s%9c%6.2f\n", menu->list[i]->id, menu->list[i]->name, menu->list[i]->category, '$' , price);
    }
  }
}

