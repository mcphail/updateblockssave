#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>

#define LIMIT PATH_MAX * 2

int main(int argc, char *argv[])
{
	FILE *fp;
	/* yes, I'm going to hell for this: */
	char filename1[LIMIT], filename2[LIMIT];

	if ( 2 != argc ) {
		fprintf(stderr, "Wrong invocation\n");
		exit(1);
	}

	printf(
"This program will check whether files can be written as a snap updates.\n"
"Install a new version of this snap in another terminal, then press ENTER\n"
"in this terminal and see if files are written.\n\n");
	getchar();

	int rv = 0;
	snprintf(filename1, LIMIT, "%s/%s", argv[1], "file1");
	filename1[LIMIT - 1] = '\0';
	fp = fopen(filename1, "w");
	if ( fp ) {
		fputs("Created from command line path\n", fp);
		fclose(fp);
	} else {
		fprintf(stderr,
		        "Failed to open file passed on command line\n"
			"Check for apparmor denials\n");
		rv++;
	}

	char *fromgetenv = getenv("SNAP_USER_DATA");
	if ( fromgetenv ) {
		snprintf(filename2, LIMIT, "%s/%s", fromgetenv, "file2");
		filename2[LIMIT - 1] = '\0';
		fp = fopen(filename2, "w");
		if ( fp ) {
			fputs("Created from getenv path\n", fp);
			fclose(fp);
		} else {
			fprintf(stderr,
			        "Failed to open file from getenv path\n"
				"Check for apparmor denials\n");
			rv++;
		}
	} else {
		fprintf(stderr,
		        "Couldn't get path from getenv for 2nd file\n");
		rv++;
	}

	exit(rv);
}
