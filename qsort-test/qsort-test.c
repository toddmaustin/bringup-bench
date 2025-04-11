#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
typedef int (*cmpfun)(const void *, const void *);
void libmin_qsort(void *base, size_t nel, size_t width, cmpfun cmp);

typedef struct object_t
{
  int value;
  char* name;

} object_t;

int cmp(const void* a, const void* b)
{
  object_t* obj1 = *((object_t**)a);
  object_t* obj2 = *((object_t**)b);
  // printf("comparing objects\n");
  return obj1->value == obj2->value ? strcmp(obj1->name, obj2->name)  : obj1->value - obj2->value;
}

// populate data
#define NAMES_SIZE (sizeof(turkish_names) / sizeof(turkish_names[0]))
char* turkish_names[] = {
  "Ahmet",
  "Mehmet",
  "Ali",
  "Veli",
  "Ayşe",
  "Fatma",
  "Zeynep",
  "Emine",
  "Mustafa",
  "Hüseyin"
};

int random_int(int min, int max)
{
  return min + rand() % (max - min + 1);
}


int main()
{
  const int test_size = 43;
  object_t** object_pointer_array = (object_t**)malloc(test_size * sizeof(object_t*));


  for (size_t i = 0; i < test_size; i++)
  {
    object_pointer_array[i] = (object_t*)malloc(sizeof(object_t));
    object_pointer_array[i]->value = random_int(0, test_size); //  birbiriyle ayni olanlar olsun diye test_size/2 verdim.
    object_pointer_array[i]->name = turkish_names[random_int(0, NAMES_SIZE - 1)];
  }
  printf("created arays\n");

  // print unsorted
  for (size_t i = 0; i < test_size; i++)
  {
    printf("%d %s\n", object_pointer_array[i]->value, object_pointer_array[i]->name);
  }

  qsort(object_pointer_array, test_size, sizeof(object_t*), cmp);
  printf("Sorted with qsort\n");
  libmin_qsort(object_pointer_array, test_size, sizeof(object_t*), (cmpfun)cmp);
  printf("Sorted with libmin_qsort\n");
}

