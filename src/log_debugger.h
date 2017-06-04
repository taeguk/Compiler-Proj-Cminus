#ifndef __LOG_DEBUG_H__
#define __LOG_DEBUG_H__

#define MALLOC(ptr, size) do { \
    ptr = malloc(size);\
    if(ptr == NULL) {\
        fprintf(listing, "%s:%d:%s: Memory allocation failed.\n", \
                __FILE__, __LINE__, __FUNCTION__); \
        assert(0);\
    }\
} while(0)

#ifdef  DEBUG

/* file:line:function: */
  #define LOG_DEBUG(fmt, ...) fprintf(listing, "%s:%d:%s: " fmt, \
                                      __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
  #define DONT_OCCUR_PRINT do { \
      fprintf(listing, "%s:%d:%s: This must not occur\n", \
              __FILE__, __LINE__, __FUNCTION__); \
      assert(0);\
  } while(0)

#else

  #define LOG_DEBUG(fmt, ...)
  #define DONT_OCCUR_PRINT

#endif /* DEBUG */

#endif /* __LOG_DEBUG_H__ */
