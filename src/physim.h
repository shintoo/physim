struct vector {
	double x;
	double y;
};

struct force {
	struct vector cmpnt;
	int time;
};

typedef union vals{
	int intgr;
	double dbl;
} vals;

int findstr(FILE *fp, char *str, fpos_t *strloc);
void readval(FILE *fp, char *str, vals *var);
void vectorread(FILE *fp, fpos_t *loc, struct vector *vect);
void sforces(FILE *fp, const int nforces, struct force forces[]);
void swindow(FILE *fp, const struct vector window[2]);
void stime(FILE *fp, struct vector *time);
void applyforces(const int nforces, const struct force forces[], struct vector objects[], const double mass, const struct vector *time);
void writegraph(FILE * fp, const struct vector objects[], const struct vector window[2], const struct vector *time);
