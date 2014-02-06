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
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "global.h"

#define MAX_VARIABLES 100

static struct variable Variables[MAX_VARIABLES];
static int N_variables = 0;

/*************************************************
 * Private functions
 ************************************************/
static int check_variable_name(const char *name)
{
   if(*name == '\0')
      return 0;

   while(*name)
   {
      if(!isalpha(*name))
	 return 0;
      name++;
   }
   return 1;
}

/**************************************************
 * Public functions
 *************************************************/
int get_variable_id(const char *name)
{
   int i;

   for(i = 0; i < N_variables; i++)
   {
      if(!strcmp(Variables[i].name, name))
	 return i;
   }
   return -1;
}

int update_variable(const char *name, double value)
{
   int i;

   i = get_variable_id(name);
   if(i != -1)
   {
      Variables[i].value = value;
   }
   return i;
}

int new_variable(const char *name, double value)
{
   if(check_variable_name(name))
   {
      if(N_variables < MAX_VARIABLES)
      {
	 Variables[N_variables].name = strdup(name);
	 Variables[N_variables].value = value;
	 N_variables++;
	 return N_variables-1;
      }
   }
   return -1;
}

void show_variables(void)
{
   int i;

   for(i = 0; i < N_variables; i++)
   {
      printf("%s = %.2f\n", Variables[i].name, Variables[i].value);
   }
}

void free_variables(void)
{
   int i;

   for(i = 0; i < N_variables; i++)
   {
      free(Variables[i].name);
   }
   N_variables = 0;
}

double get_variable_value(int var_id)
{
   assert(var_id >= 0);
   return Variables[var_id].value;
}

double affect_variable(int var_id, double value)
{
   assert(var_id >= 0);
   Variables[var_id].value = value;

   return value;
}
