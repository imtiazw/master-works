/*
 * @author Aveek Mukherjee
 */

#ifndef MAZE_H
#define MAZE_H
/*
 * All leaf nodes are rooms
 */
#include "svg.h"
#include <time.h>
#include <stdlib.h>

#define max(a,b) a>b?a:b

typedef struct _cell{
  int height;
  int width;
} cell;

typedef struct _tree{
  cell * room;
  struct _tree * first_child;
  struct _tree * second_child;
  struct _tree * parent;
} tree;

typedef struct _maze {
  tree * tr;
  cell * start_cell;
  cell * end_cell;
} maze;

/**
 * This creates a random maze by dividing a given room into multiple rooms
 * @param width   initial width of the room
 * @param height  initial height of the room
 * @return
 */
maze *maze_random (int width, int height);

/**
 * Creates svg image of any maze. This draws the room and connects them with lines.
 * @param maze
 * @param filename
 */
void maze_svg (maze *maze, char *filename);

/**
 * Creates a random split of the tree t.
 * recursively splits the subtree too
 * @param t tree to split
 * @param n	maximum times its going to split , if 0 return
 */
void create_random_sub_trees(tree *t, int n);

/**
 * This method splits a tree room into two sub rooms
 * We split by creating wall parallel to the shortest wall,
 * i.e, by splitting the long wall
 * @param t the tree whose room is to be split
 */
void split_tree_room_random(tree * t);

/**
 * prints a tree, prints the room, then prints left sub tree and then right
 * @param fp		the file on which to save svg
 * @param t			the tree
 * @param origin_x	the x origin where to start drawing
 * @param origin_y	the y origin
 */
void print_tree_room_preorder(FILE *fp , tree * t , int origin_x , int origin_y);

/**
 * Generates a random number between min and max
 * @param min	lower bound
 * @param max	upper bound
 * @return the random number
 */
int random_num(int min , int max);

#endif
