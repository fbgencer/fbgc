#include "fbgc.h"

 
struct fbgc_object * new_fbgc_matrix_object(unsigned int csize){
    

	struct fbgc_matrix_object * matrixo =  
    (struct fbgc_matrix_object*) fbgc_malloc(sizeof(struct fbgc_matrix_object) + sizeof(double) * csize );
    matrixo->base.type = MATRIX;
    matrixo->row = matrixo->column = 0;
    return (struct fbgc_object*) matrixo;
}

struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** sp, int ctr, int msize, int row){


    struct fbgc_matrix_object * m = new_fbgc_matrix_object(msize); // open ctr-row size array 

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
    }

    
    m->column = yof+y;
    /*if(m->column != y+yof) {
        cprintf(101,"Dimension mismatch at matrix creation!\n");
        return NULL;
    }*/

    //cprintf(100,"Matrix end  r:%d c:%d\n",m->row,m->column);

  //  m->row = row+1;
  //  m->column /= m->row;

    return m;
}


struct fbgc_object * add_fbgc_matrix_object(struct fbgc_object * a, struct fbgc_object * b){
    //one of them can be matrix and the other can be scalar or both can be matrix



    /*if(mat1->type != MATRIX) {
        //mat2 is matrix, we are sure no need to check!
        //swap mat1 and mat2, mat1 must 
        struct fbgc_object * t = mat1;
        mat1 = mat2;
        mat2 = t;
    }*/

}

struct fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2){
        
    //cprintf(111,"Gelen matrisler\n");
   // print_fbgc_matrix_object(mat1);
   // print_fbgc_matrix_object(mat2);

    #define m1 cast_fbgc_object_as_matrix(mat1)
    #define m2 cast_fbgc_object_as_matrix(mat2)

    if(m1->column != m2->row){
        cprintf(111,"Error at matrix multiplication, dimension problem\n");
        return NULL;
    }

    struct fbgc_matrix_object * m = new_fbgc_matrix_object(m1->row*m2->column);
    m->row = m1->row;
    m->column = m2->column;

    double * m_cont  = (double *) ( (char*)( &m->column) +sizeof(m->column) );
    double * m1_cont = (double *) ( (char*)(&m1->column) +sizeof(m1->column) );
    double * m2_cont = (double *) ( (char*)(&m2->column) +sizeof(m2->column) );

    for(int i = 0; i< m1->row; ++i){
        for(int j = 0; j< m2->column; ++j){
            for(int k = 0; k< m2->row; ++k){
                m_cont[i*m->column + j ] += m1_cont[i*m1->column + k] * m2_cont[ k * m2->column + j];
            }
        }
    }

    return m;
    #undef m1
    #undef m2
}


void print_fbgc_matrix_object(struct fbgc_object * mat){
    #define m cast_fbgc_object_as_matrix(mat)

    double * contents = matrix_object_content(mat);

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
