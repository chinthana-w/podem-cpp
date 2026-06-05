#include "input.h"
/***************************************************************************************************
Command Instructions
***************************************************************************************************/
//To Compile: make
//To Run: ./project c17.isc 
/***************************************************************************************************
 Main Function
***************************************************************************************************/
int main(int argc,char **argv)
{
FILE *Isc,*Pat,*Res;                  //File pointers used for .isc, .pattern, and .res files
int Npi,Npo,Tgat;                     //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE *Node;                           //Structure to store the ckt given in .isc file 
clock_t Start,End;                    //Clock variables to calculate the Cputime
double Cpu;                           //Total cpu time
int i,j;                              //Temporary variables




/****************PART 1.-Read the .isc file and store the information in Node structure***********/
Npi=Npo=Tgat=0;                                //Intialize values of all variables
Isc=fopen(argv[1],"r");                        //File pointer to open .isc file 
Node=(GATE *) malloc(Mnod * sizeof(GATE));     //Dynamic memory allocation for Node structure
Tgat=ReadIsc(Isc,Node);                        //Read .isc file and return index of last node
fclose(Isc);                                   //Close file pointer for .isc file
// PrintGats(Node,Tgat);                          //Print the information of each active gate in Node structure after reading .isc file
CountPri(Node,Tgat,&Npi,&Npo);                 //Count the No of Pis and Pos
// char *patternList[Mpt + 1];
//ReadPattern(Pat, patternList);
//Pat = fopen(argv[2],"r");
//fclose(Pat);
printf("Npi: %d Npo: %d\n",Npi,Npo);       //Print the no of primary inputs and outputs
printf("Total no of gates: %d\n",Tgat);        //Print the total no of gates
/***************************************************************************************************/
// ReadPattern(argv[2], patternList);
// LogicSim(Node, Tgat, patternList, argv[3]);
// reset_gates(Node, Tgat);
/***************************************************************************************************/
// podem functions
/***************************************************************************************************/

int node_index = 0;
GV_pair fault;
FILE *ptrn_result = fopen(argv[2], "w");

int success_count = 0;
int untestable_count = 0;
int timeout_count = 0;
int podem_result;

while (node_index <= Tgat) {
    if (Node[node_index].Type == 0) {
        node_index++;
        continue;
    }

    fault.gate_id = node_index;

    fault.value = D;
    // printf("-----------------------Fault: %d, SA:%d\n", fault.gate_id, fault.value - D);
    podem_result = podem(Node, &fault, Tgat, ptrn_result);
    // PrintGats(Node,Tgat);
    // printf("\n\n");
    reset_gates(Node, Tgat);

    if (podem_result == SUCCESS) {
        success_count++;
    } else if (podem_result == FAILURE) {
        untestable_count++;
    } else if (podem_result == TIMEOUT) {
        timeout_count++;
    }

    fault.value = DB;
    // printf("=---------------------Fault: %d, SA:%d\n", fault.gate_id, fault.value);
    podem_result = podem(Node, &fault, Tgat, ptrn_result);
    // PrintGats(Node,Tgat);
    // printf("\n\n");
    reset_gates(Node, Tgat);

    if (podem_result == SUCCESS) {
        success_count++;
    } else if (podem_result == FAILURE) {
        untestable_count++;
    } else if (podem_result == TIMEOUT) {
        timeout_count++;
    }

    node_index++;
}

double fault_coverage = (double) 100 * success_count / (success_count + untestable_count + timeout_count);
double untestable = (double) 100 * untestable_count / (success_count + untestable_count + timeout_count);
double timeout = (double) 100 * timeout_count / (success_count + untestable_count + timeout_count);

printf("Fault Coverage: %.2f%%\n", fault_coverage);
fprintf(ptrn_result, "\n\nFault Coverage: %.2f%%\n", fault_coverage);

printf("Untestable: %.2f%%\n", untestable);
fprintf(ptrn_result, "Untestable: %.2f%%\n", untestable);

printf("Timeout: %.2f%%\n", timeout);
fprintf(ptrn_result, "Timeout: %.2f%%\n", timeout);

fclose(ptrn_result);
}//end of main
/****************************************************************************************************************************/

