cmake_minimum_required(VERSION 3.0.0)
project(parser VERSION 0.1.0)

add_executable(parser main.c parse.c data_set.c lex.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
