/**
 *
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
 *
 */
Menu *makeMenu();

/**
 *
 */
void freeMenu(Menu *menu);

/**
 *
 */
void readMenuItems(char const *filename, Menu *menu);

/**
 *
 */
void listMenuItems(Menu *menu, int (*compare)(void const *va, void const *vb), bool (*test)(MenuItem const *item, char const *str), char const *str);
