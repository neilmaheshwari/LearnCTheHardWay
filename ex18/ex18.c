// Example format for function pointers:
//    int (*POINTER_NAME)(int a, int b)

// Example of calling a function from a
// function pointer:
// int(*tester)(int a, int b) = sorted_order;
// printf("TEST: %d is same as %d\n",
//        tester(2, 3),
//        sorted_order(2, 3));

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *message)
{
    if(errno) {
	perror(message);
    } else {
	printf("ERROR: %s\n", message);
    }

    exit(1);
}

// typedef creating a type for a function pointer
typedef int (*compare_cb)(int a, int b);

// typedef creatign a type for a sorting algorithm
typedef int *(*sort_cb)(int *numbers, int count, compare_cb cmp);

int *bubble_sort(int *numbers, int count, compare_cb cmp)
{    
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) {
 	die("Memory error.");
    };

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
	for(j = 0; j < count - 1; j++) {
	    if(cmp(target[j], target[j+1]) > 0) {
		temp = target[j+1];
		target[j+1] = target[j];
		target[j] = temp;
	    }
	}
    }

    return target;
}

int *insertion_sort(int *numbers, int count, compare_cb cmp)
{
    printf("Starting insertion sort\n");
    int i = 0;
    int j = 0;
    int temp = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) {
	die("Memory error");
    }

    memcpy(target, numbers, count * sizeof(int));

    for(i = 1; i < count; i++) {
	j = i;
	while(j > 0 && cmp(target[j - 1], target[j]) > 0) {
	    temp = target[j];
	    target[j] = target[j-1];
	    target[j-1] = temp;
	    j--;
	}
    }

    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
	return 0;
    } else {
	return a % b;
    }
}

// Test sorting is correct by doing sort then
// printing it out
void test_sorting(int *numbers, int count, sort_cb sort,  compare_cb cmp)
{
    int i = 0;
    int *sorted = sort(numbers, count, cmp);
    if(!sorted) {
	die("Failed to sort as requested");
    }

    for(i = 0; i < count; i++) {
	printf("%d", sorted[i]);
    }
    printf("\n");

    free(sorted);

    unsigned char *data = (unsigned char*)cmp;

    for(i = 0;  i < 25; i++) {
	printf("%02x:", data[i]);
    }

    printf("\n");    
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
	die("USAGE: ex18 4 3 1 5 6");
    }

    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) {
	die("Memory error");
    }

    for(i = 0; i < count; i++) {
	numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, bubble_sort, sorted_order);
    test_sorting(numbers, count, bubble_sort, reverse_order);
    test_sorting(numbers, count, bubble_sort, strange_order);

    test_sorting(numbers, count, insertion_sort, sorted_order);
    test_sorting(numbers, count, insertion_sort, reverse_order);
    test_sorting(numbers, count, insertion_sort, strange_order);
    
    free(numbers);

    return 0;
}
