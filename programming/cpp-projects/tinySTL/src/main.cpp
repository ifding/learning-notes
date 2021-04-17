#include "../include/test.h"


// main function, testing components
int main()
{
    test_utility();
    test_memory();
    test_vector();
    test_list();
    test_rbtree();
    test_set();
    test_map();
    test_unordered_map();
    test_stack();
    test_queue();
    test_algorithm();
    std::cout << test_pass << "/" << test_count 
        << " (passed " << test_pass * 100.0 / test_count << "%)" << std::endl;

    return 0;
}