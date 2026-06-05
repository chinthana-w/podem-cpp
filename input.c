#include "input.h"
#include <ctype.h>
#include <string.h>
/***************************************************************************************************
Insert an element "X" at end of LIST "Cur", if "X" is not already in "Cur". 
If the "Cur" is Null, it creates a single element List using "X"
***************************************************************************************************/
void InsertEle(LIST **Cur,int X)
{
LIST *tl=NULL;
LIST *nl=NULL;

if ((tl=(LIST *) malloc(sizeof(LIST)))==NULL){ 
  printf("LIST: Out of memory\n");  
  exit(1);  }   
else{
  tl->Id=X;  tl->Next=NULL;
  if(*Cur==NULL){  
    *Cur=tl; 
    return;  }
  nl=*Cur; 
  while(nl!=NULL){
    if(nl->Id==X){       break;       }
    if(nl->Next==NULL){  nl->Next=tl; } 
    nl=nl->Next; }  
 }
return;
}//end of InsertEle
/***************************************************************************************************
Delete an element "X" from LIST "Cur",
***************************************************************************************************/	
void DeleteEle(LIST **Cur,int X)
{
LIST *fir = (*Cur);
LIST *lst = NULL;

if (fir==NULL) return;  
while((fir->Id!=X)&&(fir!= NULL)){
  lst=fir;
  fir=fir->Next; }   
if(fir!=NULL){
   if(lst==NULL){  (*Cur)=(*Cur)->Next;       }
   else{           lst->Next = fir->Next; }
   free(fir); }
else{ 
   return; }
return;
}//end of DeleteEle
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "Cur"; Otherwise return  0  
*****************************************************************************************************************************/
int FindEle(LIST *Cur,int X)
{
LIST *temp=NULL;
	
temp=Cur;
while(temp!=NULL){
  if(temp->Id==X) return 1;
  temp=temp->Next; }
return 0;
}//end of FindEle
/***************************************************************************************************
Print the elements in LIST "Cur"          
***************************************************************************************************/
void PrintList(LIST *Cur)
{
LIST *tmp=Cur;

while(tmp!=NULL){   
  printf("%d  ", tmp->Id);
  tmp = tmp->Next; } 
return;
}//end of PrintList
/***************************************************************************************************
Count the total number  of elements in LIST "Cur"          
***************************************************************************************************/
int CountList(LIST *Cur)
{
LIST *tmp=Cur;
int size=0;

while(tmp!=NULL){   
  size++;
  tmp = tmp->Next; } 
return size;
}//end of PrintList
/***************************************************************************************************
Free all elements in  LIST "Cur"  
***************************************************************************************************/
void FreeList(LIST **Cur)
{
LIST *tmp=NULL;

if(*Cur==NULL){   return;  }
tmp=(*Cur);
while((*Cur) != NULL){
  tmp=tmp->Next;
  free(*Cur); 
  (*Cur)=tmp; }   
(*Cur)=NULL;
return;
}//end of FreeList 
/***************************************************************************************************
Initialize the paricular member of GATE structure
***************************************************************************************************/        
void InitiGat(GATE *Node,int Num)
{
Node[Num].Name=(char *) malloc(Mnam *sizeof(char));           //Dynamic memory allocation for an array
bzero(Node[Num].Name,Mnam);                                   //Clearing the string
Node[Num].Type=Node[Num].Nfi=Node[Num].Nfo=Node[Num].Mark=0;
Node[Num].Val=6;  
Node[Num].Fin=Node[Num].Fot=NULL;
// Node[Num].Rpath=Node[Num].Fpath=NULL;   
return;
}//end of InitiGat
/***************************************************************************************************
Print all contents(attribute) of all active member of GATE structure(DdNodes are not printed)
***************************************************************************************************/
void PrintGats(GATE *Node,int Tgat)
{
int i;

printf("\nId\tName\tType\t#In\t#Out\tMark\tValue\t\tFanin\t\tFanout");
for(i=1;i<=Tgat;i++){
  if(Node[i].Type!=0){
    printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t",i,Node[i].Name,Node[i].Type,Node[i].Nfi,Node[i].Nfo,Node[i].Mark,Node[i].Val);
    PrintList(Node[i].Fin);  printf("\t\t");
    PrintList(Node[i].Fot); }}
return;
}//end of PrintGats
/***************************************************************************************************
Free the memory of all contents of all members of GATE structure(DdNodes are already cleared)
***************************************************************************************************/
void ClearGat(GATE *Node,int Tgat)
{
int i;

for(i=1;i<=Tgat;i++){ 
  free(Node[i].Name);
  Node[i].Type=Node[i].Nfi=Node[i].Nfo=Node[i].Mark=Node[i].Val=0;
  FreeList(&Node[i].Fin);  FreeList(&Node[i].Fot); }
return;
}//end of ClearGat
/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(GATE *Node,int Tgat,int *Npi,int *Npo)
{
int i,j,k;

i=j=k=0;
for(i=1;i<=Tgat;i++){ 
  if(Node[i].Type!=0){
    if(Node[i].Nfi==0){      j++; }
    if(Node[i].Nfo==0){      k++; } }}
*Npi=j; *Npo=k;
return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)     
***************************************************************************************************/
int AssignType(char *Gtyp){
	if      ((strcmp(Gtyp,"inpt")==0) || (strcmp(Gtyp,"INPT")==0))       return 1;
	else if ((strcmp(Gtyp,"from")==0) || (strcmp(Gtyp,"FROM")==0))       return 2;
	else if ((strcmp(Gtyp,"buff")==0) || (strcmp(Gtyp,"BUFF")==0))       return 3;
	else if ((strcmp(Gtyp,"not")==0)  || (strcmp(Gtyp,"NOT")==0))        return 4;
	else if ((strcmp(Gtyp,"and")==0)  || (strcmp(Gtyp,"AND")==0))        return 5;
	else if ((strcmp(Gtyp,"nand")==0) || (strcmp(Gtyp,"NAND")==0))       return 6;
	else if ((strcmp(Gtyp,"or")==0)   || (strcmp(Gtyp,"OR")==0))         return 7;
	else if ((strcmp(Gtyp,"nor")==0)  || (strcmp(Gtyp,"NOR")==0))        return 8;
	else if ((strcmp(Gtyp,"xor")==0)  || (strcmp(Gtyp,"XOR")==0))        return 9;
	else if ((strcmp(Gtyp,"xnor")==0) || (strcmp(Gtyp,"XNOR")==0))       return 10;
	else                          			                     return 0;
}//end of AssignType

/**
 * Trims whitespace characters from the beginning and end of a string.
 * 
 * @param str The string to be trimmed.
 * @return A pointer to the trimmed string.
 */
char *TrimWhitespace(char *str) {
    char *end;

    // Trim leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

GATE *LineToGate(char *line, GATE *Node, int *stage_ptr, int *node_id_ptr, int *tot) {
	// skip comment lines
	if (line[0] == '*') {
		return Node;
	} 
	// split line into charachter tokens
	int line_len = strlen(line);
	int loop_index = 0;
	int token_begin = 0;
	int token_length = 0;
	char token[TokLen] = "";
	char *token_ptr = token;
	int gate_type, fanlist_node;
	int stage = *stage_ptr;
	int node_id = *node_id_ptr;
	int fanlist_length = 0;
	int fanlist_ptr = 0;

	while (loop_index < line_len) {
		if (token_length > 0 && (line[loop_index] == ' ' || line[loop_index] == '\n')){
			sprintf(token, "%.*s", token_length + 1, line + token_begin);

			switch (stage) {
				case BEGIN:
					stage = NAME;
					node_id = atoi(token);
					*node_id_ptr = node_id;
					if (node_id > *tot) *tot = node_id;
					InitiGat(Node, node_id);
					break;

				case NAME:
					strcpy(Node[node_id].Name, token);
					stage = TYPE;
					break;

				case TYPE:
					token_ptr = (char *) &token;
					gate_type = AssignType(TrimWhitespace(token_ptr));
					Node[node_id].Type = gate_type;
					if (gate_type == FROM) stage = FROMGAT;
					else stage = FANOUTS;
					break;

				case FROMGAT:
					Node[node_id].Nfi = 1;
					Node[node_id].Nfo = 1;
					
					int search_ptr = 1;
					token_ptr = (char *) &token;

					while (search_ptr <= *tot) {
						if (Node[search_ptr].Name && !strcmp(Node[search_ptr].Name, token_ptr)) {
							InsertEle(&Node[node_id].Fin, search_ptr);
							InsertEle(&Node[search_ptr].Fot, node_id);
							break;
						}
						search_ptr++;
					}
					
					stage = END;
					break;

				case FANOUTS:
					Node[node_id].Nfo = atoi(token);
					if (Node[node_id].Type == FROM) stage = END;
					else stage = FANINS;
					break;

				case FANINS:
					Node[node_id].Nfi = atoi(token);
					if (Node[node_id].Type >= BUFF) {
						stage = FANLIST;
						*stage_ptr = stage;
						return Node;
					}
					else stage = END;
					break;

				case FANLIST:
					fanlist_node = atoi(token);
					InsertEle(&Node[node_id].Fin, fanlist_node);
					InsertEle(&Node[fanlist_node].Fot, node_id);
					fanlist_length = Node[node_id].Nfi;
					fanlist_ptr++;
					if (fanlist_ptr == fanlist_length) {
						stage = END;
						*stage_ptr = stage;
					} else break;

				case END:
					stage = BEGIN;
					*stage_ptr = stage;
					return Node;
			}
		}

		if (line[loop_index] == ' '){
			token_begin = loop_index;
			token_length = 0;
		} else {
			token_length++;
		}

		loop_index++;
	}

	return Node;
}//end of LineToGate

/***************************************************************************************************
 Function to read the Bench Mark(.isc files)
***************************************************************************************************/
int ReadIsc(FILE *Isc,GATE *Node){
  //INSERT CODE HERE
  // read the isc file line by line
  char line[IscLineLen];
  int line_id=0;
  int tot=0;
  int stage=BEGIN;
  int node_id = 0;

  while (fgets(line, sizeof(line), Isc)) {
    Node = LineToGate(line, Node, &stage, &node_id, &tot);
  }
  printf("Total: %d\n", tot);

  //Return the Maximum node of the Isc file
  return tot;
}//end of ReadIsc 
/****************************************************************************************************************************/

