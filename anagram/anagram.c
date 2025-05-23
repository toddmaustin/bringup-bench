/*
 *  Anagram program by Raymond Chen,
 *  inspired by a similar program by Brian Scearce
 *
 *  This program is Copyright 1991 by Raymond Chen.
 *              (rjc@math.princeton.edu)
 *
 *  This program may be freely distributed provided all alterations
 *  to the original are clearly indicated as such.
 */

/* There are two tricks.  First is the Basic Idea:
 *
 * When the user types in a phrase, the phrase is first preprocessed to
 * determine how many of each letter appears.  A bit field is then constructed
 * dynamically, such that each field is large enough to hold the next power
 * of two larger than the number of times the character appears.  For example,
 * if the phrase is hello, world, the bit field would be
 *
 *   00 00 00 000 000 00 00
 *    d e   h   l   o  r  w
 *
 * The phrase hello, world, itself would be encoded as
 *
 *   01 01 01 011 010 01 01
 *    d e   h   l   o  r  w
 *
 * and the word hollow would be encoded as
 *
 *   00 00 01 010 010 00 01
 *    d  e  h   l   o  r  w
 *
 * The top bit of each field is set in a special value called the sign.
 * Here, the sign would be
 *
 *   10 10 10 100 100 10 10
 *    d  e  h   l   o  r  w
 *
 * The reason for packing the values into a bit field is that the operation
 * of subtracting out the letters of a word from the current phrase can be
 * carried out in parallel.  for example, subtracting the word hello from
 * the phrase hello, world, is merely
 *
 *    d e   h   l   o  r  w
 *   01 01 01 011 010 01 01 (dehllloorw)
 * - 00 00 01 010 010 00 01 (hlloow)
 * ========================
 *   01 01 00 001 000 01 00 (delr)
 *
 * Since none of the sign bits is set, the word fits, and we can continue.
 * Suppose the next word we tried was hood.
 *
 *    d e   h   l   o  r  w
 *   01 01 00 001 000 01 00 (delr)
 * - 01 00 01 000 010 00 00 (hood)
 * ========================
 *   00 00 11 000 110 01 00
 *         ^      ^
 * A sign bit is set.  (Two, actually.)  This means that hood does not
 * fit in delr, so we skip it and try another word.  (Observe that
 * when a sign bit becomes set, it screws up the values for the letters to
 * the left of that bit, but that's not important.)
 *
 * The inner loop of an anagram program is testing to see if a
 * word fits in the collection of untried letters.  Traditional methods
 * keep an array of 26 integers, which are then compared in turn.  This
 * means that there are 26 comparisons per word.
 *
 * This method reduces the number of comparisons to MAX_QUAD, typically 2.
 * Instead of looping through an array, we merely perform the indicated
 * subtraction and test if any of the sign bits is set.
 */

/* The nuts and bolts:
 *
 * The dictionary is loaded and preprocessed.  The preprocessed dictionary
 * is a concatenation of copies of the structure:
 *
 * struct dictword {
 *     char bStructureSize;             -- size of this structure
 *     char cLetters;                   -- number of letters in the word
 *     char achWord[];                  -- the word itself (0-terminated)
 * }
 *
 * Since this is a variable-sized structure, we keep its size in the structure
 * itself for rapid stepping through the table.
 *
 * When a phrase is typed in, it is first preprocessed as described in the
 * Basic Idea.  We then go through the dictionary, testing each word.  If
 * the word fits in our phrase, we build the bit field for its frequency
 * table and add it to the list of candidates.
 */

/*
 * The Second Trick:
 *
 * Before diving into our anagram search, we then tabulate how many times
 * each letter appears in our list of candidates, and sort the table, with
 * the rarest letter first.
 *
 * We then do our anagram search.
 *
 * Like most anagram programs, this program does a depth-first search.
 * Although most anagram programs do some sort of heuristics to decide what
 * order to place words in the list_of_candidates, the search itself proceeds
 * according to a greedy algorithm.  That is, once you find a word that fits,
 * subtract it and recurse.
 *
 * This anagram program exercises some restraint and does not march down
 * every branch that shows itself.  Instead, it only goes down branches
 * that use the rarest unused letter.  This helps to find dead ends faster.
 *
 * FindAnagram(unused_letters, list_of_candidates) {
 *  l = the rarest letter as yet unused
 *  For word in list_of_candidates {
 *     if word does not fit in unused_letters, go on to the next word.
 *     if word does not contain l, defer.
 *      FindAnagram(unused_letters - word, list_of_candidates[word,...])
 *  }
 * }
 *
 *
 * The heuristic of the Second Trick can probably be improved.  I invite
 * anyone willing to improve it to do so.
 */

/* Use the accompanying unproto perl script to remove the ANSI-style
 * prototypes, for those of you who have K&R compilers.
 */

#include "libmin.h"
#include "words.h"
#include "input.h"

MFILE __mwords = {
  "words",
  __words_sz,
  __words,
  0
};
MFILE *mwords = &__mwords;
  
MFILE __minput = {
  "input",
  __input_sz,
  __input,
  0
};
MFILE *minput = &__minput;
  
/* Before compiling, make sure Quad and MASK_BITS are set properly.  For best
 * results, make Quad the largest integer size supported on your machine.
 * So if your machine has long longs, make Quad an unsigned long long.
 * (I called it Quad because on most machines, the largest integer size
 * supported is a four-byte unsigned long.)
 *
 * If you need to be able to anagram larger phrases, increase MAX_QUADS.
 * If you increase it beyond 4, you'll have to add a few more loop unrolling
 * steps to FindAnagram.
 */

typedef unsigned long Quad;             /* for building our bit mask */
#define MASK_BITS 32                    /* number of bits in a Quad */

#define MAX_QUADS 2                     /* controls largest phrase */

#define MAXWORDS 26000                  /* dictionary length */
#define MAXCAND  5000                   /* candidates */
#define MAXSOL   51                     /* words in the solution */

#define ALPHABET 26                     /* letters in the alphabet */
#define ch2i(ch) ((ch)-'a')             /* convert letter to index */
#define i2ch(ch) ((ch)+'a')             /* convert index to letter */

/* IBM PC's don't like globs of memory larger than 64K without
 * special gyrations.  That's where the huges get stuck in.  And the
 * two types of allocations on an IBM PC need to be handled differently.
 *
 * HaltProcessing is called during the anagram search.	If it returns nonzero,
 * then the search is aborted.
 *
 * Cdecl is a macro expanded before the name of all functions that must
 * use C-style parameter passing.  This lets you change the default parameter
 * passing style for the other functions.
 */

/* char *malloc(); */
#   define huge
#   define far
#   define StringFormat    "%15s%c"
#   define bigmalloc       malloc
#   define smallmalloc     malloc
#   define smallmallocfail (char *)0
#   define HaltProcessing() 0           /* no easy way to interrupt on UNIX */
#   define Cdecl

/* Code to be used only when debugging lives inside Debug().
 * Code to be used only when collecting statistics lives inside Stat().
 */
#ifdef DEBUG
#define Debug(x) x
#else
#define Debug(x)
#endif

#ifdef STAT
#define Stat(x) x
#else
#define Stat(x)
#endif

/* A Word remembers the information about a candidate word. */
typedef struct {
    Quad aqMask[MAX_QUADS];  /* the word's mask */
    char * pchWord;                 /* the word itself */
    unsigned cchLength;                 /* letters in the word */
} Word;
typedef Word * PWord;
typedef Word * * PPWord;

PWord apwCand[MAXCAND];    /* candidates we've found so far */
unsigned cpwCand;                       /* how many of them? */


/* A Letter remembers information about each letter in the phrase to be
 * anagrammed.
 */

typedef struct {
    unsigned uFrequency;                /* how many times it appears */
    unsigned uShift;                    /* how to mask */
    unsigned uBits;                     /* the bit mask itself */
    unsigned iq;                        /* which Quad to inspect? */
} Letter;
typedef Letter * PLetter;

Letter alPhrase[ALPHABET]; /* statistics on the current phrase */
#define lPhrase(ch) alPhrase[ch2i(ch)]  /* quick access to a letter */

int cchPhraseLength;                    /* number of letters in phrase */

Quad aqMainMask[MAX_QUADS];/* the bit field for the full phrase */
Quad aqMainSign[MAX_QUADS];/* where the sign bits are */

int cchMinLength = 3;

/* auGlobalFrequency counts the number of times each letter appears, summed
 * over all candidate words.  This is used to decide which letter to attack
 * first.
 */
unsigned auGlobalFrequency[ALPHABET];
char achByFrequency[ALPHABET];          /* for sorting */

char * pchDictionary;               /* the dictionary is read here */

#define Zero(t) libmin_memset(t, 0, sizeof(t)) /* quickly zero out an integer array */

/* Fatal -- print a message before expiring */
void Fatal(char *pchMsg, unsigned u) {
    libmin_printf(pchMsg, u);
    libmin_fail(1);
}

/* ReadDict -- read the dictionary file into memory and preprocess it
 *
 * A word of length cch in the dictionary is encoded as follows:
 *
 *    byte 0    = cch + 3
 *    byte 1    = number of letters in the word
 *    byte 2... = the word itself, null-terminated
 *
 * Observe that cch+3 is the length of the total encoding.  These
 * byte streams are concatenated, and terminated with a 0.
 */

void ReadDict(char *pchFile) {
    char * pch;
    char * pchBase;
    unsigned long ulLen;
    unsigned cWords = 0;
    unsigned cLetters;
    int ch;

    ulLen = libmin_msize(mwords) + 2 * (unsigned long)MAXWORDS;
    pchBase = pchDictionary = (char *)libmin_malloc(ulLen);

    if(pchDictionary == NULL)
	Fatal("Unable to allocate memory for dictionary\n", 0);

    libmin_mopen(mwords, "r");

    while (!libmin_meof(mwords)) {
        pch = pchBase+2;                /* reserve for length */
        cLetters = 0;
        while ((ch = libmin_mgetc(mwords)) != '\n' && ch != EOF) {
            if (isalpha(ch)) cLetters++;
            *pch++ = ch;
        }
        *pch++ = '\0';
        *pchBase = pch - pchBase;
        pchBase[1] = cLetters;
        pchBase = pch;
        cWords++;
    }
    libmin_mclose(mwords);

    *pchBase++ = 0;

    libmin_printf("main dictionary has %u entries\n", cWords);
    if (cWords >= MAXWORDS)
	    Fatal("Dictionary too large; increase MAXWORDS\n", 0);
    libmin_printf("%lu bytes wasted\n", ulLen - (pchBase - pchDictionary));
}

void BuildMask(char * pchPhrase) {
    int i;
    int ch;
    unsigned iq;                        /* which Quad? */
    int cbtUsed;                        /* bits used in the current Quad */
    int cbtNeed;                        /* bits needed for current letter */
    Quad qNeed;                         /* used to build the mask */

    libmin_memset(alPhrase, 0, sizeof(Letter)*ALPHABET);
    libmin_memset(aqMainMask, 0, sizeof(Quad)*MAX_QUADS);
    libmin_memset(aqMainSign, 0, sizeof(Quad)*MAX_QUADS);
/*
    Zero(alPhrase);
    Zero(aqMainMask);
    Zero(aqMainSign);
*/

    /* Tabulate letter frequencies in the phrase */
    cchPhraseLength = 0;
    while ((ch = *pchPhrase++) != '\0') {
        if (isalpha(ch)) {
            ch = tolower(ch);
            lPhrase(ch).uFrequency++;
            cchPhraseLength++;
        }
    }

    /* Build  masks */
    iq = 0;                             /* which quad being used */
    cbtUsed = 0;                        /* bits used so far */

    for (i = 0; i < ALPHABET; i++) {
        if (alPhrase[i].uFrequency == 0) {
            auGlobalFrequency[i] = ~0;  /* to make it sort last */
        } else {
            auGlobalFrequency[i] = 0;
            for (cbtNeed = 1, qNeed = 1;
                 alPhrase[i].uFrequency >= qNeed;
                 cbtNeed++, qNeed <<= 1);
            if (cbtUsed + cbtNeed > MASK_BITS) {
                if (++iq >= MAX_QUADS)
		    Fatal("MAX_QUADS not large enough\n", 0);
                cbtUsed = 0;
            }
            alPhrase[i].uBits = qNeed-1;
            if (cbtUsed)
		qNeed <<= cbtUsed;
            aqMainSign[iq] |= qNeed;
            aqMainMask[iq] |= (Quad)alPhrase[i].uFrequency << cbtUsed;
            alPhrase[i].uShift = cbtUsed;
            alPhrase[i].iq = iq;
            cbtUsed += cbtNeed;
        }
    }
}

PWord
NewWord(void) {
    PWord pw;

    pw = (Word *)libmin_malloc(sizeof(Word));
    if (pw == NULL)
        Fatal("Out of memory after %d candidates\n", cpwCand);
    return pw;
}

/* wprint -- print a word, followed by a space
 *
 * We would normally just use printf, but the string being printed is
 * is a huge pointer (on an IBM PC), so special care must be taken.
 */
void wprint(char * pch) {
    libmin_printf("%s ", pch);
}

PWord NextWord(void);

/* NextWord -- get another candidate entry, creating if necessary */
PWord NextWord(void) {
    PWord pw;
    if (cpwCand >= MAXCAND)
	Fatal("Too many candidates\n", 0);
    pw = apwCand[cpwCand++];
    if (pw != NULL)
	return pw;
    apwCand[cpwCand-1] = NewWord();
    return apwCand[cpwCand-1];
}

/* BuildWord -- build a Word structure from an ASCII word
 * If the word does not fit, then do nothing.
 */
void BuildWord(char * pchWord) {
    unsigned char cchFrequency[ALPHABET];
    int i;
    char * pch = pchWord;
    PWord pw;
    int cchLength = 0;

    libmin_memset(cchFrequency, 0, sizeof(unsigned char)*ALPHABET);
    /* Zero(cchFrequency); */

    /* Build frequency table */
    while ((i = *pch++) != '\0') {
        if (!isalpha(i)) continue;
        i = ch2i(tolower(i));
        if (++cchFrequency[i] > alPhrase[i].uFrequency)
	    return;
        ++cchLength;
    }

    Debug(wprint(pchWord);)

    /* Update global count */
    for (i = 0; i < ALPHABET; i++)
        auGlobalFrequency[i] += cchFrequency[i];

    /* Create a Word structure and fill it in, including building the
     * bitfield of frequencies.
     */
    pw = NextWord();
    libmin_memset(pw->aqMask, 0, sizeof(Quad)*MAX_QUADS);
    /* Zero(pw->aqMask); */
    pw->pchWord = pchWord;
    pw->cchLength = cchLength;
    for (i = 0; i < ALPHABET; i++) {
        pw->aqMask[alPhrase[i].iq] |=
            (Quad)cchFrequency[i] << alPhrase[i].uShift;
    }
}

/* AddWords -- build the list of candidates */
void
AddWords(void) {
    char * pch = pchDictionary;     /* walk through the dictionary */

    cpwCand = 0;

    while (*pch) {
        if ((pch[1] >= cchMinLength && pch[1]+cchMinLength <= cchPhraseLength)
            || pch[1] == cchPhraseLength)
	    BuildWord(pch+2);
        pch += *pch;
    }

    libmin_printf("%d candidates\n", cpwCand);
}

void DumpCandidates(void) {
    unsigned u;

    for (u = 0; u < cpwCand; u++)
        libmin_printf(StringFormat, apwCand[u]->pchWord, (u % 4 == 3) ? '\n' : ' ');
    libmin_printf("\n");
}

PWord apwSol[MAXSOL];                   /* the answers */
int cpwLast;

Debug(
void DumpWord(Quad * pq) {
    int i;
    Quad q;
    for (i = 0; i < ALPHABET; i++) {
        if (alPhrase[i].uFrequency == 0) continue;
        q = pq[alPhrase[i].iq];
        if (alPhrase[i].uShift) q >>= alPhrase[i].uShift;
        q &= alPhrase[i].uBits;
        while (q--) libmin_putc('a'+i);
    }
    libmin_putc(' ');
}
)                                       /* End of debug code */

void DumpWords(void) {
    int i;
    for (i = 0; i < cpwLast; i++) wprint(apwSol[i]->pchWord);
    libmin_printf("\n");
}

Stat(unsigned long ulHighCount; unsigned long ulLowCount;)

#define OneStep(i) \
    if ((aqNext[i] = pqMask[i] - pw->aqMask[i]) & aqMainSign[i]) { \
        ppwStart++; \
        continue; \
    }


void
FindAnagram(Quad * pqMask, PPWord ppwStart, int iLetter)
{
    Quad aqNext[MAX_QUADS];
    register PWord pw;
    Quad qMask;
    unsigned iq;
    PPWord ppwEnd = &apwCand[0];
    ppwEnd += cpwCand;

    ;

    Debug(libmin_printf("Trying :"); DumpWord(pqMask); libmin_printf(":\n");)

    for (;;) {
        iq = alPhrase[(int)achByFrequency[iLetter]].iq;
        qMask = alPhrase[(int)achByFrequency[iLetter]].uBits <<
                alPhrase[(int)achByFrequency[iLetter]].uShift;
        if (pqMask[iq] & qMask) break;
        iLetter++;
    }

    Debug(libmin_printf("Pivoting on %c\n", i2ch(achByFrequency[iLetter]));)

    while (ppwStart < ppwEnd) {          /* Half of the program execution */
        pw = *ppwStart;                  /* time is spent in these three */

        Stat(if (++ulLowCount == 0) ++ulHighCount;)

#if MAX_QUADS > 0
        OneStep(0);                     /* lines of code. */
#endif

#if MAX_QUADS > 1
        OneStep(1);
#endif

#if MAX_QUADS > 2
        OneStep(2);
#endif

#if MAX_QUADS > 3
        OneStep(3);
#endif

#if MAX_QUADS > 4
            @@"Add more unrolling steps here, please."@@
#endif

        /* If the pivot letter isn't present, defer this word until later */
        if ((pw->aqMask[iq] & qMask) == 0) {
            *ppwStart = *--ppwEnd;
            *ppwEnd = pw;
            continue;
        }

        /* If we get here, this means the word fits. */
        apwSol[cpwLast++] = pw;
        if (cchPhraseLength -= pw->cchLength) { /* recurse */
            Debug(DumpWords();)
            /* The recursive call scrambles the tail, so we have to be
             * pessimistic.
             */
	    ppwEnd = &apwCand[0];
	    ppwEnd += cpwCand;
            FindAnagram(&aqNext[0],
			ppwStart, iLetter);
        } else DumpWords();             /* found one */
        cchPhraseLength += pw->cchLength;
        --cpwLast;
        ppwStart++;
        continue;
    }

    ;
}

int Cdecl CompareFrequency(char *pch1, char *pch2) {
    return auGlobalFrequency[(int)(*pch1)] < auGlobalFrequency[(int)(*pch2)]
        ?  -1 :
           auGlobalFrequency[(int)(*pch1)] == auGlobalFrequency[(int)(*pch2)]
        ?   0 : 1;
}

void SortCandidates(void) {
    int i;

    /* Sort the letters by frequency */
    for (i = 0; i < ALPHABET; i++) achByFrequency[i] = i;
    libmin_qsort(achByFrequency, ALPHABET, sizeof(char),
          (int (*)(const void *, const void *))CompareFrequency);

    libmin_printf("Order of search will be ");
    for (i = 0; i < ALPHABET; i++)
	    libmin_putc(i2ch(achByFrequency[i]));
    libmin_putc('\n');
}

char * GetPhrase(char * pch) {
    if (libmin_mgets(pch, 255, minput) == NULL) {
	    return NULL;
    }
    // libmin_printf("Processing: %s...\n", pch);
    return(pch);
}

char achPhrase[255];

int 
main(int cpchArgc, char **ppchArgv)
{
    ReadDict(ppchArgv[1]);

    while (GetPhrase(&achPhrase[0]) != NULL) {
        if (isdigit(achPhrase[0])) {
            cchMinLength = libmin_atoi(achPhrase);
            libmin_printf("New length: %d\n", cchMinLength);
        } else if (achPhrase[0] == '?') {
            DumpCandidates();
        } else {
            BuildMask(&achPhrase[0]);
            AddWords();
            if (cpwCand == 0 || cchPhraseLength == 0) continue;

            Stat(ulHighCount = ulLowCount = 0;)
            cpwLast = 0;
            SortCandidates();
            FindAnagram(&aqMainMask[0], &apwCand[0], 0);
            Stat(libmin_printf("%lu:%lu probes\n", ulHighCount, ulLowCount);)
        }
    }

    libmin_success();
    return 0;
}
