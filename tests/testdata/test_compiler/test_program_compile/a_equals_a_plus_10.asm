main:
        LOAD #-9;
        STORE @2;
    for:
        LOAD @0;
        INCR;
        STORE @0;

        LOAD @2;
        INCR;
        STORE @2;
        
        JZ for;
        
        HALT;