
#ifndef Defines_h
#define Defines_h

#define DEBUG

#if defined(DEBUG)
#define  LOG(...)   printf(__VA_ARGS__)
#else
#define  LOG(...)   {}
#endif




#endif /* Defines_h */
