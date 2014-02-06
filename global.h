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


#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H
enum type;
union u_type;
struct token;
struct tree;

/********************************************
 * Types
 *******************************************/

/*
OPERATOR :
  o
 / \
E   E

VALUE :
  o
 / \
|| ||

FUNCTION :
  f
 / \
E  ||

VARIABLE :
  v
 / \
|| ||

AFFECTATION :
  a
 / \
v   E

WHILE :
  w
 / \
E  ||

IF :
  i
 / \
E  ||

END : 
  e
 / \
|| ||

 */

enum type
{
   UNDEF,
   OPERATOR,
   VALUE,
   FUNCTION,
   VARIABLE,
   AFFECTATION,
   WHILE,
   IF,
   END
};



struct variable
{
   char *name;
   double value;
};

union u_type
{
   /* Type OPERATOR */
   int op_id;
   /* Type VALUE */
   double val;
   /* Type FUNCTION */
   int fun_id;
   /* Type VARIABLE */
   int var_id;
   /* Type WHILE, IF */
   int depth;
};

struct token
{
   enum type type;
   union u_type u_type;
};

struct tree
{
   struct token token;

   struct tree *left;
   struct tree *right;
};

/*************************************
 * Prototypes
 ************************************/
double calc_tree(struct tree *tree);
int get_function_id(const char *name, int name_len);
void free_variables(void);
double calc(char *expr, int *error);
int get_variable_id(const char *name);
struct tree* create_node(struct tree *l, struct token *t, struct tree *r);
int char_to_operator(int c);
int priority(int op_id);
int new_variable(const char *name, double val);
void free_tree(struct tree *tree);
void show_variables(void);
double exec_operator(int op_id, struct tree *l, struct tree *r);
double exec_function(int fun_id, struct tree *t);
double get_variable_value(int var_id);
double affect_variable(int var_id, double value);
void eval_line(char *expr, int *error);
void push_tree(struct tree *tree);
void exec_instr_list(void);

#endif
