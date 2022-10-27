/**@file menu.h
 * @author Yusef Shakhtour (yfshakht)
 * This header file holds the structs and methods used throughout the program. 
 * The methods are defined in menu.c
 */


/**
 *
 */
struct MenuItem {
  char id[5];
  char name[21];
  char category[16];
  int cost;
};
typedef struct MenuItem MenuItem;

/**
 *
 */
struct Menu {
  MenuItem **list;
  int count;
  int capacity;
};
typedef struct Menu Menu;

/**
 * Acts like a consturctor and initializes and allocates memory for 
 * a Menu struct
 * @return Menu * the menu that was initialized
 */
Menu *makeMenu();

/**
 * Free all allocated memory associated with a menu
 * @param menu the menu to free
 */
void freeMenu(Menu *menu);

/**
 * Read the MenuItems from a given file into a menu struct
 * @param filename the file to read from
 * @menu the menu to store the MenuItems into
 */
void readMenuItems(char const *filename, Menu *menu);

/**
 * Print and list the menu items in a menu in a specific format.
 * @param menu the menu to list
 * @param compare a function to compare categories of menu items if the menu is to be filtered by category
 * @param test a function that checks to see if the menuItem is of the particular category
 * @param str the desired category to filter by.
 */
void listMenuItems(Menu *menu, int (*compare)(void const *va, void const *vb), bool (*test)(MenuItem const *item, char const *str), char const *str);
