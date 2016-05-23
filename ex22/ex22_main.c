#include "ex22.h"
#include "dbg.h"

const char *MY_NAME = "Neil";

void scope_demo(int count)
{
    log_info("count is: %d", count);

    if(count > 10) {
	int count = 100; // BAD! BUGS!

	log_info("count in this scope is %d", count);
    }

    log_info("count is at exit: %d", count);

    count = 3000;

    log_info("count after assign: %d", count);
}

void pass_by_value_demo(int value)
{
    value = value * value;
    log_info("value in function: %d", value);    
}

void pass_by_reference_demo(int *reference)
{
    int temp = 0;
    temp = *reference * *reference;
    *reference = temp;
    log_info("reference in function: %d", *reference);
}

int main(int argc, char *argv[])
{
    // test out THE_AGE accessors
    log_info("My name is: %s, age: %d", MY_NAME, get_age());

    set_age(100);

    log_info("My age is now: %d", get_age());

    // test out THE_SIZE extern
    log_info("The_SIZE is: %d", THE_SIZE);
    print_size();

    THE_SIZE = 9;

    log_info("THE SIZE is now: %d", THE_SIZE);
    print_size();

    // test the ratio function static
    log_info("Ratio at first: %f", update_ratio(2.0));
    log_info("Ratio again: %f", update_ratio(10.0));
    log_info("Ratio once more: %f", update_ratio(300.0));

    // test the scope demo
    int count = 4;
    scope_demo(count);
    scope_demo(count * 20);

    log_info("count after calling scope_demo: %d", count);

    int value = 3;
    log_info("value before and after pass_by_value_demo should be the same");
    log_info("value before calling pass_by_value_demo: %d", value);
    pass_by_value_demo(value);
    log_info("value after calling pass_by_value_demo: %d", value);

    int temp = 3;
    int *reference = &temp;

    log_info("value before and after pass_by_reference_demo should be different");
    log_info("value before calling pass_by_reference: %d", *reference);
    pass_by_reference_demo(reference);
    log_info("value after calling pass_by_reference_demo: %d", *reference);
    return 0;
}    
