#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
	printf("ERROR: You need one argument.\n");
	// this is how you abort a program
	return 1;
    }

    int i = 0;
    for(i = 0; argv[1][i] != '\0'; i++) {
	char letter = argv[1][i];

	// Initialize lowercase to surpress warning 
	char lowercase = 0;

	// Chars are ASCII characters, make uppercase letters
	// lower case
	if(letter >= 65 && letter <= 90) {
	    lowercase = letter + 32;
	}
	else if (letter >= 97 && letter <= 122) {
	    lowercase = letter;
	}
	else {
	    printf("ERROR: %c is not a letter\n", letter);
	    return 1;
	}

	switch(lowercase) {
    	    case 'a':
    		printf("%d: 'A'\n", i);
    		break;
    
      	    case 'e':
    	        printf("%d: 'E'\n", i);
    	        break;
    
            case 'i':
    	        printf("%d: 'I'\n", i);
    	        break;
    
    	    case 'o':
    	        printf("%d: 'O'\n", i);
    	        break;
    
    	    case 'u':
    	        printf("%d: 'U'\n", i);
    	        break;
    
    	    case 'y':
    	        if(i > 2) {
    		    // it's only sometimes Y
    		    printf("%d: 'Y'\n", i);
    	        }
    	        break;
    
    	    default:
    	        printf("%d: %c is not a vowel\n", i, letter);
    	}
    }

    return 0;
}
