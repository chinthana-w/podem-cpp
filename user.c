#include "input.h"

/**********************************************************/
// Lookup tables for gates
/**********************************************************/
int andGate[5][5] = {
	{0, 0, 0, 0, 0},
	{0, 1, 2, 3, 4},
	{0, 2, 2, 2, 2},
	{0, 3, 2, 3, 0},
	{0, 4, 2, 0, 4}
};

int orGate[5][5] = {
	{0, 1, 2, 3, 4},
	{1, 1, 1, 1, 1},
	{2, 1, 2, 2, 2},
	{3, 1, 2, 3, 1},
	{4, 1, 2, 1, 4}
};

int notGate[5] = {1, 0, 2, 4, 3};

LIST *D_frontier;
int depth = 0;
int fault_excited = 0;

/***************************************************************************************************/
/***************************************************************************************************
 Function to read the test patterns(.pattern files)
***************************************************************************************************/
void ReadPattern(char *f_name, char *patternList[]) {
	FILE *fp = fopen(f_name, "r");
  	char line[Mlin];
	int pattern_index = 0;

	while (fgets(line, sizeof(line), fp) && pattern_index < Mpt) {
		line[strlen(line) - 1] = '\0';
		patternList[pattern_index] = malloc(strlen(line) + 1); 
    	strcpy(patternList[pattern_index], line);
		pattern_index++;
  	}
}
//end of ReadPattern 
/****************************************************************************************************************************/

// logic simulation support functions
void LogicSim(GATE *Node, int Tgat, char *patternList[], char *res_fname) {
	int node_index = 0;
	int pattern_index = 0;
	int pi_index = 0;
	int node_result = 0;

	GATE *curent_node = NULL;
	LIST *current_fanin = NULL;
	FILE *fp = fopen(res_fname, "w");

	while (pattern_index < Mpt && patternList[pattern_index] != NULL) {
		node_index = 0;
		pi_index = 0;
		while (node_index <= Tgat) {
			curent_node = &Node[node_index];
			switch (curent_node->Type) {
				case INPT:
					curent_node->Val = patternList[pattern_index][pi_index] - '0';
					pi_index++;
					break;

				case FROM:
					curent_node->Val = Node[curent_node->Fin->Id].Val;
					break;

				case BUFF:
					curent_node->Val = Node[curent_node->Fin->Id].Val;
					break;

				case NOT:
					curent_node->Val = notGate[Node[curent_node->Fin->Id].Val];
					break;

				case AND:
					node_result = 1;
					current_fanin = curent_node->Fin;

					while (current_fanin != NULL) {
						node_result = andGate[node_result][Node[current_fanin->Id].Val];
						current_fanin = current_fanin->Next;
					}
					curent_node->Val = node_result;
					break;

				case NAND:
					node_result = 1;
					current_fanin = curent_node->Fin;

					while (current_fanin != NULL) {
						node_result = andGate[node_result][Node[current_fanin->Id].Val];
						current_fanin = current_fanin->Next;
					}
					curent_node->Val = notGate[node_result];
					break;

				case OR:
					node_result = 0;
					current_fanin = curent_node->Fin;

					while (current_fanin != NULL) {
						node_result = orGate[node_result][Node[current_fanin->Id].Val];
						current_fanin = current_fanin->Next;
					}
					curent_node->Val = node_result;
					break;

				case NOR:
					node_result = 0;
					current_fanin = curent_node->Fin;

					while (current_fanin != NULL) {
						node_result = orGate[node_result][Node[current_fanin->Id].Val];
						current_fanin = current_fanin->Next;
					}
					curent_node->Val = notGate[node_result];
					break;

				case XOR:
					current_fanin = curent_node->Fin;
					curent_node->Val = Node[current_fanin->Id].Val != Node[current_fanin->Next->Id].Val;
					break;

				case XNOR:
					current_fanin = curent_node->Fin;
					curent_node->Val = Node[current_fanin->Id].Val == Node[current_fanin->Next->Id].Val;
					break;

				default:
					break;
			}
			node_index++;
		}

		printf("#%d: %s", pattern_index, patternList[pattern_index]);
		printf(" -> ");
		printPO(Node, Tgat, fp);
		printf("\n");
		pattern_index++;
	}
}

// logic simulation support functions
void LogicSimAndImpl(GATE *Node, int Tgat, GV_pair *fault, GV_pair *pi_gv) {
	int node_index = 0;
	int pattern_index = 0;
	int node_result = 0;
	fault_excited = 0;

	GATE *curent_node = NULL;
	LIST *current_fanin = NULL;

	Node[pi_gv->gate_id].Val = pi_gv->value;

	while (node_index <= Tgat) {
		curent_node = &Node[node_index];
		switch (curent_node->Type) {
			case INPT:
				break;

			case FROM:
				curent_node->Val = Node[curent_node->Fin->Id].Val;
				break;

			case BUFF:
				curent_node->Val = Node[curent_node->Fin->Id].Val;
				break;

			case NOT:
				curent_node->Val = notGate[Node[curent_node->Fin->Id].Val];
				break;

			case AND:
				node_result = 1;
				current_fanin = curent_node->Fin;

				while (current_fanin != NULL) {
					node_result = andGate[node_result][Node[current_fanin->Id].Val];
					current_fanin = current_fanin->Next;
				}
				curent_node->Val = node_result;
				break;

			case NAND:
				node_result = 1;
				current_fanin = curent_node->Fin;

				while (current_fanin != NULL) {
					node_result = andGate[node_result][Node[current_fanin->Id].Val];
					current_fanin = current_fanin->Next;
				}
				curent_node->Val = notGate[node_result];
				break;

			case OR:
				node_result = 0;
				current_fanin = curent_node->Fin;

				while (current_fanin != NULL) {
					node_result = orGate[node_result][Node[current_fanin->Id].Val];
					current_fanin = current_fanin->Next;
				}
				curent_node->Val = node_result;
				break;

			case NOR:
				node_result = 0;
				current_fanin = curent_node->Fin;

				while (current_fanin != NULL) {
					node_result = orGate[node_result][Node[current_fanin->Id].Val];
					current_fanin = current_fanin->Next;
				}
				curent_node->Val = notGate[node_result];
				break;

			case XOR:
				current_fanin = curent_node->Fin;
				curent_node->Val = Node[current_fanin->Id].Val != Node[current_fanin->Next->Id].Val;
				break;

			case XNOR:
				current_fanin = curent_node->Fin;
				curent_node->Val = Node[current_fanin->Id].Val == Node[current_fanin->Next->Id].Val;
				break;

			default:
				break;
		}

		if (node_index == fault->gate_id) inject_fault(curent_node, fault, node_index);
		update_d_frontier(Node, node_index);

		node_index++;
	}
}

void inject_fault(GATE *node, GV_pair *fault, int position) {
	if (fault->value == DB && node->Val == 0) {
		node->Val = DB;
		fault_excited = 1;
	} else if (fault->value == D && node->Val == 1) {
		node->Val = D;
		fault_excited = 1;
	} else if (node->Val == D || node->Val == DB) {
		fault_excited = 1;
	}
}

int is_podem_fail(GATE *cir, GV_pair *fault) {
	int df_null = D_frontier == NULL ? 1 : 0;
	int inactive = cir[fault->gate_id].Val == (fault->value - D) ? 1 : 0;

	return (inactive == 1 || (fault_excited == 1 && df_null == 1)) ? 1 : 0;
}

void update_d_frontier(GATE *cir, int current_node_id) {
	LIST *f_in = cir[current_node_id].Fin;

	if (cir[current_node_id].Val != XD) {
		if (FindEle(D_frontier, current_node_id) == 1) {
			DeleteEle(&D_frontier, current_node_id);
		}
		return;
	}

	while (f_in != NULL) {
		if ((cir[f_in->Id].Val == D || cir[f_in->Id].Val == DB) && cir[current_node_id].Val == XD) {
			InsertEle(&D_frontier, current_node_id);
			break;
		}
		f_in = f_in->Next;
	}
}

int fault_is_at_PO(GATE *cir, int Tgat) {
	int ptr = 0;

	while (ptr <= Tgat) {
		if (cir[ptr].Nfo == 0 && (cir[ptr].Val == D || cir[ptr].Val == DB)) return 1;
		ptr++;
	}

	return 0;
}

void printPI(GATE *Node, int Tgat, FILE *fp) {
	int i = 0;

	for(i = 1; i <= Tgat; i++) { 
		if(Node[i].Type != 0){
			if(Node[i].Nfi == 0) {
				if (Node[i].Val == XD) {
					printf("X");
					if (fp != NULL) fprintf(fp, "X");
				} else if (Node[i].Val == D) {
					printf("1");
					if (fp != NULL) fprintf(fp, "1");
				} else if (Node[i].Val == DB) {
					printf("0");
					if (fp != NULL) fprintf(fp, "0");
				} else {
					printf("%d", Node[i].Val);
					if (fp != NULL) fprintf(fp, "%d", Node[i].Val);
				}
			}
		}
	}
	return;
}

void printPO(GATE *Node, int Tgat, FILE *fp) {
	int i = 0;

	for(i = 1; i <= Tgat; i++) { 
		if(Node[i].Type != 0){
			if(Node[i].Nfo == 0) {
				printf("%d", Node[i].Val);
				if (fp != NULL) fprintf(fp, "%d", Node[i].Val);
			}
		}
	}
	if (fp != NULL) fprintf(fp, "\n");
	return;
}
/*************************************
 * end of support functions
*/


/*************************************
 * podem functions
*/
int podem(GATE *cir, GV_pair *fault, int Tgat, FILE *res_fname) {
	reset_gates(cir, Tgat);

	depth = 0;
	double start_time = clock() / CLOCKS_PER_SEC;

	int podem_result = podem_recursion(cir, Tgat, fault, start_time);

	print_pattern(res_fname, fault->gate_id, fault->value, cir, Tgat, podem_result);
	FreeList(&D_frontier);

	return podem_result;
}

void reset_gates(GATE *cir, int Tgat) {
	int node_index = 0;
	GATE *gate_iterator;

	while (node_index <= Tgat) {
		gate_iterator = &cir[node_index];
		gate_iterator->Val = XD;
		node_index++;
	}
}

int podem_recursion(GATE *cir, int Tgat, GV_pair *fault, double start_time) {

	if (clock()/CLOCKS_PER_SEC - start_time > MAX_T) return TIMEOUT;
	if (fault_is_at_PO(cir, Tgat)) return SUCCESS;

	GV_pair objective = getObjective(cir, fault);

	if (objective.gate_id == -1) return FAILURE;

	GV_pair backtrace_result = backtrace(&objective, cir);

	if (backtrace_result.gate_id == -1) return FAILURE;

	LogicSimAndImpl(cir, Tgat, fault, &backtrace_result);

	int is_fail = is_podem_fail(cir, fault);

	if (fault_is_at_PO(cir, Tgat)) return SUCCESS;

	int result;
	result = podem_recursion(cir, Tgat, fault, start_time);

	if (result == SUCCESS) {
		return SUCCESS;
	} else if (result == TIMEOUT) {
		return TIMEOUT;
	}

	backtrace_result.value = backtrace_result.value == 1 ? 0 : 1;

	LogicSimAndImpl(cir, Tgat, fault, &backtrace_result);

	if (fault_is_at_PO(cir, Tgat)) {
		return SUCCESS;
	}

	depth++;
	result = podem_recursion(cir, Tgat, fault, start_time);
	depth--;

	if (result == SUCCESS) {
		return SUCCESS;
	} else if (result == TIMEOUT) {
		return TIMEOUT;
	}

	backtrace_result.value = XD;
	LogicSimAndImpl(cir, Tgat, fault, &backtrace_result);

	return FAILURE;
}

GV_pair getObjective(GATE *cir, GV_pair *fault) {
	GV_pair objective;

	if (D_frontier == NULL && (cir[fault->gate_id].Val == D || cir[fault->gate_id].Val == DB)) {
		objective.gate_id = -1;
		return objective;
	}

	if (cir[fault->gate_id].Val == D || cir[fault->gate_id].Val == DB) {
		objective.value = get_non_cont(cir, D_frontier->Id);
		objective.gate_id = getXFanIn(cir, D_frontier->Id);
		return objective;
	}

	objective.gate_id = fault->gate_id;
	objective.value = fault->value == D ? 1 : 0;

	return objective;
}

int get_non_cont(GATE *cir, int gate_id) {
	switch (cir[gate_id].Type) {
		case AND:
			return 1;
		case NAND:
			return 1;
		case OR:
			return 0;
		case NOR:
			return 0;
		default:
			return -1;
	}
}

int getXFanIn(GATE *cir, int gate_id) {
	if (cir[gate_id].Type == INPT && cir[gate_id].Val == XD) {
		return gate_id;
	}

	LIST *fin = cir[gate_id].Fin;

	while (cir[fin->Id].Val != XD) {
		fin = fin->Next;
		if (fin == NULL) return -1;
	}

	return fin->Id;
}

GV_pair backtrace(GV_pair *objective, GATE *cir) {
	int num_inv = 0;
	GV_pair result;
	int current_gate_id = objective->gate_id;
	GATE current_gate = cir[current_gate_id];

	result.gate_id = objective->gate_id;

	while (current_gate.Nfi != 0) {
		if (current_gate.Type == NOT || current_gate.Type == NAND || current_gate.Type == NOR) {
			num_inv++;
		}
		current_gate_id = getXFanIn(cir, current_gate_id);
		if (current_gate_id == -1) {
			result.gate_id = -1;
			return result;
		}
		current_gate = cir[current_gate_id];
	}

	result.gate_id = current_gate_id;
	result.value = num_inv % 2 == 0 ? objective->value : !objective->value;

	return result;
}

void print_pattern(FILE *fp, int id, int fault, GATE *Node, int Tgat, int state) {
	fprintf(fp, "g%d/", id);
	if (fault == D) {
		fprintf(fp, "0 ");
	} else {
		fprintf(fp, "1 ");
	}

	if (state == SUCCESS) {
		fprintf(fp, "SUCCESS");
	} else if (state == TIMEOUT) {
		fprintf(fp, "TIMEOUT");
	} else {
		fprintf(fp, "UNTESTABLE");
	}

	fprintf(fp, "\n");
}
