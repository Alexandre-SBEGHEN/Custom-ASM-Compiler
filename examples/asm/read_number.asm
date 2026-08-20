main:
    LOAD #69; # E
    PRINT_CHAR;
    LOAD #110; # n
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #114; # r
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #97; # a
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #110; # n
    PRINT_CHAR;
    LOAD #117; # u
    PRINT_CHAR;
    LOAD #109; # m
    PRINT_CHAR;
    LOAD #98; # b
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #114; # r
    PRINT_CHAR;

    LOAD #58; # :
    PRINT_CHAR;
    LOAD #32; # [SPACE]
    PRINT_CHAR;

    READ_INT;
    STORE @0;

    # Answer

    LOAD #43; # +
    PRINT_CHAR;
    LOAD #49; # 1
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #105; # i
    PRINT_CHAR;
    LOAD #115; # s
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #110; # n
    PRINT_CHAR;
    LOAD #111; # o
    PRINT_CHAR;
    LOAD #119; # w
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD @0;
    INCR;
    PRINT_INT;

    LOAD #10; # [LINE FEED]
    PRINT_CHAR;

    HALT;