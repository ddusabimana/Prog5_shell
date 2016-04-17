//.----------------------------------------------------------------------------
//.
//.smsh1.h
//.
//.----------------------------------------------------------------------------
//.
//.Hash Includes
//.
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
//.
//.Hash Defines
//.
#define   TRUE               (1)
#define   FALSE              (0)
#define   YES                (1)
#define   NO                 (0)
#define   PROMPT_SZ          (32)
#define	  is_delim(x)        ((x) == ' ' || (x) == '\t')
#define   DFL_PROMPT         "\033[7m350:prog5>\033[0m"
#define   DFL_PROMPT_HEAD    "\033[7mprog5:"
#define   DFL_PROMPT_TAIL    ">\033[0m"
//.
//.Function Prototypes
//.
int	process(void);
int execute(char **);
void preptoexit(int *);
void freelist(char **);
void * emalloc(size_t);
int getnumargs(char **);
void setup(int *, char *);
int reapanychildren(void);
char ** splitline(char *);
char * newstr(char *, int);
char * next_cmd(char *, FILE *);
void * erealloc(void *, size_t);
void fatal(char *, char *, int);
int processbuiltincmds(char **);
//.
//.----------------------------------------------------------------------------
//.
