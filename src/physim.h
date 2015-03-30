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
void swindow(FILE *fp, struct vector window[2]);
void stime(FILE *fp, struct vector *time);
void applyforces(const int nforces, const int numob, struct vector object[numob], const struct force forces[nforces], const double mass, const struct vector *time);
void writegraph(FILE * fp, const int nobj, const struct vector object[nobj], const struct vector window[2], const struct vector *time);
