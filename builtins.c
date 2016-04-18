//.----------------------------------------------------------------------------
//. builtins.c - code used by small shell to process builtins
//.----------------------------------------------------------------------------
//.
//.Hash Includes
//.
#include "smsh1.h"
//.
//.
//.Externs
//.
extern char * cmdline;
//.
//.Purpose : Process all built-in commands
//.Returns : = 0 if cmd was not processed
//.          > 0 if cmd processed & was a success
//.Errors  : < 0 if cmd processed & there was an error
//.
int processbuiltincmds(char ** arglist)
{
	int nRetCode;
	//.
	nRetCode = 0;
	//.
	if(arglist[0] == NULL) { //.Empty Command-Line
		return(-1);
	}
	if(strcmp(arglist[0], "exit") == 0) { //.Exit -----------------------------
		nRetCode = 1;
		if(arglist[1] == NULL) {
			exit(0);
		}
		else {
			nRetCode = atoi(arglist[1]);
			exit(nRetCode);
		}
	}
	//.
	if(strcmp(arglist[0], "cd") == 0) { //.Change Directory -------------------
		nRetCode = 2;
		if(arglist[1] == NULL) {
			//printf("Specify a directory to change to.\n");
		  chdir(getenv("HOME"));
		  //	nRetCode = 2;
		}
		else {
			chdir(arglist[1]);
			if(errno < 0) { //.Catch Errors
				printf("cd Error : %s\n", strerror(errno));
				nRetCode = -2;
			}
		}
	}
	//.
	return(nRetCode);
}
//.
//.----------------------------------------------------------------------------
//.
