//
// Created by wmsksf on 27/11/19.
//

#ifndef PROJECT_JJ_MACROS_H
#define PROJECT_JJ_MACROS_H

#define ALLOC_CHECK(value) \
({ \
    void *val = (value); \
    if(val == NULL) \
    { \
        fprintf(stderr, "/Failed to allocate memory on file %s on line %d\n", __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
})

#define USAGE(argv) \
({ \
    fprintf(stderr, "Usage %s [-f1 filename1] [-rel1 column of relation1] [–f2 filename2] [–rel2 column of relation2]\n", argv); \
    exit(EXIT_FAILURE); \
})

#endif //PROJECT_JJ_MACROS_H
