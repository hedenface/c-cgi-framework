#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main(int argc, char const *argv[])
{
    /*
    html(head(title("test")), "\n", body(form("attr", "test=\"pooop\"", input("attr", "type=\"text\""))));
    free_ptrs();
    */
    setenv("QUERY_STRING", "key1=val1&key2");
    get_request_vars();
    printf("%s\n", get_request_var("key1"));
    printf("%s\n", get_request_var("key2"));
    free_ptrs();
    free_request_vars();
    return 0;
}
