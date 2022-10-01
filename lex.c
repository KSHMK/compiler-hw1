#include "lex.h"

int check_1_9(char data)
{
    return '1' <= data && data <= '9';
}

int check_0(char data)
{
    return data == '0';
}

int check_0_9(char data)
{
    return check_0(data) || check_1_9(data);
}

int check_a_z(char data)
{
    return 'a' <= data && data <= 'z';
}

int check_A_Z(char data)
{
    return 'A' <= data && data <= 'Z';
}

int check_a_zA_Z(char data)
{
    return check_a_z(data) || check_A_Z(data);
}

int check_dot(char data)
{
    return data == '.';
}

int check_double_quote(char data)
{
    return data == '"';
}

int check_plus(char data)
{
    return data == '+';
}

int check_minus(char data)
{
    return data == '-';
}

int check_asterisk(char data)
{
    return data == '*';
}

int check_slash(char data)
{
    return data == '/';
}

int check_equal(char data)
{
    return data == '=';
}

int check_colon(char data)
{
    return data == ':';
}

int check_semicolon(char data)
{
    return data == ';';
}

int check_space(char data)
{
    return data == ' ';
}

int check_tab(char data)
{
    return data == '\t';
}

int check_enter(char data)
{
    return data == '\n';
}

int check_nothing(char data)
{
    return data != '\n';
}