#include <arguments_mini.h>

#include <stdio.h>


/*
void my_on_print(char* a)
{
	fputs(a, stdout);
}
*/

//int main(int, char**)
int main(int unusedArgc, char** unusedArgv)
{
	//char validArgv[][] =
	char* validArgv[] =
		{
			"", //< i.e. 1st parameter will be executable name..?
			"pear",
			"-color=red",
			"-tint=gray",
			"--recycle"
		};
	//int validArgc = length(validArgv);
	int validArgc = 1+4;

	//char invalidArgv[][] =
	char* invalidArgv[] =
		{
			"", //< i.e. 1st parameter will be executable name..?
			"apple",
			"pear" //< 2 arguments (not allowed)
			//OR
			//"-color" //< missing '=' character
			//OR
			//"-color=red",
			//"-color=yellow" //< twice the same parameter (not allowed)
			//OR
			//"-color=red",
			//"--color" //< twice the same parameter (not allowed)
			//OR
			//"---" //< invalid parameter (starts with --- <- not allowed)
			//OR
			//"-" //< invalid parameter (missing parametername)
			//OR
			//"--" //< invalid parameter (missing parametername)
		};
	//int invalidArgc = length(invalidArgv);
	int invalidArgc = 1+2;

	// NOTE: don't set on_print callback (not testing error logging here)
	// TODO: test callbacks too
	if(am_parse(validArgc, validArgv) != 1)
	{
		fputs("error: am_parse failed with valid argv (false negative)\n", stdout);
		return 1;
	}

	//am_set_on_print(&my_on_print);

	if(am_parse(invalidArgc, invalidArgv) != 0)
	{
		fputs("error: am_parse succeeded with invalid argv (false positive)\n", stdout);
		return 1;
	}

	return 0;
}
