/***************************************************************************
    This file is a part of Interpretor.

    Interpretor is a simple interpretor of a basic language.
    Copyright (C) 2011  DURET Simon

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "global.h"

struct function
{
   const char *name;
   double (*callback)(struct tree*);
};


/*********************************************
 * Private functions
 ********************************************/
static double fun_sqrt(struct tree *tree)
{
   return sqrt(calc_tree(tree));
}

static double fun_abs(struct tree *tree)
{
   return abs(calc_tree(tree));
}

static double fun_cos(struct tree *tree)
{
   return cos(calc_tree(tree));
}

static double fun_print(struct tree *tree)
{
   double result;

   result = calc_tree(tree);
   printf("%.2f\n", result);

   return result;
}

static struct function Functions[] = 
{
   {"sqrt", fun_sqrt},
   {"abs", fun_abs},
   {"cos", fun_cos},
   {"print", fun_print},
};

/**************************************************
 * Public functions 
 *************************************************/
int get_function_id(const char *name, int name_len)
{
   unsigned int i;

   for(i = 0; i < sizeof(Functions)/sizeof(*Functions); i++)
   {
      if(!strncmp(Functions[i].name, name, name_len))
	 return i;
   }
   return -1;
}

double exec_function(int fun_id, struct tree *tree)
{
   assert(fun_id >= 0);
   
   return Functions[fun_id].callback(tree);
}
