#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "bp_api.h"

#define FALSE 0
#define TRUE 1
#define TARGET_SIZE 30
#define VB 1
#define SM_SIZE(x) pow(2,x+1)
#define BYTESIZE 8


class Row{
	public:
		int history;
		int tag;
		int *SM;
		int target;
		int valid_bit;
		Row():history(0),tag(-1),valid_bit(0),target(0){}
};


class BTB_Table{
	public:
	
		 Row* Rows;
		 int history_Size;
		 int tag_Size;
		 int table_Size;
		 int *global_SM;
		 bool isGlobal;
		 int isShared;
		 bool isGlobalHistory;
		 int global_History;
		 int fsmState;
		 
		 int br_num;
		 int flush_num;
		 int size;
		 
};

BTB_Table BTB;



static bool update_stat_machine(int *cell, bool taken){
	switch(*cell){
		case 0: //SNT
		if(taken) {(*cell)++; return true;}
		break;
		
		case 1: //WNT
		if(taken){(*cell)++;return true;}
		else (*cell)--;
		break;
		
		case 2: //WT
		if(taken){(*cell)++; return true;}
		else{ (*cell)--;}
		break;
		
		case 3: //ST
		if(!taken)(*cell)--;
		else return true;
		break;
	}
	return false;
}

int BP_init(unsigned btbSize, unsigned historySize, unsigned tagSize, unsigned fsmState,
bool isGlobalHist, bool isGlobalTable, int Shared){
	
	BTB.isGlobalHistory = isGlobalHist;
	BTB.fsmState = fsmState;
	BTB.history_Size = historySize;
	BTB.tag_Size = tagSize;
	BTB.table_Size = btbSize;
	BTB.isGlobal = isGlobalTable;
	BTB.isShared = Shared;
	BTB.global_History = 0;
	
	BTB.Rows=new Row[btbSize]();
	
	if(!isGlobalTable){
			//local table, local or global history
			//BTB.Rows=new Row[btbSize]();
			
			for(unsigned i=0; i<btbSize; i++){
				BTB.Rows[i].SM= new int[(int)pow(2,historySize)];
				BTB.Rows[i].valid_bit = 0;
				for(unsigned j=0; j< (unsigned)pow(2,historySize); j++)
					BTB.Rows[i].SM[j] = fsmState;
			}
			
			//update stats
			
			if(isGlobalHist){
				//local table, global history
				BTB.size = btbSize*(tagSize + TARGET_SIZE + VB + SM_SIZE(historySize)) + historySize;
			}
			else{
				//local table, local history
				BTB.size = btbSize*(tagSize + TARGET_SIZE + VB + historySize + SM_SIZE(historySize));
			}
			
	}
	else{
		//global table
		BTB.global_SM = new int[(int)pow(2,historySize)];
		for(int j=0; j<pow(2,historySize); j++)
			BTB.global_SM[j] = fsmState;		
		
		if(isGlobalHist){
			//global table, global history, Gshare or regular global
			//update stats
			BTB.size = btbSize*(tagSize + TARGET_SIZE +VB) + historySize + SM_SIZE(historySize);
		}
		else{
			//global table, local history, Lshare or regular shared array
			//update stats
			BTB.size = btbSize*(tagSize + VB + TARGET_SIZE + historySize) + SM_SIZE(historySize);		
			
		}
	}
	return 0;
}
	

bool BP_predict(uint32_t pc, uint32_t *dst){
	
	int mask = 0;
	int pc_tag;
	bool taken = FALSE;
	int fsm_offset = 0;
	
	switch(BTB.table_Size){
		case 32:
		mask = 0b1111100;
		pc_tag = (pc>>7)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 16:
		mask = 0b111100;
		pc_tag = (pc>>6)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 8:
		mask = 0b11100;
		pc_tag = (pc>>5)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 4:
		mask = 0b1100;
		pc_tag = (pc>>4)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 2:
		mask = 0b100;
		pc_tag = (pc>>3)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 1:
		pc_tag = (pc>>2)&((int)(pow(2,BTB.tag_Size)-1));
	}
	
	int offset = pc&mask;
	offset = offset>>2;
	
	if(!BTB.isGlobal){
		//local table
		if(BTB.Rows[offset].tag==pc_tag){
			//tag found
			if(BTB.isGlobalHistory){
				//local table, global history
				taken = (bool)(BTB.Rows[offset].SM[BTB.global_History]>>1);
				if(taken){
					*dst = BTB.Rows[offset].target;
					return taken;
				}
				else{
					*dst = pc+4;
					return taken;
				}
			}
			else{
				//local table, local history
				taken = (bool)(BTB.Rows[offset].SM[BTB.Rows[offset].history]>>1);
				if(taken){
					*dst = BTB.Rows[offset].target;
					return taken;
				}
				else{
					*dst = pc+4;
					return taken;
				}
			}
		
		}
		else{
		//tag not found
			*dst = pc+4;
			return taken; //default false
			}
	}
	else{
	//global table
		if(!BTB.isGlobalHistory){
		//global table, local history
			if(BTB.Rows[offset].tag==pc_tag){
				//tag found
				
				if(BTB.isShared==1){
					//global table, local history, Lshare
					fsm_offset = BTB.Rows[offset].history ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)) ; //need to be checked!!!!!!! PC XOR HISTORY
				}
				else if(BTB.isShared==2){
					//global table, local history, MIDshare
					fsm_offset = BTB.Rows[offset].history ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1));
				}
				else{
					//global table, local history, shared array
					fsm_offset = BTB.Rows[offset].history& ((int)pow(2,BTB.history_Size)-1);
				}
				
				taken = (bool)(BTB.global_SM[fsm_offset]>>1);
					if(taken){
						*dst = BTB.Rows[offset].target;
						return taken;
					}
					else{
						*dst = pc+4;
						return taken;
					}	
				}
			else{
				//tag not found
			*dst = pc+4;
			return taken;
			}
		}
		else{
			//global table, global history
			if(BTB.Rows[offset].tag==pc_tag){
				//tag found
				if(BTB.isShared==1){
				//global table, global history, Gshare
					fsm_offset = BTB.global_History ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY
				}
				else if(BTB.isShared==2){
				//global table, global history, MIDshare
					fsm_offset = BTB.global_History ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!מ!!!!!! PC XOR HISTORY					
				}
				else{
					//global table, global history, shared array
					fsm_offset = BTB.global_History& ((int)pow(2,BTB.history_Size)-1);
				}
				
				taken = (bool)(BTB.global_SM[fsm_offset]>>1);
				if(taken){
					*dst = BTB.Rows[offset].target; 
					return taken;
				}
				else{
					*dst = pc+4;
					return taken;
				}
					
			}
			else{
				//tag not found
				*dst = pc+4;
				return taken;
			}		
		}
	}	
}

void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
	
	bool change = false;
	bool flush = (!(targetPc==pred_dst) && taken) || (!taken && (pred_dst != pc+4));
	//update stats
	BTB.br_num++;
	if(flush) BTB.flush_num++;
	
	int mask = 0;
	int pc_tag;
	int fsm_offset = 0;
	
	switch(BTB.table_Size){
		case 32:
		mask = 0b1111100;
		pc_tag = (pc>>7)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 16:
		mask = 0b111100;
		pc_tag = (pc>>6)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 8:
		mask = 0b11100;
		pc_tag = (pc>>5)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 4:
		mask = 0b1100;
		pc_tag = (pc>>4)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 2:
		mask = 0b100;
		pc_tag = (pc>>3)&((int)(pow(2,BTB.tag_Size)-1));
		break;
		
		case 1:
		pc_tag = (pc>>2)&((int)(pow(2,BTB.tag_Size)-1));
	}
	
	int offset = pc&mask;
	offset = offset>>2;	
	
	if(!BTB.isGlobal){
		//local table
		if(BTB.Rows[offset].tag == pc_tag){
			//tag found
			if(BTB.isGlobalHistory){
				//local table, global history
				//update state machine
				change = update_stat_machine(&BTB.Rows[offset].SM[BTB.global_History] , taken);
				
				//update target
				if(change){
					BTB.Rows[offset].target = targetPc;
					//else BTB.Rows[offset].target=pc+4;
					
				}	
					
				//update history
				BTB.global_History = ((BTB.global_History<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;
			}
			else{
				//local table, local history
				//update state machine
				change = update_stat_machine(&BTB.Rows[offset].SM[BTB.Rows[offset].history] , taken);
				
				//update target
				if(change){
					BTB.Rows[offset].target = targetPc;
				}
				
				//update history
				BTB.Rows[offset].history = ((BTB.Rows[offset].history<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;
			}
		}
		else{
			//tag not found - insert new line
			BTB.Rows[offset].tag = pc_tag;
			BTB.Rows[offset].target = targetPc;
			for(int j=0; j< pow(2,BTB.history_Size); j++)
				BTB.Rows[offset].SM[j] = BTB.fsmState;		
			
			if(BTB.isGlobalHistory){
				 //local table, global history - need to update global history
				 
				 //update state machine
				 update_stat_machine(&BTB.Rows[offset].SM[BTB.global_History] , taken); //not sure if we need to update in the first time we insert				 
				 
				 BTB.global_History = ((BTB.global_History<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;

			}
			else{
				//local table, local history - need to set new local history
				//BTB.Rows[offset].history = 0;
				//update state machine
				update_stat_machine(&BTB.Rows[offset].SM[0] , taken); //not sure if we need to update in the first time we insert
				BTB.Rows[offset].history = (unsigned)taken;
			} 
			
			
		}
	}
	else{
		//global table
		if(!BTB.isGlobalHistory){
			//global table, local history
			if(BTB.Rows[offset].tag==pc_tag){
				//tag found - need to update state machine, history and target
				
				//update state machine
				if(BTB.isShared==1){
					//global table, local history, Lshare
					fsm_offset = BTB.Rows[offset].history ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY
				}
				else if(BTB.isShared==2){
					//global table, local history, MIDshare
					fsm_offset = BTB.Rows[offset].history ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY					
				}
				else{
					//global table, local history, shared array
					fsm_offset = BTB.Rows[offset].history & ((int)pow(2,BTB.history_Size)-1) ;
				}
				change = update_stat_machine(&BTB.global_SM[fsm_offset] , taken);
				
				//update target
				if(change){
					//need to update target address
					BTB.Rows[offset].target = targetPc;
				}
				
				//update local history
				BTB.Rows[offset].history = ((BTB.Rows[offset].history<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;

			}
			else{
				//global table, local history - tag not found - insert new line
				BTB.Rows[offset].tag = pc_tag;
				BTB.Rows[offset].target = targetPc; //taken ? targetPc : (pc+4);
				
				
				
				
				
				//fsm update
				
				
				if(BTB.isShared==1){
					//global table, local history, Lshare
					//fsm_offset = BTB.Rows[offset].history ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY
					fsm_offset = 0 ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1));
				}
				else if(BTB.isShared==2){
					//global table, local history, MIDshare
					//fsm_offset = BTB.Rows[offset].history ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY					
					fsm_offset = 0 ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1));
				}
				else{
					//global table, local history, shared array
					//fsm_offset = BTB.Rows[offset].history;
					fsm_offset=0;
				}				
				
				update_stat_machine(&BTB.global_SM[fsm_offset] , taken);  //should we change global table if pc are not recognized???
			
				//history update
				BTB.Rows[offset].history = (unsigned)taken;
				//BTB.Rows[offset].history =0;
			}
		}
		else{
			//global table, global history
			if(BTB.Rows[offset].tag==pc_tag){
				//tag found
					
				//update FSM
				if(BTB.isShared==1){
					//global table, local history, Gshare
					fsm_offset = BTB.global_History ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY
				}
				else if(BTB.isShared==2){
					//global table, local history, MIDshare
					fsm_offset = BTB.global_History ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY					
				}
				else{
					//global table, local history, shared array
					fsm_offset = BTB.global_History& ((int)pow(2,BTB.history_Size)-1);
				}
				
				change = update_stat_machine(&BTB.global_SM[fsm_offset],taken);
				
				//update target address
				if(change){
				BTB.Rows[offset].target = targetPc;
				}
				
				//update history
				BTB.global_History = ((BTB.global_History<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;				
				
			}
			else{
				//global table, global history - tag not found - insert new line
				BTB.Rows[offset].tag=pc_tag;
				BTB.Rows[offset].target = targetPc; //taken ? targetPc : (pc+4);

				
				//fsm update
				
				if(BTB.isShared==1){
					//global table, global history, Gshare
					fsm_offset = BTB.global_History ^ ((pc>>2) & ((int)pow(2,BTB.history_Size)-1)); //need to be checked!!!!!!! PC XOR HISTORY
				}
				else if(BTB.isShared==2){
					fsm_offset = BTB.global_History ^ ((pc>>16) & ((int)pow(2,BTB.history_Size)-1));
				}
				else{
					//global table, local history, shared array
					fsm_offset = BTB.global_History& ((int)pow(2,BTB.history_Size)-1);
				}				
				
				update_stat_machine(&BTB.global_SM[fsm_offset] , taken);
				
				
				//history update - maybe we are not supposed to change history if this is a new rule
				BTB.global_History = ((BTB.global_History<<1)&((int)pow(2,BTB.history_Size)-1)) + (unsigned)taken;				
				}
		}
	}
	
}

void BP_GetStats(SIM_stats *curStats){
	
	//int byte_Size = BTB.size / BYTESIZE;
	
	curStats->flush_num = BTB.flush_num;
	curStats->br_num = BTB.br_num;
	curStats->size = BTB.size;
	/*
	for(int i=0; i<BTB.table_Size; i++)
		delete BTB.Rows[i].SM;
		
	delete BTB.Rows;
	delete BTB.global_SM;
			
	*/
	return ;
	
	//free memory
}



