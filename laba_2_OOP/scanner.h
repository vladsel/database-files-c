/************************************************************************
*file: scanner.h
*purpose: declarations for scanner functions, types, constants, structures
*author: Seletkov Vlad
*written: 04/11/2020
*last modified: 10/11/2020
*************************************************************************/

#ifndef LABA_2_OOP_SCANNER_H
#define LABA_2_OOP_SCANNER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

typedef enum {
    OK, ERROR
} error_t;

typedef struct {
	int id; //unique identifier
	char *manufacturer; //manufacturer
	int year; //year of manufacture
	char *model; //model name
	float price; //price
	int x_size; //the horizontal size of the scan area
	int y_size; //the vertical size of the scan area
} SCAN_INFO;

typedef struct {
	int rec_nmb; //number of records
	SCAN_INFO* recs; //records 
}RECORD_SET;

/* function prototypes*/

error_t create_db(const char *csv, const char *db);

int make_index(const char *db, const char *field_name);

RECORD_SET* get_recs_by_index(const char *dba, char *indx_field);

void reindex(const char *db);

void del_scanner(const char *db, int id);

void add_scanner(const char *db, const char *scanner_str);

void print_db(const char *db);

RECORD_SET *select(const char *db, const char *field, const char *value);

void print_rec_set(RECORD_SET *rs);

#endif //LABA_2_OOP_SCANNER_H
