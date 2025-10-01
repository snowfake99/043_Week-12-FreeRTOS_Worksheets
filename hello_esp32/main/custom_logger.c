#include "esp_log.h"

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"

#define LOG_BOLD(COLOR)  "\033[1;" COLOR "m"
#define LOG_RESET_COLOR  "\033[0m"

void custom_log(const char* tag, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    printf(LOG_BOLD(LOG_COLOR_CYAN) "[CUSTOM] %s: " LOG_RESET_COLOR, tag);
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

// Usage
custom_log("SENSOR", "Temperature: %d°C", 25);