
#pragma once

#include <stdlib.h>

void	*malloc_track(const char *fn, int line, size_t size);
void	*ft_calloc_track(const char *fn, int line, size_t count, size_t size);
char	*ft_strdup_track(const char *fn, int line, const char *s1);
void	*ft_memdup_track(const char *fn, int line, const char *s1, size_t n);
char	*ft_strjoin_track(const char *fn, int line, const char *s1, const char *s2);
char	**ft_split_track(const char *fn, int line, const char *str, char c);
char	*ft_strtrim_track(const char *fn, int line, char const *s1, char const *set);
char	*ft_substr_track(const char *fn, int line, char const *s, unsigned int start, size_t len);
char	*readline_track(const char *fn, int line, const char *prompt);

int		ft_asprintf_track(const char *fn, int line, char **ret,
			const char *fmt, ...) __attribute__((format(printf, 4, 5)));


void	free_track(const char *fn, int line, void *ptr);

// #define TRACK_ALLOC
#ifdef TRACK_ALLOC

#define malloc_wrap(size) malloc_track(__FUNCTION__, __LINE__, (size))
#define ft_calloc_wrap(count, size) ft_calloc_track(__FUNCTION__, __LINE__, (count), (size))
#define ft_strdup_wrap(s1) ft_strdup_track(__FUNCTION__, __LINE__, (s1))
#define ft_memdup_wrap(s1, n) ft_memdup_track(__FUNCTION__, __LINE__, (s1), (n))
#define ft_strjoin_wrap(s1, s2) ft_strjoin_track(__FUNCTION__, __LINE__, (s1), (s2))
#define ft_split_wrap(str, c) ft_split_track(__FUNCTION__, __LINE__, (str), (c))
#define ft_strtrim_wrap(s1, set) ft_strtrim_track(__FUNCTION__, __LINE__, (s1), (set))
#define ft_substr_wrap(s, start, len) ft_substr_track(__FUNCTION__, __LINE__, (s), (start), (len))
#define ft_asprintf_wrap(ret, fmt, ...) ft_asprintf_track(__FUNCTION__, __LINE__, (ret), (fmt), __VA_ARGS__)
#define free_wrap(ptr) free_track(__FUNCTION__, __LINE__, (ptr))

#else

#define malloc_wrap(size) malloc((size))
#define ft_calloc_wrap(count, size) ft_calloc((count), (size))
#define ft_strdup_wrap(s1) ft_strdup((s1))
#define ft_memdup_wrap(s1, n) ft_memdup((s1), (n))
#define ft_strjoin_wrap(s1, s2) ft_strjoin((s1), (s2))
#define ft_split_wrap(str, c) ft_split((str), (c))
#define ft_strtrim_wrap(s1, set) ft_strtrim((s1), (set))
#define ft_substr_wrap(s, start, len) ft_substr((s), (start), (len))
#define ft_asprintf_wrap(ret, fmt, ...) ft_asprintf((ret), (fmt), __VA_ARGS__)
#define free_wrap(ptr) free((ptr))

#endif
