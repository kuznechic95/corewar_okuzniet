//
// Created by Oleksandr KUZNIETSOV on 2019-02-23.
//
#include "corewar.h"

/*
** Функция принимает 3 параметра :
** Карта, с которой необходимо достать аргументы;
** Позиция на карте, с которой мы начнем считывать аргументы;
** Количество байтов, которые нужно считать.
** Пока количество байтов больше нуля происходит следующее:
** Уменьшаем количество байтов;
** Записываем во временую переменую аргумент/байт на карте и выполняем
** побитовый сдвиг на количесвто байтов которе осталось * 8 (8бит);
** Выполняем побитовое "ИЛИ" между результатом и временной переменной, и
** записываем результат в result.
**
** Пример: пусть мы получаем карту (00 10 0а 1b ff), позицию 1 и кол-во байт 4
** temp = 0000 0000 0000 0000 0000 0000 0001 0000 << 3 * 8 =
** = 0001 0000 0000 0000 0000 0000 0000 0000
** result = result | temp
** result = 0001 0000 0000 0000 0000 0000 0000 0000
** temp = 0000 0000 0000 0000 0000 0000 0000 1010
** result = result | temp
** result = 0001 0000 0000 1010 0000 0000 0000 0000
** temp = 0000 0000 0000 0000 0000 0000 0001 1011
** result = result | temp
** result = 0001 0000 0000 1010 0001 1011 0000 0000
** temp = 0000 0000 0000 0000 0000 0000 1111 1111
** result = result | temp
** result = 0001 0000 0000 1010 0001 1011 1111 1111 == 0x100a1bff
*/

unsigned int	get_bytecode_arg_from_map(t_map *map, unsigned int pos, int size)
{
	unsigned int	result;
	unsigned int	temp;
	
	result = 0;
	while (size > 0)
	{
		size--;
		temp = (unsigned int)(map[pos % MEM_SIZE].cell) << (size * 8);
		result = result | temp;
		pos++;
	}
	return (result);
}


//this is shit!!!
int 	read_op_arg(t_vm *vm, t_carriage *carr, int arg_num)
{
	
	t_operations	*op;
	int				arg;
	
//this?
	op = &g_operations[DEC(op->opcode)];
	//or this? 	what is the problem?
//	op = g_operations[DEC(carr->opcode)].function(vm, carr);
	arg = 0;
	if (T_REG & carr->type_arg[arg_num])
		arg = get_bytecode_arg_from_map(vm->map, carr->position, REG_SIZE);
	if (T_IND & carr->type_arg[arg_num])
		arg = get_bytecode_arg_from_map(vm->map, carr->position, IND_SIZE);
	if (T_REG & carr->type_arg[arg_num])
		arg = get_bytecode_arg_from_map(vm->map, carr->position, op->dir_size);
	return (arg);
}

void		op_live(t_vm *vm, t_carriage *carr)
{
	int32_t			bot_id;
	t_bot			*bot;

	carr->position += LEN_OF_OPERATION;
	//read arguments
	bot_id = read_op_arg(vm, carr, 0);
	carr->cycle_carr_last_reported_live = vm->current_cycle;
	vm->number_of_lives++;
	if (bot_id <= -1 && bot_id >= -((int32_t)vm->number_of_bots))
	{
		bot = &vm->bot[DEC(-(bot_id))];
		bot->cycle_bot_last_said_alive = vm->current_cycle;
		bot->number_bot_said_alive++;
		vm->winner = bot;
	}
	//надо сообщить визуалке, что ячейка, где каретка ответила live должна ещё 50 ходов
	// светиться жирным и цветом игрока, который ответил live!
	carr->position = SIZE_DIR(1);
	//ft_printf("Live.On map %0.2x\n", vm->map[carriage->position].cell);
}




//int32_t		get_op_arg(t_vm *vm, t_cursor *cursor, uint8_t index, t_bool mod)
//{
//	t_op		*op;
//	int32_t		value;
//	int32_t		addr;
//
//	op = &g_op[INDEX(cursor->opcode)];
//	value = 0;
//	if (cursor->args_types[INDEX(index)] & T_REG)
//		value = cursor->reg[INDEX(get_byte(vm, cursor->pc, cursor->step))];
//	else if (cursor->args_types[INDEX(index)] & T_DIR)
//		value = bytecode_to_int32(vm->arena,
//								  cursor->pc + cursor->step,
//								  op->t_dir_size);
//	else if (cursor->args_types[INDEX(index)] & T_IND)
//	{
//		addr = bytecode_to_int32(vm->arena,
//								 cursor->pc + cursor->step,
//								 IND_SIZE);
//		value = bytecode_to_int32(vm->arena,
//								  cursor->pc + (mod ? (addr % IDX_MOD) : addr),
//								  DIR_SIZE);
//	}
//	cursor->step += step_size(cursor->args_types[INDEX(index)], op);
//	return (value);
//}

//
//#include "ft_printf.h"
//#include "corewar_op.h"
//#include "corewar_vs.h"
//
//inline static void	log_live(uint32_t cursor_id, int32_t player_id)
//{
//	ft_printf("P %4d | live %d\n", cursor_id, player_id);
//}
//
//inline static void	log_live_msg(int32_t player_id, char *player_name)
//{
//	ft_printf("Player %d (%s) is said to be alive\n",
//			  player_id,
//			  player_name);
//}
//
//void				op_live(t_vm *vm, t_cursor *cursor)
//{
//	int32_t		player_id;
//	t_player	*player;
//
//	cursor->step += OP_CODE_LEN;
//	player_id = get_op_arg(vm, cursor, 1, false);
//	vm->lives_num++;
//	cursor->last_live = vm->cycles;
//	player = NULL;
//	if (player_id <= -1 && player_id >= -((int32_t)vm->players_num))
//	{
//		player = vm->players[INDEX(FT_ABS(player_id))];
//		player->last_live = vm->cycles;
//		player->current_lives_num++;
//		vm->last_alive = player;
//		if (vm->vs)
//		{
//			vm->vs->map[cursor->pc].wait_cycles_live = CYCLE_TO_WAIT;
//			vm->vs->map[cursor->pc].player_live = player;
//		}
//	}
//	if (vm->log & OP_LOG)
//		log_live(cursor->id, player_id);
//	if (vm->log & LIVE_LOG && player)
//		log_live_msg(FT_ABS(player_id), player->name);
//}




//
//int32_t		bytecode_to_int32(const uint8_t *arena, int32_t addr, int32_t size)
//{
//	int32_t		result;
//	t_bool		sign;
//	int			i;
//
//	result = 0;
//	sign = (t_bool)(arena[calc_addr(addr)] & 0x80);
//	i = 0;
//	while (size)
//	{
//		if (sign)
//			result += ((arena[calc_addr(addr + size - 1)] ^ 0xFF) << (i++ * 8));
//		else
//			result += arena[calc_addr(addr + size - 1)] << (i++ * 8);
//		size--;
//	}
//	if (sign)
//		result = ~(result);
//	return (result);
//}
//
//void		int32_to_bytecode(uint8_t *arena, int32_t addr, int32_t value,
//							  int32_t size)
//{
//	int8_t i;
//
//	i = 0;
//	while (size)
//	{
//		arena[calc_addr(addr + size - 1)] = (uint8_t)((value >> i) & 0xFF);
//		i += 8;
//		size--;
//	}
//}
