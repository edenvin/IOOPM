#ifndef __format_h
#define __format_h
/*
 * format.h
 *
 * This file contains the public specifications for the format string
 * used as a course project for the 2012 IOOP/M course.
 *
 */
#include <imalloc.h>

typedef char* format_string;

/* 
 * Returns the size needed equivalent to store the arguments in str, e.g. if str = d then return the size of an int
 */
chunk_size format_string_to_size (format_string str);

 #endif