//By Daniel Dusabimana, Megan Anderson, and Samuel Capotosto
/**  smsh1.c  small-shell version 1
 **             first really useful version after prompting shell
 **             this one parses the command line into strings
 **             uses fork, exec, wait, and ignores signals
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT      "$$$"

int main()
{
  int nRetCode;
  char  *cmdline, *prompt, **arglist;
  int   result;
  void  setup();

  prompt = DFL_PROMPT ;
  setup();

  while ( (cmdline = next_cmd(prompt, stdin)) != NULL )
    {
      if ((arglist = splitline(cmdline)) != NULL)
        {

          // Edit his code: compared command from user to see if he wan to get out.
          if(strcmp(arglist[0], "exit") == 0)
            {
              if(arglist[1] == NULL)
                {
                  exit(0);
                }
              else
                {
                  nRetCode = atoi(arglist[1]);
                  exit(nRetCode);
                }
            }//end of if

          //problem code
          if((strcmp(arglist[0], "cd") == 0)  )
            {
              break;
            }
          else if( strncmp("cd", cmdline, 2) == 0 )
            {
              char *token = strtok(cmdline, " ");
              token = strtok(NULL, " ");

              chdir(token);
            }
          else if (arglist[1] != NULL &&  strncmp(arglist[1], "..", 2))
            {
              system("cd ..");
            }
          else
            {
              system(cmdline);
	      // return 0;
            }
          //end of problem code

	  // result = execute(arglist);
          freelist(arglist);
          free(cmdline);
        }//end of if
      // return 0;
    }
}//end of main

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}
