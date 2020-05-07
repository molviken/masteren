/*
 * FSM.h
 *
 * Created: 16/04/2020 14:16:42
 *  Author: oyste
 */ 


#ifndef FSM_H_
#define FSM_H_



enum States{
	ST_INIT,
	ST_SLEEP,
	ST_ACTIVE
};




int FSM_init();
void FSM_run();
void FSM_system_init(void);

#endif /* FSM_H_ */