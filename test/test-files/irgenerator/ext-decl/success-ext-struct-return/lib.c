typedef struct { double x, y; } Point;
typedef struct { int a, b, c; } Triple;
typedef struct { long long a, b, c, d; } Large;
Point getOrigin(void) { Point p = {1.5, 2.5}; return p; }
Triple getTriple(void) { Triple t = {3, 4, 5}; return t; }
Large getLarge(long long base) { Large l = {base, base + 1, base + 2, base + 3}; return l; }
