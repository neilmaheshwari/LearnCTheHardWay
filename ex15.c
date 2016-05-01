#include <stdio.h>

int main(int argc, char *argv[])
{
    // create two arrays we care about
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
	"Allan", "Frank", "Mary", "John", "Lisa"
    };

    /*
      Representation of ages, names, cur_age, and cur_name without fully
      enumerating the names array
      -----------------------------------------------------------------
      | ages     | * ----=======================================>| 23 |
      | names    | * ----|------------------------------------   | 43 |
      | cur_age  | * -----                                   |   | 12 |
      | cur_name | * ---------                               |   | 89 |
                             |                               |   | 2  |
                             --------------------------------===>| A  |   
                                                                 | l  |
                                                                 | l  |  
                                                                 | a  |
                                                                 | n  |                                                                    | \0 |
    */
    
    // save get the size of ages
    int count = sizeof(ages) / sizeof(int);
    int i = 0;

    // first way using indexing
    for(i = 0; i < count; i++) {
	printf("%s has %d years alive.\n", names[i], ages[i]);
    }
    
    printf("---\n");

    // setup the pointers to the start of the arrays
    int *cur_age = (int *) *names;
    char **cur_name = names;

    // second way using pointers
    for (i = 0; i < count; i++) {
	char *cur_age_char = *(cur_name+i);
	printf("%s is %d years old.\n", *(cur_name+i), *(cur_age+i));
	printf("Their first name starts with %c.\n", *(cur_age_char));
    }

    printf("---\n");

    // third way, pointers are just arrays
    for(i = 0; i < count; i++) {
	printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");

    // fourth way with pointers in a stupid complex way
    for(cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++)
    {
	printf("%s lived %d years so far.\n", *cur_name, *cur_age);
    }

    return 0;
}
