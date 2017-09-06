/*
 * maze.c
 *
 *  Created on: 24-Sep-2015
 *      Author: aveekmukherjee
 */

#include "maze.h"

maze *maze_random (int width, int height)
{
	maze * m = malloc(sizeof(maze));

	m->tr = malloc(sizeof(tree));
	m->tr->room = malloc(sizeof(cell));
	m->tr->room->width = width;
	m->tr->room->height = height;
	m->tr->first_child = NULL;
	m->tr->second_child = NULL;

	//int r = (int)random_num(5,(width+height));
	int r = (width+height);

	tree * t = m->tr;
	create_random_sub_trees(t, r);

	return m;
}

void maze_svg (maze *maze, char *filename){
	FILE * fp = fopen(filename , "w");

	if(fp == NULL){
		printf("Sorry! the requested file could not be opened!\n");
		return;
	}
	set_svg_color("blue");

	svg_header(fp , maze->tr->room->width + 100  , maze->tr->room->height +100 );
	print_tree_room_preorder(fp, maze->tr , 0 , 0);
	svg_footer(fp);

	fclose(fp);
}

void create_random_sub_trees(tree *t, int n)
{
	//printf("n is %d\n" , n );
	if(n <= 100)
		return;

	split_tree_room_random(t);
	create_random_sub_trees(t->first_child , t->first_child->room->height + t->first_child->room->width);
	create_random_sub_trees(t->second_child , t->second_child->room->height + t->second_child->room->width);
}

void split_tree_room_random(tree * t)
{
	cell * c= t->room;
	cell *child1, *child2;

	if(c->height > c->width){
		//if height greater, split along height
		child1 = malloc(sizeof(cell));
		child1->width = c->width;
		child1->height = random_num(0,c->height);

		child2 = malloc(sizeof(cell));
		child2->width = c->width;
		child2->height = c->height - child1->height;
	}else{
		//else, split along width
		child1 = malloc(sizeof(cell));
		child1->height = c->height;
		child1->width = random_num(0,c->width);

		child2 = malloc(sizeof(cell));
		child2->height = c->height;
		child2->width = c->width - child1->width;
	}

	t->first_child = (tree*)malloc(sizeof(tree));
	t->first_child->room = child1;
	t->first_child->first_child = NULL;
	t->first_child->second_child = NULL;

	t->second_child = (tree*)malloc(sizeof(tree));
	t->second_child->room = child2;
	t->second_child->first_child = NULL;
	t->second_child->second_child = NULL;
}

void print_tree_room_preorder(FILE *fp , tree * t , int origin_x , int origin_y)
{
	if(t==NULL)
		return;

	int x_shift =0 , y_shift =0;

	//draw the room in the tree
	svg_rect (fp, origin_x , origin_y , origin_x+t->room->width, origin_y+t->room->height);

	/**
	 * I need to find the shifts, if the children are born by width partition,
	 * 2nd child should see a shift in x coordinates, else the shift will be in y coordinate
	 */
	if(t->first_child != NULL){
		if(t->first_child->room->width == t->room->width)
		{
			//born by height partition
			y_shift = t->first_child->room->height;
		}else{
			//born by width partition
			x_shift = t->first_child->room->width;
		}
	}

	if(t->first_child)
		print_tree_room_preorder(fp , t->first_child , origin_x , origin_y );

	if(t->second_child)
		print_tree_room_preorder(fp , t->second_child , origin_x + x_shift , origin_y +y_shift );

	if(t->first_child && t->second_child){
		//Draw a door
		if(t->first_child->room->width < t->room->width){
			//if split happened in width
			int x1 = t->first_child->room->width + origin_x;
			int y1 = t->first_child->room->height/2 + origin_y;
			svg_simple_rect(fp,x1-5,y1-5, x1+5 , y1+10 );
		}else{
			//split happened in height
			int x1 = t->first_child->room->width/2 + origin_x;
			int y1 = t->first_child->room->height + origin_y;
			svg_simple_rect(fp,x1-5,y1-5, x1+10 , y1+5 );
		}
	}
}

int random_num(int min , int max){
	srand(time(NULL));
	return (rand()%(max-min)) + min;
}

