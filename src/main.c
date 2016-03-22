/*
 * TS3 Vanity Generator
 *
 * Copyright (c) 2016 ECB2
 */

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>

#include <teamspeak/public_definitions.h>
#include <teamspeak/public_errors.h>
#include <teamspeak/clientlib_publicdefinitions.h>
#include <teamspeak/clientlib.h>

#ifdef _WIN32
#define SLEEP(x) Sleep(x)
#define strdup(x) _strdup(x)
#else
#define SLEEP(x) usleep(x*1000)
#endif

char* programPath(char* programInvocation){
	char* path;
	char* end;
	int length;
	char pathsep;

	if (programInvocation == NULL) return strdup("");

#ifdef _WIN32
	pathsep = '\\';
#else
	pathsep = '/';
#endif

	end = strrchr(programInvocation, pathsep);
	if (!end) return strdup("");

	length = (end - programInvocation)+2;
	path = (char*) malloc(length);
	strncpy(path, programInvocation, length-1);
	path[length-1] = 0;

	return path;
}

int main(int argc, char **argv) {
	uint64 scHandlerID;
	unsigned int error;
	char *version;
	char *identity;
	char *identitypub;
	char * path;
	char *theprefix = argv[1];
	int lowercaseIterator;
	struct ClientUIFunctions funcs;
	memset(&funcs, 0, sizeof(struct ClientUIFunctions));
	path = programPath(argv[0]);
	error = ts3client_initClientLib(&funcs, NULL, LogType_USERLOGGING, NULL, path);
	free(path);

    printf("Searching for identitys starting with \"%s\"!\n", theprefix);

    while(1==1) {
	if((error = ts3client_createIdentity(&identity)) != ERROR_ok) {
	    printf("Error creating identity: %d\n", error);
	    return 1;
	}
	lowercaseIterator = 0;
	ts3client_identityStringToUniqueIdentifier(identity, &identitypub);

	while(identitypub[lowercaseIterator]) {
	    identitypub[lowercaseIterator] = tolower(identitypub[lowercaseIterator]);
	    lowercaseIterator++;
	}
	
	if(prefix(theprefix,identitypub) == 1) printf("===Identity found!===\nPublic: %s\nPrivate: %s\n\n",identitypub, identity);
	ts3client_freeMemory(identity);
	identity = NULL;
	ts3client_freeMemory(identitypub);
	identitypub = NULL;
	SLEEP(1);
    }
    getchar();
    return 0;
}
int prefix(const char *pre, const char *str)
{
    if(strncmp(pre, str, strlen(pre)) == 0) return 1;
    return 0;
}
