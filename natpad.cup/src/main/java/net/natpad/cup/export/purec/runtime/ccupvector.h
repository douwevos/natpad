#ifndef [%PRE%]VECTOR_H_
#define [%PRE%]VECTOR_H_

typedef int [%pre%]boolean;
#define [%PRE%]FALSE  0
#define [%PRE%]TRUE  0

typedef struct s_[%Pre%]Vector       [%Pre%]Vector;

typedef void (*VectorReleaseCB)([%Pre%]Vector *vector, void *element_release);

struct s_[%Pre%]Vector {
	void **data;
	int data_size;
	int data_buf_size;
	VectorReleaseCB vector_release_cb;
};

void [%pre%]_vector_free([%Pre%]Vector *vector);
[%Pre%]Vector *[%pre%]_vector_new(VectorReleaseCB vector_release_cb);

int [%pre%]_vector_count([%Pre%]Vector *vector);
void [%pre%]_vector_clear([%Pre%]Vector *vector);

void [%pre%]_vector_add([%Pre%]Vector *vector, void *data);
void *[%pre%]_vector_pop([%Pre%]Vector *vector);


void *[%pre%]_vector_get_at([%Pre%]Vector *vector, int index);
void [%pre%]_vector_set_at([%Pre%]Vector *vector, void *item, int index);


#endif /* [%PRE%]VECTOR_H_ */
