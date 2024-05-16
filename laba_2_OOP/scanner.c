/************************************************************************
*file: scanner.c
* These functions are declared in the include file "scanner.h".
*related files: none
*author: Seletkov Vlad
*written: 04/11/2020
*last modified: 10/11/2020
************************************************************************/

#include "scanner.h"

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#pragma warning(disable : 6053)

/*
the function writes from the csv text file. In the binary
db file of the data about the scanners of the induced structure. 
The csv (Comma Separated Values) file format is transferred 
to the skin row of the file, one record is stored,
and the first field is displayed as a symbol ";"
*/
error_t create_db(const char *csv, const char *db) {
	if (csv == NULL || db == NULL) {
		perror("create_db() csv or db NULL error");
		printf("\n");
		return ERROR;
	}
	FILE *f_ptr_csv = NULL, *f_ptr_db = NULL;
	f_ptr_csv = fopen(csv, "r");
	f_ptr_db = fopen(db, "wb");
	if (f_ptr_csv == NULL || f_ptr_db == NULL) {
		perror("create_db() file_csv or file_db NULL error");
		printf("\n");
		return ERROR;
	}
	SCAN_INFO *scan_ptr = NULL;
	scan_ptr = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	if (scan_ptr == NULL)
		return ERROR;
	int notes = 0, notes_temp = 0, flag = 1, position = 0;
	char temp[256] = { 0 }, check_temp[256] = { 0 };
	fwrite(&notes, sizeof(notes), 1, f_ptr_db);
	while (1) {
		fgets(temp, 256, f_ptr_csv);
		if (feof(f_ptr_csv))
			break;
		position = ftell(f_ptr_csv);
		rewind(f_ptr_csv);
		for (int i = 0; i <= notes_temp - 1; i++) {
			flag = 0;
			fgets(check_temp, 256, f_ptr_csv);
			int j = 0;
			while (temp[j] != '\0' || check_temp[j] != '\0') {
				if (temp[j] != check_temp[j]) {
					flag = 1;
					break;
				}
				j++;
			}
			if (flag == 0)
				break;
		}
		fseek(f_ptr_csv, position, 0);
		notes_temp++;
		if (flag == 1) {
			scan_ptr->id = notes;
			char *str = NULL;
			str = strtok(temp, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->manufacturer = strdup(str);
			str = strtok(NULL, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->year = atoi(str);
			str = strtok(NULL, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->model = strdup(str);
			str = strtok(NULL, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->price = atof(str);
			str = strtok(NULL, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->x_size = atoi(str);
			str = strtok(NULL, ";\n");
			if (str == NULL) {
				printf("create_db() csv error\n");
				free(scan_ptr);
				fclose(f_ptr_csv);
				fclose(f_ptr_db);
				return ERROR;
			}
			scan_ptr->y_size = atoi(str);
			notes++;
			fwrite(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
		}
	}
	rewind(f_ptr_db);
	fwrite(&notes, sizeof(notes), 1, f_ptr_db);
	free(scan_ptr);
	fclose(f_ptr_csv);
	fclose(f_ptr_db);
	return OK;
}

/*
function creates an index file for the specified db file. 
The index file contains the record numbers of the db file,
sorted in descending order by field_name. 
The index file name consists of field_name 
and the idx extension.
*/
int make_index(const char *db, const char *field_name) {
	if (field_name == NULL || db == NULL) {
		perror("make_index() field_name or db NULL error");
		printf("\n");
		return 0;
	}
	char str_idx[25] = { 0 };
	strcpy(str_idx, field_name);
	strcat(str_idx, ".idx");
	FILE *f_ptr_db = NULL, *f_ptr_idx = NULL;
	f_ptr_db = fopen(db, "rb");
	f_ptr_idx = fopen(str_idx, "w");
	//f_ptr_idx = fopen(str_idx, "wb");
	if (f_ptr_db == NULL || f_ptr_idx == NULL) {
		perror("make_index() file NULL error");
		printf("\n");
		return 0;
	}
	int notes = 0, i = 0, k = 0;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	if (notes <= 0) {
		printf("make_index() no records\n");
		return OK;
	}
	SCAN_INFO *scan_ptr = NULL;
	scan_ptr = (SCAN_INFO*)malloc(notes * sizeof(SCAN_INFO));
	while (1) {
		fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
		i++;
		if (feof(f_ptr_db))
			break;
	}
	if (strcmp(field_name, "id") == 0) {
		SCAN_INFO *temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (scan_ptr[k].id > scan_ptr[j].id)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "manufacturer") == 0) {
		SCAN_INFO *temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (strcmp(scan_ptr[k].manufacturer, scan_ptr[j].manufacturer) > 0)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "year") == 0) {
		SCAN_INFO *temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (scan_ptr[k].year > scan_ptr[j].year)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "model") == 0) {
		SCAN_INFO *temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (strcmp(scan_ptr[k].model, scan_ptr[j].model) > 0)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "price") == 0) {
		SCAN_INFO *temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (scan_ptr[k].price > scan_ptr[j].price)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "x_size") == 0) {
		SCAN_INFO* temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (scan_ptr[k].x_size > scan_ptr[j].x_size)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else if (strcmp(field_name, "y_size") == 0) {
		SCAN_INFO* temp = NULL;
		temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
		if (temp == NULL)
			return ERROR;
		for (i = 0; i <= notes - 1; i++) {
			k = i;
			for (int j = i; j <= notes - 1; j++)
				if (scan_ptr[k].y_size > scan_ptr[j].y_size)
					k = j;
			//fwrite(&scan_ptr[k].id, sizeof(scan_ptr[k].id), 1, f_ptr_idx);
			fprintf(f_ptr_idx, "%d ", scan_ptr[k].id);
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
		free(temp);
	}
	else {
		free(scan_ptr);
		fclose(f_ptr_db);
		fclose(f_ptr_idx);
		printf("make_index() field_name error\n");
		return 1;
	}
	free(scan_ptr);
	fclose(f_ptr_db);
	fclose(f_ptr_idx);
	return 1;
}

/*
function returns the RECORD_SET * pointer to the record
set obtained using the index file indx_field.idx.
*/
RECORD_SET *get_recs_by_index(const char *dba, char *indx_field) {
	if (dba == NULL || indx_field == NULL) {
		perror("get_recs_by_index() dba or indx_field NULL error");
		printf("\n");
		return NULL;
	}
	char str_indx[25] = { 0 };
	strcpy(str_indx, indx_field);
	strcat(str_indx, ".idx");
	FILE* f_ptr_db = NULL, * f_ptr_idx = NULL;
	f_ptr_db = fopen(dba, "rb");
	f_ptr_idx = fopen(str_indx, "r");
	//f_ptr_idx = fopen(str_indx, "rb");
	if (f_ptr_db == NULL || f_ptr_idx == NULL) {
		perror("get_recs_by_index() file NULL error");
		printf("\n");
		return NULL;
	}
	int notes = 0, i = 0, k = 0;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	RECORD_SET *rs = NULL;
	SCAN_INFO *scan_ptr = NULL, *temp = NULL;
	scan_ptr = (SCAN_INFO*)malloc(notes * sizeof(SCAN_INFO));
	temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	rs = (RECORD_SET*)malloc(sizeof(RECORD_SET));
	rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes);
	rs->rec_nmb = notes;
	int *data_idx = NULL;
	data_idx = (int*)malloc(sizeof(int) * notes);
	while (1) {
		fscanf(f_ptr_idx, "%d", &data_idx[i]);
		i++;
		if (feof(f_ptr_idx))
			break;
	}
	i = 0;
	while (1) {
		fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
		i++;
		if (feof(f_ptr_db))
			break;
	}
	for (i = 0; i <= notes - 1; i++) {
		k = i;
		for (int j = i; j <= notes - 1; j++) {
			if (data_idx[k] == scan_ptr[j].id) {
				k = j;
				break;
			}
		}
		rs->recs[i] = scan_ptr[k];
		if (i != k) {
			*temp = scan_ptr[i];
			scan_ptr[i] = scan_ptr[k];
			scan_ptr[k] = *temp;
		}
	}
	free(scan_ptr);
	free(temp);
	free(data_idx);
	fclose(f_ptr_db);
	fclose(f_ptr_idx);
	return rs;
}

/*
function creates index files for ALL fields of the SCAN_INFO structure.
*/
void reindex(const char *db) {
	if (db == NULL) {
		perror("reindex() db NULL error");
		printf("\n");
		return;
	}
	make_index(db, "id");
	make_index(db, "manufacturer");
	make_index(db, "year");
	make_index(db, "model");
	make_index(db, "price");
	make_index(db, "x_size");
	make_index(db, "y_size");
}

/*
function removes an entry with the specified id
from the db file. After deleting the record,
you must call the reindex () function.
*/
void del_scanner(const char *db, int id) {
	if (db == NULL || id < 0)
		return;
	FILE *f_ptr_db = NULL;
	f_ptr_db = fopen(db, "r+b");
	if (f_ptr_db == NULL) {
		perror("del_scanner() db file NULL error");
		printf("\n");
		return;
	}
	int notes = 0, pos_del = 0, pos_last = 0, flag = 0, fd_db = 0;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	if (notes <= 0) {
		printf("del_scanner() no records\n");
		return;
	}
	SCAN_INFO *scan_ptr = NULL;
	scan_ptr = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	if (scan_ptr == NULL)
		return;
	while (1) {
		fread(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
		if (feof(f_ptr_db))
			break;
		if (scan_ptr->id == id) {
			pos_del = ftell(f_ptr_db) - sizeof(SCAN_INFO);
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("del_scanner() no id error\n");
		return;
	}
	if (notes == 1) {
		notes--;
		rewind(f_ptr_db);
		fwrite(&notes, sizeof(notes), 1, f_ptr_db);
		fd_db = fileno(f_ptr_db);
		if (chsize(fd_db, sizeof(notes)) != 0) {
			perror("del_scanner() resize error file");
			printf("\n");
			return;
		}
	}
	else { 
		notes--;
		rewind(f_ptr_db);
		fwrite(&notes, sizeof(notes), 1, f_ptr_db);
		fseek(f_ptr_db, 0, SEEK_END);
		pos_last = ftell(f_ptr_db) - sizeof(SCAN_INFO);
		if (pos_last != pos_del) {
			fseek(f_ptr_db, pos_last, SEEK_SET);
			fread(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
			fseek(f_ptr_db, pos_del, SEEK_SET);
			fwrite(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
		}
		fd_db = fileno(f_ptr_db);
		if (chsize(fd_db, sizeof(notes) + (notes * sizeof(SCAN_INFO))) != 0) {
			perror("del_scanner() resize error file");
			printf("\n");
			return;
		}
	}
	free(scan_ptr);
	fclose(f_ptr_db);
	reindex(db);
}

/*
function adds to the db file an entry
from the csv line scanner_str. It is
necessary to ensure the uniqueness
of the id field. Then you need to call
the function reindex ().
*/
void add_scanner(const char *db, const char *scanner_str) {
	if (db == NULL || scanner_str == NULL) {
		perror("add_scanner() db or scanner_str NULL error");
		printf("\n");
		return;
	}
	FILE *f_ptr_db = NULL;
	f_ptr_db = fopen(db, "r+b");
	if (f_ptr_db == NULL) {
		perror("add_scanner() file db error");
		printf("\n");
		return;
	}
	char temp[256] = { 0 };
	int notes = 0, id_max = -1;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	SCAN_INFO *scan_ptr = NULL, *scan_temp = NULL;
	scan_ptr = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	scan_temp = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	strncpy(temp, scanner_str, 256);
	if (scan_ptr == NULL || scan_temp == NULL)
		return;
	char *str = NULL;
	str = strtok(temp, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->manufacturer = strdup(str);
	str = strtok(NULL, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->year = atoi(str);
	str = strtok(NULL, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->model = strdup(str);
	str = strtok(NULL, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->price = atof(str);
	str = strtok(NULL, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->x_size = atoi(str);
	str = strtok(NULL, ";\n");
	if (str == NULL) {
		printf("add_scanner() scanner_str error\n");
		free(scan_ptr);
		free(scan_temp);
		return;
	}
	scan_ptr->y_size = atoi(str);
	while (1) {
		fread(scan_temp, sizeof(SCAN_INFO), 1, f_ptr_db);
		if (feof(f_ptr_db))
			break;
		if (strcmp(scan_temp->manufacturer, scan_ptr->manufacturer) == 0 && scan_temp->year == scan_ptr->year && strcmp(scan_temp->model, scan_ptr->model) == 0 && scan_temp->price == scan_ptr->price && scan_temp->x_size == scan_ptr->x_size && scan_temp->y_size == scan_ptr->y_size) {
			fclose(f_ptr_db);
			return;
		}
		else if (scan_temp->id >= id_max)
			id_max = scan_temp->id;
	}
	notes++;
	id_max++;
	scan_ptr->id = id_max;
	rewind(f_ptr_db);
	fwrite(&notes, sizeof(notes), 1, f_ptr_db);
	fseek(f_ptr_db, 0, SEEK_END);
	fwrite(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
	free(scan_ptr);
	free(scan_temp);
	fclose(f_ptr_db);
}

/*
function reads scanner data from a db file
and outputs it to a *.txt file
*/
void print_db(const char *db) {
	if (db == NULL) {
		perror("print() db NULL error");
		printf("\n");
		return;
	}
	char str_txt[25];
	char *position = NULL;
	strcpy(str_txt, db);
	position = strrchr(str_txt, '.');
	if (position == NULL)
		return;
	*position = '\0';
	strcat(str_txt, ".txt");
	FILE *f_ptr_db = NULL, *f_ptr_txt = NULL;
	f_ptr_db = fopen(db, "rb");
	f_ptr_txt = fopen(str_txt, "w");
	if (f_ptr_db == NULL || f_ptr_txt == NULL) {
		perror("print() db or txt file error");
		printf("\n");
		return;
	}
	SCAN_INFO* scan_ptr = NULL;
	int notes = 0;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	scan_ptr = (SCAN_INFO*)malloc(sizeof(SCAN_INFO));
	if (scan_ptr == NULL)
		return;
	fprintf(f_ptr_txt, "|___________________________________________________________________________________________________|\n");
	fprintf(f_ptr_txt, "|id\t   manufacturer\t\tyear\t      model\t\t price\t        x_size\t      y_size|\n");
	fprintf(f_ptr_txt, "|___________________________________________________________________________________________________|");
	while (1) {
		fread(scan_ptr, sizeof(SCAN_INFO), 1, f_ptr_db);
		if (feof(f_ptr_db))
			break;
		fprintf(f_ptr_txt, "\n|%d\t\t%s\t\t%d\t     %s\t\t%.3f\t\t  %d\t\t%d |", scan_ptr->id, scan_ptr->manufacturer, scan_ptr->year, scan_ptr->model, scan_ptr->price, scan_ptr->x_size, scan_ptr->y_size);
	}
	fprintf(f_ptr_txt, "\n|___________________________________________________________________________________________________|");
	free(scan_ptr);
	fclose(f_ptr_db);
	fclose(f_ptr_txt);
}

/*
function returns a RECORD_SET *pointer to a set of 
records in which the field value matches the value.
*/
RECORD_SET *select(const char *db, const char *field, const char *value) {
	if (db == NULL || field == NULL || value == NULL) {
		perror("select() db or field or value NULL error");
		printf("\n");
		return NULL;
	}
	FILE* f_ptr_db = NULL;
	f_ptr_db = fopen(db, "rb");
	if (f_ptr_db == NULL) {
		perror("select() db file NULL error");
		printf("\n");
		return NULL;
	}
	int notes = 0, notes_value = 0, i = 0, j = 0;
	fread(&notes, sizeof(notes), 1, f_ptr_db);
	if (notes <= 0) {
		printf("select() no records\n");
		return NULL;
	}
	SCAN_INFO *scan_ptr = NULL;
	RECORD_SET *rs = NULL;
	rs = (RECORD_SET*)malloc(sizeof(RECORD_SET));
	scan_ptr = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes);
	if (scan_ptr == NULL || rs == NULL)
		return;
	if (strcmp("id", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (scan_ptr[i].id == atoi(value))
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (scan_ptr[i].id == atoi(value)) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("manufacturer", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (strcmp(scan_ptr[i].manufacturer, value) == 0)
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (strcmp(scan_ptr[i].manufacturer, value) == 0) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("year", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (scan_ptr[i].year == atoi(value))
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (scan_ptr[i].year == atoi(value)) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("model", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (strcmp(scan_ptr[i].model, value) == 0)
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (strcmp(scan_ptr[i].model, value) == 0) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("price", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (scan_ptr[i].price == atof(value))
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (scan_ptr[i].price == atof(value)) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("x_size", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (scan_ptr[i].x_size == atoi(value))
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (scan_ptr[i].x_size == atoi(value)) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else if (strcmp("y_size", field) == 0) {
		while (1) {
			fread(&scan_ptr[i], sizeof(SCAN_INFO), 1, f_ptr_db);
			if (feof(f_ptr_db))
				break;
			if (scan_ptr[i].y_size == atoi(value))
				notes_value++;
			i++;
		}
		rs->recs = (SCAN_INFO*)malloc(sizeof(SCAN_INFO) * notes_value);
		rs->rec_nmb = notes_value;
		for (i = 0; i <= notes - 1; i++) {
			if (scan_ptr[i].y_size == atoi(value)) {
				rs->recs[j] = scan_ptr[i];
				j++;
			}
		}
	}
	else {
		free(rs);
		free(scan_ptr);
		fclose(f_ptr_db);
		perror("select() no field");
		printf("\n");
		return NULL;
	}
	if (notes_value <= 0) {
		free(rs);
		free(scan_ptr);
		fclose(f_ptr_db);
		printf("select() no value\n");
		return NULL;
	}
	free(scan_ptr);
	fclose(f_ptr_db);
	return rs;
}

/*
function prints the contents of rs.
*/
void print_rec_set(RECORD_SET *rs) {
	if (rs == NULL) {
		return;
	}
	printf("|___________________________________________________________________________________________________|\n");
	printf("|id\t   manufacturer\t\tyear\t      model\t\t price\t        x_size\t      y_size|\n");
	printf("|___________________________________________________________________________________________________|");
	for (int i = 0; i <= rs->rec_nmb - 1; i++) {
		printf("\n|%d\t\t%s\t\t%d\t     %s\t\t%.3f\t\t  %d\t\t%d |", rs->recs[i].id, rs->recs[i].manufacturer, rs->recs[i].year, rs->recs[i].model, rs->recs[i].price, rs->recs[i].x_size, rs->recs[i].y_size);
	}
	printf("\n|___________________________________________________________________________________________________|\n\n");
}
