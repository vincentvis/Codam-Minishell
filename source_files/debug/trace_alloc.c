
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdbool.h>
#include "libft.h"
#include "ft_printf.h"
#include "utils/vector.h"

/*
	SEARCH AND REPLACE

	from non-tracked to tracked:

	search:
	( readline|_strdup|_memdup|_calloc|_strjoin|_split|_strtrim|_substr| malloc|\tfree)\(
	replace:
	\1_wrap(__FUNCTION__, __LINE__,

	search:
	_wrap\(
	replace:
	// with nothing


	creates toggable wrappers
	if TRACE_ALLOC is defined in the header file it calles tracker functions
	for each allocation (malloc/calloc/substr/strjoin etc)
	if you need more allocatinow rappers you'll have to add them.. good luck :P

	if TRACE_ALLOC is NOT defined the %alloc_wrap functions are just macros
	that expand to the normal function without anything added

*/


typedef struct s_alloc_track {
	char	allocated_pointer[16];
	size_t	size;
	size_t	count;
	const char	*type;
	const char	*alloc_type;
	const char	*alloc_fn;
	int		alloc_line;
	const char	*free_fn;
	int		free_line;
	bool	is_freed;
}	t_alloc_track;


bool	alloc_vector_resize(t_vect *vector)
{
	void	*new_array;

	new_array = malloc((vector->capacity * 2) * vector->item_size);
	if (new_array == NULL)
		return (false);
	vector->capacity = vector->capacity * 2;
	ft_memcpy(new_array, vector->array,
		vector->number_of_items * vector->item_size);
	free(vector->array);
	vector->array = new_array;
	return (true);
}

void	alloc_vector_destroy(t_vect *vector)
{
	size_t			iterator;
	unsigned char	*pointer;

	if (vector == NULL)
		return ;
	if (vector->del != NULL)
	{
		iterator = 0;
		pointer = vector->array;
		while (iterator < vector->number_of_items)
		{
			vector->del(pointer + (iterator * vector->item_size));
			iterator++;
		}
	}
	free(vector->array);
	free(vector);
}

void	*alloc_vector_push_back(t_vect *target, void *data)
{
	unsigned char	*insert_point;

	if (target == NULL)
		return (NULL);
	if (target->capacity == target->number_of_items)
	{
		if (alloc_vector_resize(target) == false)
			return (NULL);
	}
	insert_point = ((unsigned char *)target->array)
		+ (target->number_of_items * target->item_size);
	ft_memcpy(insert_point, data, target->item_size);
	target->number_of_items++;
	return (target->array);
}

t_vect	*alloc_vector_init(size_t initial_capacity, size_t item_size,
		void (*del)(void*))
{
	t_vect	*vector;

	if (initial_capacity == 0)
		initial_capacity = VECTOR_DEFAULT_INIT_CAPACITY;
	vector = malloc(sizeof(*vector));
	if (vector == NULL)
		return (NULL);
	vector->array = malloc(initial_capacity * item_size);
	if (vector->array == NULL)
	{
		free(vector);
		return (NULL);
	}
	vector->capacity = initial_capacity;
	vector->item_size = item_size;
	vector->del = del;
	vector->number_of_items = 0;
	return (vector);
}





t_vect	*tracker(int action)
{
	static t_vect	*vector;

	if (vector == NULL)
	{
		vector = alloc_vector_init(256, sizeof(t_alloc_track), NULL);
	}
	if (action == 1)
	{
		alloc_vector_destroy(vector);
		vector = NULL;
	}
	return (vector);
}

#include <string.h>

void	track_new(const char *fn, int line, char *type, size_t size, size_t count, void *ptr)
{
	t_alloc_track	track;

	ft_sprintf(track.allocated_pointer, "%p", ptr);
	track.size = size;
	track.count = count;
	track.type = type;
	track.alloc_fn = fn;
	track.alloc_line = line;
	track.free_fn = NULL;
	track.free_line = -1;
	track.is_freed = false;

	alloc_vector_push_back(tracker(0), &track);
}

t_alloc_track	*find_tracked_ptr(void *ptr, char start)
{
	t_vect			*vector = tracker(0);
	t_alloc_track	*data = vector->array;
	size_t			i = 0;
	char			ptrstring[16];
	int				len;

	len = ft_sprintf(ptrstring, "%p", ptr);
	ptrstring[0] = start;
	while (i < vector->number_of_items)
	{
		if (memcmp(ptrstring, data[i].allocated_pointer, len) == 0)
			return (&data[i]);
		i++;
	}
	return (NULL);
}

void	print_tracked(void)
{
	t_vect			*vector = tracker(0);
	t_alloc_track	*data = vector->array;
	size_t			i = 0;
	int				len;

	ft_dprintf(2,
		"\033[1;32m"
		"ptr\t\t\t"
		"allocator\t\t\t\t"
		"type      \t"
		"freed"
		"\033[0m"
		"\n");

	while (i < vector->number_of_items)
	{
		len = strlen(data[i].alloc_fn);
		ft_dprintf(2,
			"%-14s\t\t"
			"%-.32s:%-3i%.*s\t"
			"%-10s\t"
			"%s"
			"\n",
			data[i].allocated_pointer,
			data[i].alloc_fn, data[i].alloc_line, len < 32 ? 32 - len : 0, "                           ",
			data[i].type,
			data[i].is_freed ? "true" : "false"
			);
		i++;
	}
}




void	free_track(const char *fn, int line, void *ptr)
{
	(void)fn;
	(void)line;

	// ft_dprintf(2, "from : %s:%i\n", fn, line);
	t_alloc_track	*alloc_reference = find_tracked_ptr(ptr, '0');

	if (alloc_reference == NULL)
	{
		t_alloc_track	*freed_reference = find_tracked_ptr(ptr, 'f');

		if (freed_reference == NULL)
		{
			ft_dprintf(2,
				"(%p) Trying to double free, alloc from %s:%i, free from: %s:%i\n",
				ptr, fn, line,
				freed_reference->free_fn, freed_reference->free_line);
		}
		else
		{
			ft_dprintf(2,
				"(%p) Trying to free not allocated pointer from %s:%i\n",
				ptr, fn, line);
		}
	}
	else
	{
		alloc_reference->allocated_pointer[0] = 'f';
		alloc_reference->is_freed = true;
		alloc_reference->free_fn = fn;
		alloc_reference->free_line = line;
	}
	free(ptr);
}

void	*malloc_track(const char *fn, int line, size_t size)
{
	void			*ptr;

	ptr = malloc(size);
	track_new(fn, line, "malloc", size, 0, ptr);
	return (ptr);
}

void	*ft_calloc_track(const char *fn, int line, size_t count, size_t size)
{
	void			*ptr;

	ptr = ft_calloc(count, size);
	track_new(fn, line, "calloc", size, count, ptr);
	return (ptr);
}

char	*ft_strdup_track(const char *fn, int line, const char *s1)
{
	void			*ptr;

	ptr = ft_strdup(s1);
	track_new(fn, line, "strdup", 0, 0, ptr);
	return (ptr);
}

void	*ft_memdup_track(const char *fn, int line, const char *s1, size_t n)
{
	void			*ptr;

	ptr = ft_memdup(s1, n);
	track_new(fn, line, "memdup", 0, 0, ptr);
	return (ptr);
}

char	*ft_strjoin_track(const char *fn, int line, const char *s1, const char *s2)
{
	void			*ptr;

	ptr = ft_strjoin(s1, s2);
	track_new(fn, line, "strjoin", 0, 0, ptr);
	return (ptr);
}

char	**ft_split_track(const char *fn, int line, const char *str, char c)
{
	char			**ptr;

	ptr = ft_split(str, c);
	track_new(fn, line, "split", 0, 0, ptr);
	return (ptr);
}

char	*ft_strtrim_track(const char *fn, int line, char const *s1, char const *set)
{
	void			*ptr;

	ptr = ft_strtrim(s1, set);
	track_new(fn, line, "strtrim", 0, 0, ptr);
	return (ptr);
}

char	*ft_substr_track(const char *fn, int line, char const *s, unsigned int start, size_t len)
{
	void			*ptr;

	ptr = ft_substr(s, start, len);
	track_new(fn, line, "substr", 0, 0, ptr);
	return (ptr);
}

char	*readline_track(const char *fn, int line, const char *prompt)
{
	void			*ptr;

	ptr = readline(prompt);
	track_new(fn, line, "readline", 0, 0, ptr);
	return (ptr);
}

#include <stdarg.h>

int		ft_asprintf_track(const char *fn, int line, char **ret, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	int x = ft_vasprintf(ret, fmt, ap);
	va_end(ap);
	track_new(fn, line, "vasprintf", 0, 0, *ret);
	return (x);
}

