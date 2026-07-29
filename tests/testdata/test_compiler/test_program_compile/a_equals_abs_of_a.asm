main:
    LOAD @0;
    INCR;
    STORE @1;

    LOAD @0;
    INCR;

    JZ absolute;
    JUMP end;

absolute:
    LOAD @0;
    INCR;
    INCR;
    STORE @0;

    LOAD @1;
    INCR;
    STORE @1;

    JZ absolute;

end:
    HALT;