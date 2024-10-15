/***************************************************************************
 *   fc.c                                     Version 20180915.214818      *
 *                                                                         *
 *   Fraction Calculator                                                   *
 *   Copyright (C) 2018         by Iago Gade Gusmao Carrazzoni             *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *   To contact the author, please write to:                               *
 *   Iago Gade Gusmao Carrazzoni                                           *
 *   Email: iagocarrazzoni@gmail.com                                       *
 *   Webpage: http://www.upe.br                                            *
 *   Phone: +55 (81) 99479-0584                                            *
 ***************************************************************************/

/* ---------------------------------------------------------------------- */
/**
 * @file frac.c
 * @ingroup GroupUnique
 * @brief Fraction Calculator
 * @details This program really do a nice job as a template, and template only!
 * @version 20180915.214818
 * @date 2018-09-15
 * @author Iago Gade Gusmao Carrazzoni <<iagocarrazzoni@gmail.com>>
 * @par Webpage
 * <<a href="http://www.upe.br">www.upe.br</a>>
 * @copyright (c) 2018 GNU GPL v2
 * @note This program is free software: you can redistribute it
 * and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA. 02111-1307, USA.
 * Or read it online at <<http://www.gnu.org/licenses/>>.
 *
 *
 * @todo Now that you have the template, hands on! Programme!
 * @warning Be carefull not to lose your mind in small things.
 * @bug This file right now does nothing usefull
 *
 */

/*
 * Instrucoes para compilar:
 *   $gcc frac.c -o frac.x -Wall
 *        -Wextra -ansi -pedantic-errors -g -O0 -DDEBUG=1 -DVERSION="0.1.160612.142044"
 *   ou preferencialmente inclua um makefile e use:
 *   $make
 *
 * Modelo de indentacao:
 * Estilo: --style=allman ou -A1
 *
 * Opcoes: -A1 -s4 -k3 -xj -SCNeUpv
 *
 *  * No vi use:
 *      :% !astyle -A1 -s4 -k3 -xj -SCNeUpv
 *  * No linux:
 *      $astlye -A1 -s4 -k3 -xj -SCNeUpv frac.c
 */

/* ---------------------------------------------------------------------- */
/* includes */

#include "libmin.h"
#include "input-txt.h"

MFILE input = {
  "input.h",
  __input_sz,
  __input,
  0
};
MFILE *minput = &input;

/* ---------------------------------------------------------------------- */
/* definitions */

#ifndef VERSION /* gcc -DVERSION="0.1.160612.142628" */
#define VERSION "20180915.214818" /**< Version Number (string) */
#endif

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

#if DEBUG==0
#define NDEBUG
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) libmin_printf("[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define SBUFF 60 /**< String buffer */
#define MYMAX 11

/* ---------------------------------------------------------------------- */
/* globals */

static int verb = 0; /**< verbose level, global within the file */

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void frac_init(char exp[SBUFF]); /* global initialization function */
void divtokens(char exp[SBUFF], int frac[MYMAX]); /* break the string in tokens */
void avaliatokens(char *aux[SBUFF],  int frac[MYMAX], int i); /* break the string in tokens */
void misto(int mist, int frac[MYMAX], int i); /* fraction to mixed number */
float calcula(int frac[MYMAX], int r[MYMAX]); /* computes the fraction */
void simplifica(int r[MYMAX]); /* simplifies the result */
void print(char exp[SBUFF], int r[MYMAX], float dec, int frac[MYMAX]); /* print the results */

/* main */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This is the main event of the evening
 * @details Ladies and Gentleman... It's tiiiime!
 * Fightiiiiing at the blue corner,
 * he, who has compiled more C code than any other adversary in the history,
 * he, who has developed UNIX and Linux, and is an inspiration to maaany languages
 * and compilers, the GNU C Compiler, GCC!
 * Fightiiiiing at the red corner, the challenger, in his first fight, lacking of any
 * valid experience but angrily, blindly, and no doubtfully, will try to
 * compile this program without errors. He, the student, the apprentice,
 * the developer, iggc!!
 *
 * @param[in] argc Argument counter
 * @param[in] argv Argument strings (argument values)
 *
 * @retval 0 If succeed (EXIT_SUCCESS).
 * @retval 1 Or another error code if failed.
 *
 * @par Example
 * @code
 *    $./frac -h
 * @endcode
 *
 * @warning   Be carefull with...
 * @bug There is a bug with...
 * @todo Need to do...
 * @note You can read more about it at <<a href="http://www.upe.br">www.upe.br</a>>
 * @author Iago Gade Gusmao Carrazzoni
 * @version 20180915.214818
 * @date 2018-09-15
 *
 */
int main(int argc, char *argv[])
{
    int opt; /* return from getopt() */
    char exp[SBUFF]; /* expression */
    int frac[MYMAX];
    int r[MYMAX];
    float dec;
    char rep[SBUFF];
    int repi = 1;

    /* IFDEBUG("Starting optarg loop..."); */

    /* getopt() configured options:
     *        -h  help
     *        -V  version
     *        -v  verbose
     */
    opterr = 0;
    while((opt = libmin_getopt(argc, argv, "vhV")) != EOF)
        switch(opt)
        {
            case 'h':
                help();
                break;
            case 'V':
                copyr();
                break;
            case 'v':
                verb++;
                break;
            case '?':
            default:
                libmin_printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return 1;
        }

    if(verb)
        libmin_printf("Verbose level set at: %d\n", verb);

    do
    {
        frac_init(exp); /* initialization function */
        divtokens(exp, frac); /* break the string in tokens */
        dec = calcula(frac, r); /* computes the fraction */
        print(exp, r, dec, frac); /* prints the results */
        libmin_printf("\nDo you want to do another operation?\nenter 1 for yes\n");
        libmin_mgets(rep, SBUFF, minput);
        repi = libmin_atoi(rep);
    }while(repi == 1);

    libmin_success();
    return 0;
}

/* functions */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints help information and exit
 * @details Prints help information (usually called by opt -h)
 * @return Void
 * @author Iago Gade Gusmao Carrazzoni
 * @version 20180915.214818
 * @date 2018-09-15
 *
 */
void help(void)
{
    IFDEBUG("help()");
    libmin_printf("%s - %s\n", "frac", "Fraction Calculator");
    libmin_printf("\nUsage: %s [-h|-v]\n", "frac");
    libmin_printf("\nOptions:\n");
    libmin_printf("\t-h,  --help\n\t\tShow this help.\n");
    libmin_printf("\t-V,  --version\n\t\tShow version and copyright information.\n");
    libmin_printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    /* add more options here */
    libmin_printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    libmin_printf("\nTodo:\n\tLong options not implemented yet.\n");
    libmin_printf("\nAuthor:\n\tWritten by %s <%s>\n\n", "Iago Gade Gusmao Carrazzoni", "iagocarrazzoni@gmail.com");
    libmin_fail(1);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief Prints version and copyright information and exit
 * @details Prints version and copyright information (usually called by opt -V)
 * @return Void
 * @author Iago Gade Gusmao Carrazzoni
 * @version 20180915.214818
 * @date 2018-09-15
 *
 */
void copyr(void)
{
    IFDEBUG("copyr()");
    libmin_printf("%s - Version %s\n", "frac", VERSION);
    libmin_printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 <http://gnu.org/licenses/gpl.html>. This  is  free  software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2018, "Iago Gade Gusmao Carrazzoni", "iagocarrazzoni@gmail.com");
    if(verb > 3) libmin_printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
    libmin_fail(1);
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup GroupUnique
 * @brief This function initializes some operations before start
 * @details Details to be written in
 * multiple lines
 *
 * @pre You need to call foo() before calling this function
 *
 * @param[in] i Input parameter that does bla
 * @param[out] o Parameter that outputs ble
 * @param[in,out] z The @a z variable is used as input and output
 *
 * @retval 0 Returned when bli
 * @retval 1 Error code: function returned blo
 *
 * @par Example
 * @code
 *    if(x == funcexample(i, o, z))
 *       printf("And that is it\n");
 * @endcode
 *
 * @return Void
 *
 * @warning Be carefull with blu
 * @todo Need to implement it. Its empty now. This doxygen tags are overwhelming.
 * Mandatory tags are: ingroup, brief, details, param, return, author and date.
 * The others are optional.
 *
 * @deprecated This function will be deactivated in version +11
 * @see help()
 * @see copyr()
 * @bug There is a bug with x greater than y
 * @note You can read more about it at <<a href="http://www.upe.br">www.upe.br</a>>
 *
 * @author Iago Gade Gusmao Carrazzoni
 * @version 20180915.214818
 * @date 2018-09-15
 * @copyright Use this tag only if not the same as the whole file
 *
 */
void frac_init(char exp[SBUFF])
{
    /* IFDEBUG("frac_init()"); */
    libmin_printf("-------------------\n");
    libmin_printf("FRACTION CALCULATOR\nPlease entry the desired operation:\n(It is necessary to add a space between the fractions, like a/b + c/d)\n-> ");
    libmin_mgets(exp, SBUFF, minput); /* expression input */

    return;
}

/* ---------------------------------------------------------------------- */
void divtokens(char exp[SBUFF],  int frac[MYMAX]) /* break the string in tokens */
{
    char *token; /* token pointer */
    char exp2[SBUFF];
    char *aux[SBUFF];
    int i = 0;

    libmin_strcpy(exp2, exp); /* to maintain the original expression untouched */

    token = libmin_strtok(exp2, " "); /* divides the equation into tokens */
    while(token != NULL)
    {
        aux[i] = token;
        token = libmin_strtok(NULL, " ");
        i++;
    }

    frac[MYMAX - 2] = i; /* number of elements of the expression */

    avaliatokens(aux, frac, i);

    return;
}

/* ---------------------------------------------------------------------- */
void avaliatokens(char *aux[SBUFF],  int frac[MYMAX], int i) /* analyzes the tokens, to check the fractions */
{
    int j, k = 0;
    char *token2;
    int mist;

    /* 1 mist first fraction */
    if(((libmin_strcmp(aux[2], "+") == 0) || (libmin_strcmp(aux[2], "-") == 0) || (libmin_strcmp(aux[2], "*") == 0) || (libmin_strcmp(aux[2], "/") == 0)) && i == 4)
        mist = 1;
    /* 1 mist second fraction */
    if(((libmin_strcmp(aux[1], "+") == 0) || (libmin_strcmp(aux[1], "-") == 0) || (libmin_strcmp(aux[1], "*") == 0) || (libmin_strcmp(aux[1], "/") == 0)) && i == 4)
        mist = 2;
    
    for(j = 0; j < i; j++)
    {
        if((libmin_strcmp(aux[j], "+") == 0))
        {
            frac[MYMAX - 1] = 1;
            j++;
        }
        if((libmin_strcmp(aux[j], "-") == 0))
        {
            frac[MYMAX - 1] = 2;
            j++;
        }
        if((libmin_strcmp(aux[j], "*") == 0))
        {
            frac[MYMAX - 1] = 3;
            j++;
        }
        if((libmin_strcmp(aux[j], "/") == 0))
        {
            frac[MYMAX - 1] = 4;
            j++;
        }

        token2 = libmin_strtok(aux[j], "/"); /* divides the equation into tokens */
        while(token2 != NULL)
        {
            frac[k] = libmin_atoi(token2); /* stores tokens in the array */
            token2 = libmin_strtok(NULL, "/");
            k++;
        }
    }
    misto(mist, frac, i);

    return;
}

/* ---------------------------------------------------------------------- */
void misto(int mist, int frac[MYMAX], int i) /* fraction to mixed number */
{
    /* 2 fractions no mist */
    if(i == 3)
    {
        frac[MYMAX - 2] = frac[0]; /* num 1 */
        frac[MYMAX - 3] = frac[1]; /* den 1 */
        frac[MYMAX - 4] = frac[2]; /* num 2 */
        frac[MYMAX - 5] = frac[3]; /* den 2*/ 
        return;
    }
    /* 1 mist first fraction */
    if(mist == 1)
    {
        frac[0] = frac[2] * frac[0] + frac[1]; /* num */
        frac[1] = frac[2]; /* den */
        frac[2] = frac[3]; /* num */
        frac[3] = frac[4]; /* den */
        frac[MYMAX - 2] = frac[0]; /* num 1 */
        frac[MYMAX - 3] = frac[1]; /* den 1 */
        frac[MYMAX - 4] = frac[2]; /* num 2 */
        frac[MYMAX - 5] = frac[3]; /* den 2*/ 

        return;
    }
    /* 1 mist second fraction */
    if(mist == 2)
    {
        frac[2] = frac[4] * frac[2] + frac[3];
        frac[3] = frac[4];
        frac[MYMAX - 2] = frac[0]; /* num 1 */
        frac[MYMAX - 3] = frac[1]; /* den 1 */
        frac[MYMAX - 4] = frac[2]; /* num 2 */
        frac[MYMAX - 5] = frac[3]; /* den 2*/ 
        return;
    }
    /* 2 mist */
    if(i == 5)
    {
        frac[0] = frac[2] * frac[0] + frac[1]; /* num */
        frac[1] = frac[2]; /* den */
        frac[2] = frac[5] * frac[3] + frac[4]; /* num */
        frac[3] = frac[5]; /* den */
        frac[MYMAX - 2] = frac[0]; /* num 1 */
        frac[MYMAX - 3] = frac[1]; /* den 1 */
        frac[MYMAX - 4] = frac[2]; /* num 2 */
        frac[MYMAX - 5] = frac[3]; /* den 2*/ 
        return;
    }
    return;
}

/* ---------------------------------------------------------------------- */
float calcula(int frac[MYMAX], int r[MYMAX]) /* computes the fraction */
{
    float dec = 0, a, b;

    r[0] = frac[1] * frac[3]; /* mmc */
    switch(frac[MYMAX - 1]) /* operation */
    {
        case 1: /* + */
            r[1] = (r[0] / frac[1]) * frac[0] + (r[0] / frac[3]) * frac[2];
            a = r[1];
            b = r[0];
            dec = a / b; /* decimal */
            simplifica(r);
            break;
        case 2: /* - */
            r[1] = (r[0] / frac[1]) * frac[0] - (r[0] / frac[3]) * frac[2];
            a = r[1];
            b = r[0];
            dec = a / b; /* decimal */
            simplifica(r);
            break;
        case 3: /* * */
            r[1] = frac[0] * frac[2];
            a = r[1];
            b = r[0];
            dec = a / b; /* decimal */
            simplifica(r);
            break;
        case 4: /* / */
            r[0] = frac[1] * frac[2];
            r[1] = frac[0] * frac[3];
            a = r[1];
            b = r[0];
            dec = a / b; /* decimal */
            simplifica(r);
            break;
    }

    return dec;
}

/* ---------------------------------------------------------------------- */
void simplifica(int r[MYMAX]) /* simplifies the result */
{
    int i;
    int aux1, aux2;

    aux1 = r[1];
    aux2 = r[0];

    for(i = 1; i < MYMAX + 2; i++)
    {
        if(aux1%i == 0 && aux2%i == 0) /* simplified */
        {
            aux1 /= i;
            aux2 /= i;
        }
    }
    r[3] = aux1;
    r[4] = aux2;

    return;
}

/* ---------------------------------------------------------------------- */
void print(char exp[SBUFF], int r[MYMAX], float dec, int frac[MYMAX]) /* print the results */
{
    char op;

    if(frac[MYMAX - 1] == 1)
        op = '+';
    if(frac[MYMAX - 1] == 2)
        op = '-';
    if(frac[MYMAX - 1] == 3)
        op = '*';
    if(frac[MYMAX - 1] == 4)
        op = '/';
    
    libmin_printf("\nEXPRESSION: %sFRACTION: %d/%d %c %d/%d\nINTERMEDIATE: %d/%d\nRESULT: %d/%d\nDECIMAL: %.3f\n", exp, frac[MYMAX - 2], frac[MYMAX - 3], op, frac[MYMAX - 4], frac[MYMAX - 5], r[1], r[0], r[3], r[4], dec);

    return;
}

/* ---------------------------------------------------------------------------- */
/* vi: set ai cin et ts=4 sw=4 tw=0 wm=0 fo=croqltn : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc>  Version 20160714.153029           */
