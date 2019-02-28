//
// Created by Oleksandr KUZNIETSOV on 2019-02-23.
//
#include "corewar.h"

#define DEC(i) (i-1)


int32_t 	read_op_arg(t_vm *vm, t_carriage carriage, int opcode, int mod)
{
	
	t_operations	*operations;
	int32_t			arg_value;
	
	operations = &g_operations[DEC(opcode)];
	arg_value = 0;
	if ()
	
}

void		ft_live(t_vm *vm, t_carriage *carriage)
{
	int32_t			bot_id;
	t_bot			*bot;
	
	//if (vm->map[carriage->position].cell)
	//	i++;
	
	vm->number_of_lives++;
	bot = read_op_arg(vm, carriage, 1, 0);
	carriage->cycle_carriage_last_reported_live = vm->current_cycle;
	if (bot->id <= -1 && bot->id >= -((int32_t)vm->number_of_bots))
	{
	
	}
	
	
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
