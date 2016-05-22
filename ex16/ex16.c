#include <stdio.h>

// Include assert.h to have access to assert preprocessor macro
// If a debug symbol is defined, then the assert macro evalutes
// the expression, and if the epxression is false, writes info to
// stderr and calls abort(). Assert is ignored if the debug
// symbol is not defined
#include <assert.h>

// Include stdlib.h to import malloc and free. malloc allocates
// memory on the heap. free releases memory back to the heap
#include <stdlib.h>

// Include string.h to import strdup. strdup duplicates a string
// and allocates memory via malloc that can be freed using free.
#include <string.h>

// Define a struct. A struct is a data type that defines a physically
// grouped list of variables to be placed under one name in a block
// of memory. The different variables can be access via a single
// pointer.
//
// The struct Person has variables name, age, height and weight.
// name is a point to a char, age is an int, height is an int,
// and weight is an int.
struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

// Defines a function Person_create that takes in a char pointer, int,
// int, and int and returns the struct Person
struct Person *Person_create(char *name, int age, int height, int weight)
{
    // sizeof(struct Person) calculates the amount of storage, in
    // bytes, required to store the struct Person.
    // malloc tries to allocote the number of bytes needed to
    // store the struct Person, and returns the pointer to the
    // allocated memory.
    struct Person *who = malloc(sizeof(struct Person));

    // If the memory can not allocate for some reason, malloc returns
    // NULL. In debug, assert will log some data about the failure to
    // stderr and call abort(). abort raises the SIGABRT signal and
    // the abnormal termination of the process unless the SIGBART
    // signal is caught. abort does not return. If the SIGBART signal
    // is ignored, or caught by a handler that returns, the abort()
    // function will still terminate the process
    assert(who != NULL);

    // Duplicate the char array that *name points to. This is so
    // the struct Person owns the string and is in charge of its
    // allocation. Set the variable name in the struct to a pointer
    // pointing to the first character of the duplicated string
    who->name = strdup(name);
    // Set the variables named age, height, and weight respectively.
    // Note that the `->` operator takes a pointer to the struct, and
    // the name of the struct's variable you want to get or set
    who->age = age;
    who->height = height;
    who->weight = weight;

    // Return the struct person with the fields populated
    return who;
}

// Function for destroying (returning memory to the heap) a
// Person
void Person_destroy(struct Person *who)
{
    // Checks if Person is NULL, logs an error to stderr and
    // aborts if the Person is NULL (when debug symbols are defined)
    assert(who != NULL);

    // Frees the memory allocated for the char array containing
    // the persons name. Not doing this will cause a "memory leak"
    free(who->name);

    // Returns memory allocated to store the instance of the Person
    // back to the OS
    free(who);
}

void Person_print_on_stack(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

struct Person Person_create_on_stack(char *name, int age, int height, int weight)
{
    struct Person who = { .name = name, .age = age, .height = height, .weight = weight };
    return who;
}

// Pretty prints a Person
// Function takes in a pointer to a Person
void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[])
{
    // make two people structures
    struct Person *joe = Person_create(
	"Joe Alex", 32, 64, 140);

    struct Person *frank = Person_create(
	"Frank Blank", 20, 72, 180);

    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    // make everyone age 20 years and print them again
    // Increment's joe's age by 20
    joe->age += 20;
    // Decrements joe's height by 2
    joe->height -= 2;
    // Increment Joe's weight by 40
    joe->weight += 40;
    // Pretty prints joe
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    // destroy them both so we clean up
    // Returns memory allocated for both structs to OS
    Person_destroy(joe);
    Person_destroy(frank);

    printf("---\n");

    struct Person neil = Person_create_on_stack("Neil", 21, 68, 145);
    Person_print_on_stack(neil);

    // Returns 0 to tell OS that program exited succesfully
    return 0;
}
