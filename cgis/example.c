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
        text_input_1_attr = strdup("type=\"text\" name=\"text_input_1\" class=\"form-control\"");
    }
    else {
        size_t attr_len = strlen("type=\"text\" name=\"text_input_1\" class=\"form-control\" value=\"\"") + strlen(text_input_1_value) + 1;
        text_input_1_attr = calloc(attr_len, sizeof(char));
        snprintf(text_input_1_attr, attr_len, "type=\"text\" name=\"text_input_1\" class=\"form-control\" value=\"%s\"", text_input_1_value);
    }

    if (text_input_2_value == NULL) {
        text_input_2_attr = strdup("type=\"text\" name=\"text_input_2\" class=\"form-control\"");
    }
    else {
        size_t attr_len = strlen("type=\"text\" name=\"text_input_2\" class=\"form-control\" value=\"\"") + strlen(text_input_2_value) + 1;
        text_input_2_attr = calloc(attr_len, sizeof(char));
        snprintf(text_input_2_attr, attr_len, "type=\"text\" name=\"text_input_2\" class=\"form-control\" value=\"%s\"", text_input_2_value);
    }

    char * success_div = strdup("");

    if (text_input_1_value != NULL && text_input_2_value != NULL
        && !strcmp(text_input_1_value, "text1") && !strcmp(text_input_2_value, "text2")) {
        free(success_div);
        success_div = div(
                        "attr",
                            "class=\"alert alert-primary\"",
                        "CONGRATULATIONS YOU DID IT"
                      );
    }

    html(
        head(
            title("Example CGI"),
            link(
                "attr",
                    "href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css\" "
                    "rel=\"stylesheet\" "
                    "integrity=\"sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6\" "
                    "crossorigin=\"anonymous\""
            )
        ),
        body(
            div(
                "attr",
                    "class=\"container\"",
                h1("Example CGI Form"),
                success_div,
                form(
                    div(
                        "attr",
                            "class=\"form-group\"",
                        label(
                            "Text Input #1",
                            input(
                                "attr", text_input_1_attr
                            )
                        )
                    ),
                    div(
                        "attr",
                            "class=\"form-group\"",
                        label(
                            "Text Input #2",
                            input(
                                "attr", text_input_2_attr
                            )
                        )
                    ),
                    div(
                        "attr",
                            "style=\"margin-top: 1em\"",
                        input(
                            "attr",
                                "type=\"submit\" "
                                "value=\"Submit\" "
                                "name=\"submit\" "
                                "class=\"btn btn-primary\""
                        )
                    )
                )
            )
        )
    );

    free(text_input_1_attr);
    free(text_input_2_attr);
    free_ptrs();
    free_request_vars();
    return 0;
}
