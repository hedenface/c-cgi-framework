#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

free_ptr_node * ptr_node_head;

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

void add_ptr_to_free_list(void * ptr)
{
    free_ptr_node * ptr_node_tmp = NULL;

    if (ptr == NULL) {
        return;
    }

    ptr_node_tmp = malloc(sizeof(* ptr_node_tmp));
    ptr_node_tmp->ptr = ptr;
    ptr_node_tmp->next = NULL;

    if (ptr_node_head == NULL) {
        ptr_node_head = ptr_node_tmp;
    }
    else {
        ptr_node_tmp->next = ptr_node_head;
        ptr_node_head = ptr_node_tmp;
    }
}

void free_ptrs()
{
    free_ptr_node * ptr_node_next = ptr_node_head;
    free_ptr_node * ptr_node_tmp = NULL;

    while (ptr_node_next != NULL) {
        free(ptr_node_next->ptr);
        ptr_node_tmp = ptr_node_next;
        ptr_node_next = ptr_node_next->next;
        free(ptr_node_tmp);
    }
}

char * _element(char * tag, int closing_tag, int num_args, ...)
{
    int i = 0;
    char * arg = NULL;
    char * attr = NULL;
    int in_attr = FALSE;
    int free_attr = FALSE;
    char * inner_html = NULL;
    size_t inner_html_len = 0;
    size_t inner_html_new_len = 0;
    char * html = NULL;
    size_t html_len = 0;
    va_list ap;

    va_start(ap, num_args);

    for (; i < num_args; i++) {

        arg = va_arg(ap, char *);

        if (!strcasecmp(arg, "attr")) {
            in_attr = TRUE;
            continue;
        }

        if (in_attr == TRUE) {
            size_t attr_len = strlen(arg) + 1 + 1;
            attr = calloc(attr_len, sizeof(char));
            free_attr = TRUE;
            snprintf(attr, attr_len, " %s", arg);
            in_attr = FALSE;
            continue;
        }

        if (closing_tag == TRUE) {
            if (inner_html == NULL) {
                inner_html_len = strlen(arg) + 1;
                inner_html = calloc(inner_html_len, sizeof(char));
                snprintf(inner_html, inner_html_len, "%s", arg);
                continue;
            }

            inner_html_new_len = inner_html_len + strlen(arg) + 1;
            inner_html = realloc(inner_html, inner_html_new_len);
            snprintf(inner_html + inner_html_len - 1, inner_html_new_len - inner_html_len + 1, "%s\n", arg);
            inner_html_len = inner_html_new_len;
        }
    }

    va_end(ap);

    if (attr == NULL) {
        attr = "";
    }

    if (closing_tag == TRUE) {
        html_len = strlen("<") + strlen(tag) + strlen(attr) + strlen(">") + strlen(inner_html) + strlen("</") + strlen(tag) + strlen(">") + 1;
        html = calloc(html_len, sizeof(char));
        snprintf(html, html_len, "<%s%s>%s</%s>", tag, attr, inner_html, tag);
    }
    else {
        html_len = strlen("<") + strlen(tag) + strlen(attr) + strlen(" />") + 1;
        html = calloc(html_len, sizeof(char));
        snprintf(html, html_len, "<%s%s />\n", tag, attr);
    }

    if (inner_html != NULL) {
        free(inner_html);
    }

    if (free_attr == TRUE) {
        free(attr);
    }

    add_ptr_to_free_list(html);

    return html;
}

int main(int argc, char const *argv[])
{
    html(head(title("test")), "\n", body(form("attr", "test=\"pooop\"", input("attr", "type=\"text\""))));
    free_ptrs();
    return 0;
}
