#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_opt_value(char *opcode)
{
	char value[20], label[20];	
	FILE *file = fopen("optab.txt", "r");
	
	// skipping first line
	fscanf(file, "%s\t%s\n", label, value);

	fscanf(file, "%s\t%s\n", label, value);
	while (strcmp(label, "END") != 0)
	{
		if (strcmp(label, opcode) == 0) {
			fclose(file);			
			return atoi(value);
		}
		fscanf(file, "%s\t%s\n", label, value);
	} 

	fclose(file);
	
	return 0;
}

int get_symt_value(char *symbol)
{
	char value[20], label[20];	
	FILE *file = fopen("symtbl.txt", "r");

	while (!feof(file))
	{
		fscanf(file, "%s\t%s\n", label, value);
		if (strcmp(label, symbol) == 0) {
			fclose(file);			
			return atoi(value);
		}
	} 

	fclose(file);
	
	return 0;
}

int main(void)
{
	char locctr[20], label[20], opcode[20], operand[20];
	int opt_value = 69, symt_value = 69;

	FILE *intermediate_file = fopen("out.txt", "r");
	FILE *twoout = fopen("twoout.txt", "w");
	
	// Reading first line
    fscanf(intermediate_file, "\t%s\t%s\t%s\n", 
		label, 
		opcode, 
		locctr);
	fprintf(twoout, "%s\t%s\t%s\n", 
		label, 
		opcode, 
		locctr);

	
	while (!feof(intermediate_file))
	{		
		fscanf(intermediate_file, "%s\t%s\t%s\t%s\n", 
			locctr, 
			label, 
			opcode,
			operand);
		
		opt_value = get_opt_value(opcode);
		symt_value = get_symt_value(operand);

		if (symt_value == 0) symt_value = atoi(operand);

		fprintf(twoout, "%s\t%s\t%s\t%s\t%.02d%.04d\n", 
			locctr, 
			label, 
			opcode,
			operand, 		
			opt_value, symt_value);
		
		symt_value = 0;
	}	

	fclose(intermediate_file);
	fclose(twoout);
	
	return 0;
}
