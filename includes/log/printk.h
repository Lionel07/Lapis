#ifndef PRINTK_H
#define PRINTK_H

#define LOG_IMMUT 100
#define LOG_PANIC 10
#define LOG_ALERT 9
#define LOG_CRIT 8
#define LOG_ERR 7
#define LOG_WARN 6
#define LOG_NOTICE 5
#define LOG_INFO 4
#define LOG_DEBUG 3
#define LOG_DEFAULT 0
#define LOG_CONT -1
#define LOG_TAG -2

void printk(const signed int severity, const char *fmt, ...);

#endif