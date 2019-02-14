#ifndef CMODULE_OBJECT_H
#define CMODULE_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object * get_cfun_obj_from_substr(struct fbgc_object * field_obj,const char *s1,const char *s2);

#ifdef  __cplusplus
}
#endif

#endif