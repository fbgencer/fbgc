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

    //virtual col and row to check size
    int vr = 0, vc = 0;
    int x = 0,y = 0;
    for (int i = 0; i < ctr; ++i){
            
        if(sp[i]->type == MATRIX){
            struct fbgc_matrix_object * msub = cast_fbgc_object_as_matrix(sp[i]);
            struct fbgc_object * obj = sp[i];
            double * ds = (double *) ( (char*)(&msub->column) +sizeof(msub->column) );
            
                for(int q = 0; q< msub->row; ++q ){
                    for(int w = 0; w< msub->column; ++w){
                        db[q * msub->column + w ] = ds[q*msub->column+w];
                        cprintf(010,"ds[%d,%d] : %f\n",q,w,ds[q*msub->column+w]);
                    }
                }
            
            //m->column = msub->column;
            vc = msub->column;
            x = msub->row-1;
            //y = 0;
            cprintf(111,"msub r:%d, c:%d\n",msub->row,msub->column);
            cprintf(111,"x :%d, vc: %d column %d\n",x,vc,m->column);

            //return NULL;
            //db[x++] 
        }
        else if(sp[i]->type != ROW){
            vc++;
            db[ x*m->column + (y++) ] = convert_fbgc_object_to_double(sp[i]);
            
            if(i == ctr-1 && m->column == 0){
                m->column = vc;
            }
        }   
        else if(sp[i]->type == ROW){
            m->column = vc;
            vc = 0;
            x++;
            y = 0;

        }
    }

    if(m->column != vc) {
        cprintf(101,"Dimension mismatch at matrix creation!\n");
        return NULL;
    }

    cprintf(100,"Matrix end  r:%d c:%d\n",m->row,m->column);

  //  m->row = row+1;
  //  m->column /= m->row;

    return m;
}


void append_row_to_fbgc_matrix_object(struct fbgc_object * mat,struct fbgc_object * data){
    /*if(cast_fbgc_object_as_matrix(mat)->row == 0){
        for(unsigned int i = cast_fbgc_object_as_matrix(mat)->column; i>=0; i--){
            data = data->next;
            *(cast_fbgc_object_as_matrix(mat)->contents+i) = convert_fbgc_object_to_double(data);
        }
        cast_fbgc_object_as_matrix(mat)->row = 1;
    }*/
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
