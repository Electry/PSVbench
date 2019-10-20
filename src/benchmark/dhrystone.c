#include <psp2/kernel/processmgr.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#pragma GCC push_options
#pragma GCC optimize ("Ofast")

#include "dhrystone.h"
#include "dhrystone_impl.h"

#define MAX_THREADS 3

// Per-thread Global Variables:
Rec_Pointer Ptr_Glob[MAX_THREADS], Next_Ptr_Glob[MAX_THREADS];
int Int_Glob[MAX_THREADS];
bool Bool_Glob[MAX_THREADS];
char Ch_1_Glob[MAX_THREADS], Ch_2_Glob[MAX_THREADS];
int Arr_1_Glob[MAX_THREADS][50];
int Arr_2_Glob[MAX_THREADS][50][50];

DECL_DHRYSTONE_ALL(0)
DECL_DHRYSTONE_ALL(1)
DECL_DHRYSTONE_ALL(2)

#pragma GCC pop_options
