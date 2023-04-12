//
// ctype.h
//
// Character types
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef CTYPE_H
#define CTYPE_H

//
// Character types
//

#define _UPPER          0x1     // Upper case letter
#define _LOWER          0x2     // Lower case letter
#define _DIGIT          0x4     // Digit[0-9]
#define _SPACE          0x8     // Tab, carriage return, newline, vertical tab or form feed
#define _PUNCT          0x10    // Punctuation character
#define _CONTROL        0x20    // Control character
#define _BLANK          0x40    // Space char
#define _HEX            0x80    // Hexadecimal digit

#define _LEADBYTE       0x8000                      // Multibyte leadbyte
#define _ALPHA          (0x0100 | _UPPER| _LOWER)   // Alphabetic character


#ifdef  __cplusplus
extern "C" {
#endif

extern unsigned short *_pctype; // pointer to table for char's

int _isctype(int c, int mask);

int isspace(int c);
int isupper(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int ispunct(int c);
int isalpha(int c);
int isalnum(int c);
int isprint(int c);
int isgraph(int c);
int iscntrl(int c);
int isleadbyte(int c);

int toupper(int c);
int tolower(int c);

#ifdef  __cplusplus
}
#endif

#ifndef _CTYPE_DISABLE_MACROS

#define isalpha(c)     (_pctype[(c)] & (_UPPER | _LOWER))
#define isupper(c)     (_pctype[(c)] & _UPPER)
#define islower(c)     (_pctype[(c)] & _LOWER)
#define isdigit(c)     (_pctype[(c)] & _DIGIT)
#define isxdigit(c)    (_pctype[(c)] & _HEX)
#define isspace(c)     (_pctype[(c)] & _SPACE)
#define ispunct(c)     (_pctype[(c)] & _PUNCT)
#define isalnum(c)     (_pctype[(c)] & (_UPPER | _LOWER | _DIGIT))
#define isprint(c)     (_pctype[(c)] & (_BLANK | _PUNCT | _UPPER | _LOWER | _DIGIT))
#define isgraph(c)     (_pctype[(c)] & (_PUNCT | _UPPER | _LOWER | _DIGIT))
#define iscntrl(c)     (_pctype[(c)] & _CONTROL)
#define isleadbyte(c)  (_pctype[(unsigned char)(c)] & _LEADBYTE)

#define tolower(c)     (isupper(c) ? ((c) - 'A' + 'a') : (c))
#define toupper(c)     (islower(c) ? ((c) - 'a' + 'A') : (c))

#endif

#endif
