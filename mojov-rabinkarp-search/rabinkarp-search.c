/* Rabin-Karp rolling hash string search algorithm */

#include "libmin.h"

#define TRUE  1
#define FALSE 0

// valid SIZE's: 380, 760 (default), 1140, 1520
#define SIZE 1520

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

// D is the number of characters in the input alphabet 
#define D 256 

/* simple string search algorithm */
void
search(char *txt, int n, char *pat, int m, int *ret)
{
  int q = INT_MAX; 

  // value of h would be "pow(d, M-1)%q" 
  int h = 1;
  for (int i = 0; i < m - 1; i++) 
    h = (h * D) % q; 

  int32_t p = 0; // hash value for pattern 
  int32_t t = 0; // hash value for txt 

  // calculate the hash value of pattern and first window of text 
  for (int i = 0; i < m; i++) 
  { 
    p = (D * p + pat[i]) % q; 
    t = (D * t + txt[i]) % q; 
  } 

  // slide the pattern over text one by one 
  for (int i = 0; i <= n - m; i++) 
    { 
  
    // check the hash values of current window of text 
    // and pattern. If the hash values match then only 
    // check for characters one by one 
    ret[i] = (p == t);
  
    // calculate hash value for next window of text: Remove leading digit, add trailing digit 
    if (i < n-m) 
    { 
      t = (D*(t - ((int32_t)txt[i])*h) + txt[i+m])%q; 
  
      // we might get negative value of t, converting it to positive 
      if (t < 0) 
        t = (t + q); 
    } 
  } 
}


int
main(void) 
{ 
  int n = libmin_strlen(inp_txt); // String lengths are public
  int m = libmin_strlen(inp_pat); // String lengths are public
  libmin_printf("n = %d, m = %d\n", n, m);
  
  char *txt = libmin_malloc(sizeof(char)*(n+1));
  int k;
  for (k=0; k < n; k++)
    txt[k] = inp_txt[k];
  txt[k] = '\0';

  char *pat = libmin_malloc(sizeof(char)*(m+1));
  for (k=0; k < m; k++)
    pat[k] = inp_pat[k];
  txt[k] = '\0';

  // Return vector
  int *ret = (int *)libmin_malloc(sizeof(int)*n);
  for (int i=0; i<n; i++)
    ret[i] = FALSE; 

  // Run search
  search(txt, n, pat, m, ret);

  // print results
  for(int i=0; i<n; i++)
  {
    if (ret[i]) 
    { 
      libmin_printf("pattern occurs at shift = %d\n", i);
    }
  }

  libmin_success();
  return 0;
} 

