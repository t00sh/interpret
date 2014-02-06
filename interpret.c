#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"

/********************************************
 * Internal commands
 *******************************************/


int handle_commands(const char *buffer)
{
   if(!strcmp(buffer, "variables"))
      show_variables();
   else if(!strcmp(buffer, "quit"))
      exit(0);
   else if(!strcmp(buffer, "delete_vars"))
      free_variables();
   else
      return 0;

   return 1;
}
/********************************************
 * Miscelleanous functions
 *******************************************/
void delete_spaces(char *buff)
{
   char *ptr;
   
   ptr = buff;
   while(*buff)
   {
      *ptr = *buff;
      if(!isspace(*ptr))
	 ptr++;
      buff++;
   }
   *ptr = '\0';
}

void delete_comments(char *buff)
{
   char *p;
   
   if((p = strchr(buff, '#')) != NULL)
   {
      *p = '\0';
   }
}

void chomp(char *buff)
{
   char *p;

   if((p = strchr(buff, '\n')) != NULL)
   {
      *p = '\0';
   }
}

void print_prompt(void)
{
   printf("> ");
   fflush(stdout);
}
/*********************************************
 * Entry point program
 ********************************************/
int main(void)
{
   int error;
   char buffer[1024];
   int line_num;


   line_num = 0;
   error = 0;
   print_prompt();

   while(fgets(buffer, sizeof(buffer), stdin) != NULL)
   {
      line_num++;
      chomp(buffer);
      delete_spaces(buffer);
      delete_comments(buffer);

      if(*buffer)
      {
	 printf(" '%s'\n", buffer);
	 eval_line(buffer, &error);
	 if(error)
	 {
	    printf("error at line %d !\n", line_num);
	    return EXIT_FAILURE;
	 }
	 print_prompt();
      }

   }

   free_variables();

   return EXIT_SUCCESS;
}
