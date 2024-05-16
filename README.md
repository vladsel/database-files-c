# Database.
The goal of the work is to master the methods of processing disk files. (04.11.2020)
2.1. Theoretical information.
The following library functions may be useful when completing the task.
//changes the file size.
int _chsize( int fd, long size ); // required header <io.h>
Parameters
fd-file descriptor referring to open file.
size-new length of file in bytes.

fd can be obtained using the function
//gets the file descriptor associated with a stream.
int _fileno( FILE *stream);
Parameters
stream - pointer to FILE structure.

2.2. Job assignment.
Data about flatbed scanners are stored in the view structure
typedef struct {
int id;//unique identifier
char* manufacturer;// manufacturer
int year; // year of manufacture
char *model; // name of the model
float price; // price
int x_size; // horizontal size of the scanning area
int y_size;// vertical size of the scanning area
} SCAN_INFO;

1. Write a function, create_db(const cha* csv, const char *db) that writes from a csv text file. in the binary file db data about the scanners of the given structure. The csv (Comma Separated Values) file format assumes that each line of the file stores one record, and its fields are separated from each other by the ";" character.
Example,
HP;JetScan;2007;959.99;210;297;300
ASUS;AsScan;2009;299.99;210;297;100

The structure of the db file: in the first sizeof( int ) bytes, an int type value is placed, which determines the number of records in the file, then, without gaps, records about scanners are placed. An id field of int type is added to each line from the csv, which is a unique record identifier. The function should ensure that there are no duplicate entries in the file.
When writing it, you may need the following library functions:
// Find a substring.
char *strstr( const char *string1, const char *string2 );//<string.h>
Return Value
Returns a pointer to the first occurrence of string2 in string1, or NULL if string2 does not appear in string1. If string2 points to a string of zero length, the function returns string1.
Parameters
string1 - Null-terminated string to search
string2 - Null-terminated string to search for
Example:
char *delimiter = “;”;
char *rec = "HP;MyScanner1;999.99;210;297;300;16;";
ptr = strstr(rec, delimiter);

Conversion functions:
//<math.h> or <stdlib.h>
Convert strings to double (atof), integer (atoi), or long (atol).
double atof( const char *string );
int atoi( const char *string );
long atol( const char *string );
Parameters
string - string to be converted
Return Value
Each function returns the double, int, or long value produced by interpreting the input characters as a number. The return value is 0 (for atoi), 0L (for atol), or 0.0 (for atof) if the input cannot be converted to a value of that type. The return value is undefined in case of overflow.

2. Write a function
//returns 1 on success and 0 otherwise
int make_index(const char *db, const char *field_name),
which creates an index file for the given db file. The index file contains the record numbers of the db file, sorted in non-descending order by field_name. The name of the index file consists of field_name and the extension idx. For example, a db file contains the following records

HP;JetScan7;2007;959.99;210;297;300
ASUS;AsScan9;2009;299.99;210;297;100
HP;LaserScan9;2009;959.99;210;297;100
ASUS;AsScan7;2007;999.99;210;297;300
Dell;DellScan1;2006;999.99;210;297;200
Dell;DellScan2;2005;999.99;210;297;150

Then, after calling make_index(“Scanner.db”, “manufacturer”), the manufacturer.idx file should contain the following sequence of record numbers
3 1 4 5 0 2

3. Write the function RECORD_SET * get_recs_by_index(const char *dba , char *indx_field ), which returns the RECORD_SET pointer * to the set of records obtained using the index file indx_field.idx. A recordset is defined by the following structure

typedef struct {
int rec_nmb;//number of records
SCAN_INFO *recs;//records
}RECORD_SET;

4. Write a void reindex(const char *db) function that creates index files on ALL fields of the SCAN_INFO structure.

5. Write the void del_scanner(const char *db, int id) function, which deletes the record with the given id from the db file. After deleting the record, you need to call the reindex() function.

6. Write the void add_scanner(const char *db, const char* scanner_str) function, which adds a csv record of the scanner_str line to the db file. At the same time, it is necessary to ensure the uniqueness of the id field. Then you need to call the reindex() function.
7. Write a void print_db(const char *db) function that reads data about scanners from a db file and outputs them to a *.txt file.
8. Write the function RECORD_SET* select(const char *db, const char *field, const char *value), which returns the RECORD_SET * pointer to a set of records in which the value of the field matches value. Example,
RECORD_SET* rs = select("Scanners.db", "y_size", "297");
9. Write a void print_rec_set(RECORD_SET *rs) function that prints the contents of rs.
