#ifndef LOGGING_H__
#define LOGGING_H__

#ifdef LOG
#define LOG_DEBUG(x) fprintf(stderr, "Parser event: %s\n", #x)
#define LOG_PRINT(s, args...) fprintf(stderr, s "\n", ##args)
#else
#define LOG_DEBUG(c) while(0)
#define LOG_PRINT(s, args...) while(0)
#endif

#endif
