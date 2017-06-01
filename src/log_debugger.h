#ifndef __LOG_DEBUG_H__
#define __LOG_DEBUG_H__


#ifdef  DEBUG
/* file:line:function: */
#define LOG_DEBUG(fmt, ...) fprintf(listing, "%s:%d:%s: " fmt, \
                                           __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#endif /* DEBUG */

#define DONT_OCCUR_PRINT do { \
    fprintf(listing, "%s:%d:%s: This must not occur", \
                    __FILE__, __LINE__, __FUNCTION__); \
    } while(0)


#endif /* __LOG_DEBUG_H__ */
