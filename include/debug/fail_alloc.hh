
#ifndef ALLOC_FAIL_H
# define ALLOC_FAIL_H

# ifdef DO_ALLOC_FAILING

bool	should_malloc_fail;
bool	should_calloc_fail;
bool	should_substr_fail;
bool	should_strdup_fail;

bool	should_alloc_fail(bool should_fail, int action);

#define real__malloc(s) malloc(s)
#define malloc(s) should_alloc_fail(should_malloc_fail, 0) ? NULL : real__malloc((s))

#define real__ftcalloc(c, s) ft_calloc((c), (s))
#define ft_calloc(a, s) should_alloc_fail(should_calloc_fail, 0) ? NULL : real__ftcalloc((a), (s))

#define real__substr(a,b,c) ft_substr((a),(b),(c))
#define ft_substr(a,b,c) should_alloc_fail(should_substr_fail, 0) ? NULL : real__substr((a),(b),(c))

#define real__strdup(str) ft_strdup((str))
#define ft_strdup(str) should_alloc_fail(should_strdup_fail, 0) ? NULL : real__strdup((str))

# endif

#endif
