/**@file kiosk.c
 * @author Yusef Shakhtour (yfshakht)
 * This file defines much of what is needed to work with orders 
 * and allows orders and menus to communicate accordingly
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "menu.h"


/**
 * An OrderItem which contains the quantity of a particualr menuItem
 * and the menuItem desired.
 */
struct OrderItem {
  int quantity;
  MenuItem *m;
};
typedef struct OrderItem OrderItem;

/**
 * An Order which contains a list of orderItems
 * a count of the order items and the capcity of order
 */
struct Order {
  int count;
  int capacity;
  OrderItem **list;

};
typedef struct Order Order;

/**
 * This method will compare two menuItems based on their category followed by ID
 * @param va pointer to a menuItem
 * @param vb pointer to a menuItem
 * @return int whether the comparison was greater, less, or equal
 */
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


/**
 * Compare function to compare order two order items to be 
 * sorted based on cost of total items followed by id
 * @param va pointer to an orderitem
 * @param vb pointer to an orderitem
 * @return int whether the comparison was greater, less, or equal;
 */
static int orderCompare(void const *va, void const *vb) {
  OrderItem const *o1 = *(OrderItem **) va;
  OrderItem const *o2 = *(OrderItem **) vb;

  if ((o1->quantity * o1->m->cost) > (o2->quantity * o2->m->cost)) {
    return -1;
  }
  else if ((o1->quantity * o1->m->cost) < (o2->quantity * o2->m->cost)) {
    return 1;
  }

  if (strcmp(o1->m->id, o2->m->id) > 0) {
    return 1;
  }
  else if (strcmp(o1->m->id, o2->m->id) < 0) {
    return -1;
  }
  return 0;
}

/**
 * Defined to return true and will be used when the client lists menu
 * @param item the menuItem to check category
 * @param str the category
 * @return bool true
 */
static bool trueTest(MenuItem const *item, char const *str) {
  return true;
}

/**
 * This compare function will check to see if the menuItem's category matches str
 * @param item the menuItem to check
 * @param str the category to check
 * @return bool if the two were equal
 */
static bool testCat(MenuItem const *item, char const *str) {
  if (strcmp(item->category, str) == 0) {
    return true;
  }
  return false;
}

/**
 * A helper method that adds a menuItem to an order
 * @param order the order to add to
 * @param menu the menu to read from
 * @param id the id of the desired item
 * @param quantity the quantity ordered
 */
static void addOrder(Order *order, Menu *menu, char *id, int quantity) {  //WORKING
  MenuItem *cpy; //Unneccesary middleman

  for (int i = 0; i < menu->count; i++) {
    if (strcmp(menu->list[i]->id, id) == 0) {  //Loop through menu to find desired item
      cpy = menu->list[i];
      break;
    }
  }

  if (order->count >= order->capacity) {
    order->capacity = (order->capacity) * 2;
    order->list = (OrderItem **)realloc(order->list, order->capacity * sizeof(OrderItem *));
  }

  order->list[order->count] = (OrderItem *)malloc(sizeof(OrderItem));
  order->list[order->count]->m = cpy;
  order->list[order->count]->quantity = quantity;
  order->count = order->count + 1;
}

/**
 * A helper method that removes a menuItem from an order
 * @param order teh order to remove from
 * @param id the id to remove
 * @param quantity the number to remove
 */
static void removeOrder(Order *order, char *id, int quantity) {  //WORKING
  for (int i = 0; i < order->count; i++) {
    if (strcmp(order->list[i]->m->id, id) == 0) {
      if (quantity < order->list[i]->quantity) {
        order->list[i]->quantity = order->list[i]->quantity - quantity;
        break;
      }
      else {
        OrderItem *temp = order->list[i];
	for (int j = i; j < order->count; j++) {
          order->list[j] = order->list[j + 1];
        }
        order->list[order->count - 1] = NULL;
        free(temp);
        order->count = order->count - 1;
        break;
      }
    }
  } 
}

/**
 * A helper method that will free allocated memory from an order
 * @param order the order to free
 */
static void freeOrder(Order *order) {
  for (int i = 0; i < order->count; i++) {
    free(order->list[i]);
  }
  free(order->list);
  free(order);
}

/**
 * A helper method that acts as a constructor for an order
 * @return Order the constructed order
 */
static Order *makeOrder() {  //WORKING
  Order *order = malloc(sizeof(Order));
  order->capacity = 5;
  order->count = 0;
  order->list = (OrderItem **)malloc(order->capacity * sizeof(OrderItem *));
  return order;
}

/**
 * A fucntion that lists an order is a specific format
 * @param order the order to list
 * @param compare a function to help sort the order by category
 */
void listOrderItems(Order *order, int (*compare)(void const *va, void const *vb)) {  //WORKING
  double total = 0;
  qsort(order->list, order->count, sizeof(order->list[0]), compare);
  printf("%-5s%-21s%8s %-16s%3s\n", "ID", "Name", "Quantity", "Category", "Cost");
  for (int i = 0; i < order->count; i++) {
    double price = ((double)((order->list[i]->m->cost) / 100.0)) * ((double) (order->list[i]->quantity));
    total += price;
    printf("%-5s%-21s%8d %-16s%c%6.2f\n", order->list[i]->m->id, order->list[i]->m->name, order->list[i]->quantity, order->list[i]->m->category, '$', price);
  }
  printf("%-5s%47c%6.2f\n", "Total", '$', total);
}

/**
 * This helper method will check to ensure a particualr id exists in menu
 * @param menu the menu to check
 * @param id the id to match
 * @return bool if it matches or not
 */
static bool menuCheck(Menu *menu, char *id) {
  for (int i = 0; i < menu->count; i++) {
    if (strcmp(menu->list[i]->id, id) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * This helper method will check to ensure a particular id exists in an order
 * @param order the order to check
 * @param id the id to match
 * @return bool if it matches or not
 */
static bool orderCheck(Order *order, char *id) {
  for (int i = 0; i < order->count; i++) {
    if (strcmp(order->list[i]->m->id, id) == 0) {
      return true;
    }
  }
  return false;
}


/**
 * Program starting point where input is taken from stdin and depending on the command passed in 
 * will respond accordingly.
 * @param argc the number of arguments from command line
 * @param argv a list of command line arguments
 * @return program exit status
 */
int main(int argc, char *argv[]) 
{

  Menu *menu;
  menu = makeMenu();
  Order *order;
  order = makeOrder();

  if (argc <= 1) {
    fprintf(stderr, "usage: kiosk <menu-file>*\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 1; i < argc; i++) {
    readMenuItems(argv[i], menu);
  }

  char *command = (char *)malloc(100);   //Initializing the command gets lost after readLine in the while condition
  command[0] = '\0';
  char temp[100];
  while ((command = readLine(stdin)) != NULL) {
    printf("%s", "cmd> ");
    printf("%s\n", command);
    sscanf(command, "%s", temp);
    
    if (strcmp(temp, "list") == 0) {
      sscanf(command, "%*s%s", temp);
      if (strcmp(temp, "menu") == 0) {
        if (sscanf(command, "%*s%*s%s", temp) <= 0) {
          listMenuItems(menu, compare, trueTest, "\0");
          printf("\n");
        }
        else {
          printf("Invalid command\n\n");
        }
      }
      else if (strcmp(temp, "order") == 0) {
        if (sscanf(command, "%*s%*s%s", temp) <= 0) {
          listOrderItems(order, orderCompare);
          printf("\n");
        }
        else {
          printf("Invalid command\n\n");
        }
      }
      else if (strcmp(temp, "category") == 0) {
        char cat[16];
        sscanf(command, "%*s%*s%s", cat);
        if (sscanf(command, "%*s%*s%*s%s", temp) <= 0) {
          listMenuItems(menu, compare, testCat, cat);
          printf("\n");
        }
        else {
          printf("Invalid command\n\n");
        }
      }
    }
    else if (strcmp(temp, "add") == 0) {
      char id[5];
      int quantity;
      sscanf(command, "%*s%s%d", id, &quantity);
      if (menuCheck(menu, id) == true) {
        if (sscanf(command, "%*s%*s%*d%s", temp) <= 0) {
          addOrder(order, menu, id, quantity);
          printf("\n");
        }
        else {
          printf("Invalid command\n\n");
        }
      }
      else {
        printf("Invalid command\n\n");
      }
    }
    else if (strcmp(temp, "remove") == 0) {
      char id[5];
      int quantity;
      sscanf(command, "%*s%s%d", id, &quantity);
      if (orderCheck(order, id)) {
        if (sscanf(command, "%*s%*s%*d%s", temp) <= 0) {
          removeOrder(order, id, quantity);
          printf("\n");
        }
        else {
          printf("Invalid command\n\n");
        }
      }
      else {
        printf("Invalid command\n\n");
      }
    }
    else if (strcmp(temp, "quit") == 0) {
      if (sscanf(command, "%*s%s", temp) <= 0) {
        goto nExit;
      }
      else {
        printf("Invalid command\n\n");
      }
    }
    else {
      printf("Invalid command\n\n");
    }

  free(command);
  }
  printf("%s", "cmd> ");    
  nExit: {};

  free(command);
  freeMenu(menu);
  freeOrder(order);

  return EXIT_SUCCESS;
}
