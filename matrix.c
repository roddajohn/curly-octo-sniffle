#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

/*-------------- struct matrix *new_matrix() --------------
Inputs:  int rows
         int cols 
Returns: 

Once allocated, access the matrix as follows:
m->m[r][c]=something;
if (m->lastcol)... 
*/
struct matrix *new_matrix(int rows, int cols) {
  double **tmp;
  int i;
  struct matrix *m;

  tmp = (double **)malloc(rows * sizeof(double *));
  for (i=0;i<rows;i++) {
      tmp[i]=(double *)malloc(cols * sizeof(double));
    }

  m=(struct matrix *)malloc(sizeof(struct matrix));
  m->m=tmp;
  m->rows = rows;
  m->cols = cols;
  m->lastcol = 0;

  return m;
}


/*-------------- void free_matrix() --------------
Inputs:  struct matrix *m 
Returns: 

1. free individual rows
2. free array holding row pointers
3. free actual matrix
*/
void free_matrix(struct matrix *m) {

  int i;
  for (i=0;i<m->rows;i++) {
      free(m->m[i]);
    }
  free(m->m);
  free(m);
}


/*======== void grow_matrix() ==========
Inputs:  struct matrix *m
         int newcols 
Returns: 

Reallocates the memory for m->m such that it now has
newcols number of collumns
====================*/
void grow_matrix(struct matrix *m, int newcols) {
  
  int i;
  for (i=0;i<m->rows;i++) {
      m->m[i] = realloc(m->m[i],newcols*sizeof(double));
  }
  m->cols = newcols;
}


/*-------------- void print_matrix() --------------
Inputs:  struct matrix *m 
Returns: 

print the matrix
*/
void print_matrix(struct matrix *m) {

  int i, j;
  for (i=0; i < m->rows; i++) {
    for (j=0; j < m->cols; j++) 
      printf("%f ", m->m[i][j]);
    printf("\n");
  }
}

/*-------------- void ident() --------------
Inputs:  struct matrix *m <-- assumes m is a square matrix
Returns: 

turns m in to an identity matrix
*/
void ident(struct matrix *m) {

  int r, c;
  for (r=0; r < m->rows; r++) {
    for (c=0; c < m->cols; c++)  {

      if ( r == c )
	m->m[r][c] = 1;
      else
	m->m[r][c] = 0;
    }
  }
}


/*-------------- void scalar_mult() --------------
Inputs:  double x
         struct matrix *m 
Returns: 

multiply each element of m by x
*/
void scalar_mult(double x, struct matrix *m) {

  int r, c;
  for (r=0; r < m->rows; r++) 
    for (c=0; c < m->cols; c++)  {
      
      double t = m->m[r][c];
      m->m[r][c] = t * x;
    }
}


/*-------------- void matrix_mult() --------------
Inputs:  struct matrix *a
         struct matrix *b 
Returns: 

a*b -> b
*/
void matrix_mult(struct matrix *a, struct matrix *b) {
  int r, c;
  struct matrix * tmp;
  
  tmp = new_matrix(4, 1);
  
  for (c=0; c < b->cols; c++)  {
    for (r=0; r < 4; r++) 
      tmp->m[r][0] = b->m[r][c];

    for (r=0; r < 4; r++) {
      b->m[r][c] =  a->m[r][0] * tmp->m[0][0] + 
	a->m[r][1] * tmp->m[1][0] +
	a->m[r][2] * tmp->m[2][0] +
	a->m[r][3] * tmp->m[3][0];
    }
  }
  
  free_matrix(tmp);
}



/*-------------- void copy_matrix() --------------
Inputs:  struct matrix *a
         struct matrix *b 
Returns: 

copy matrix a to matrix b
*/
void copy_matrix(struct matrix *a, struct matrix *b) {

  int r, c;

  for (r=0; r < a->rows; r++) 
    for (c=0; c < a->cols; c++)  
      b->m[r][c] = a->m[r][c];  
}

/*======== struct matrix * make_translate() ==========
Inputs:  int x
         int y
         int z 
Returns: The translation matrix created using x, y and z 
as the translation offsets.
====================*/
struct matrix * make_translate(double x, double y, double z) {

  struct matrix * m = new_matrix(4, 4);
  ident(m);

  m->m[0][3] = x;
  m->m[1][3] = y;
  m->m[2][3] = z;

  return m;
}

/*======== struct matrix * make_scale() ==========
Inputs:  int x
         int y
         int z 
Returns: The translation matrix creates using x, y and z
as the scale factors
====================*/
struct matrix * make_scale(double x, double y, double z) {

  struct matrix * m = new_matrix(4, 4);
  ident(m);

  m->m[0][0] = x;
  m->m[1][1] = y;
  m->m[2][2] = z;

  return m;
}

/*======== struct matrix * make_rotX() ==========
Inputs:  double theta

Returns: The rotation matrix created using theta as the 
angle of rotation and X as the axis of rotation.
====================*/
struct matrix * make_rotX(double theta) {

  struct matrix * m = new_matrix(4, 4);
  ident(m);

  m->m[1][1] = cos( theta );
  m->m[1][2] = -1 *  sin( theta );
  m->m[2][1] = sin( theta );
  m->m[2][2] = cos( theta );
  return m;
}

/*======== struct matrix * make_rotY() ==========
Inputs:  double theta
         char c 
Returns: The rotation matrix created using theta as the 
angle of rotation and Y as the axis of rotation.
====================*/
struct matrix * make_rotY(double theta) {

  struct matrix * m = new_matrix(4, 4);
  ident(m);
    
    m->m[0][0] = cos( theta );
    m->m[0][2] = -1 * sin( theta );
    m->m[2][0] = sin( theta );
    m->m[2][2] = cos( theta );

  return m;
}

/*======== struct matrix * make_rotZ() ==========
Inputs:  double theta
         char c 
Returns: The rotation matrix created using theta as the 
angle of rotation and Z as the axis of rotation.
====================*/
struct matrix * make_rotZ(double theta) {

  struct matrix * m = new_matrix(4, 4);
  ident(m);

  m->m[0][0] = cos( theta );
  m->m[0][1] = -1 *  sin( theta );
  m->m[1][0] = sin( theta );
  m->m[1][1] = cos( theta );

  return m;
}

/*======== struct matrix * make_bezier()) ==========
  Inputs:   
  Returns: 

  The correct 4x4 matrix that can be used to generate
  the coefiecients for a bezier curve

  03/16/12 14:36:19
  jdyrlandweaver
  ====================*/
struct matrix * make_bezier() {

  struct matrix *m = new_matrix(4, 4);
  ident(m);

  m->m[0][0] = -1;
  m->m[0][1] = 3;
  m->m[0][2] = -3;
  m->m[0][3] = 1;

  m->m[1][0] = 3;
  m->m[1][1] = -6;
  m->m[1][2] = 3;
  m->m[1][3] = 0;

  m->m[2][0] = -3;
  m->m[2][1] = 3;
  m->m[2][2] = 0;
  m->m[2][3] = 0;

  m->m[3][0] = 1;
  m->m[3][3] = 0;

  return m;
}

/*======== struct matrix * make_hermite()) ==========
  Inputs:   
  Returns: 

  The correct 4x4 matrix that can be used to generate
  the coefiecients for a hermite curve

  03/16/12 14:36:19
  jdyrlandweaver
  ====================*/
struct matrix * make_hermite() {

  struct matrix *m = new_matrix(4, 4);
  ident(m);

  m->m[0][0] = 2;
  m->m[0][1] = -2;
  m->m[0][2] = 1;
  m->m[0][3] = 1;

  m->m[1][0] = -3;
  m->m[1][1] = 3;
  m->m[1][2] = -2;
  m->m[1][3] = -1;

  m->m[2][0] = 0;
  m->m[2][1] = 0;
  m->m[2][2] = 1;
  m->m[2][3] = 0;

  m->m[3][0] = 1;
  m->m[3][3] = 0;

  return m;
}

/*======== struct matrix * generate_curve_coefs() ==========
  Inputs:   double p1
            double p2
	    double p3
	    double p4
	    int type
  Returns: 
  
  A matrix containing the values for a, b, c and d of the
  equation at^3 + bt^2 + ct + d for the curve defined 
  by p1, p2, p3 and p4.
  
  Type determines whether the curve is bezier or hermite

  03/16/12 14:42:46
  jdyrlandweaver
  ====================*/
struct matrix * generate_curve_coefs( double p1, double p2, 
				      double p3, double p4, int type) {
  
  struct matrix * inverse;
  struct matrix * coefs;

  if ( type == BEZIER_MODE )    
    inverse = make_bezier();
  else
    inverse = make_hermite();

  coefs = new_matrix(4, 1);

  if ( type == BEZIER_MODE ) {
    coefs->m[0][0] = p1;
    coefs->m[1][0] = p2;
    coefs->m[2][0] = p3;
    coefs->m[3][0] = p4;
  }  

  else {
    coefs->m[0][0] = p1;
    coefs->m[1][0] = p3;
    coefs->m[2][0] = p2 - p1;
    coefs->m[3][0] = p4 - p3;
  }
 
  matrix_mult(inverse, coefs);

  free_matrix(inverse);
    
  return coefs;  
}

