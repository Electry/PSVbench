#ifndef DHRYSTONE_IMPL_H
#define DHRYSTONE_IMPL_H

#define DECL_DHRYSTONE_ALL(thread)                                                 \
    bool Func_3_T##thread(Enumeration Enum_Par_Val);                               \
    bool Func_2_T##thread(char Str_1_Par_Ref[31], char Str_2_Par_Ref[31]);         \
    Enumeration Func_1_T##thread(char Ch_1_Par_Val, char Ch_2_Par_Val);            \
    void Proc_8_T##thread(int Arr_1_Par_Ref[50], int Arr_2_Par_Ref[50][50],        \
                            int Int_1_Par_Val, int Int_2_Par_Val);                 \
    void Proc_7_T##thread(int Int_1_Par_Val, int Int_2_Par_Val, int *Int_Par_Ref); \
    void Proc_6_T##thread(Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par);    \
    void Proc_5_T##thread();                                                       \
    void Proc_4_T##thread();                                                       \
    void Proc_3_T##thread(Rec_Pointer *Ptr_Ref_Par);                               \
    void Proc_2_T##thread(int *Int_Par_Ref);                                       \
    void Proc_1_T##thread(register Rec_Pointer Ptr_Val_Par);                       \
    DECL_Proc_1(thread)                                                            \
    DECL_Proc_2(thread)                                                            \
    DECL_Proc_3(thread)                                                            \
    DECL_Proc_4(thread)                                                            \
    DECL_Proc_5(thread)                                                            \
    DECL_Proc_6(thread)                                                            \
    DECL_Proc_7(thread)                                                            \
    DECL_Proc_8(thread)                                                            \
    DECL_Func_1(thread)                                                            \
    DECL_Func_2(thread)                                                            \
    DECL_Func_3(thread)                                                            \
    DECL_MAIN(thread)

/*
 * DHRYSTONE benchmark implementation
 *  Based on: C, version 2.2 dated 1996-03-08
 *  Source: http://homepages.cwi.nl/~steven/dry.c
 */
#define DECL_MAIN(thread)                                                          \
    int run_dhrystone_T##thread() {                                                \
        int Int_1_Loc;                                                             \
        register int Int_2_Loc;                                                    \
        int Int_3_Loc;                                                             \
        register char Ch_Index;                                                    \
        Enumeration Enum_Loc;                                                      \
        char Str_1_Loc[31];                                                        \
        char Str_2_Loc[31];                                                        \
        register int Run_Index;                                                    \
        register int Number_Of_Runs = 10000000;                                    \
                                                                                   \
        uint32_t Begin_Time, End_Time, User_Time;                                  \
        float Dhrystones_Per_Second;                                               \
                                                                                   \
        Next_Ptr_Glob[thread] = (Rec_Pointer)malloc(sizeof(Rec_Type));             \
        Ptr_Glob[thread] = (Rec_Pointer)malloc(sizeof(Rec_Type));                  \
                                                                                   \
        Ptr_Glob[thread]->Ptr_Comp                    = Next_Ptr_Glob[thread];     \
        Ptr_Glob[thread]->Discr                       = Ident_1;                   \
        Ptr_Glob[thread]->variant.var_1.Enum_Comp     = Ident_3;                   \
        Ptr_Glob[thread]->variant.var_1.Int_Comp      = 40;                        \
        strcpy(Ptr_Glob[thread]->variant.var_1.Str_Comp,                           \
                "DHRYSTONE PROGRAM, SOME STRING");                                 \
        strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");                       \
                                                                                   \
        Arr_2_Glob[thread][8][7] = 10;                                             \
                                                                                   \
        Begin_Time = sceKernelGetProcessTimeLow();                                 \
                                                                                   \
        for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index) {            \
            Proc_5_T##thread();                                                    \
            Proc_4_T##thread();                                                    \
            Int_1_Loc = 2;                                                         \
            Int_2_Loc = 3;                                                         \
            strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");                   \
            Enum_Loc = Ident_2;                                                    \
            Bool_Glob[thread] = !Func_2_T##thread(Str_1_Loc, Str_2_Loc);           \
                                                                                   \
            while (Int_1_Loc < Int_2_Loc) {                                        \
                Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;                             \
                Proc_7_T##thread(Int_1_Loc, Int_2_Loc, &Int_3_Loc);                \
                Int_1_Loc += 1;                                                    \
            }                                                                      \
                                                                                   \
            Proc_8_T##thread(Arr_1_Glob[thread],                                   \
                    Arr_2_Glob[thread], Int_1_Loc, Int_3_Loc);                     \
            Proc_1_T##thread(Ptr_Glob[thread]);                                    \
            for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob[thread]; ++Ch_Index) {      \
                if (Enum_Loc == Func_1_T##thread(Ch_Index, 'C'))   {               \
                    Proc_6_T##thread(Ident_1, &Enum_Loc);                          \
                    strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");           \
                    Int_2_Loc = Run_Index;                                         \
                    Int_Glob[thread] = Run_Index;                                  \
                }                                                                  \
            }                                                                      \
                                                                                   \
            Int_2_Loc = Int_2_Loc * Int_1_Loc;                                     \
            Int_1_Loc = Int_2_Loc / Int_3_Loc;                                     \
            Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;                   \
            Proc_2_T##thread(&Int_1_Loc);                                          \
        }                                                                          \
                                                                                   \
        End_Time = sceKernelGetProcessTimeLow();                                   \
        User_Time = End_Time - Begin_Time;                                         \
                                                                                   \
        Dhrystones_Per_Second = ((float)HZ * (float)Number_Of_Runs)                \
                                    / (float)User_Time;                            \
                                                                                   \
        return Dhrystones_Per_Second / 1757;                                       \
    }

#define DECL_Proc_1(thread)                                                        \
    void __attribute__ ((noinline)) Proc_1_T##thread(                              \
                register Rec_Pointer Ptr_Val_Par) {                                \
        register Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;                  \
                                                                                   \
        structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob[thread]);                   \
        Ptr_Val_Par->variant.var_1.Int_Comp = 5;                                   \
        Next_Record->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp; \
        Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;                             \
        Proc_3_T##thread(&Next_Record->Ptr_Comp);                                  \
                                                                                   \
        if (Next_Record->Discr == Ident_1) {                                       \
            Next_Record->variant.var_1.Int_Comp = 6;                               \
            Proc_6_T##thread(Ptr_Val_Par->variant.var_1.Enum_Comp,                 \
                    &Next_Record->variant.var_1.Enum_Comp);                        \
            Next_Record->Ptr_Comp = Ptr_Glob[thread]->Ptr_Comp;                    \
            Proc_7_T##thread(Next_Record->variant.var_1.Int_Comp, 10,              \
                    &Next_Record->variant.var_1.Int_Comp);                         \
        }                                                                          \
        else {                                                                     \
            structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);                    \
        }                                                                          \
    }

#define DECL_Proc_2(thread)                                                        \
    void __attribute__ ((noinline)) Proc_2_T##thread(int *Int_Par_Ref) {           \
        int Int_Loc;                                                               \
        Enumeration Enum_Loc;                                                      \
                                                                                   \
        Int_Loc = *Int_Par_Ref + 10;                                               \
        do {                                                                       \
            if (Ch_1_Glob[thread] == 'A') {                                        \
                Int_Loc -= 1;                                                      \
                *Int_Par_Ref = Int_Loc - Int_Glob[thread];                         \
                Enum_Loc = Ident_1;                                                \
            }                                                                      \
        } while (Enum_Loc != Ident_1);                                             \
    }

#define DECL_Proc_3(thread)                                                        \
    void __attribute__ ((noinline)) Proc_3_T##thread(Rec_Pointer *Ptr_Ref_Par) {   \
        if (Ptr_Glob[thread] != NULL)                                              \
            *Ptr_Ref_Par = Ptr_Glob[thread]->Ptr_Comp;                             \
        Proc_7_T##thread(10, Int_Glob[thread],                                     \
                &Ptr_Glob[thread]->variant.var_1.Int_Comp);                        \
    }

#define DECL_Proc_4(thread)                                                        \
    void __attribute__ ((noinline)) Proc_4_T##thread() {                           \
        bool Bool_Loc;                                                             \
                                                                                   \
        Bool_Loc = Ch_1_Glob[thread] == 'A';                                       \
        Bool_Glob[thread] = Bool_Loc | Bool_Glob[thread];                          \
        Ch_2_Glob[thread] = 'B';                                                   \
    }

#define DECL_Proc_5(thread)                                                        \
    void __attribute__ ((noinline)) Proc_5_T##thread() {                           \
        Ch_1_Glob[thread] = 'A';                                                   \
        Bool_Glob[thread] = false;                                                 \
    }

#define DECL_Proc_6(thread)                                                        \
    void __attribute__ ((noinline)) Proc_6_T##thread(                              \
            Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par) {                 \
        *Enum_Ref_Par = Enum_Val_Par;                                              \
                                                                                   \
        if (!Func_3_T##thread(Enum_Val_Par))                                       \
            *Enum_Ref_Par = Ident_4;                                               \
                                                                                   \
        switch (Enum_Val_Par) {                                                    \
        case Ident_1:                                                              \
            *Enum_Ref_Par = Ident_1;                                               \
            break;                                                                 \
        case Ident_2:                                                              \
            if (Int_Glob[thread] > 100)                                            \
            *Enum_Ref_Par = Ident_1;                                               \
            else                                                                   \
            *Enum_Ref_Par = Ident_4;                                               \
            break;                                                                 \
        case Ident_3:                                                              \
            *Enum_Ref_Par = Ident_2;                                               \
            break;                                                                 \
        case Ident_4:                                                              \
            break;                                                                 \
        case Ident_5:                                                              \
            *Enum_Ref_Par = Ident_3;                                               \
            break;                                                                 \
        }                                                                          \
    }

#define DECL_Proc_7(thread)                                                        \
    void __attribute__ ((noinline)) Proc_7_T##thread(                              \
            int Int_1_Par_Val, int Int_2_Par_Val, int *Int_Par_Ref) {              \
        int Int_Loc;                                                               \
                                                                                   \
        Int_Loc = Int_1_Par_Val + 2;                                               \
        *Int_Par_Ref = Int_2_Par_Val + Int_Loc;                                    \
    }

#define DECL_Proc_8(thread)                                                        \
    void __attribute__ ((noinline)) Proc_8_T##thread(                              \
                int Arr_1_Par_Ref[50], int Arr_2_Par_Ref[50][50],                  \
                int Int_1_Par_Val, int Int_2_Par_Val) {                            \
        register int Int_Index;                                                    \
        register int Int_Loc;                                                      \
                                                                                   \
        Int_Loc = Int_1_Par_Val + 5;                                               \
        Arr_1_Par_Ref[Int_Loc] = Int_2_Par_Val;                                    \
        Arr_1_Par_Ref[Int_Loc + 1] = Arr_1_Par_Ref[Int_Loc];                       \
        Arr_1_Par_Ref[Int_Loc + 30] = Int_Loc;                                     \
        for (Int_Index = Int_Loc; Int_Index <= Int_Loc + 1; ++Int_Index)           \
            Arr_2_Par_Ref[Int_Loc][Int_Index] = Int_Loc;                           \
        Arr_2_Par_Ref[Int_Loc][Int_Loc - 1] += 1;                                  \
        Arr_2_Par_Ref[Int_Loc + 20][Int_Loc] = Arr_1_Par_Ref[Int_Loc];             \
        Int_Glob[thread] = 5;                                                      \
    }

#define DECL_Func_1(thread)                                                        \
    Enumeration __attribute__ ((noinline)) Func_1_T##thread(                       \
                char Ch_1_Par_Val, char Ch_2_Par_Val) {                            \
        char Ch_1_Loc;                                                             \
        char Ch_2_Loc;                                                             \
                                                                                   \
        Ch_1_Loc = Ch_1_Par_Val;                                                   \
        Ch_2_Loc = Ch_1_Loc;                                                       \
        if (Ch_2_Loc != Ch_2_Par_Val) {                                            \
            return Ident_1;                                                        \
        } else {                                                                   \
            Ch_1_Glob[thread] = Ch_1_Loc;                                          \
            return Ident_2;                                                        \
        }                                                                          \
    }

#define DECL_Func_2(thread)                                                        \
    bool __attribute__ ((noinline)) Func_2_T##thread(                              \
            char Str_1_Par_Ref[31], char Str_2_Par_Ref[31]) {                      \
        register int Int_Loc;                                                      \
        char Ch_Loc;                                                               \
                                                                                   \
        Int_Loc = 2;                                                               \
        while (Int_Loc <= 2)                                                       \
            if (Func_1_T##thread(Str_1_Par_Ref[Int_Loc],                           \
                    Str_2_Par_Ref[Int_Loc + 1]) == Ident_1) {                      \
                Ch_Loc = 'A';                                                      \
                Int_Loc += 1;                                                      \
            }                                                                      \
                                                                                   \
        if (Ch_Loc >= 'W' && Ch_Loc < 'Z')                                         \
            Int_Loc = 7;                                                           \
                                                                                   \
        if (Ch_Loc == 'R') {                                                       \
            return true;                                                           \
        } else {                                                                   \
            if (strcmp(Str_1_Par_Ref, Str_2_Par_Ref) > 0) {                        \
                Int_Loc += 7;                                                      \
                Int_Glob[thread] = Int_Loc;                                        \
                return true;                                                       \
            } else {                                                               \
                return false;                                                      \
            }                                                                      \
        }                                                                          \
    }

#define DECL_Func_3(thread)                                                        \
    bool __attribute__ ((noinline)) Func_3_T##thread(Enumeration Enum_Par_Val) {   \
        Enumeration Enum_Loc;                                                      \
                                                                                   \
        Enum_Loc = Enum_Par_Val;                                                   \
        if (Enum_Loc == Ident_3)                                                   \
            return true;                                                           \
        else                                                                       \
            return false;                                                          \
    }

#endif
