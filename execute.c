//.----------------------------------------------------------------------------
//. execute.c - code used by small shell to execute commands
//.----------------------------------------------------------------------------
//.
//.Hash Includes
//.
#include "smsh1.h"
//.
//.
//.Purpose : Run a program passing it arguments
//.Returns : Status returned via wait, or -1 on error
//.Errors  : -1 on fork or wait errors
//.
int execute(char * argv[])
{
	int	ret, pid, argc;
	int isbg, child_info;
	//.
	child_info = -1;
	ret = pid = argc = isbg = 0;
	argc = getnumargs(argv);
	//.printf("argc : %d\n", argc);
	//.
	if(argc < 1) {
		return(0);
	}
	pid = getpid();
	if(strcmp(argv[argc - 1], "&") == 0) { //.Background process ?
		isbg = 1; //.Child needs to be a background process
		argv[argc - 1] = 0x0; //.Nix the '&' modifier for child
		argc = getnumargs(argv);
	}
	if((pid = fork()) == -1) { //.Fork
		perror("Error : fork ");
	}
	else if(pid == 0) { //.If Child
		//.setpgid(0, 0); //.Change Process-Group
		pid = getpid();
		signal(SIGINT, SIG_DFL); //.Default signal action
		signal(SIGQUIT, SIG_DFL); //.Default signal action
		//.if(isbg == 1) { //.Only if background process
		//.	printf("[PID : %d : %s ] - Starting in background\n", pid, argv[0]); //.Print Child's PID & Name
		//.}
		execvp(argv[0], argv); //.Exec Process
		perror("Error : execute : execvp ");
		exit(-1);
	}
	else { //.If Parent, wait for child to complete
		//.printf("parent:execute\n");
		if(isbg == 0) { //.However, wait only if child is a foreground process
			ret = waitpid(pid, &child_info, 0); //.Wait for child to complete
			if(ret == -1) {
				perror("Error : execute : wait ");
			}
			else { //.For debugging only
				if(WIFEXITED(child_info)) {
					//.printf("[PID : %d ] - Completed Normally.\n", pid);
				}
				else {
					//.printf("[PID : %d ] - Completed Abnormally.\n", pid);
				}
			}
		}
		else {
			printf("[PID : %d : %s ] - Started in background\n", pid, argv[0]); //.Print Child's PID & Name
		}
		ret = reapanychildren(); //.Reap any completed children
	}
	//.
	return(child_info);
}


int reapanychildren(void)
{
	int ret, status;
	//.
	ret = status = 0;
	//.printf("reapanychildren\n");
	ret = waitpid(-1, &status, WNOHANG); //.Check if any children have completed
	if(ret == 0) { //.No child has completed
		return(0); //.Return, nothing to see here
	}
	if(ret == -1) {
		if(errno != ECHILD) {
			perror("Error : reapanychildren : wait ");
		}
	}
	else { //.At least one child has completed
		if(WIFEXITED(status)) { //.Was it Good ?
			printf("[Bg-PID : %d ] - Completed Normally.\n", ret);
		}
		else {
			printf("[Bg-PID : %d ] - Completed Abnormally.\n", ret);
		}
	}
	return(ret);
}


int getnumargs(char ** arglist)
{
	int argc;
	char ** argv;
	//.
	argc = 0;
	argv = arglist;
	while(*argv) {
		//.printf("%s\n", *argv);
		argc++;
		argv++;
	}
	return(argc);
}
//.
//.----------------------------------------------------------------------------
//.
