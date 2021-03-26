#ifndef _C_CGI_FRAMEWORK_LIB_H
#define _C_CGI_FRAMEWORK_LIB_H

#ifdef FALSE
#  undef FALSE
#endif
#ifdef TRUE
#  undef TRUE
#endif
#define TRUE 1
#define FALSE 0

typedef struct free_ptr_node_t {
    void * ptr;
    struct free_ptr_node_t * next;
} free_ptr_node;

void add_ptr_to_free_list(void * ptr);
void free_ptrs();
char * _element(char * tag, int closing_tag, int num_args, ...);
void get_request_vars();
void free_request_vars();
char * get_request_var(char * lookup_key);

#define NUM_ARGS(...) (sizeof((char *[]){__VA_ARGS__})/sizeof(char *))
#define element(tag, closing_tag, ...) _element(tag, closing_tag, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#define element_with_content(tag, ...) element(tag, TRUE, __VA_ARGS__)
#define element_without_content(tag, ...) element(tag, FALSE, __VA_ARGS__)

#define html(...) printf("<!doctype html>\n\n%s\n", _html(__VA_ARGS__))
#define _html(...) element_with_content("html", __VA_ARGS__)

#define head(...) element_with_content("head", __VA_ARGS__)
#define title(...) element_with_content("title", __VA_ARGS__)

#define body(...) element_with_content("body", __VA_ARGS__)
#define form(...) element_with_content("form", __VA_ARGS__)
#define label(...) element_with_content("label", __VA_ARGS__)
#define input(...) element_without_content("input", __VA_ARGS__)
#define textarea(...) element_with_content("textarea", __VA_ARGS__)

#define h1(...) element_with_content("h1", __VA_ARGS__)
#define h2(...) element_with_content("h2", __VA_ARGS__)
#define h3(...) element_with_content("h3", __VA_ARGS__)
#define h4(...) element_with_content("h4", __VA_ARGS__)
#define h5(...) element_with_content("h5", __VA_ARGS__)
#define h6(...) element_with_content("h6", __VA_ARGS__)

#endif
