main: #partie principale
    LOAD #-9;
        STORE @2;
for:
        LOAD @0; INCR; STORE @0; # 0 -> 0
#blabla
     LOAD @2; # on charge
        INCR;
STORE @2;

        JZ for;HALT;