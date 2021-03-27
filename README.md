# C CGI Framework

> A lightweight and basic framework for building and processing CGI forms in C

[![Build Status](https://travis-ci.org/hedenface/c-cgi-framework.svg?branch=main)](https://travis-ci.org/hedenface/c-cgi-framework)

## Quick Example

```bash
git clone https://github.com/hedenface/c-cgi-framework
cd c-cgi-framework
./configure
make lib
cd cgis
make example
sudo make install-example
```

> *Note:* This will install `example.cgi` to either `/usr/lib/cgi-bin` or `/var/www/cgi-bin` (if the first is not detected). Otherwise, you can specify your `cgi-bin/` during `./configure` by specifying `--with-cgidir`

Now navigate to http://localhost/cgi-bin/example.cgi. If you're lucky, you should see a super cool (and modern) bootstrap form:

![Example Form](https://github.com/hedenface/c-cgi-framework/blob/main/media/example.png)

Now, take a moment to read the source code to see what is expected of you. ***JUST KIDDING***. Type `text1` in the first text box, and `text2` in the second text box, and then submit the form. You should now be greeted with something like the following:

![Example Submissed Form](https://github.com/hedenface/c-cgi-framework/blob/main/media/example_submissed.png)

## Details and Boring API Stuff

The library (`lib/lib.c` and corresponding `include/lib.h`) is broken into two parts: [HTML Building](#html-building) and [Query String Parsing](#query-string-parsing). These subjects are broken down in the following subsections.

### HTML Building

The full list of available elements can be found in [include/lib.h](https://github.com/hedenface/c-cgi-framework/blob/main/include.lib.h). These are all essentially wrappers to the `_element()` function. There are 2 kinds of HTML tags:

1. The kind that has a closing tag
1. The kind that doesn't have a closing tag

For the kind that has a closing tag you'll notice the wrapper `element_with_content()`, and for the kind without a closing tag, you'll notice `element_without_content()`. As an example, the well-known `<h1>` tag expects a closing tag (`</h1>`). So, if we were going to define an `h1()` wrapper, we would call `element_with_content()`. *In **fact***: this is exactly what we do! Check it out:

```c
#define h1(...) element_with_content("h1", __VA_ARGS__)
```

Which expands to:

```c
element("h1", TRUE, __VA_ARGS__)
```

Which then expands to:

```c
_element("h1", TRUE, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)
```

Kewl!

So let's talk about the variadic arguments that are passed into these macros and ultimately the behind-the-scenes function. Inside of an element that has content (an element with a corresponding closing tag), then any of the arguments passed are wrapped in that elements tags. For example:

```c
h1("this is some text")
```

Would return `<h1>this is some text<h1>`, whereas:

```c
h1("this is some text", "OH CRAP SO IS THIIIIIS WHAAATTT?!?!")
```

Would return `<h1>this is some textOH CRAP SO IS THIIIIIS WHAAATTT?!?!<h1>` (*please note the lack of a space between the two strings*).

This example is rather trivial when we think about an `h1` tag, but it becomes a bit more powerful perhaps with a series of nested `div` tags:

```c
div(div(h1("first")), div(h2("second")), div(h3("third")))
```

Would result in the following HTML being returned (spacing and newlines added by yours truly):

```html
<div>
    <div>
        <h1>first</h1>
    </div>
    <div>
        <h2>second</h2>
    </div>
    <div>
        <h3>third</h3>
    </div>
</div>
```

So far, all we've talked about are elements which have closing tags. Elements that **don't** have closing tags only expect a special set of arguments.

If you pass an argument `attr`, then the element will process ***the following argument*** as the attributes of that element. Perhaps we should see an example!

```c
div("attr", "class=\"div-class\"", h1("text"))
```

Will generate

```html
<div class="div-class"><h1>text</h1></div>
```

Whoops! That was an element with content! How silly of me. Here's an example of an `input` tag:

```c
input("attr", "type=\"text\" value=\"\"");
```

Will generate (please note that there is no content between `input` tags):

```html
<input type="text" value="" />
```

### Query String Parsing

When you want to build a CGI executable, you'll need to start by calling `print_document_headers()` and `get_request_vars()`.

You can obtain the values of specific query string variables using the `get_request_var(key)` lookup function.

When you're done, you'll need to call `free_ptrs()` followed by `free_request_vars()`.

#### `print_document_headers()`

HTTP expects at the very least a `Content-Type` header. This function prints that header.

#### `get_request_vars()`

This function looks through the `QUERY_STRING` environment variable and places each key/value (or simply key) into a global variable called `request_vars`.

#### `get_request_var(char * lookup_key)`

This function will look through the request variables (`request_vars`) **ONLY AFTER THEY HAVE BEEN INITIALIZED WITH `get_request_vars()`** and return the appropriate value. If there is no match, then `NULL` is returned.

#### `free_ptrs()`

Along the way, all of the element wrapper functions return allocated memory. Instead of doing fancy management upon return, we just keep track of the memory in a linked list. This function walks the list and frees the pointers as it walks.

#### `free_request_vars()`

This frees up the memory that the `request_vars` variable allocates.

## Who Would Write Something Like This?

Sadists.

## Who Would Use Something Like This?

Masochists.

## Why Would I Consider Using This Framework?

Please see the detailed responses in sections [Who Would Write Something Like This?](#who-would-write-something-like-this) and [Who Would Use Something Like This?](#who-would-use-something-like-this).

***JUST KIDDING***. This framework makes it rather trivial to grab request variables. This framework also makes it rather trivial to build functional HTML very quickly - not only that, but if it's missing an element (it's likely missing a whole #@!$load) it is **extremely trivial** to add new elements.
