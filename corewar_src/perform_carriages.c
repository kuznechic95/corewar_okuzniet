/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_carriages.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdomansk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 14:02:54 by bdomansk          #+#    #+#             */
/*   Updated: 2019/02/24 14:02:56 by bdomansk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"


/*
** Если в каретке еще не сохраненна команда, а место на котором стоит каретка
** содержит не коректную команду, то каретка передвигается на 1 шаг вперед.
** Иначе, если команда еще не записана, то записываев в каретку команду и
** количество циклов, которое картке нужно ожидать выполения команды.
** Уменьшаем количество циклов ожидания.
** Если количесвто циклов ожидания равно 0, то мы выполняем команду,
** передвигаем каретку, и убираем записаную команду из каретки.
** В этом коментарии команда употреблялась в значении opcode.
*/

unsigned char	read_data(t_map *map, int pos, int num_bytes_to_read)
{
	return (map[(pos + num_bytes_to_read) % MEM_SIZE].cell);
}

/*
** 0xC0 T_REG
** 0x30 T_DIR
** 0xC  T_IND
** T_REG	r	01
** T_DIR	%	10
** T_IND		11
**
*/

void		set_tac(t_carriage *carr, unsigned char code, unsigned char i)
{
	carr->type_arg[DEC(i)] = g_ass_ar[DEC(code)];

}

void		read_types_args_codes(t_operations *op, t_vm *vm, t_carriage *carr)
{
	unsigned char type_code;
	
	if (op->number_args)
	{
		type_code = read_data(vm->map, carr->position, 1);
		set_tac(carr, ((unsigned char) (type_code & 0xC0) >> 6), 1);
		if (op->number_args > 1)
			set_tac(carr, ((unsigned char) (type_code & 0x30) >> 4), 2);
		if (op->number_args > 2)
			set_tac(carr, ((unsigned char) (type_code & 0xC) >> 2), 3);
	}
}

int 		is_valid_arg(t_operations *op, t_vm *vm, t_carriage *carr)
{
	(void)op;
	(void)vm;
	(void)carr;
	
	return (1);
}

static void	perform_carriage(t_carriage *carr, t_vm *vm)
{
	int			pos;
	t_operations *op;
	
	pos = carr->position;
	if (!carr->opcode && (vm->map[pos].cell > 16 || !vm->map[pos].cell))
		carr->position = (pos + 1) % MEM_SIZE;
	else
	{
		if (!carr->opcode)
		{
			carr->opcode = OPCODE(vm->map[pos].cell);
			carr->cycles_left = CYCLES(carr->opcode);
		}
		if (carr->cycles_left > 0)
			carr->cycles_left--;
		if (carr->cycles_left == 0)
		{
			op = &g_operations[DEC(carr->opcode)];
	//		ft_printf("op = &g_operations[DEC(carr->opcode)]; : %d \n", op->opcode);
			read_types_args_codes(op, vm, carr);
			if (is_valid_arg(op, vm, carr))
				g_operations[carr->opcode - 1].function(vm, carr);
			//ft_printf("Выполняем cmd %d, двигаем каретку\n", carr->opcode);
	//		ft_printf("Выполняем cmd %d, двигаем каретку\n", carr->type_arg[0]);
			carr->opcode = 0;
		}
	}
}

void		perform_carriages(t_vm *vm)
{
	t_carriage	*start;

	start = vm->carriage;
	while (vm->carriage)
	{
		perform_carriage(vm->carriage, vm);
		vm->carriage = vm->carriage->next;
	}
	vm->carriage = start;
}



//
//t_bool			is_arg_types_valid(t_cursor *cursor, t_op *op)
//{
//	int32_t i;
//
//	i = 0;
//	while (i < op->args_num)
//	{
//		if (!(cursor->args_types[i] & op->args_types[i]))
//			return (false);
//		i++;
//	}
//	return (true);
//}
//
//static t_bool	is_register(t_vm *vm, int32_t pc, int32_t step)
//{
//	int8_t r_id;
//
//	r_id = get_byte(vm, pc, step);
//	return (t_bool)(r_id >= 1 && r_id <= REG_NUMBER);
//}
//
//t_bool			is_args_valid(t_cursor *cursor, t_vm *vm, t_op *op)
//{
//	int32_t		i;
//	uint32_t	step;
//
//	i = 0;
//	step = (OP_CODE_LEN + (op->args_types_code ? ARGS_CODE_LEN : 0));
//	while (i < op->args_num)
//	{
//		if ((cursor->args_types[i] == T_REG)
//			&& !is_register(vm, cursor->pc, step))
//			return (false);
//		step += step_size(cursor->args_types[i], op);
//		i++;
//	}
//	return (true);
//}

