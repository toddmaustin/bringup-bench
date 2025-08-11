/*
 *
 * Mini regex-module inspired by Rob Pike's regex code described in:
 *
 * http://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
 *
 *
 *
 * Supports:
 * ---------
 *   '.'        Dot, matches any character
 *   '^'        Start anchor, matches beginning of string
 *   '$'        End anchor, matches end of string
 *   '*'        Asterisk, match zero or more (greedy)
 *   '+'        Plus, match one or more (greedy)
 *   '?'        Question, match zero or one (non-greedy)
 *   '[abc]'    Character class, match if one of {'a', 'b', 'c'}
 *   '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'} -- NOTE: feature is currently broken!
 *   '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
 *   '\s'       Whitespace, \t \f \r \n \v and spaces
 *   '\S'       Non-whitespace
 *   '\w'       Alphanumeric, [a-zA-Z0-9_]
 *   '\W'       Non-alphanumeric
 *   '\d'       Digits, [0-9]
 *   '\D'       Non-digits
 *
 *
 */
#include "libmin.h"

#ifndef RE_DOT_MATCHES_NEWLINE
/* Define to 0 if you DON'T want '.' to match '\r' + '\n' */
#define RE_DOT_MATCHES_NEWLINE 1
#endif

/* Typedef'd pointer to get abstract datatype. */
typedef struct regex_t* re_t;


/* Compile regex string pattern to a regex_t-array. */
re_t re_compile(const char* pattern);


/* Find matches of the compiled pattern inside text. */
int re_matchp(re_t pattern, const char* text, int* matchlength);


/* Find matches of the txt pattern inside text (will compile automatically first). */
int re_match(const char* pattern, const char* text, int* matchlength);


/*
 *
 * Mini regex-module inspired by Rob Pike's regex code described in:
 *
 * http://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
 *
 *
 *
 * Supports:
 * ---------
 *   '.'        Dot, matches any character
 *   '^'        Start anchor, matches beginning of string
 *   '$'        End anchor, matches end of string
 *   '*'        Asterisk, match zero or more (greedy)
 *   '+'        Plus, match one or more (greedy)
 *   '?'        Question, match zero or one (non-greedy)
 *   '[abc]'    Character class, match if one of {'a', 'b', 'c'}
 *   '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'} -- NOTE: feature is currently broken!
 *   '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
 *   '\s'       Whitespace, \t \f \r \n \v and spaces
 *   '\S'       Non-whitespace
 *   '\w'       Alphanumeric, [a-zA-Z0-9_]
 *   '\W'       Non-alphanumeric
 *   '\d'       Digits, [0-9]
 *   '\D'       Non-digits
 *
 *
 */

/* Definitions: */

#define MAX_REGEXP_OBJECTS      30    /* Max number of regex symbols in expression. */
#define MAX_CHAR_CLASS_LEN      40    /* Max length of character-class buffer in.   */


enum { UNUSED, DOT, BEGIN, END, QUESTIONMARK, STAR, PLUS, CHAR, CHAR_CLASS, INV_CHAR_CLASS, DIGIT, NOT_DIGIT, ALPHA, NOT_ALPHA, WHITESPACE, NOT_WHITESPACE, /* BRANCH */ };

typedef struct regex_t
{
  unsigned char  type;   /* CHAR, STAR, etc.                      */
  union
  {
    unsigned char  ch;   /*      the character itself             */
    unsigned char* ccl;  /*  OR  a pointer to characters in class */
  } u;
} regex_t;

/* Private function declarations: */
static int matchpattern(regex_t* pattern, const char* text, int* matchlength);
static int matchcharclass(char c, const char* str);
static int matchstar(regex_t p, regex_t* pattern, const char* text, int* matchlength);
static int matchplus(regex_t p, regex_t* pattern, const char* text, int* matchlength);
static int matchone(regex_t p, char c);
static int matchdigit(char c);
static int matchalpha(char c);
static int matchwhitespace(char c);
static int matchmetachar(char c, const char* str);
static int matchrange(char c, const char* str);
static int matchdot(char c);
static int ismetachar(char c);

/* Public functions: */
int re_match(const char* pattern, const char* text, int* matchlength)
{
  return re_matchp(re_compile(pattern), text, matchlength);
}

int re_matchp(re_t pattern, const char* text, int* matchlength)
{
  *matchlength = 0;
  if (pattern != 0)
  {
    if (pattern[0].type == BEGIN)
    {
      return ((matchpattern(&pattern[1], text, matchlength)) ? 0 : -1);
    }
    else
    {
      int idx = -1;

      do
      {
        idx += 1;

        if (matchpattern(pattern, text, matchlength))
        {
          if (text[0] == '\0')
            return -1;

          return idx;
        }
      }
      while (*text++ != '\0');
    }
  }
  return -1;
}

re_t re_compile(const char* pattern)
{
  /* The sizes of the two static arrays below substantiates the static RAM usage of this module.
     MAX_REGEXP_OBJECTS is the max number of symbols in the expression.
     MAX_CHAR_CLASS_LEN determines the size of buffer for chars in all char-classes in the expression. */
  static regex_t re_compiled[MAX_REGEXP_OBJECTS];
  static unsigned char ccl_buf[MAX_CHAR_CLASS_LEN];
  int ccl_bufidx = 1;

  char c;     /* current char in pattern   */
  int i = 0;  /* index into pattern        */
  int j = 0;  /* index into re_compiled    */

  while (pattern[i] != '\0' && (j+1 < MAX_REGEXP_OBJECTS))
  {
    c = pattern[i];

    switch (c)
    {
      /* Meta-characters: */
      case '^': {    re_compiled[j].type = BEGIN;           } break;
      case '$': {    re_compiled[j].type = END;             } break;
      case '.': {    re_compiled[j].type = DOT;             } break;
      case '*': {    re_compiled[j].type = STAR;            } break;
      case '+': {    re_compiled[j].type = PLUS;            } break;
      case '?': {    re_compiled[j].type = QUESTIONMARK;    } break;
/*    case '|': {    re_compiled[j].type = BRANCH;          } break; <-- not working properly */

      /* Escaped character-classes (\s \w ...): */
      case '\\':
      {
        if (pattern[i+1] != '\0')
        {
          /* Skip the escape-char '\\' */
          i += 1;
          /* ... and check the next */
          switch (pattern[i])
          {
            /* Meta-character: */
            case 'd': {    re_compiled[j].type = DIGIT;            } break;
            case 'D': {    re_compiled[j].type = NOT_DIGIT;        } break;
            case 'w': {    re_compiled[j].type = ALPHA;            } break;
            case 'W': {    re_compiled[j].type = NOT_ALPHA;        } break;
            case 's': {    re_compiled[j].type = WHITESPACE;       } break;
            case 'S': {    re_compiled[j].type = NOT_WHITESPACE;   } break;

            /* Escaped character, e.g. '.' or '$' */
            default:
            {
              re_compiled[j].type = CHAR;
              re_compiled[j].u.ch = pattern[i];
            } break;
          }
        }
        /* '\\' as last char in pattern -> invalid regular expression. */
/*
        else
        {
          re_compiled[j].type = CHAR;
          re_compiled[j].ch = pattern[i];
        }
*/
      } break;

      /* Character class: */
      case '[':
      {
        /* Remember where the char-buffer starts. */
        int buf_begin = ccl_bufidx;

        /* Look-ahead to determine if negated */
        if (pattern[i+1] == '^')
        {
          re_compiled[j].type = INV_CHAR_CLASS;
          i += 1; /* Increment i to avoid including '^' in the char-buffer */
          if (pattern[i+1] == 0) /* incomplete pattern, missing non-zero char after '^' */
          {
            return 0;
          }
        }
        else
        {
          re_compiled[j].type = CHAR_CLASS;
        }

        /* Copy characters inside [..] to buffer */
        while (    (pattern[++i] != ']')
                && (pattern[i]   != '\0')) /* Missing ] */
        {
          if (pattern[i] == '\\')
          {
            if (ccl_bufidx >= MAX_CHAR_CLASS_LEN - 1)
            {
              //fputs("exceeded internal buffer!\n", stderr);
              return 0;
            }
            if (pattern[i+1] == 0) /* incomplete pattern, missing non-zero char after '\\' */
            {
              return 0;
            }
            ccl_buf[ccl_bufidx++] = pattern[i++];
          }
          else if (ccl_bufidx >= MAX_CHAR_CLASS_LEN)
          {
              //fputs("exceeded internal buffer!\n", stderr);
              return 0;
          }
          ccl_buf[ccl_bufidx++] = pattern[i];
        }
        if (ccl_bufidx >= MAX_CHAR_CLASS_LEN)
        {
            /* Catches cases such as [00000000000000000000000000000000000000][ */
            //fputs("exceeded internal buffer!\n", stderr);
            return 0;
        }
        /* Null-terminate string end */
        ccl_buf[ccl_bufidx++] = 0;
        re_compiled[j].u.ccl = &ccl_buf[buf_begin];
      } break;

      /* Other characters: */
      default:
      {
        re_compiled[j].type = CHAR;
        re_compiled[j].u.ch = c;
      } break;
    }
    /* no buffer-out-of-bounds access on invalid patterns - see https://github.com/kokke/tiny-regex-c/commit/1a279e04014b70b0695fba559a7c05d55e6ee90b */
    if (pattern[i] == 0)
    {
      return 0;
    }

    i += 1;
    j += 1;
  }
  /* 'UNUSED' is a sentinel used to indicate end-of-pattern */
  re_compiled[j].type = UNUSED;

  return (re_t) re_compiled;
}

void re_print(regex_t* pattern)
{
  const char* types[] = { "UNUSED", "DOT", "BEGIN", "END", "QUESTIONMARK", "STAR", "PLUS", "CHAR", "CHAR_CLASS", "INV_CHAR_CLASS", "DIGIT", "NOT_DIGIT", "ALPHA", "NOT_ALPHA", "WHITESPACE", "NOT_WHITESPACE", "BRANCH" };

  int i;
  int j;
  char c;
  for (i = 0; i < MAX_REGEXP_OBJECTS; ++i)
  {
    if (pattern[i].type == UNUSED)
    {
      break;
    }

    libmin_printf("type: %s", types[pattern[i].type]);
    if (pattern[i].type == CHAR_CLASS || pattern[i].type == INV_CHAR_CLASS)
    {
      libmin_printf(" [");
      for (j = 0; j < MAX_CHAR_CLASS_LEN; ++j)
      {
        c = pattern[i].u.ccl[j];
        if ((c == '\0') || (c == ']'))
        {
          break;
        }
        libmin_printf("%c", c);
      }
      libmin_printf("]");
    }
    else if (pattern[i].type == CHAR)
    {
      libmin_printf(" '%c'", pattern[i].u.ch);
    }
    libmin_printf("\n");
  }
}



/* Private functions: */
static int matchdigit(char c)
{
  return isdigit(c);
}
static int matchalpha(char c)
{
  return isalpha(c);
}
static int matchwhitespace(char c)
{
  return isspace(c);
}
static int matchalphanum(char c)
{
  return ((c == '_') || matchalpha(c) || matchdigit(c));
}
static int matchrange(char c, const char* str)
{
  return (    (c != '-')
           && (str[0] != '\0')
           && (str[0] != '-')
           && (str[1] == '-')
           && (str[2] != '\0')
           && (    (c >= str[0])
                && (c <= str[2])));
}
static int matchdot(char c)
{
#if defined(RE_DOT_MATCHES_NEWLINE) && (RE_DOT_MATCHES_NEWLINE == 1)
  (void)c;
  return 1;
#else
  return c != '\n' && c != '\r';
#endif
}
static int ismetachar(char c)
{
  return ((c == 's') || (c == 'S') || (c == 'w') || (c == 'W') || (c == 'd') || (c == 'D'));
}

static int matchmetachar(char c, const char* str)
{
  switch (str[0])
  {
    case 'd': return  matchdigit(c);
    case 'D': return !matchdigit(c);
    case 'w': return  matchalphanum(c);
    case 'W': return !matchalphanum(c);
    case 's': return  matchwhitespace(c);
    case 'S': return !matchwhitespace(c);
    default:  return (c == str[0]);
  }
}

static int matchcharclass(char c, const char* str)
{
  do
  {
    if (matchrange(c, str))
    {
      return 1;
    }
    else if (str[0] == '\\')
    {
      /* Escape-char: increment str-ptr and match on next char */
      str += 1;
      if (matchmetachar(c, str))
      {
        return 1;
      }
      else if ((c == str[0]) && !ismetachar(c))
      {
        return 1;
      }
    }
    else if (c == str[0])
    {
      if (c == '-')
      {
        return ((str[-1] == '\0') || (str[1] == '\0'));
      }
      else
      {
        return 1;
      }
    }
  }
  while (*str++ != '\0');

  return 0;
}

static int matchone(regex_t p, char c)
{
  switch (p.type)
  {
    case DOT:            return matchdot(c);
    case CHAR_CLASS:     return  matchcharclass(c, (const char*)p.u.ccl);
    case INV_CHAR_CLASS: return !matchcharclass(c, (const char*)p.u.ccl);
    case DIGIT:          return  matchdigit(c);
    case NOT_DIGIT:      return !matchdigit(c);
    case ALPHA:          return  matchalphanum(c);
    case NOT_ALPHA:      return !matchalphanum(c);
    case WHITESPACE:     return  matchwhitespace(c);
    case NOT_WHITESPACE: return !matchwhitespace(c);
    default:             return  (p.u.ch == c);
  }
}

static int matchstar(regex_t p, regex_t* pattern, const char* text, int* matchlength)
{
  int prelen = *matchlength;
  const char* prepoint = text;
  while ((text[0] != '\0') && matchone(p, *text))
  {
    text++;
    (*matchlength)++;
  }
  while (text >= prepoint)
  {
    if (matchpattern(pattern, text--, matchlength))
      return 1;
    (*matchlength)--;
  }

  *matchlength = prelen;
  return 0;
}

static int matchplus(regex_t p, regex_t* pattern, const char* text, int* matchlength)
{
  const char* prepoint = text;
  while ((text[0] != '\0') && matchone(p, *text))
  {
    text++;
    (*matchlength)++;
  }
  while (text > prepoint)
  {
    if (matchpattern(pattern, text--, matchlength))
      return 1;
    (*matchlength)--;
  }

  return 0;
}

static int matchquestion(regex_t p, regex_t* pattern, const char* text, int* matchlength)
{
  if (p.type == UNUSED)
    return 1;
  if (matchpattern(pattern, text, matchlength))
      return 1;
  if (*text && matchone(p, *text++))
  {
    if (matchpattern(pattern, text, matchlength))
    {
      (*matchlength)++;
      return 1;
    }
  }
  return 0;
}


/* Iterative matching */
static int matchpattern(regex_t* pattern, const char* text, int* matchlength)
{
  int pre = *matchlength;
  do
  {
    if ((pattern[0].type == UNUSED) || (pattern[1].type == QUESTIONMARK))
    {
      return matchquestion(pattern[0], &pattern[2], text, matchlength);
    }
    else if (pattern[1].type == STAR)
    {
      return matchstar(pattern[0], &pattern[2], text, matchlength);
    }
    else if (pattern[1].type == PLUS)
    {
      return matchplus(pattern[0], &pattern[2], text, matchlength);
    }
    else if ((pattern[0].type == END) && pattern[1].type == UNUSED)
    {
      return (text[0] == '\0');
    }
/*  Branching is not working properly
    else if (pattern[1].type == BRANCH)
    {
      return (matchpattern(pattern, text) || matchpattern(&pattern[2], text));
    }
*/
  (*matchlength)++;
  }
  while ((text[0] != '\0') && matchone(*pattern++, *text++));

  *matchlength = pre;
  return 0;
}

/*
 * Testing various regex-patterns
 */


#define OK    ((char*) 1)
#define NOK   ((char*) 0)


char* test_vector[][4] =
{
  { OK,  "\\d",                       "5",                (char*) 1      },
  { OK,  "\\w+",                      "hej",              (char*) 3      },
  { OK,  "\\s",                       "\t \n",            (char*) 1      },
  { NOK, "\\S",                       "\t \n",            (char*) 0      },
  { OK,  "[\\s]",                     "\t \n",            (char*) 1      },
  { NOK, "[\\S]",                     "\t \n",            (char*) 0      },
  { NOK, "\\D",                       "5",                (char*) 0      },
  { NOK, "\\W+",                      "hej",              (char*) 0      },
  { OK,  "[0-9]+",                    "12345",            (char*) 5      },
  { OK,  "\\D",                       "hej",              (char*) 1      },
  { NOK, "\\d",                       "hej",              (char*) 0      },
  { OK,  "[^\\w]",                    "\\",               (char*) 1      },
  { OK,  "[\\W]",                     "\\",               (char*) 1      },
  { NOK, "[\\w]",                     "\\",               (char*) 0      },
  { OK,  "[^\\d]",                    "d",                (char*) 1      },
  { NOK, "[\\d]",                     "d",                (char*) 0      },
  { NOK, "[^\\D]",                    "d",                (char*) 0      },
  { OK,  "[\\D]",                     "d",                (char*) 1      },
  { OK,  "^.*\\\\.*$",                "c:\\Tools",        (char*) 8      },
  { OK,  "^[\\+-]*[\\d]+$",           "+27",              (char*) 3      },
  { OK,  "[abc]",                     "1c2",              (char*) 1      },
  { NOK, "[abc]",                     "1C2",              (char*) 0      },
  { OK,  "[1-5]+",                    "0123456789",       (char*) 5      },
  { OK,  "[.2]",                      "1C2",              (char*) 1      },
  { OK,  "a*$",                       "Xaa",              (char*) 2      },
  { OK,  "a*$",                       "Xaa",              (char*) 2      },
  { OK,  "[a-h]+",                    "abcdefghxxx",      (char*) 8      },
  { NOK, "[a-h]+",                    "ABCDEFGH",         (char*) 0      },
  { OK,  "[A-H]+",                    "ABCDEFGH",         (char*) 8      },
  { NOK, "[A-H]+",                    "abcdefgh",         (char*) 0      },
  { OK,  "[^\\s]+",                   "abc def",          (char*) 3      },
  { OK,  "[^fc]+",                    "abc def",          (char*) 2      },
  { OK,  "[^d\\sf]+",                 "abc def",          (char*) 3      },
  { OK,  "\n",                        "abc\ndef",         (char*) 1      },
  { OK,  "b.\\s*\n",                  "aa\r\nbb\r\ncc\r\n\r\n",(char*) 4      },
  { OK,  ".*c",                       "abcabc",           (char*) 6      },
  { OK,  ".+c",                       "abcabc",           (char*) 6      },
  { OK,  "[b-z].*",                   "ab",               (char*) 1      },
  { OK,  "b[k-z]*",                   "ab",               (char*) 1      },
  { NOK, "[0-9]",                     "  - ",             (char*) 0      },
  { OK,  "[^0-9]",                    "  - ",             (char*) 1      },
  { OK,  "0|",                        "0|",               (char*) 2      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "0s:00:00",         (char*) 0      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "000:00",           (char*) 0      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "00:0000",          (char*) 0      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "100:0:00",         (char*) 0      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "00:100:00",        (char*) 0      },
  { NOK, "\\d\\d:\\d\\d:\\d\\d",      "0:00:100",         (char*) 0      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:0:0",            (char*) 5      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:00:0",           (char*) 6      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:0:00",           (char*) 5      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:0:0",           (char*) 6      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:00:0",          (char*) 7      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:0:00",          (char*) 6      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:00:00",          (char*) 6      },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:00:00",         (char*) 7      },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world !",    (char*) 12     },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "hello world !",    (char*) 12     },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello World !",    (char*) 12     },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world!   ",  (char*) 11     },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world  !",   (char*) 13     },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "hello World    !", (char*) 15     },
  { NOK, "\\d\\d?:\\d\\d?:\\d\\d?",   "a:0",              (char*) 0      }, /* Failing test case reported in https://github.com/kokke/tiny-regex-c/issues/12 */
/*
  { OK,  "[^\\w][^-1-4]",     ")T",          (char*) 2      },
  { OK,  "[^\\w][^-1-4]",     ")^",          (char*) 2      },
  { OK,  "[^\\w][^-1-4]",     "*)",          (char*) 2      },
  { OK,  "[^\\w][^-1-4]",     "!.",          (char*) 2      },
  { OK,  "[^\\w][^-1-4]",     " x",          (char*) 2      },
  { OK,  "[^\\w][^-1-4]",     "$b",          (char*) 2      },
*/
  { OK,  ".?bar",                      "real_bar",        (char*) 4      },
  { NOK, ".?bar",                      "real_foo",        (char*) 0      },
  { NOK, "X?Y",                        "Z",               (char*) 0      },
  { OK, "[a-z]+\nbreak",              "blahblah\nbreak",  (char*) 14     },
  { OK, "[a-z\\s]+\nbreak",           "bla bla \nbreak",  (char*) 14     },
};


void re_print(re_t);

int main()
{
    char* text;
    char* pattern;
    int should_fail;
    int length;
    int correctlen;
    size_t ntests = sizeof(test_vector) / sizeof(*test_vector);
    size_t nfailed = 0;
    size_t i;

    libtarg_start_perf();
    for (i = 0; i < ntests; ++i)
    {
        pattern = test_vector[i][1];
        text = test_vector[i][2];
        should_fail = (test_vector[i][0] == NOK);
        correctlen = (int)(long int)(test_vector[i][3]);

        int m = re_match(pattern, text, &length);

        if (should_fail)
        {
            if (m != (-1))
            {
                libmin_printf("\n");
                re_print(re_compile(pattern));
                libmin_printf("[%lu/%lu]: pattern '%s' matched '%s' unexpectedly, matched %i chars. \n", (i+1), ntests, pattern, text, length);
                nfailed += 1;
            }
        }
        else
        {
            if (m == (-1))
            {
                libmin_printf("\n");
                re_print(re_compile(pattern));
                libmin_printf("[%lu/%lu]: pattern '%s' didn't match '%s' as expected. \n", (i+1), ntests, pattern, text);
                nfailed += 1;
            }
            else if (length != correctlen)
            {
                libmin_printf("[%lu/%lu]: pattern '%s' matched '%i' chars of '%s'; expected '%i'. \n", (i+1), ntests, pattern, length, text, correctlen);
                nfailed += 1;
            }
        }
    }
    libtarg_stop_perf();

    // printf("\n");
    libmin_printf("%lu/%lu tests succeeded.\n", ntests - nfailed, ntests);

    if (nfailed != 0)
      return nfailed; /* 0 if all tests passed */
    else
      libmin_success();
    return 0;
}

