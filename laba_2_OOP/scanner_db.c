#include "scanner.h"

int main() {
	create_db("scanners.csv", "scanners.db");
	print_db("scanners.db");
	reindex("scanners.db");
	add_scanner("scanners.db", "ASUS;2009;LimVAS9;989.50;417;417");
	print_db("scanners.db");
	del_scanner("scanners.db", 5);
	print_db("scanners.db");
	RECORD_SET* rs = select("scanners.db", "manufacturer", "ASUS");
	print_rec_set(rs);
	if (rs != NULL) {
		free(rs->recs);
		free(rs);
	}
	del_scanner("scanners.db", 3);
	print_db("scanners.db");
	rs = get_recs_by_index("scanners.db", "year");
	print_rec_set(rs);
	if (rs != NULL) {
		free(rs->recs);
		free(rs);
	}
	return OK;
}
