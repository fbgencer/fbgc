#include "fbgc.h"



struct fbgc_object * new_fbgc_matrix_object(size_t row, size_t col, char tp){
    size_t sz = row*col;
	struct fbgc_matrix_object * mo =  (struct fbgc_matrix_object*) fbgc_malloc(sizeof(struct fbgc_matrix_object) + sizeof(double) * sz );
    mo->base.type = MATRIX;
    mo->row = row;
    mo->column = col;

    //tp == -1 => do not initialize;
    //tp == 0 => zeros
    //tp == 1 => ones
    double * ct = content_fbgc_matrix_object(mo);

    switch(tp){
        case ZEROS_MATRIX:
        case ONES_MATRIX:
        {
            double x = tp*(1.0);
            while(sz--)
            {
                *(ct+sz) = x;
            }
            break;            
        }
    }
    return mo;
}

struct fbgc_object * new_fbgc_matrix_object_from_range(struct fbgc_object * robj){

    #define r cast_fbgc_object_as_range(robj)

    fbgc_token tok = MAX(get_fbgc_object_type(r->start),get_fbgc_object_type(r->end));
    if(r->step != NULL) tok = MAX(get_fbgc_object_type(r->step),tok);

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

        struct fbgc_matrix_object * m = new_fbgc_matrix_object(1,sz,UNINITIALIZED_MATRIX);
        double * d = content_fbgc_matrix_object(m);

        while(sz--){
            *(d+sz) = ( (cast_fbgc_object_as_int(r->start)->content) +(step*(double)sz));
        }

        return m;

        /*
        if(step>0)
            return  (no >= cast_fbgc_object_as_int(r->end)->content ) ? NULL :
            new_fbgc_int_object(no);
        else
            return  (no > cast_fbgc_object_as_int(r->end)->content ) ?  new_fbgc_int_object(no) : NULL ;*/
    }
   /* else if(tok == DOUBLE){
        double step = (r->step == NULL) ? 1.0 : convert_fbgc_object_to_double(r->step); 
        double no = convert_fbgc_object_to_double(r->start) + (step*index);

        if(step>0)
            return  (no >= convert_fbgc_object_to_double(r->end) ) ? NULL:
            new_fbgc_double_object(no);
        else
            return  (no > convert_fbgc_object_to_double(r->end)  ) ?  new_fbgc_double_object(no) : NULL ;        
    }
    */
    #undef r

}


struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** sp, int ctr, int msize, int row){


    struct fbgc_matrix_object * m = (struct fbgc_matrix_object * )new_fbgc_matrix_object(msize,1,10); // open ctr-row size array 

    double * db = (double *) ( (char*)(&m->column) +sizeof(m->column) );
    //cprintf(111,"m %p DB: %p\n",m,db); 


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
            struct fbgc_object * obj = sp[i];
            double * ds = (double *) ( (char*)(&msub->column) +sizeof(msub->column) );
            
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
            //cprintf(111,"Now row, writing db..\n");
            //cprintf(111,"xof :%d, yof: %d column %d\n",xof,yof,m->column);
            //cprintf(111,"x=%d, y=%d\n",x,y);
            db[ (xof + x*m->column) + (yof + y++) ] = convert_fbgc_object_to_double(sp[i]);
            
            //if(i == ctr-1 && m->column == 0){
            //m->column = yof+y;
            //}
        }   
        else if(sp[i]->type == ROW){
            m->column = y+yof;
            y = 0;
            x++;
        }
        else return NULL;
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



struct fbgc_object * subscript_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c){
    #define m cast_fbgc_object_as_matrix(mat)

    if(r >= m->row || c >= m->column) return NULL;
    double * d = content_fbgc_matrix_object(m);
    return new_fbgc_double_object(d[ r * m->column + c ]);

    #undef m
}  


struct fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2){
        
    #define m1 cast_fbgc_object_as_matrix(mat1)
    #define m2 cast_fbgc_object_as_matrix(mat2)

    if(m1->column != m2->row){
        cprintf(111,"Error at matrix multiplication, dimension problem\n");
        return NULL;
    }

    struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(m1->row,m2->column,UNINITIALIZED_MATRIX);


    double * m_cont  = content_fbgc_matrix_object(m);
    double * m1_cont = content_fbgc_matrix_object(mat1);
    double * m2_cont = content_fbgc_matrix_object(mat2);

    int i,j,k;
    for(i = 0; i< m1->row; ++i){
        for(j = 0; j< m2->column; ++j){
            for(k = 0; k< m2->row; ++k){
                m_cont[i*m->column + j ] += m1_cont[i*m1->column + k] * m2_cont[ k * m2->column + j];
            }
        }
    }

    return (struct fbgc_object *) m;
    #undef m1
    #undef m2
}

struct fbgc_object * add_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2){
        
    #define m1 cast_fbgc_object_as_matrix(mat1)
    #define m2 cast_fbgc_object_as_matrix(mat2)

    if(m1->column != m2->column && m1->row != m2->row){
        cprintf(111,"Error at matrix multiplication, dimension problem\n");
        return NULL;
    }

    struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(m1->row,m1->column,UNINITIALIZED_MATRIX);

    double * m_cont  = content_fbgc_matrix_object(m);
    double * m1_cont = content_fbgc_matrix_object(mat1);
    double * m2_cont = content_fbgc_matrix_object(mat2);

    for(int i = 0; i< m1->row; ++i){
        for(int j = 0; j< m1->column; ++j){
            int index =  i*m1->column + j; 
            m_cont[index] = m1_cont[ index ] + m2_cont[ index ];
        }
    }

    return (struct fbgc_object *) m;
    #undef m1
    #undef m2
}



struct fbgc_object * binary_op_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){

    assert(a->type == MATRIX && b->type == MATRIX);

    #define m1 cast_fbgc_object_as_matrix(a)
    #define m2 cast_fbgc_object_as_matrix(b)

    if(op == STAR && m1->column != m2->row){
        cprintf(111,"Error at matrix multiplication, dimension problem\n");
        return NULL;
    }
    else if(op != STAR && m1->column != m2->column && m1->row != m2->row){
        cprintf(111,"Error dimension problem\n");
        return NULL;
    }


    struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) new_fbgc_matrix_object(m1->row,m2->column,UNINITIALIZED_MATRIX);


    double * m_cont  = content_fbgc_matrix_object(m);
    double * m1_cont = content_fbgc_matrix_object(a);
    double * m2_cont = content_fbgc_matrix_object(b);


for(int i = 0; i< m1->row; ++i){
    for(int j = 0; j< m2->column; ++j){
        int index =  i*m1->column + j; 

switch(op)
{
    case STARSTAR:
    {
        return NULL;
    }
    case SLASHSLASH:
    {
       return NULL; // m_cont[index] = (m1_cont[ index ] * m2_cont[ index ])/(m1_cont[ index ] + m2_cont[ index ]);
    }
    case LO_EQ:
    {
        m_cont[index] = m1_cont[ index ] <= m2_cont[ index ];
        break;
    }
    case GR_EQ:
    {
        m_cont[index] = m1_cont[ index ] >= m2_cont[ index ];
        break;
    }
    case EQ_EQ:
    {
        m_cont[index] = (m1_cont[index] == m2_cont[ index ]);
        break;
    }
    case NOT_EQ:
    {
        m_cont[index] = m1_cont[ index ] != m2_cont[ index ];
        break;
    }
    case R_SHIFT:
    {
        return NULL;
    }
    case L_SHIFT:
    {
        return NULL;
    }
    case CARET:
    {
       return NULL;
    }
    case PERCENT:
    {
        return NULL;
    }
    case LOWER:
    {
        m_cont[index] = m1_cont[ index ] < m2_cont[ index ];
        break;
    }
    case GREATER:
    {
        m_cont[index] = m1_cont[ index ] > m2_cont[ index ];
        break;
    }
    case PIPE:
    {
        m_cont[index] = m1_cont[ index ] || m2_cont[ index ];
        break;
    }
    case AMPERSAND:
    {
        m_cont[index] = m1_cont[ index ] && m2_cont[ index ];
        break;
    }
    case SLASH:
    {
        m_cont[index] = m1_cont[ index ] - m2_cont[ index ];
        break;
    }
    case STAR:
    {
        for(int k = 0; k< m2->row; ++k){
            m_cont[i*m->column + j ] += m1_cont[i*m1->column + k] * m2_cont[ k * m2->column + j];
        }
        break;
    }
    case MINUS:
    {
        m_cont[index] = m1_cont[ index ] - m2_cont[ index ];
        break;
    }
    case PLUS:
    {
        m_cont[index] = m1_cont[ index ] + m2_cont[ index ];
        break;
    }
}

    }
}
    return m;

    #undef m1
    #undef m2

}




void print_fbgc_matrix_object(struct fbgc_object * mat){
    #define m cast_fbgc_object_as_matrix(mat)

    double * contents = content_fbgc_matrix_object(mat);

    cprintf(010,"[");
    for(int i = 0; i<m->row; i++){
        
        for(int j = 0; j<m->column; j++){
            cprintf(011,"%f",contents[i * m->column + j]);
            if(j != m->column-1) cprintf(010,",");
        }
        if(i!= m->row-1) cprintf(010,";");
    }
    cprintf(010,"]");
    #undef m 
}
