#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "degenerics.h"

/***************************************************************************************************************/
/*************************************       VECTOR TEMPLATE STUFF START      **********************************/
/***************************************************************************************************************/

#define DEFAULT_VECTOR_SIZE 16

// TODO: figure out how to fetch type spellings of both *data pointer and data we wish to store in vector
// TODO: figure out runtime error macros instead of using printf
// .typ_spllng = STRINGIFY(TYPE) Unfortunately this doesn't work at the compile time. Figure out another way to do this
#define __VECTOR_IF__(TYPE) \
  struct		 \
  {			 \
    size_t size;	 \
    size_t current;	 \
    bool init;		 \
    TYPE *data;		 \
  }

typedef __VECTOR_IF__(uint8_t) VectorIf_t;

#define specialize_vector(VTOR_NAME, TYPE)	\
  __VECTOR_IF__(TYPE)				\
       VTOR_NAME =				\
    {						\
      .size = DEFAULT_VECTOR_SIZE,		\
      .current = 0,				\
      .init = false,				\
      .data = NULL,				\
    }

#define initialize_vector(VTOR_NAME)		\
  {									\
    VTOR_NAME.data = (__typeof__(VTOR_NAME.data)) calloc(VTOR_NAME.size, sizeof(*VTOR_NAME.data)); \
    VTOR_NAME.init = (VTOR_NAME.data) ? true : false;			\
  }

/*************************************       VECTOR TEMPLATE STUFF END    **************************************/


#define __get_element_at(vtor, index, size) &vtor->data[(index)*(size)]
#define __current_element(vtor, size) __get_element_at(vtor, vtor->current, size)

static inline Error_e _push_back(VectorIf_t *vtor, void *elem, size_t elem_sz, const char *vtor_name, const char *file, int line)  __attribute__((nonnull(1,2)));
static inline Error_e _push_back(VectorIf_t *vtor, void *elem, size_t elem_sz, const char *vtor_name, const char *file, int line)
{
  assert__(vtor->init) {
    printf("DEGENERICS RUNTIME ERROR @ %s:%d! Vector %s not initialized!\n", file, line, vtor_name);
  }
  /* Regrow array  */
  if (vtor->current >= vtor->size)
    {
      vtor->size = vtor->size * 2;
      vtor->data = reallocarray(vtor->data, vtor->size, elem_sz);
      if (!vtor->data)
	return Error_NOMEM;
    }

  memcpy(__current_element(vtor,elem_sz), elem, elem_sz);
  vtor->current++;

  return Error_OK;
}
static inline Error_e _push_back_const(VectorIf_t *vtor, uintptr_t cnst, size_t elem_sz, const char *vtor_name, const char *file, int line)  __attribute__((nonnull(1)));
static inline Error_e _push_back_const(VectorIf_t *vtor, uintptr_t cnst, size_t elem_sz, const char *vtor_name, const char *file, int line)
{
  return _push_back(vtor, &cnst, elem_sz, vtor_name, file, line);
}

static inline void *_vector_at(VectorIf_t *vtor, size_t index, size_t elem_sz, const char *vtor_name, const char *file, int line) __attribute__((nonnull(1, 4, 5)));
static inline void *_vector_at(VectorIf_t *vtor, size_t index, size_t elem_sz, const char *vtor_name, const char *file, int line)
{
  assert__(vtor->current != 0) {
    printf("DEGENERICS RUNTIME ERROR @ %s:%d! Vector %s is empty!\n", file, line, vtor_name);
  }

  assert__((index < vtor->size)) {
    printf("DEGENERICS RUNTIME ERROR @ %s:%d! Index %d out of bounds. Vector %s.size=%d\n", file, line, (int)index, vtor_name, (int)vtor->size);
  }

  if (index > (vtor->current - 1))
    {
      printf("DEGENERICS RUNTIME WARNING @ %s:%d! Requested index %d not yet in used! Current idx: %d Reading uninitialized memory!", file, line, (int)index, vtor->current);
    }

  return __get_element_at(vtor, index, elem_sz);
}

#define vector_push_back(VECTOR, ELEMENT)	\
  {						\
    TYPE_CHECK(Vector, VECTOR, ELEMENT, push_back);		\
    _push_back((VectorIf_t *)&VECTOR, &ELEMENT, sizeof(ELEMENT), STRINGIFY(VECTOR), __FILE__, __LINE__); \
  }						\

#define vector_push_back_const(VECTOR, ELEMENT) \
  {						\
    TYPE_CHECK(Vector, VECTOR, ELEMENT, push_back_const);				\
    _push_back_const(((VectorIf_t *)&VECTOR), ELEMENT, sizeof(*VECTOR.data), STRINGIFY(VECTOR), __FILE__, __LINE__); \
  }

#define vector_pop_back(VECTOR)			\
  {						\
    VECTOR.current = (VECTOR.current == 0) ? 0 : (VECTOR.current - 1);	\
  }

#define vector_get_tail(VECTOR)			\
  {						\
    (__typeof__(VECTOR.data)) __get_element_at(VECTOR, ((VECTOR.current) ? VECTOR.current - 1 : 0), sizeof(*VECTOR.data)); \
  }

#define vector_get_head(VECTOR)			\
  {						\
    (__typeof__(VECTOR.data)) __get_element_at(VECTOR, 0, sizeof(*VECTOR.data)); \
  }
  
#define vector_get_current_ptr(VECTOR)		\
  {						\
    (__typeof__(VECTOR.data)) __get_current(VECTOR, sizeof(*VECTOR.data)); \
  }

#define vector_at(VECTOR, INDEX)		\
  ((__typeof__(VECTOR.data)) _vector_at((VectorIf_t *)&VECTOR, INDEX, sizeof(*VECTOR.data), STRINGIFY(VECTOR), __FILE__, __LINE__))

#define vector_at_(VECTOR, INDEX)		\
  (*vector_at(VECTOR, INDEX))

#endif
