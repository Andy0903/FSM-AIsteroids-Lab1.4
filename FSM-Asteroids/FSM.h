#pragma once

//FSM state types
enum
{
	FSM_STATE_NONE,
	FSM_STATE_APPROACH,
	FSM_STATE_ATTACK,
	FSM_STATE_EVADE,
	FSM_STATE_GETPOWERUP,
	FSM_STATE_IDLE,
    FSM_STATE_COUNT
};

//FSM machine types
enum
{
	FSM_MACH_NONE,
	FSM_MACH_MAINSHIP,
    FSM_MACH_COUNT
};
