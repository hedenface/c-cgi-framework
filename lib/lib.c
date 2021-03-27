#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

free_ptr_node * ptr_node_head = NULL;

char ** request_vars = NULL;
int request_var_count = 0;

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
        if (ptr_node_next->ptr != NULL) {
            free(ptr_node_next->ptr);
            ptr_node_next->ptr = NULL;
        }
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

void get_request_vars()
{
    char * query = NULL;
    char * str = getenv("QUERY_STRING");
    int i = 0;

    if (str == NULL) {
        return;
    }

    request_var_count = 1;

    query = strdup(str);
    str = query;
    while (*str) {
        if (*str++ == '&') {
            request_var_count++;
        }
    }

    request_vars = calloc(request_var_count, sizeof(char *));

    str = strtok(query, "&");
    while (str != NULL) {
        request_vars[i++] = strdup(str);
        str = strtok(NULL, "&");
    }

    free(query);
}

void free_request_vars()
{
    int i = 0;

    if (request_vars == NULL) {
        return;
    }

    for (; i < request_var_count; i++) {
        free(request_vars[i]);
    }

    free(request_vars);
}

char * get_request_var(char * lookup_key)
{
    char * key = NULL;
    char * value = NULL;
    char * token = NULL;
    int i = 0;
    int found_key = FALSE;

    if (request_var_count == 0) {
        return NULL;
    }

    for (; i < request_var_count; i++) {
        key = strtok(request_vars[i], "=");

        if (key == NULL) {
            return NULL;
        }

        value = strtok(NULL, "=");

        if (!strcmp(lookup_key, key)) {
            if (value == NULL) {
                value = key;
            }
            found_key = TRUE;
            break;
        }
    }

    if (found_key == TRUE) {
        token = strdup(value);
        add_ptr_to_free_list(token);
    }

    return token;
}

void print_http_headers()
{
    printf("%s\r\n\r\n", "Content-type: text/html; charset=utf-8");
}
