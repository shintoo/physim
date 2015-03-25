typedef struct {
	double x;
	double y;
} vector;

typedef struct {
	vector cmpnt;
	unsigned double time;
} force;

fpos_t findstr(FILE *fp, char *str);
void readval(FILE *fp, char *str, void *var);
void vectorread(FILE *fp, vector *vect);
void sforces(FILE *fp, const int nforces, force forces[]);
void swindow(FILE *fp, const vector window[2]);
void stime(FILE *fp, vector *time);
void applyforces(const int nforces, const force forces[], vector objects[], const double mass, const vector *time);
void display(const vector objects[], const vector window[2], const vector *time);
