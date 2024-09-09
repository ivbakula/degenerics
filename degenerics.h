#ifndef _GENERICS_H
#define _GENERICS_H

#include <assert.h>

#define UCODE_RED "\x1B[31m"
#define UCODE_BLUE "\x1B[34m"
#define UCODE_NORMAL "\x1B[0m"

#define CONCAT_(x,y) x ## y
#define CONCAT(x,y) CONCAT_(x,y)

/***************************************************************************************************************/
/*************************************     COMPILE TIME ERROR HANDLING STUFF  **********************************/
/***************************************************************************************************************/

#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)

#define DO_PRAGMA(x) _Pragma(#x)
#define ERROR(msg) DO_PRAGMA(GCC error msg)

/* TODO figure out how to get type name spelling for *data field (CONTAINER.typ_spllng)*/
#define TYPE_MISMATCH_ERR_MSG(CONTAINER_TYPE, CONTAINER, DATA, FUNCTION) STRINGIFY(Type mismatch in FUNCTION! CONTAINER_TYPE CONTAINER storage type is CONTAINER.typ_spllng. Please doublecheck type of variable STRINGIFY(DATA)) 

/***************************************************************************************************************/
/*************************************     RUNTIME ERROR HANDLING STUFF  ***************************************/
/***************************************************************************************************************/

#define assert__(x) for (; !(x); assert(x))

typedef enum
  {
    Error_OK,
    Error_NOT_INITIALIZED,
    Error_NULL,
    Error_NOMEM,
    Error_SIZE_MISMATCH,
    Error_EMPTY,
  } Error_e;

/***************************************************************************************************************/
/*************************************        TYPE CHECKING STUFF START       **********************************/
/***************************************************************************************************************/

#define TYPE_CHECK_(CONTAINER, DATA)		\
  (__builtin_types_compatible_p(__typeof__(*CONTAINER.data), __typeof__(DATA)))

#define TYPE_CHECK(CONTAINER_TYPE, CONTAINER, DATA, FUNCTION)	\
  ({						\
    extern int __attribute__((error(TYPE_MISMATCH_ERR_MSG(CONTAINER_TYPE, CONTAINER, DATA, FUNCTION)))) \
      CONCAT(cc_type_check_, __LINE__)();				\
    (TYPE_CHECK_(CONTAINER, DATA) ? 0 : CONCAT(cc_type_check_, __LINE__)()), 0; \
  })

#define FORCE_PRIMITIVE(CONTAINER, DATA, ERR_MSG)			\
  ({									\
    extern int __attribute__((error(DEGENERIC_ERR_MESSAGE_START		\
				    DEGENERIC_ERR_MESSAGE_FL		\
				    STRINGIFY(ERR_MSG)			\
				    DEGENERIC_ERR_MESSAGE_END		\
				    )))					\
      CC_TYPE_CHECK_F(STRINGIFY(CONTAINER.typ_spllng))();		\
    ((TYPE_CHECK_(CONTAINER, DATA)) ? 0 : CC_TYPE_CHECK_F(STRINGIFY(CONTAINER.typ_spllng))()), 0; \
  })

/*************************************        TYPE CHECKING STUFF END       ************************************/

#endif
