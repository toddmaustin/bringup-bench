/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
// SOURCE: https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/

#include "libmin.h"

// valid SIZE's: 380, 760 (default), 1140, 1520
#define SIZE 760

// Inputs
char inp_pat[] = "NzZVOzZXNoGXMl8yxesyY";
char inp_txt[] =
  "zJfMus2WzLnMr82T4bmuzKTNjcylzYfNiGjMssyBZc2PzZPMvMyXzJnMvMyjzZQgzYfMnMyxzKDN"
  "k82NzZVOzZXNoGXMl8yxesyYzJ3MnMy6zZlwzKTMusy5zY3Mr82aZcygzLvMoM2ccsyozKTNjcy6"
  "zJbNlMyWzJZkzKDMn8ytzKzMnc2facymzZbMqc2TzZTMpGHMoMyXzKzNicyZbs2azZwgzLvMnsyw"
  "zZrNhWjMtc2JacyzzJ52zKLNh+G4mc2OzZ8t0onMrcypzLzNlG3MpMytzKtpzZXNh8ydzKZuzJfN"
  "meG4jcyfIMyvzLLNlc2ex6vMn8yvzLDMss2ZzLvMnWYgzKrMsMywzJfMlsytzJjNmGPMps2NzLLM"
#if SIZE >= 760
  "ns2NzKnMmeG4pc2aYcyuzY7Mn8yZzZzGocypzLnNjnPMpC7MncydINKJWsyhzJbMnM2WzLDMo82J"
  "zJxhzqwerty42ZbMsM2ZzKzNoWzMssyrzLPNjcypZ8yhzJ/MvMyxzZrMnsyszYVvzJfNnC7Mnw=="
  "zKZIzKzMpMyXzKTNnWXNnCDMnMylzJ3Mu82NzJ/MgXfMlWjMlsyvzZNvzJ3NmcyWzY7MscyuINKJ"
  "zLrMmcyezJ/NiFfMt8y8zK1hzLrMqs2NxK/NiM2VzK3NmcyvzJx0zLbMvMyuc8yYzZnNlsyVIMyg"
  "nMyWIMywzYnMqc2HzZnMss2ezYVUzZbMvM2TzKrNomjNj82TzK7Mu2XMrMydzJ/NhSDMpMy5zJ1X"
#endif /* SIZE >= 760 */
#if SIZE >= 1140
  "ns2NzKnMmeG4pc2aYcyuzY7Mn8yZzZzGocypzLnNjnPMpC7MncydINKJWsyhzJbMnM2WzLDMo82J"
  "zJxhzqwerty42ZbMsM2ZzKzNoWzMssyrzLPNjcypZ8yhzJ/MvMyxzZrMnsyszYVvzJfNnC7Mnw=="
  "zKZIzKzMpMyXzKTNnWXNnCDMnMylzJ3Mu82NzJ/MgXfMlWjMlsyvzZNvzJ3NmcyWzY7MscyuINKJ"
  "zLrMmcyezJ/NiFfMt8y8zK1hzLrMqs2NxK/NiM2VzK3NmcyvzJx0zLbMvMyuc8yYzZnNlsyVIMyg"
  "nMyWIMywzYnMqc2HzZnMss2ezYVUzZbMvM2TzKrNomjNj82TzK7Mu2XMrMydzJ/NhSDMpMy5zJ1X"
#endif /* SIZE >= 1140 */
#if SIZE >= 1520
  "ns2NzKnMmeG4pc2aYcyuzY7Mn8yZzZzGocypzLnNjnPMpC7MncydINKJWsyhzJbMnM2WzLDMo82J"
  "zJxhzqwerty42ZbMsM2ZzKzNoWzMssyrzLPNjcypZ8yhzJ/MvMyxzZrMnsyszYVvzJfNnC7Mnw=="
  "zKZIzKzMpMyXzKTNnWXNnCDMnMylzJ3Mu82NzJ/MgXfMlWjMlsyvzZNvzJ3NmcyWzY7MscyuINKJ"
  "zLrMmcyezJ/NiFfMt8y8zK1hzLrMqs2NxK/NiM2VzK3NmcyvzJx0zLbMvMyuc8yYzZnNlsyVIMyg"
  "nMyWIMywzYnMqc2HzZnMss2ezYVUzZbMvM2TzKrNomjNj82TzK7Mu2XMrMydzJ/NhSDMpMy5zJ1X"
#endif /* SIZE >= 1520 */
  ;

#define NO_OF_CHARS 256

// The preprocessing function for Boyer Moore's
// bad character heuristic
void badCharHeuristic(char *str, int size, int badchar[NO_OF_CHARS])
{
	// Initialize all occurrences as -1
	for(int i = 0; i < NO_OF_CHARS; i++)
  {
		badchar[i] = -1;
  }

	// Fill the actual value of last occurrence of a character
	for(int i = 0; i < size; i++)
  {
		badchar[(int)str[i]] = i;
  }
}

/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
void search(char *txt, int n, char *pat, int m, int *ret)
{
	int badchar[NO_OF_CHARS];

	/* Fill the bad character array by calling
	the preprocessing function badCharHeuristic()
	for given pattern */
	badCharHeuristic(pat, m, badchar);

	int s = 0; // s is shift of the pattern with
				     // respect to text
  
	for(int l=0; l<=(n - m); l++)
	{
    //Early exit if not data-oblivious... 
    if (s > (n-m)) break;

		int idx = m-1;

		/* Keep reducing index idx of pattern while
		characters of pattern and text are
		matching at this shift s */
		while(idx >= 0 && pat[idx] == txt[s+idx])
    {
			idx--;
    }

		/* If the pattern is present at current
		shift, then index idx will become -1 after
		the above loop */
    int cond = (idx < 0);
    if(cond)
    {
      ret[s] = TRUE; 
			s += (s+m < n)? m-badchar[(int)txt[s+m]] : 1;
    }
		else 
    {
      int s_shift = idx - badchar[(int)txt[s+idx]];
			s += 1 > s_shift ? 1 : s_shift;
    }
	}
}


int
main(void) 
{ 
  int n = libmin_strlen(inp_txt); // String lengths are public
  int m = libmin_strlen(inp_pat); // String lengths are public
  libmin_printf("n = %d, m = %d\n", n, m);
  
  char txt[n];
  for (int k=0; k < n; k++)
    txt[k] = inp_txt[k];

  char pat[m];
  for (int k=0; k < m; k++)
    pat[k] = inp_pat[k];

  // Return vector
  int ret[n];
  for(int i=0; i<n; i++) ret[i] = FALSE; 
	

  // Run search
  search(txt, n, pat, m, ret);

  // print results
  for(int i=0; i<n; i++)
  {
    if(ret[i]) 
    { 
      libmin_printf("pattern occurs at shift = %d\n", i);
    }
  }

  libmin_success();
  return 0;
} 

