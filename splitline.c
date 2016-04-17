//.----------------------------------------------------------------------------
//.splitline.c - commmand reading & parsing functions for smsh
//.char ** splitline(char * str) - parse a string
//.char * next_cmd(char * prompt, FILE * fp) - get next command
//.----------------------------------------------------------------------------
//.
//.Hash Includes
//.
#include "smsh1.h"
//.
//.Purpose : Read next command line from fp
//.Returns : Dynamically allocated string holding command line
//.Errors  : NULL at EOF ( NaE ) calls fatal from emalloc()
//.Notes   : Allocates space in BUFSIZ chunks
//.
char * next_cmd(char * prompt, FILE * fp)
{
	int	c; //.Input char
	int	pos = 0; //.Current position
	int	bufspace = 0; //.Total size
	char * buf; //.Buffer
	//.
	printf("%s", prompt); //.Print Prompt
	while((c = getc(fp)) != EOF) {
		if(pos + 1 >= bufspace) { //.Need space ? 1 for 0
			if(bufspace == 0) { //.y: 1st time
				buf = emalloc(BUFSIZ);
			}
			else { //.Or expand
				buf = erealloc(buf, bufspace + BUFSIZ);
			}
			bufspace += BUFSIZ; //.Update size
		}
		if(c == '\n') { //.End of command ?
			break;
		}
		buf[pos++] = c; //.No, add to buffer
	}
	if((c == EOF) && (pos == 0)) { //.EOF & No input
		return(NULL);
	}
	buf[pos] = '\0'; //.Say so
	return(buf);
}


//.
//.Purpose : split a line into array of white-space separated tokens
//.Returns : a NULL-terminated array of pointers to copies of the tokens
//.          or NULL if line if no tokens on the line
//.Action  : Traverse the array, locate strings, make copies
//.Note    : strtok() could work, but we may want to add quotes later
//.
char ** splitline(char * line)
{
	int	len;
	int	spots; //.Spots in table
	int	argnum; //.Slots used
	int	bufspace; //.Bytes in table
	char * cp; //.Pos in string
	char * start;
	char ** args;
	//.
	cp = line;
	spots = argnum = bufspace = 0;
	if(line == NULL) { //.Handle special case
		return(NULL);
	}
	bufspace = BUFSIZ;
	args = emalloc(BUFSIZ); //.Initialize array
	spots = BUFSIZ / sizeof(char *);
	while(*cp != '\0') {
		while(is_delim(*cp)) { //.Skip leading spaces
			cp++;
		}
		if(*cp == '\0')	{ //.Quit at End-of-String
			break;
		}
		if((argnum + 1) >= spots) { //.Make sure the array has room (+1 for NULL)
			args = erealloc(args, bufspace + BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ / sizeof(char *));
		}
		start = cp; //.Mark start, then find end of word
		len = 1;
		while((*++cp != '\0') && (!(is_delim(*cp)))) {
			len++;
		}
		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;
	return(args);
}


//.
//.Purpose : Constructor for strings
//.Returns : A string, never NULL
//.
char * newstr(char * s, int l)
{
	char * rv;
	//.
	rv = emalloc(l + 1);
	rv[l] = '\0';
	strncpy(rv, s, l);
	return(rv);
}


//.
//.Purpose : Free the list returned by splitline
//.Returns : Nothing
//.Action  : Free all strings in list & then free the list
//.
void freelist(char ** list)
{
	char ** cp;
	//.
	cp = list;
	while(*cp) {
		free(*cp++);
	}
	free(list);
}


void * emalloc(size_t n)
{
	void *rv;
	//.
	if((rv = malloc(n)) == NULL) {
		fatal("Out of Memory", "", 1);
	}
	return(rv);
}


void * erealloc(void * p, size_t n)
{
	void * rv;
	//.
	if((rv = realloc(p, n)) == NULL) {
		fatal("realloc failed", "", 1);
	}
	return(rv);
}
//.
//.----------------------------------------------------------------------------
//.
