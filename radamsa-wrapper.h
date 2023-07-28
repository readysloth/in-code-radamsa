#ifndef RADAMSA_WRAPPER
#define RADAMSA_WRAPPER

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <radamsa.h>

#ifdef __cplusplus
}
#endif

static void fuzz_and_log(const void *data, size_t size)
{
    static int iteration = 0;
    static bool radamsa_initialized = false;

    iteration++;
    char *radamsa_skip = getenv("RADAMSA_SKIP");

    if (radamsa_skip && iteration < atoi(radamsa_skip)){
        printf("Skipping iteration %i of %s\n", iteration, radamsa_skip);
        return;
    }

    char prefix[] = "radamsa-";
    char suffix[] = ".fuzz";

    const int buffer_size = sizeof(prefix)+sizeof(suffix)+10;
    char filename[buffer_size];
    snprintf(filename, buffer_size, "%s%i%s", prefix, iteration, suffix);

    if (!radamsa_initialized){
        srand(time(NULL));
        radamsa_init();
        radamsa_initialized = true;
    }

    FILE *log_file = fopen(filename, "wb");
    fprintf(log_file, "non-modified data\n");
    fprintf(log_file, "-----------------\n");
    fwrite(data, size, sizeof(uint8_t), log_file);

    radamsa_inplace((uint8_t*)data, size, size, rand());

    fprintf(log_file, "\nfuzzed data\n");
    fprintf(log_file, "-----------\n");
    fwrite(data, size, sizeof(uint8_t), log_file);
    fclose(log_file);
}


#define RADAMSA_FUZZ_SIMPLE(ENTITY) fuzz_and_log(&ENTITY, sizeof(ENTITY))

#endif /* end of include guard: RADAMSA_WRAPPER_H */
