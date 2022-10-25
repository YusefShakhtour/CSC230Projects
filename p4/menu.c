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
  Menu *menu;
  menu->capacity = 5;
  menu->count = 0;
  menu->list = (MenuItem *)malloc(sizeof(MenuItem *) * menu->capacity);  //When out of capaciy, make new menu with double capcaity and copy items from old menu to new menu then free old menu. 
  menu->list[0] = (MenuItem *)malloc(sizeof(MenuItem));
  return menu;
}

void freeMenu(Menu *menu) {
  

}

void readMenuItems(char const *filename, Menu *menu) {
  FILE *fp = fopen(filename, "r");
  char id[5];
  char category[16];
  int cost;
  char name[21];
  int cap = menu->capacity;
  int count = menu->count;
  while (strcmp(readLine(fp), '\0') != 0) {
    if (count >= cap) {
      menu->list = realloc(menu->list, sizeof(MenuItem *) * (cap * 2)); //If count is ever greater or equal to capacity, reallocate memory for the list in menu and double the capcity
      cap = cap * 2;
      menu->capacity = cap;
    }
    char *str = readLine(fp);  //Will this continue where left off after reading first line?
    sscanf(str, "%s%s%d%s", id, category, &cost, name);
    strcpy(menu->list[count]->id, id);
    strcpy(menu->list[count]->name, name);
    strcpy(menu->list[count]->category, category);
    menu->list[count]->cost = cost;
    count++;
    menu->count = count;  //Update count in menu
  }
}


void listMenuItems(Menu *menu, int (*compare)( void const *va, void const *vb ), bool (*test)( MenuItem const *item, char const *str ), char const *str) {
//Compare and test functions should be declared in kiosk.c? In there I need to store the addresses to these functions and call them appropriately based on the commands send by client. 
  
  
}

int main() {


}



