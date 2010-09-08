#ifndef LOGGING_H__
#define LOGGING_H__

#ifdef LOG

#define DEBUG 1
#define INFO 2

#if LOG == DEBUG
#define LOG_DEBUG_EVT(x) fprintf(stderr, "D- Parser event: %s\n", x)
#define LOG_DEBUG_PRINT(s, args...) fprintf(stderr, "D- " s "\n", ##args)
#else
#define LOG_DEBUG_EVT(x) while(0)
#define LOG_DEBUG_PRINT(s, args...) while(0)
#endif

#if LOG == DEBUG || LOG == INFO
#define LOG_INFO_PRINT(s, args...) fprintf(stderr, "I- " s "\n", ##args)
#endif

#else // ifdef LOG

#define LOG_DEBUG_EVT(c) while(0)
#define LOG_DEBUG_PRINT(s, args...) while(0)
#define LOG_INFO_PRINT(s, args...) while(0)

#endif // ifdef LOG

#endif // ifndef LOGGING_H__
