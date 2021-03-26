#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib.h"

int main(void)
{
    print_http_headers();
    get_request_vars();

    char * text_input_1_value = get_request_var("text_input_1");
    char * text_input_2_value = get_request_var("text_input_2");

    char * text_input_1_attr = NULL;
    char * text_input_2_attr = NULL;

    if (text_input_1_value == NULL) {
        text_input_1_attr = strdup("type=\"text\" name=\"text_input_1\"");
    }
    else {
        size_t attr_len = strlen("type=\"text\" name=\"text_input_1\" value=\"\"") + strlen(text_input_1_value) + 1;
        text_input_1_attr = calloc(attr_len, sizeof(char));
        snprintf(text_input_1_attr, attr_len, "type=\"text\" name=\"text_input_1\" value=\"%s\"", text_input_1_value);
    }

    if (text_input_2_value == NULL) {
        text_input_2_attr = strdup("type=\"text\" name=\"text_input_2\"");
    }
    else {
        size_t attr_len = strlen("type=\"text\" name=\"text_input_2\" value=\"\"") + strlen(text_input_2_value) + 1;
        text_input_2_attr = calloc(attr_len, sizeof(char));
        snprintf(text_input_2_attr, attr_len, "type=\"text\" name=\"text_input_2\" value=\"%s\"", text_input_2_value);
    }

    char * success_div = strdup("");

    if (!strcmp(text_input_1_value, "text1") && !strcmp(text_input_2_value, "text2")) {
        free(success_div);
        success_div = div(
                        "attr", "style=\"background-color: blue; color: white; font-weight: bold\"",
                        "CONGRATULATIONS YOU DID IT"
                      );
    }

    html(
        head(
            title("Example CGI")
        ),
        body(
            h1("Example CGI Form"),
            success_div,
            form(
                label(
                    "Text Input #1",
                    input(
                        "attr", text_input_1_attr
                    )
                ),
                label(
                    "Text Input #2",
                    input(
                        "attr", text_input_2_attr
                    )
                ),
                input(
                    "attr", "type=\"submit\" value=\"Submit\" name=\"submit\""
                )
            )
        )
    );

    free(success_div);
    free(text_input_1_attr);
    free(text_input_2_attr);
    free_ptrs();
    free_request_vars();
    return 0;
}