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
#include "global.h"

static struct tree* eval(char *expr, int *error);

/********************************************
 * Private functions
 *******************************************/
/******************************************** 
   =========== GRAMMAR ============

   E = expression
   F = function
   R = float
   o = operator
   V = variable

   E --> if(E)
   E --> while(E)
   E --> end
   E --> (E)
   E --> F(E)
   E --> V=E
   E --> R
   E --> V
   E --> EoE

   o --> [+ - / ^ * % > < <= >= == !=]{1}
   V --> [A-Za-z]+
   F --> [A-Za-z]+
   R --> -{0,1}[0-9.]+

*******************************************/


/* E --> (E) */
static struct tree*  eval_brace(char **expr, int *error)
{
   int level, min_level;
   char *ptr;

   ptr = *expr;

   if(*ptr == '(')
   {
      min_level = 1;
      level = 0;

      while(*ptr != '\0')
      {
	 if(*ptr == '(')
	    level++;
	 else if(*ptr == ')')
	    level--;

	 if(min_level > level && *(ptr+1) != '\0')
	    min_level = level;

	 ptr++;
      }
      if(min_level == 1 && **expr == '(' && *(ptr-1) == ')')
      {
	 **expr = '\0';
	 *(ptr-1) = '\0';
	 (*expr)++;
	 return eval(*expr, error);
      }
   }
   return NULL;
}

/* E --> if(E) */
struct tree* eval_if(char *expr, int *error)
{
   struct tree *tree = NULL;
   struct token token;

   if(!strncmp(expr, "if(", 3))
   {
      expr += 2;
      tree = eval_brace(&expr, error);
      if(tree != NULL)
      {
	 token.type = IF;
	 return create_node(tree, &token, NULL);
      }
   }
   return tree;
}

static struct tree* eval_while(char *expr, int *error)
{
   struct tree *tree = NULL;
   struct token token;

   if(!strncmp(expr, "while(", 6))
   {
      expr += 5;
      tree = eval_brace(&expr, error);
      if(tree != NULL)
      {
	 token.type = WHILE;
	 return create_node(tree, &token, NULL);
      }
   }
   return tree;
}
/* E --> end */
static struct tree* eval_end(char *expr)
{
   struct token token;

   if(!strcmp(expr, "end"))
   {
      token.type = END;
      return create_node(NULL, &token, NULL);
   }
   return NULL;
}

/* E --> F(E) */
static struct tree* eval_function(char *expr, int *error)
{
   int id;
   int len;
   struct token token;
   struct tree *tree = NULL;
   char *p;

   if((p = strchr(expr, '(')) != NULL)
   {
      len = p-expr;
      id = get_function_id(expr, len);
      if(id != -1)
      {
	 token.type = FUNCTION;
	 token.u_type.fun_id = id;
	 expr += len;
	 tree = eval_brace(&expr, error);
	 if(tree == NULL)
	 {
	    return NULL;
	 }
	 else
	 {
	    return create_node(tree, &token, NULL);
	 }
      }
   }
   return NULL;
}
/* E --> V=E */
static struct tree* eval_affectation(char *expr, int *error)
{
   struct token token;
   char *ptr;

   if((ptr = strchr(expr, '=')) != NULL)
   {
      *ptr = '\0';
      if(get_variable_id(expr) != -1 || new_variable(expr, 0.0) != -1)
      {
	 token.type = AFFECTATION;
	 return create_node(eval(expr, error), &token, eval(ptr+1, error));
      }
      *ptr = '=';
   }
   return NULL;
}

/* E --> EoE */
static struct tree* eval_operator(char *expr, int *error)
{
   int level;
   int operator_id;
   struct token token;
   char *left, *right;

   token.type = UNDEF;
   level = 0;
   left = right = expr;
   
   while(*expr)
   {
      operator_id = char_to_operator(*expr);
      if(operator_id != -1)
      {
	 if(level == 0)
	 {
	    if(token.type == UNDEF || (priority(operator_id) <= priority(token.u_type.op_id)))
	    {
	       token.type = OPERATOR;
	       token.u_type.op_id = operator_id;
	       right = expr;
	    }
	 }
      }
      else if(*expr == '(')
      {
	 level++;
      }
      else if(*expr == ')')
      {
	 level--;
      }
      expr++;
   }

   if(token.type != UNDEF)
   {
      *(right++) = '\0';
      return create_node(eval(left, error), &token, eval(right, error));
   }

   return NULL;
}

/* E --> V */
static struct tree* eval_variable(char *expr)
{
   int var_id;
   struct token token;

   var_id = get_variable_id(expr);
   if(var_id != -1)
   {
      token.type = VARIABLE;
      token.u_type.var_id = var_id;
      return create_node(NULL, &token, NULL);
   }
   return NULL;
}

/* E --> R */
static struct tree* eval_float(char *expr)
{
   struct token token;
   
   token.type = VALUE;
   token.u_type.val = strtod(expr, &expr);

   if(*expr == '\0')
   {
      return create_node(NULL, &token, NULL);
   }

   return NULL;
}

/* Recursive evaluation */
static struct tree* eval(char *expr, int *error)
{
   struct tree *tree = NULL;

#ifdef DEBUG
   printf("----> '%s'\n", expr);
#endif
   if(*expr == '\0')
      *error = 1;

   if(*error == 0)
   {  
      if((tree = eval_brace(&expr, error)));
      else if((tree = eval_function(expr, error)));
      else if((tree = eval_affectation(expr, error)));
      else if((tree = eval_float(expr)));	
      else if((tree = eval_variable(expr)));
      else if((tree = eval_operator(expr, error)));
      else
      {
	 *error = 1;
      }
   }
   return tree;
}

/******************************************
 * Public functions
 *****************************************/
void eval_line(char *expr, int *error)
{
   struct tree *tree;

   if((tree = eval_end(expr)));
   else if((tree = eval_if(expr, error)));
   else if((tree = eval_while(expr, error)));
   else ((tree = eval(expr, error)));
   
   if(tree == NULL || *error)
   {
      *error = 1;
      return;
   }

   push_tree(tree);
   exec_instr_list();
}
