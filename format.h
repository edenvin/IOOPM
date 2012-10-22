#ifndef __format_h
#define __format_h
/*
 * format.h
 *
 * This file contains the public specifications for the formatstring
 * used as a course project for the 2012 IOOP/M course.
 *
 */
#include <imalloc.h>
typedef char* formatstring;

/* 
 *Returns the size needed equivalent to the flag, e.g. flag d returns the size of an int
 */
chunk_size flag_to_size (char *flag);

 #endif