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
    int *cur_age = ages;
    char **cur_name = names;

    // second way using pointers
    for (i = 0; i < count; i++) {
	printf("%s is %d years old.\n", *(cur_name+i), *(cur_age+i));
    }

    printf("---\n");

    // third way, pointers are just arrays
    for(i = 0; i < count; i++) {
	printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");

    // fourth way with pointers in a stupid complex way
    for(cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++){
	printf("%s lived %d years so far.\n", *cur_name, *cur_age);
    }

    printf("---\n");
    
    // fifth way using indexing, starting at end of the array
    for (i = 0; i < count; i++){
	printf("[Backwards] %s has %d years alive.\n",
	       names[count - 1 - i], ages[count - 1 - i]);
    }

    printf("---\n");

    // sixth way using pointers, starting at end of the array
    cur_age = ages;
    cur_name = names;
    for (i = 0; i < count; i++){
	printf("[Backwards] %s is %d years old.\n",
	       *(cur_name + count - i - 1),
	       *(cur_age + count - i - 1));
    }

    printf("---\n");

    // seventh way, pointers are just arrays, starting at end of
    // the array
    for(i =0; i < count; i++) {
	printf("[Backwards] %s is %d years old again.\n",
	       cur_name[count - i - 1],
	       cur_age[count - i - 1]);
    }
    printf("---\n");

    // eighth way, with pointers in a stupid complex way,
    // starting at the end of the array
    for(cur_name = names + count - 1, cur_age = ages + count - 1;
	(cur_age - ages) >= 0; cur_name--, cur_age--) {
	printf("[Backwards] %s lived %d so far.\n",
	       *cur_name, *cur_age);
    }
    
    return 0;
}
