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

//int		check_position(new_pos)
//{
//	new_pos %= MEM_SIZE;
//	if (new_pos < 0)
//		new_pos += MEM_SIZE;
//	return (new_pos);
//}

unsigned char	read_data(t_vm *vm, int pos, int num_bytes_to_read)
{
	return (vm->map[(pos + num_bytes_to_read) % MEM_SIZE].cell);
}

/*
** 0xC0 T_REG
** 0x30 T_DIR
** 0xC0 T_IND
**
**
*/


void		read_types_args_codes(t_operations *op, t_vm *vm, t_carriage *carr)
{
	unsigned char type_code;
	
	if (op->number_args)
	{
		type_code = read_data(vm, carr->position, 1);
		if (op->number_args >= 1 && ((char)(type_code & 0xC0) >> 6))
			carr->type_arg[0] = 1;
		if (op->number_args >= 2 && ((char)(type_code & 0x30) >> 4))
			carr->type_arg[1] = T_DIR;
		if (op->number_args == 3 && ((char)(type_code & 0xC) >> 2))
			carr->type_arg[2] = T_IND;
	}
	else
		op->type_args[0] = carr->type_arg[0];
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
			ft_printf("op = &g_operations[DEC(carr->opcode)]; : %d \n", op->opcode);
			read_types_args_codes(op, vm, carr);
			if (is_valid_arg(op, vm, carr))
				g_operations[carr->opcode - 1].function(vm, carr);
			//ft_printf("Выполняем cmd %d, двигаем каретку\n", carr->opcode);
			ft_printf("Выполняем cmd %d, двигаем каретку\n", carr->type_arg[0]);
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



//static void	set_arg_type(int8_t arg_type, int8_t index, t_cursor *cursor)
//{
//	cursor->args_types[INDEX(index)] = g_arg_code[INDEX(arg_type)];
//}
//
//void		parse_types_code(t_vm *vm, t_cursor *cursor, t_op *op)
//{
//	int8_t args_types_code;
//
//	if (op->args_types_code)
//	{
//		args_types_code = get_byte(vm, cursor->pc, 1);
//		if (op->args_num >= 1)
//			set_arg_type((int8_t)((args_types_code & 0xC0) >> 6), 1, cursor);
//		if (op->args_num >= 2)
//			set_arg_type((int8_t)((args_types_code & 0x30) >> 4), 2, cursor);
//		if (op->args_num >= 3)
//			set_arg_type((int8_t)((args_types_code & 0xC) >> 2), 3, cursor);
//	}
//	else
//		cursor->args_types[0] = op->args_types[0];
//}


//
//inline int8_t	get_byte(t_vm *vm, int32_t pc, int32_t step)
//{
//	return (vm->arena[calc_addr(pc + step)]);
//}


//int32_t		calc_addr(int32_t addr)
//{
//	addr %= MEM_SIZE;
//	if (addr < 0)
//		addr += MEM_SIZE;
//	return (addr);
//}