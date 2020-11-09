#include <ctype.h>
#include <stdio.h>

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
    return islower(c) || isupper(c);
}

int isblank(int c) {
    return c == ' ' || c == '\t';
}

int iscntrl(int c) {
    // According to standard ASCII character set, control characters are between
    // ASCII codes 0x00 (NUL), 0x1F (US), and 0x7F (DEL).
    return (c >= 0x00 && c <= 0x1F) || (c == 0x7F);
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isgraph(int c) {
    // TODO: This function checks whether the passed character has graphical
    // representation using locale, would be nice to make it do so.
    return isprint(c);
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int isprint(int c) {
    return !iscntrl(c);
}

int ispunct(int c) {
    return isgraph(c) && !isalnum(c);
}

int isspace(int c) {
    return isblank(c) || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

int isxdigit(int c) {
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int tolower(int c) {
    return isupper(c) ? c - ('A' - 'a') : c;
}

int toupper(int c) {
    return islower(c) ? c - ('a' - 'A') : c;
}
