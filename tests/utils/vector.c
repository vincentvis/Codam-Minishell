#include <criterion/criterion.h>
#include "../include/utils/vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Test(vector, vector_init)
{
	t_vect	*vector_defaultcapacity;
	t_vect	*vector;

	vector_defaultcapacity = vector_init(0, sizeof(int), NULL);
	cr_assert_not_null(vector_defaultcapacity);
	cr_assert(vector_defaultcapacity->capacity == VECTOR_DEFAULT_INIT_CAPACITY);
	cr_assert(vector_defaultcapacity->item_size == sizeof(int));
	cr_assert(vector_defaultcapacity->number_of_items == 0);
	cr_assert_null(vector_defaultcapacity->del);

	vector = vector_init(25, sizeof(long), NULL);
	cr_assert_not_null(vector);
	cr_assert(vector->capacity == 25);
	cr_assert(vector->item_size == sizeof(long));
	cr_assert(vector->number_of_items == 0);
	cr_assert_null(vector->del);

	vector_destroy(vector_defaultcapacity);
	vector_destroy(vector);
}

Test(vector, vector_push_back_and_check_resize)
{
	t_vect	*vector;
	long	*array;
	long	*array_original;

	vector = vector_init(2, sizeof(long), NULL);
	array_original = vector->array;
	cr_assert(vector->capacity == 2);
	long value = 25;
	array = vector_push_back(vector, &value);
	value = 55;
	array = vector_push_back(vector, &value);
	value = 62655;
	array = vector_push_back(vector, &value);

	cr_assert(array[0] == 25);
	cr_assert(array[1] == 55);
	cr_assert(array[2] == 62655);
	cr_assert(vector->capacity == 4);

	cr_assert(array_original != array);// should be different because of resize

	vector_destroy(vector);
}

typedef struct s_large_struct
{
	char	*char_pointer;
	char	single_char;
	size_t	some_counter;
	char	buffer[44];
	long	four_longs[4];
	size_t	another_number;
}	t_large_struct;

Test(vector, uses_struct)
{
	t_vect	*vector;
	t_large_struct	something = {
		.char_pointer = NULL,
		.single_char = 'a',
		.some_counter = 2434837,
		.buffer = "diskelsidjdiskelsidjdiskelsidjdiskelsidj122",
		.four_longs = {243L,99L,29384710L,-234890L},
		.another_number = 9999999
	};
	t_large_struct	*array;

	vector = vector_init(4, sizeof(t_large_struct), NULL);
	cr_assert_not_null(vector);
	cr_assert(vector->capacity == 4);
	array = vector_push_back(vector, &something);

	something.four_longs[2] = -9999;
	something.four_longs[3] = 1234567890;

	cr_assert(array[0].single_char == 'a');
	cr_assert(memcmp(array[0].buffer, something.buffer, 44) == 0);
	cr_assert(array[0].four_longs[3] == -234890L);

	array = vector_push_back(vector, &something);
	cr_assert(array[1].four_longs[2] = -9999);
	cr_assert(array[1].four_longs[3] = something.four_longs[3]);

	vector_destroy(vector);
}

static int count_sixes = 0;

void	test_del_funtion(void *item)
{
	if (*((int*)item) == 6)
		count_sixes++;
}

Test(vector, del_callback)
{
	t_vect	*vector;

	vector = vector_init(8, sizeof(int), test_del_funtion);
	int	six = 6;
	int other_number = 25;
	int i = 0;
	int num_sixes = 0;
	while (i < 100)
	{
		if (i % 3 == 0)
		{
			vector_push_back(vector, &six);
			num_sixes++;
		}
		else
			vector_push_back(vector, &other_number);
		i++;
	}
	vector_destroy(vector);
	cr_assert(num_sixes == count_sixes);
}

Test(vector, peek)
{
	t_vect	*vector;
	int	num;

	vector = vector_init(8, sizeof(int), NULL);
	cr_expect(vector_peek_32(vector) == 0);
	num = 25;
	vector_push_back(vector, &num);
	cr_expect(vector_peek_32(vector) == num);
	num = -199;
	vector_push_back(vector, &num);
	cr_expect(vector_peek_32(vector) == num);
	num = 8274;
	vector_push_back(vector, &num);
	cr_expect(*((int*)vector_peek(vector)) == num);
	vector_destroy(vector);
}
