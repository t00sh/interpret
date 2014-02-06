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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "global.h"

void free_tree(struct tree *tree)
{
   if(tree != NULL)
   {
      free_tree(tree->left);
      free_tree(tree->right);
      free(tree);
   }
}

struct tree* create_node(struct tree *left, struct token *token, struct tree *right)
{
   struct tree *node;

   if((node = malloc(sizeof(struct tree))) == NULL)
   {
      perror("malloc ");
      exit(EXIT_FAILURE);
   }
   
   node->token = *token;
   node->left = left;
   node->right = right;

   return node;
}

double calc_tree(struct tree *tree)
{
   assert(tree != NULL);

   switch(tree->token.type)
   {
      case OPERATOR:
	 return exec_operator(tree->token.u_type.op_id, tree->left, tree->right);
      case VALUE:
	 return tree->token.u_type.val;
      case FUNCTION:
	 return exec_function(tree->token.u_type.fun_id, tree->left);
      case VARIABLE:
	 return get_variable_value(tree->token.u_type.var_id);
      case AFFECTATION:
	 return affect_variable(tree->left->token.u_type.var_id, calc_tree(tree->right));
      case WHILE:
      case IF:
	 return calc_tree(tree->left);
      case END:
      case UNDEF:
	 return 0.0;
   }
   return 0.0;
}
