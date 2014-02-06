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
#include <assert.h>
#include "global.h"

struct operator
{
   int c;
   double (*callback)(struct tree*, struct tree*);
   int priority;
};

/***************************************************
 * Private functions
 **************************************************/
static double op_add(struct tree *left, struct tree *right)
{
   return calc_tree(left) + calc_tree(right);
}

static double op_sub(struct tree *left, struct tree *right)
{
   return calc_tree(left) - calc_tree(right);
}

static double op_pow(struct tree *left, struct tree *right)
{
   return pow(calc_tree(left), calc_tree(right));
}

static double op_div(struct tree *left, struct tree *right)
{
   return calc_tree(left) / calc_tree(right);
}

static double op_mul(struct tree *left, struct tree *right)
{
   return calc_tree(left) * calc_tree(right);
}

static double op_mod(struct tree *left, struct tree *right)
{
   return fmod(calc_tree(left), calc_tree(right));
}

static double op_greater(struct tree *left, struct tree *right)
{
   return calc_tree(left) > calc_tree(right);
}

static double op_lower(struct tree *left, struct tree *right)
{
   return calc_tree(left) < calc_tree(right);
}

static double op_not(struct tree *left, struct tree *right)
{
   return calc_tree(left) != calc_tree(right);
}

static double op_equal(struct tree *left, struct tree *right)
{
   return calc_tree(left) == calc_tree(right);
}
static struct operator Operators[] =
{
   {'<', op_lower, 1},
   {'>', op_greater, 1},
   {'!', op_not, 1},
   {'~', op_equal, 1},
   {'+', op_add, 2},
   {'-', op_sub, 2},
   {'*', op_mul, 3},
   {'/', op_div, 3},
   {'%', op_mod, 3},
   {'^', op_pow, 4},
};

/***************************************************
 * Public functions
 **************************************************/
/* Return the priority of the operator ID */
int priority(int operator_id)
{
   if(operator_id < 0)
      return -1;
   if((unsigned int)operator_id >= sizeof(Operators)/sizeof(*Operators))
      return -1;
   return Operators[operator_id].priority;
}

/* Return the ID of the operator, if c is an operator, else -1 */
int char_to_operator(int c)
{
   unsigned int i;

   for(i = 0; i < sizeof(Operators)/sizeof(*Operators); i++)
   {
      if(c == Operators[i].c)
	 return i;
   }
   return -1;
}

double exec_operator(int op_id, struct tree *left, struct tree *right)
{
   assert(op_id >= 0);
   return Operators[op_id].callback(left, right);
}
