/**
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "menu.h"


static int compare(void const *va, void const *vb) {
  MenuItem const *m1 = *(MenuItem **) va;
  MenuItem const *m2 = *(MenuItem **) vb;

  if (strcmp(m1->category, m2->category) > 0) {
    return 1;
  }
  else if (strcmp(m1->category, m2->category) < 0) {
    return -1;
  }

  if (strcmp(m1->id, m2->id) > 0) {
    return 1;
  }
  else if (strcmp(m1->id, m2->id) < 0) {
    return -1;
  }
  return 0;
}

static bool trueTest(MenuItem const *item, char const *str) {
  return true;
}

static bool testCat(MenuItem const *item, char const *str) {
  if (strcmp(item->category, str) == 0) {
    return true;
  }
  return false;
}
/**
 *
 */
//void listOrderItems(Order *order, int (*compare)(void const *va, void const *vb)) {

//}


/**
 *
 */
int main() {
  Menu *menu;
  menu = makeMenu();
  readMenuItems("menu-c.txt", menu);
  char temp[10] = "\0";
  
  if (temp[0] == '\0') {
    listMenuItems(menu, compare, trueTest, temp);
  }
  else {
    listMenuItems(menu, compare, testCat, temp);
  }

  return 0;
}
