#include "utils/error.h"

void plushError_print_error(const char* error, ...) {
    va_list args;
    va_start(args, error);

    printf("[%sPlush Error%s] ", COLOR_REDBOLD, COLOR_BASE);

    int errorLength = strlen(error);
    for (int i=0; i<errorLength; i++) {
        char c = error[i];
        if (c == '%') {
            i++;
            c = error[i];

            switch (c) {
            case 'd':
                printf("%d", va_arg(args, int));
                break;

            case 's':
                printf("%s", va_arg(args, char*));
                break;

            case 'c':
                putchar(va_arg(args, int));
                break;
            
            default:
                printf("%%%c", c);
                break;
            }
        } else
            putchar(c);
    }

    putc('\n', stdout);
    fflush(stdout);
    va_end(args);
}

void plushError_print_warn(const char* warn, ...) {
    va_list args;
    va_start(args, warn);

    printf("[%sPlush Warn%s] ", COLOR_YELLOWBOLD, COLOR_BASE);

    int warnLength = strlen(warn);
    for (int i = 0; i < warnLength; i++) {
        char c = warn[i];
        if (c == '%') {
            i++;
            c = warn[i];

            switch (c) {
                case 'd':
                    printf("%d", va_arg(args, int));
                    break;

                case 's':
                    printf("%s", va_arg(args, char*));
                    break;

                case 'c':
                    putchar(va_arg(args, int));
                    break;

                default:
                    printf("%%%c", c);
                    break;
            }
        } else
            putchar(c);
    }

    putc('\n', stdout);
    fflush(stdout);
    va_end(args);
}
