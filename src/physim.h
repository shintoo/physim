typedef struct vector {
	double x;
	double y;
} Vector;

typedef struct force {
	struct vector cmpnt;
	int time;
} Force;

typedef union vals{
	int intgr;
	double dbl;
} vals;

int FindString(FILE *fp, char *str, fpos_t *strloc);
void ReadValue(FILE *fp, char *str, vals *var);
void ReadVector(FILE *fp, fpos_t *loc, Vector *vect);
void ReadForces(FILE *fp, const int NumForces, Force forces[]);
void ReadWindow(FILE *fp, Vector window[2]);
void ReadTime(FILE *fp, Vector *time);
void ApplyForces(const int NumForces, const int NumObj, Vector object[NumObj], const Force forces[NumForces], const double mass, const Vector *time);
void WriteGraph(FILE *fp, const int NumObj, const Vector object[NumObj], const Vector window[2], const Vector *time);
