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

    LOAD #108; # l
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #114; # r
    PRINT_CHAR;

    LOAD #58; # :
    PRINT_CHAR;
    LOAD #32; # [SPACE]
    PRINT_CHAR;

    READ_CHAR;
    STORE @0;

    # Answer

    LOAD #89; # Y
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #97; # a
    PRINT_CHAR;
    LOAD #104; # h
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD @0; # Saved letter
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #105; # i
    PRINT_CHAR;
    LOAD #115; # s
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #97; # a
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #103; # g
    PRINT_CHAR;
    LOAD #114; # r
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #97; # a
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;

    LOAD #32; # [SPACE]
    PRINT_CHAR;

    LOAD #108; # l
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;
    LOAD #116; # t
    PRINT_CHAR;
    LOAD #101; # e
    PRINT_CHAR;
    LOAD #114; # r
    PRINT_CHAR;
    LOAD #33; #
    PRINT_CHAR;

    LOAD #10; # [LINE FEED]
    PRINT_CHAR;

    HALT;
