
/*
* File:   Regxp.h
* Author: solution
*
* Created on 11. listopad 2011, 16:00
*/
#include <stdio.h>
#include <string.h>
#include <pcre.h>

#define COUNT(a, b) (sizeof(a)/sizeof(b))

#ifndef REGXP_H
#define	REGXP_H
#define OVECCOUNT 30

struct resulter {
	char *subStringName;
	char *match;
	struct resulter *subMatch;
	struct pcreInfo *info;
};

struct pcreInfo {
	char *pattern;
	int patternGroups;
	int countOfMatches;
};

char *preg_error(int rc);

pcre *preg_compile(char* regxp);

char **preg_match(char *regxp, char *data);

struct resulter **preg_match_all(char *regxp, char *data);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif



#endif	/* REGXP_H */