#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    // go through each string in argv

    int i = 0;
    while(i < argc) {
        printf("arg %d: %s\n", i, argv[i]);
        i++;
    }

    // let's make our own array of strings
    char *states[] = {
        "California",
        "Oregon", 
        "Washington",
        "Texas"
    };

    int num_states = 4;
    i = 0; // watch for this
    while(i < num_states) {
        printf("state %d: %s\n", i, states[i]);
	if(i == 3){
	    break;
	}
	i++;
    }

    // Copy values from argv into states
    // This demonstrates that we are not copying the strings
    i = 0;
    while(i < num_states){
        states[i] = argv[i];
        i++;
    }

    argv[2][0] = 'x';

    printf ("was the string copied? %s\n", states[2]);
}
