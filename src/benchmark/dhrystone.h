#ifndef DHRYSTONE_H
#define DHRYSTONE_H

#define structassign(d, s) d = s

#define HZ 1000000

typedef enum { Ident_1, Ident_2, Ident_3, Ident_4, Ident_5 } Enumeration;
typedef struct record {
    struct record *Ptr_Comp;
    Enumeration Discr;
    union {
        struct {
            Enumeration Enum_Comp;
            int Int_Comp;
            char Str_Comp[31];
        } var_1;
        struct {
            Enumeration E_Comp_2;
            char Str_2_Comp[31];
        } var_2;
        struct {
            char Ch_1_Comp;
            char Ch_2_Comp;
        } var_3;
    } variant;
} Rec_Type, *Rec_Pointer;

int run_dhrystone_T0();
int run_dhrystone_T1();
int run_dhrystone_T2();

#endif
