#include "fbgc.h"



struct fbgc_object * new_fbgc_matrix_object(fbgc_token sub_tp, size_t row, size_t col, char tp){
	size_t len = row*col;

	// if sub_tp == DOUBLE than open only an array with sz length
	// if sub_tp == COMPLEX than open an array twice length as double, first array will contain real numbers while second will hold imaginary part

/*  struct fbgc_matrix_object * mo = (tp == DOUBLE) 
	(struct fbgc_matrix_object*) fbgc_malloc_object(sizeof(struct fbgc_matrix_object) + (sizeof(double) * len) ) :
	(struct fbgc_matrix_object*) fbgc_malloc_object(sizeof(struct fbgc_matrix_object) + (sizeof(struct raw_complex) * len) ) :
*/

	struct fbgc_matrix_object * mo = (struct fbgc_matrix_object*) fbgc_malloc_object(sizeof(struct fbgc_matrix_object) + (1 + (sub_tp == COMPLEX) ) *(sizeof(double) * len) );

	mo->base.type = MATRIX;
	mo->row = row;
	mo->column = col;
	mo->sub_type = sub_tp;

	//tp == -1 => do not initialize;
	//tp == 0 => zeros
	//tp == 1 => ones
	//tp == 2 => eye
	double * ct = content_fbgc_matrix_object(mo);

	switch(tp){
		case ZEROS_MATRIX:
		case ONES_MATRIX:
		{
			//tp = zero for ZEROS
			//tp = one for ones
			double x = tp*(1.0);
			if(sub_tp == COMPLEX) len *= 2;
			while(len--)
			{
				*(ct+len) = x;
			}

			break;            
		}
		case EYE_MATRIX:
		{
			for(size_t i = 0; i<row; i++){
				for(len = 0; len<col; len++){
					*(ct + i*col + len ) = (i == len) ? 1.0 : 0;
				}
			}
			break;
		}
	}
	return (struct fbgc_object *) mo;
}

struct fbgc_object * new_fbgc_matrix_object_from_range(struct fbgc_object * robj){

	#define r cast_fbgc_object_as_range(robj)

	fbgc_token tok = MAX(r->start->type,r->end->type);
	if(r->step != NULL) tok = MAX(r->step->type,tok);

	if(tok == INT){
		//if tok is int we are absolutely sure that each element of range object is integer..
		int step = (r->step == NULL) ? 1 : cast_fbgc_object_as_int(r->step)->content; 
		if(r->step == NULL && cast_fbgc_object_as_int(r->start)->content > cast_fbgc_object_as_int(r->end)->content)  step = -1;

		if( (step > 0 && cast_fbgc_object_as_int(r->start)->content >= cast_fbgc_object_as_int(r->end)->content) || 
			(step < 0 && cast_fbgc_object_as_int(r->end)->content >= cast_fbgc_object_as_int(r->start)->content)
			)
		{
			cprintf(100,"Range object is not a vector.\n");   
			return NULL;
		}

		size_t sz;
		if(step>0) sz = 1 + ( cast_fbgc_object_as_int(r->end)->content - cast_fbgc_object_as_int(r->start)->content ) /( step );
		else sz = 1 + ( cast_fbgc_object_as_int(r->start)->content - cast_fbgc_object_as_int(r->end)->content ) /( -step );

		struct fbgc_matrix_object * m = (struct fbgc_matrix_object *)new_fbgc_matrix_object(DOUBLE,1,sz,UNINITIALIZED_MATRIX);
		double * d = content_fbgc_matrix_object(m);

		while(sz--){
			*(d+sz) = ( (cast_fbgc_object_as_int(r->start)->content) +(step*(double)sz));
		}

		return (struct fbgc_object *) m;
	#undef r
	}

	return NULL;
}


struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** sp, int ctr, int msize, int row,fbgc_token sub_type){

	struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type,msize,1,UNINITIALIZED_MATRIX); // open ctr-row size array 

	double * db = content_fbgc_matrix_object(m);

	m->row = row+1;

	//Offset indexes
	int xof = 0, yof = 0;
	//iterable indexes
	int x = 0,y = 0;

	for (int i = 0; i < ctr; ++i){
		if(sp[i]->type == MATRIX){
			xof += x;
			yof += y;
		   // cprintf(101,">>xof :%d, yof: %d column %d\n",xof,yof,m->column);
			struct fbgc_matrix_object * msub = cast_fbgc_object_as_matrix(sp[i]);
			//struct fbgc_object * obj = sp[i];
			double * ds = content_fbgc_matrix_object(msub);
			
		   // cprintf(101,">>msub r:%d, c:%d\n",msub->row,msub->column);

				for(int q = 0; q< msub->row; ++q ){
					for(int w = 0; w< msub->column; ++w){
						int _x_ = (xof + q * msub->column);
						int _y_ = yof+w;
						db[  _x_ + _y_ ] = ds[q*msub->column+w];
					   // cprintf(010,"db[%d,%d] : %f\n",_x_,_y_,db[_x_+_y_]);
					}
					xof++;
				}
			
			//m->column = msub->column;
			yof += msub->column;
			//xof += msub->row-1;
			m->row = xof;
			m->column = yof;
			xof = 0;
			y = 0;
			
			//cprintf(101,">>xof :%d, yof: %d column %d\n",xof,yof,m->column);

			//return NULL;
			//db[x++] 
		}
		else if(sp[i]->type != ROW){

			if(sub_type == DOUBLE)
				db[ (xof + x*m->column) + (yof + y++) ] = convert_fbgc_object_to_double(sp[i]);
			else{
				struct raw_complex z = convert_fbgc_object_to_complex(sp[i]);
				size_t db_index = ((xof + x*m->column) + (yof + y))*2;

				db[ db_index ]  = z.real;
				db[ db_index + 1]  = z.imag; 

				//cprintf(010,"db[%d]=%g,db[%d+msize]=%g\n",db_index,z.real,db_index+1,z.imag);

				y++;
			}
		}   
		else if(sp[i]->type == ROW){
			m->column = y+yof;
			y = 0;
			x++;
		}
		else assert(0);
	}

	
	m->column = yof+y;
	/*if(m->column != y+yof) {
		cprintf(101,"Dimension mismatch at matrix creation!\n");
		return NULL;
	}*/

	//cprintf(100,"Matrix end  r:%d c:%d\n",m->row,m->column);

  //  m->row = row+1;
  //  m->column /= m->row;

	return (struct fbgc_object * )m;
}



struct fbgc_object * get_object_in_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c){
	#define m cast_fbgc_object_as_matrix(mat)

	if(r >= m->row || c >= m->column) return NULL;
	double * d = content_fbgc_matrix_object(m) + r * m->column + c;
	if(m->sub_type == COMPLEX){   
		return new_fbgc_complex_object(*d,*(d+1) );
	}
	else 
		return	new_fbgc_double_object(*d);


	#undef m
}  

struct fbgc_object * set_object_in_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c,struct fbgc_object * obj){
	#define m cast_fbgc_object_as_matrix(mat)

	if(r >= m->row || c >= m->column) return NULL;

	double * d = content_fbgc_matrix_object(mat) + r * m->column + c;
	if(m->sub_type == COMPLEX)
	{   
		struct raw_complex z = convert_fbgc_object_to_complex(obj);
		*d = z.real;
		*(d+1) = z.imag;
	}
	else 
		*d = convert_fbgc_object_to_double(obj);

	#undef m
	return mat;
}  

struct fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2){
	
	
	#define m1 cast_fbgc_object_as_matrix(mat1)
	#define m2 cast_fbgc_object_as_matrix(mat2)

	if(m1->column != m2->row){
		cprintf(111,"Error at matrix multiplication, dimension problem\n");
		return NULL;
	}

	if(m1->row >= 50 && m2->column >=50) return fast_multiply_fbgc_matrix_object(mat1,mat2);

	fbgc_token sub_type = MAX(m1->sub_type,m2->sub_type);

	struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type,m1->row,m2->column,UNINITIALIZED_MATRIX);

	struct raw_complex a = {0,0},b = {0,0},z = {0,0};


	char ism1_complex = cast_fbgc_object_as_matrix(m1)->sub_type == COMPLEX;
	char ism2_complex = cast_fbgc_object_as_matrix(m2)->sub_type == COMPLEX;
	char ism_complex = sub_type == COMPLEX;


	for(size_t i = 0; i< m1->row; ++i){
		for(size_t j = 0; j< m2->column; ++j){
			for(size_t k = 0; k< m2->row; ++k){

				size_t m1_index = (i * m1->column + k)<<(ism1_complex);
				size_t m2_index = (k * m2->column + j)<<(ism2_complex);
				size_t m_index  = (i * m->column  + j)<<(ism_complex);

				//cprintf(111,"%d %d %d\n",m1_index,m2_index,m_index);

				a.real = *(content_fbgc_matrix_object(m1) + m1_index);
				if(ism1_complex){
					a.imag = *(complex_content_fbgc_matrix_object(m1) + m1_index);
				}

				b.real = *(content_fbgc_matrix_object(m2) + m2_index);
				if(ism2_complex){
					b.imag = *(complex_content_fbgc_matrix_object(m2) + m2_index);
				}

				z.real = a.real * b.real - a.imag * b.imag;
				z.imag = a.real * b.imag + a.imag * b.real;

				*(content_fbgc_matrix_object(m)+m_index) += z.real;
				if(ism_complex){
					*(complex_content_fbgc_matrix_object(m)+m_index) += z.imag;
				}

				//original double version of matrix multiplication
				//m_cont[ i*m->column + j ] += m1_cont[i*m1->column + k] * m2_cont[ k * m2->column + j];
			}
		}
	}



	return (struct fbgc_object *) m;
	#undef m1
	#undef m2
}


struct fbgc_object * fast_multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2){
		
	#define m1 cast_fbgc_object_as_matrix(mat1)
	#define m2 cast_fbgc_object_as_matrix(mat2)

	if(m1->column != m2->row){
		cprintf(111,"Error at matrix multiplication, dimension problem\n");
		return NULL;
	}


/*
for (int p = 0; p < m2->column; p++)
{
	for (int q = 0; q < m2->row; q++)
	{
		tempmat[p][q] = matice2[q][p];
	}
}
for(int i = 0; i < m1->row; i++)
{
	for (int j = 0; j < tm2->row; j++)
	{
		temp = 0;
		for (int k = 0; k < m1->column; k++)
		{
			temp += matice1[i][k] * tempmat[j][k];
		}
		matice3[i][j] = temp;
	}
}*/

	fbgc_token sub_type = MAX(m1->sub_type,m2->sub_type);
	struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type,m1->row,m2->column,UNINITIALIZED_MATRIX);

	char ism1_complex = cast_fbgc_object_as_matrix(m1)->sub_type == COMPLEX;
	char ism2_complex = cast_fbgc_object_as_matrix(m2)->sub_type == COMPLEX;
	char ism_complex = sub_type == COMPLEX;

	struct fbgc_matrix_object * tm2 = (struct fbgc_matrix_object *)transpose_fbgc_matrix_object(mat2);

	double * dm1 = content_fbgc_matrix_object(m1);
	double * dtm2 = content_fbgc_matrix_object(tm2);
	double * dm = content_fbgc_matrix_object(m);

	double * ap, * bp;

	/*
		Bad cache performance, needs to be improved but does the job..
	*/

	for(size_t i = 0; i<m1->row; ++i){
		for(size_t j = 0; j<tm2->row; ++j){

			struct raw_complex z = {0,0};
			size_t asz = i * m1->column;
			size_t bsz = j * tm2->column;

			for(size_t k = 0; k< m1->column; ++k){

				ap = dm1  + ((asz + k)<<1);
				bp = dtm2 + ((bsz + k)<<1);
				
				z.real += (ap[0] * bp[0]);
				if(ism1_complex && ism2_complex)
					 z.real -= ap[1] * bp[1];

				if(ism1_complex)
					z.imag += ap[0] * bp[1];
				if(ism2_complex)
					z.imag += ap[1] * bp[0];
					
			}
			
			size_t m_index =  2*(i * m->column  + j);
			*(dm+m_index) = z.real;
			if(ism_complex) 
				*(dm+m_index+1) = z.imag;
				
		}
	}

	return (struct fbgc_object *) m;
	#undef m1
	#undef m2
}


struct fbgc_object * transpose_fbgc_matrix_object(struct fbgc_object * mat1){


	#define m1 cast_fbgc_object_as_matrix(mat1)
	
	fbgc_token is_complex =  m1->sub_type == COMPLEX;   
	struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(m1->sub_type,m1->column,m1->row,UNINITIALIZED_MATRIX);

	double *m1d = content_fbgc_matrix_object(m1);
	double *md = content_fbgc_matrix_object(m);

	for(size_t i = 0; i<m1->row; ++i){
		for(size_t j = 0; j< m1->column; ++j){
			size_t m1_index = (i*m1->column+j)<<is_complex;
			size_t m_index = (j*m1->row+i)<<is_complex;
			*(md+m_index) = *(m1d+m1_index);
			if(is_complex)
				*(md+m_index+1) = *(m1d+m1_index+1);
		}
	}
	return (struct fbgc_object * ) m;

	#undef m1
}



struct fbgc_object * operator_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){

   if(b == NULL) b = a;

   if(a->type == MATRIX && b->type == MATRIX){
		if(op == STAR){
			//fast call matrix multiplication..
			return multiply_fbgc_matrix_object(a,b);
		}
		else if(op != STAR && column_fbgc_matrix_object(a) != column_fbgc_matrix_object(b) && row_fbgc_matrix_object(a) != row_fbgc_matrix_object(b) ){
			cprintf(111,"Error dimension problem\n");
			return NULL;
		}        
	}

	size_t row = 0;
	size_t column = 0;

	// 1 -> same row a>b
	// 2 -> same column a>b
	// 3 -> same row and column
	// 6 -> same col, row a < row b
	// 7 -> same row, col a < col b

	char index_type = 0;
	fbgc_token sub_type;

	if(a->type == MATRIX){
		row = row_fbgc_matrix_object(a);
		column = column_fbgc_matrix_object(a);
		sub_type = cast_fbgc_object_as_matrix(a)->sub_type;
	}
	if(b->type == MATRIX){
		index_type += (row == row_fbgc_matrix_object(b));
		index_type += 2*(column == column_fbgc_matrix_object(b));
		
		index_type += 4*(row < row_fbgc_matrix_object(b));
		row = MAX(row,row_fbgc_matrix_object(b));
		
		index_type += 6*(column < column_fbgc_matrix_object(b));
		column = MAX(column,column_fbgc_matrix_object(b)); 

		if(sub_type == DOUBLE) sub_type = cast_fbgc_object_as_matrix(b)->sub_type;
	}

	struct raw_complex a1,b1;
	
	if(a->type >= INT && a->type <=COMPLEX){
		a1 = convert_fbgc_object_to_complex(a);
		sub_type = MAX(sub_type,a->type);
	}
	else if(b->type >= INT && b->type<= COMPLEX){
		b1 = convert_fbgc_object_to_complex(b);
		sub_type = MAX(sub_type,b->type);
	}  

	struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type,row,column,ZEROS_MATRIX);

	double * m_cont;
   // double a1,b1;


	/* Three types of matrix operations are allowed
	 * a = [1,2,3;4,5,6]
	 * b = [1,2,3;4,5,6]
	 * c = [10;20]
	 * d = [10,20,30]
	 * a+b -> a and b must be iterated with the same index 
	 * a+c -> index_c must change at the second row of a
	 * a+d -> index_d must change at the second row of a 
	 */

	const char a_complex = a->type == MATRIX && cast_fbgc_object_as_matrix(a)->sub_type == COMPLEX;
	const char b_complex = b->type == MATRIX && cast_fbgc_object_as_matrix(b)->sub_type == COMPLEX;
	const char m_complex = sub_type == COMPLEX;



for(size_t i = 0; i< row; ++i){
	for(size_t j = 0; j< column; ++j){

		size_t index =  (i*column + j);
		size_t m_index = index<<m_complex;
		size_t a_index,b_index;
		m_cont = content_fbgc_matrix_object(m)+m_index;

		switch(index_type){
			case 0:
					a_index = index;
					break;
			case 1: //same row a>b
					a_index = index;
					b_index = i;
					break;  
			case 2: //same column a>b
					a_index = index;
					b_index = j; 
					break;
			case 3: // Equal row and column number (row1 = row2,col1 = col2) 
					a_index = b_index = index;
					break;
			case 6: // 6 -> same col, row a < row b
					a_index = j;
					b_index = index; 
					break;
			case 7:// 7 -> same row, col a < col 
					a_index = i;
					b_index = index;      
					break;
			case 10:
					b_index = index;
					break;
		}

		a_index <<= a_complex;
		b_index <<= b_complex;
		
		
		if(index_type != 10){
			a1.real = *(content_fbgc_matrix_object(a)+ a_index);
			a1.imag = (a_complex) ? *(complex_content_fbgc_matrix_object(a)+ a_index ) : 0;
		}
		//if index_type != 0
		if(index_type){
			b1.real = *(content_fbgc_matrix_object(b)+b_index);
			b1.imag = (b_complex) ? *(complex_content_fbgc_matrix_object(b)+ b_index ) : 0;
		}      

		if(m_complex){
			struct raw_complex z = operator_method_raw_complex(a1,b1,op,NULL);
			*m_cont = z.real;
			*(m_cont+1) = z.imag; 
		}
		else
			*m_cont = operator_method_double(a1.real,b1.real,op,NULL);
	}
}
	return (struct fbgc_object *)m;

}




uint8_t print_fbgc_matrix_object(struct fbgc_object * mat){
	#define m cast_fbgc_object_as_matrix(mat)

	double * contents = content_fbgc_matrix_object(mat);
	//size_t sz = m->row*m->column;
	cprintf(010,"[");  
	for(int i = 0; i<m->row; i++){
		
		for(int j = 0; j<m->column; j++){
			size_t index = (i * m->column + j);
			
			if(m->sub_type == COMPLEX){
				cprintf(011,"%g%+gj",contents[index*2],contents[index*2+1]);
			}
			else cprintf(011,"%g",contents[index]);
			if(j != m->column-1) cprintf(010,",");
		}
		if(i!= m->row-1) cprintf(010,";");
	}
	return cprintf(010,"]");
	#undef m 
}

/*
    #define m cast_fbgc_object_as_matrix(self)

    double * contents = content_fbgc_matrix_object(self);

    int width = 8;
    int slash_widht = m->column*width+1;
	if(m->sub_type == COMPLEX)
		slash_widht += 5;

    //fprintf(stdout,"%2s%*s\n%1s","/",slash_widht,"\\","|");
	char iscomplex = (m->sub_type == COMPLEX);

    for(int i = 0; i<m->row; ++i){
        for(int j = 0; j<m->column; ++j){

        	size_t index = (i * m->column + j)<<iscomplex;

    		fprintf(stdout,"%*g",width,contents[index]);
		    
		    if(iscomplex){
    			fprintf(stdout,"%+gj%*s",contents[index+1],width," " );
    		}
        }
        if(i!=m->row-1) 
        	fprintf(stdout,"\n");
        //if(i!= m->row-1) fprintf(stdout,"%3s\n%1s","|","|");
    }
    //fprintf(stdout,"%3s\n%2s%*s","|","\\",slash_widht,"/");
    #undef m 
break;
*/


struct fbgc_object * abs_operator_fbgc_matrix_object(struct fbgc_object * self){
	struct fbgc_object * sz_tuple = new_fbgc_tuple_object(2);
	content_fbgc_tuple_object(sz_tuple)[0] = new_fbgc_int_object(cast_fbgc_object_as_matrix(self)->row);
	content_fbgc_tuple_object(sz_tuple)[1] = new_fbgc_int_object(cast_fbgc_object_as_matrix(self)->column);
	size_fbgc_tuple_object(sz_tuple) = 2;
	return sz_tuple;
}

const struct fbgc_object_property_holder fbgc_matrix_object_property_holder = {
	.bits = 
	_BIT_PRINT |
	_BIT_BINARY_OPERATOR |
	_BIT_ABS_OPERATOR
	,
	.properties ={
		{.print = &print_fbgc_matrix_object},
		{.binary_operator = &operator_fbgc_matrix_object},
		{.abs_operator = &abs_operator_fbgc_matrix_object},
	}
};
