//.----------------------------------------------------------------------------
//.Daniel Dusabimana, Samuel Capotosto, & Megan Anderson
//.
//.smsh1.c - small-shell Version 1.0
//.After prompting, the shell parses the command line into strings & uses fork,
//.exec, wait, & ignores signals
//.----------------------------------------------------------------------------
//.
//.Hash Includes
//.
#include "smsh1.h"
//.
//.
//.
int main(int argc, char ** argv)
{
	int pid;
	int nResult;
	char * cmdline;
	char ** arglist;
	char prompt[PROMPT_SZ];
	//.
	memset(prompt, 0x0, PROMPT_SZ);
	strcpy(prompt, DFL_PROMPT);
	setup(&pid, prompt);
	while((cmdline = next_cmd(prompt, stdin)) != NULL) {
		if((arglist = splitline(cmdline)) != NULL) {
			nResult = processbuiltincmds(arglist); //.Try to process builtins
			if(nResult != 0) {
				freelist(arglist);
				free(cmdline);
				nResult = reapanychildren(); //.Reap any completed children
				continue;
			}
			nResult = execute(arglist); //.!-builtin
			freelist(arglist);
			free(cmdline);
		}
	}
	nResult = reapanychildren(); //.Reap any completed children
	return(0);
}


//.
//.Purpose : Initialize shell - ignore [Ctrl+C] & [Ctrl+\]
//.Returns : PID. calls fatal() on error
//.
void setup(int * ppid, char * strPromptBuff)
{
	int nPid;
	char strPid[PROMPT_SZ];
	//.
	nPid = getpid();
	*ppid = nPid;
	memset(strPid, 0x0, PROMPT_SZ);
	memset(strPromptBuff, 0x0, PROMPT_SZ);
	sprintf(strPid, "%d", nPid);
	strcat(strPromptBuff, DFL_PROMPT_HEAD);
	strcat(strPromptBuff, strPid);
	strcat(strPromptBuff, DFL_PROMPT_TAIL);
	//.
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}


//.
//.Purpose : Initialize shell - ignore [Ctrl+C] & [Ctrl+\]
//.Returns : PID. calls fatal() on error
//.
void preptoexit(int * ppid)
{
	//.
}


//.
//.Purpose : Prints error & exits
//.Returns : Nothing
//.
void fatal(char * s1, char * s2, int n)
{
	fprintf(stderr, "Error : %s, %s\n", s1, s2);
	exit(n);
}
//.
//.----------------------------------------------------------------------------
//.
