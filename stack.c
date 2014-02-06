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


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "global.h"

/*
while(a>2)
   if(7>2)
      a=7
   end
end


L[0] = WHILE(a>2) DEPTH 1
L[1] = IF(7>2)    DEPTH 2
L[2] = a=7        DEPTH 2
L[3] = END        DEPHT 1
L[4] = END        DEPHT 0
*/

struct instruction
{
   struct tree *tree;
   int jmp;
};

#define MAX_INSTR_LIST 150

static struct instruction Instr_list[MAX_INSTR_LIST];
static int Instr_size = 0;
static int Current_depth = 0;


/****************************************
 * Private functions
 ***************************************/
static void free_instr_list(void)
{
   int i;

   for(i = 0; i < Instr_size; i++)
   {
      free_tree(Instr_list[i].tree);
   }
   Instr_size = 0;
}


static int get_next_end(int eip)
{
   int level;
   enum type type;

   level = 0;
   
   while(eip < Instr_size)
   {
      type = Instr_list[eip].tree->token.type;
      if(type == IF || type == WHILE)
      {
	 level++;
      }
      else if(type == END)
      {
	 level--;
	 if(level == 0)
	    return eip;
      }
      eip++;
   }
   return -1;
}

/* Create the jmp for the WHILE and the IF */
static void create_the_jmps(void)
{
   enum type type;
   int eip;
   int jmp;

   for(eip = 0; eip < Instr_size; eip++)
   {
      type = Instr_list[eip].tree->token.type;
      if(type == WHILE)
      {
	 jmp = get_next_end(eip);
	 Instr_list[eip].jmp = jmp;
	 Instr_list[jmp].jmp = eip;
      }
      else if(type == IF)
      {
	 jmp = get_next_end(eip);
	 Instr_list[eip].jmp = jmp;
	 Instr_list[jmp].jmp = -1;
      }
      else if(type != END)
      {
	 Instr_list[eip].jmp = -1;
      }
   }
}

/* Execute the instr liste */
static void exec_all_instr(void)
{
   int eip;
   enum type type;
   
   eip = 0;
   while(eip < Instr_size)
   {
      type = Instr_list[eip].tree->token.type;
      if(type == IF)
      {
	 if(!((int)calc_tree(Instr_list[eip].tree)))
	    eip = Instr_list[eip].jmp+1;
	 else
	    eip++;
      }
      else if(type == WHILE)
      {
	 if(!((int)calc_tree(Instr_list[eip].tree)))
	    eip = Instr_list[eip].jmp+1;
	 else
	    eip++;
      }
      else if(type == END)
      {
	 if(Instr_list[eip].jmp != -1)
	    eip = Instr_list[eip].jmp;
	 else
	    eip++;
      }
      else
      {
	 calc_tree(Instr_list[eip].tree);
	 eip++;
      }
   }
}
/*******************************************
 * Public functions
 ******************************************/
void exec_instr_list(void)
{
   if(Current_depth == 0)
   {
      create_the_jmps();
      exec_all_instr();
      free_instr_list();
   }
}

void push_tree(struct tree *tree)
{
   assert(tree != NULL);

   if(Instr_size < MAX_INSTR_LIST)
   {
      Instr_list[Instr_size].tree = tree;
      Instr_size++;
      if(tree->token.type == IF || tree->token.type == WHILE)
      {
	 Current_depth++;
      }
      else if(tree->token.type == END)
      {
	 Current_depth--;
      }
   }
}
