#include "libmin.h"

#ifndef TIME
#ifndef CLOCK
#ifndef OS2TMR
#undef TIMES
#define TIMES
#endif
#endif
#endif
                /* Use times(2) time function unless    */
                /* explicitly defined otherwise         */

#ifdef CLOCK
#ifdef HZ
#undef HZ
#endif
#define HZ	CLK_TCK
#endif
		/* Use Microsoft C hi-res clock */

#define Mic_secs_Per_Second     1000000L
                /* Berkeley UNIX C returns process times in seconds/HZ */

#ifdef  NOSTRUCTASSIGN
#define structassign(d, s)      libmin_memcpy(&(d), &(s), sizeof(d))
#else
#define structassign(d, s)      d = s
#endif

#ifdef  NOENUM
#define Ident_1 0
#define Ident_2 1
#define Ident_3 2
#define Ident_4 3
#define Ident_5 4
  typedef int   Enumeration;
#else
  typedef       enum    {Ident_1, Ident_2, Ident_3, Ident_4, Ident_5}
                Enumeration;
#endif
        /* for boolean and enumeration types in Ada, Pascal */

/* General definitions: */

#define Null 0
                /* Value of a Null pointer */
#define true  1
#define false 0

typedef int     One_Thirty;
typedef int     One_Fifty;
typedef char    Capital_Letter;
typedef int     Boolean;
typedef char    Str_30 [31];
typedef int     Arr_1_Dim [50];
typedef int     Arr_2_Dim [50] [50];

typedef struct record
    {
    struct record *Ptr_Comp;
    Enumeration    Discr;
    union {
          struct {
                  Enumeration Enum_Comp;
                  int         Int_Comp;
                  char        Str_Comp [31];
                  } var_1;
          struct {
                  Enumeration E_Comp_2;
                  char        Str_2_Comp [31];
                  } var_2;
          struct {
                  char        Ch_1_Comp;
                  char        Ch_2_Comp;
                  } var_3;
          } variant;
      } Rec_Type, *Rec_Pointer;

/* Global Variables: */

static Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
static Rec_Type rec1, rec2;

int             Int_Glob;
static Boolean         Bool_Glob;
char            Ch_1_Glob;
static char            Ch_2_Glob;
static int             Arr_1_Glob [50];
static int             Arr_2_Glob [50] [50];

  /*
  forward declaration necessary since Enumeration may not simply be int
  */

#ifndef ROPT
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
#define REG register
#endif

/* variables for time measurement: */

#define Too_Small_Time 2
                /* Measurements should last at least 2 seconds */

REG    int      Pnumber_of_runs;

/* end of variables for time measurement */

static int Proc_1 (struct record*);
static int Proc_2 (int*);
static int Proc_3 (struct record**);
static int Proc_4 (void); /* without parameters */
static int Proc_5 (void); /* without parameters */
int PProc_7 (int, int, int*);
int PProc_8 (Arr_1_Dim, Arr_2_Dim, int, int);
Enumeration PFunc_1 (char, char);
Boolean PFunc_2 (Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
Boolean PFunc_3 (Enumeration);
int PProc_6 (Enumeration, Enumeration*);

int
main(void)
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
  int nofr = 10;

        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Pnumber_of_runs;

  /* Initializations */

  Next_Ptr_Glob = &rec1;
  Ptr_Glob = &rec2;

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  libmin_strcpy (Ptr_Glob->variant.var_1.Str_Comp,
          "DHRYSTONE PROGRAM, SOME STRING");
  libmin_strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Pnumber_of_runs > 32000,  */
        /* overflow may occur for this array element.                   */

  libmin_printf("\n");
  libmin_printf("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  libmin_printf("\n");

  Pnumber_of_runs = nofr; // HCH

  libmin_printf("Execution starts, %d runs through Dhrystone\n",Pnumber_of_runs);

  /***************/
  /* Start timer */
  /***************/

  for (Run_Index = 1; Run_Index <= Pnumber_of_runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    libmin_strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! PFunc_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      PProc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    PProc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == PFunc_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        PProc_6 (Ident_1, &Enum_Loc);
        libmin_strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

  libmin_printf("Execution ends\n");
  libmin_printf("\n");
  libmin_printf("Final values of the variables used in the benchmark:\n");
  libmin_printf("\n");
  libmin_printf("Int_Glob:            %d\n", Int_Glob);
  libmin_printf("        should be:   %d\n", 5);
  libmin_printf("Bool_Glob:           %d\n", Bool_Glob);
  libmin_printf("        should be:   %d\n", 1);
  libmin_printf("Ch_1_Glob:           %c\n", Ch_1_Glob);
  libmin_printf("        should be:   %c\n", 'A');
  libmin_printf("Ch_2_Glob:           %c\n", Ch_2_Glob);
  libmin_printf("        should be:   %c\n", 'B');
  libmin_printf("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  libmin_printf("        should be:   %d\n", 7);
  libmin_printf("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  libmin_printf("        should be:   Pnumber_of_runs + 10\n");
  libmin_printf("Ptr_Glob->\n");
  libmin_printf("  Ptr_Comp:          (implementation-dependent)\n");
  libmin_printf("        should be:   (implementation-dependent)\n");
  libmin_printf("  Discr:             %d\n", Ptr_Glob->Discr);
  libmin_printf("        should be:   %d\n", 0);
  libmin_printf("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  libmin_printf("        should be:   %d\n", 2);
  libmin_printf("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  libmin_printf("        should be:   %d\n", 17);
  libmin_printf("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  libmin_printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  libmin_printf("Next_Ptr_Glob->\n");
  libmin_printf("  Ptr_Comp:          (implementation-dependent)\n");
  libmin_printf("        should be:   (implementation-dependent), same as above\n");
  libmin_printf("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  libmin_printf("        should be:   %d\n", 0);
  libmin_printf("  Enum_Comp:         %d\n",
         Next_Ptr_Glob->variant.var_1.Enum_Comp);
  libmin_printf("        should be:   %d\n", 1);
  libmin_printf("  Int_Comp:          %d\n",
         Next_Ptr_Glob->variant.var_1.Int_Comp);
  libmin_printf("        should be:   %d\n", 18);
  libmin_printf("  Str_Comp:          %s\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  libmin_printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  libmin_printf("Int_1_Loc:           %d\n", Int_1_Loc);
  libmin_printf("        should be:   %d\n", 5);
  libmin_printf("Int_2_Loc:           %d\n", Int_2_Loc);
  libmin_printf("        should be:   %d\n", 13);
  libmin_printf("Int_3_Loc:           %d\n", Int_3_Loc);
  libmin_printf("        should be:   %d\n", 7);
  libmin_printf("Enum_Loc:            %d\n", Enum_Loc);
  libmin_printf("        should be:   %d\n", 1);
  libmin_printf("Str_1_Loc:           %s\n", Str_1_Loc);
  libmin_printf("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  libmin_printf("Str_2_Loc:           %s\n", Str_2_Loc);
  libmin_printf("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  libmin_printf("\n");

  libmin_success();
  return 0;
}


static int Proc_1 (Rec_Pointer Ptr_Val_Par)
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    PProc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    PProc_7 (Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
  return 0;
} /* Proc_1 */


static int Proc_2 (One_Fifty *Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc = 0;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
  return 0;
} /* Proc_2 */


static int Proc_3 (Rec_Pointer *Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */
{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  PProc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
  return 0;
} /* Proc_3 */


static int Proc_4 (void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
  return 0;
} /* Proc_4 */


static int Proc_5 (void) /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
  return 0;
} /* Proc_5 */


/*
 *************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_2.c (part 3 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 *************************************************************************
 */

#ifndef REG
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#endif

extern  int     Int_Glob;
extern  char    Ch_1_Glob;

static int Proc_1 (struct record*);
static int Proc_2 (int*);
static int Proc_3 (struct record**);
static int Proc_4 (void); /* without parameters */
static int Proc_5 (void); /* without parameters */
int PProc_7 (int, int, int*);
int PProc_8 (Arr_1_Dim, Arr_2_Dim, int, int);
Enumeration PFunc_1 (char, char);
Boolean PFunc_2 (Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
Boolean PFunc_3 (Enumeration);
int PProc_6 (Enumeration, Enumeration*);


int
PProc_6 (Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par)
/*********************************/
    /* executed once */
    /* Enum_Val_Par == Ident_3, Enum_Ref_Par becomes Ident_2 */
{
  *Enum_Ref_Par = Enum_Val_Par;
  if (! PFunc_3 (Enum_Val_Par))
    /* then, not executed */
    *Enum_Ref_Par = Ident_4;
  switch (Enum_Val_Par)
     {
     case Ident_1:
        *Enum_Ref_Par = Ident_1;
        break;
     case Ident_2:
        if (Int_Glob > 100)
           /* then */
           *Enum_Ref_Par = Ident_1;
        else
           *Enum_Ref_Par = Ident_4;
        break;
     case Ident_3: /* executed */
        *Enum_Ref_Par = Ident_2;
        break;
     case Ident_4:
        break;
     case Ident_5:
        *Enum_Ref_Par = Ident_3;
        break;
     } /* switch */
  return 0;
} /* PProc_6 */


int
PProc_7 (int Int_1_Par_Val, int Int_2_Par_Val, int *Int_Par_Ref)
/**********************************************/
    /* executed three times                                      */
    /* first call:      Int_1_Par_Val == 2, Int_2_Par_Val == 3,  */
    /*                  Int_Par_Ref becomes 7                    */
    /* second call:     Int_1_Par_Val == 10, Int_2_Par_Val == 5, */
    /*                  Int_Par_Ref becomes 17                   */
    /* third call:      Int_1_Par_Val == 6, Int_2_Par_Val == 10, */
    /*                  Int_Par_Ref becomes 18                   */
{
  One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 2;
  *Int_Par_Ref = Int_2_Par_Val + Int_Loc;
  return 0;
} /* PProc_7 */


int PProc_8 (Arr_1_Dim Arr_1_Par_Ref,
             Arr_2_Dim Arr_2_Par_Ref,
             int Int_1_Par_Val,
             int Int_2_Par_Val)
/*********************************************************************/
    /* executed once      */
    /* Int_Par_Val_1 == 3 */
    /* Int_Par_Val_2 == 7 */
{
  REG One_Fifty Int_Index;
  REG One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 5;
  Arr_1_Par_Ref [Int_Loc] = Int_2_Par_Val;
  Arr_1_Par_Ref [Int_Loc+1] = Arr_1_Par_Ref [Int_Loc];
  Arr_1_Par_Ref [Int_Loc+30] = Int_Loc;
  for (Int_Index = Int_Loc; Int_Index <= Int_Loc+1; ++Int_Index)
     Arr_2_Par_Ref [Int_Loc] [Int_Index] = Int_Loc;
  Arr_2_Par_Ref [Int_Loc] [Int_Loc-1] += 1;
  Arr_2_Par_Ref [Int_Loc+20] [Int_Loc] = Arr_1_Par_Ref [Int_Loc];
  Int_Glob = 5;
  return 0;
} /* PProc_8 */


Enumeration PFunc_1 (Capital_Letter Ch_1_Par_Val, Capital_Letter Ch_2_Par_Val)
/*************************************************/
    /* executed three times                                         */
    /* first call:      Ch_1_Par_Val == 'H', Ch_2_Par_Val == 'R'    */
    /* second call:     Ch_1_Par_Val == 'A', Ch_2_Par_Val == 'C'    */
    /* third call:      Ch_1_Par_Val == 'B', Ch_2_Par_Val == 'C'    */
{
  Capital_Letter        Ch_1_Loc;
  Capital_Letter        Ch_2_Loc;

  Ch_1_Loc = Ch_1_Par_Val;
  Ch_2_Loc = Ch_1_Loc;
  if (Ch_2_Loc != Ch_2_Par_Val)
    /* then, executed */
    return (Ident_1);
  else  /* not executed */
     {
     Ch_1_Glob = Ch_1_Loc;
     return (Ident_2);
     }
} /* PFunc_1 */


Boolean PFunc_2 (Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref)
/*************************************************/
    /* executed once */
    /* Str_1_Par_Ref == "DHRYSTONE PROGRAM, 1'ST STRING" */
    /* Str_2_Par_Ref == "DHRYSTONE PROGRAM, 2'ND STRING" */
{
  REG One_Thirty        Int_Loc;
      Capital_Letter    Ch_Loc = 0;

  Int_Loc = 2;
  while (Int_Loc <= 2) /* loop body executed once */
    if (PFunc_1 (Str_1_Par_Ref[Int_Loc], Str_2_Par_Ref[Int_Loc+1]) == Ident_1)
      /* then, executed */
       {
       Ch_Loc = 'A';
       Int_Loc += 1;
       } /* if, while */
  if (Ch_Loc >= 'W' && Ch_Loc < 'Z')
    /* then, not executed */
    Int_Loc = 7;
  if (Ch_Loc == 'R')
    /* then, not executed */
    return (true);
  else /* executed */
     {
     if (libmin_strcmp (Str_1_Par_Ref, Str_2_Par_Ref) > 0)
      /* then, not executed */
        {
        Int_Loc += 7;
        Int_Glob = Int_Loc;
        return (true);
        }
     else /* executed */
        return (false);
     } /* if Ch_Loc */
} /* PFunc_2 */


Boolean PFunc_3 (Enumeration Enum_Par_Val)
/***************************/
    /* executed once        */
    /* Enum_Par_Val == Ident_3 */
{
  Enumeration Enum_Loc;

  Enum_Loc = Enum_Par_Val;
  if (Enum_Loc == Ident_3)
    /* then, executed */
    return (true);
  else /* not executed */
    return (false);
} /* PFunc_3 */

