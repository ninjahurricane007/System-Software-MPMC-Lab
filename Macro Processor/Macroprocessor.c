#include<stdio.h>
#include<string.h>
char label[30],opcode[30],operand[30],arg_return[30];
int nametab_checker(FILE *fp,char *m_name);
void macro_expand(FILE *fop,FILE *farg,FILE *fd);
void macro_define(FILE *fn,FILE *fd,FILE *fin);
void argtab_finder(FILE *fp,int argpos);
int main()
{
    
    int retval;
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;

    fp1 = fopen("input.txt","r");
    fp2 = fopen("nametab.txt","w+");
    fp3 = fopen("deftab.txt","w+");
    fp4 = fopen("argtab.txt","w+");
    fp5 = fopen("op.txt","w");


    //scanning input file till end
    while(!feof(fp1))
    {
        fscanf(fp1,"%s %s %s",label,opcode,operand);
        if(strcmp(opcode,"END")==0)
        {
            break;
        }
        retval = nametab_checker(fp2,opcode);
        if(retval==1)//opcode in nametab. ie it is macro call
        {
            macro_expand(fp5,fp4,fp3);
        }
        else if(strcmp(opcode,"MACRO")==0)
        {
            macro_define(fp2,fp3,fp1);
        }
        else//normal statement from source program.so just add to expanded file fp5
        {
            fprintf(fp5,"%s %s %s\n",label,opcode,operand);
        }
    }
    fprintf(fp5,"%s %s %s",label,opcode,operand);//adding end statement to expanded file

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);

    return 0;
}
void macro_expand(FILE *fop,FILE *farg,FILE *fd)
{
    //closing and reopening argtab so that its contents are cleared for every time we process a different macro call statement
    fclose(farg);
    FILE *farg1;
    farg1 = fopen("argtab.txt","w+");
    int i,len,arg_num,retval;
    len = strlen(operand);
    char dlabel[30],dopcode[30],doperand[30];
    fseek(fd,0,SEEK_SET);
    
    //getting macro prototype statement from deftab in case of multiple macro definitions in deftab.so we need to get to the macro definition of that particular macro call
    while(!feof(fd))
    {
        fscanf(fd,"%s %s %s",dlabel,dopcode,doperand);
        if(strcmp(dlabel,opcode)==0)
        {
            printf("\nThe line is %s %s %s",dlabel,dopcode,doperand);
            break;
        }
    }

    //storing the arguments form macro call to the argtab
    for(i=0;i<len;i++)
    {
        if(operand[i]==',')//macro call like ( macro_name para1,para2..)so we need to put para1 and para2 to argtab
        {
            fprintf(farg1,"\n");
        }
        else
        {
            fprintf(farg1,"%c",operand[i]);
        }
    }

    //putting macro call statement as comment in expnaded file
    strcpy(label,".");
    fprintf(fop,"%s %s %s\n",label,opcode,operand);

    //macro expansion

    while(!feof(fd))//taking each statement from deftab
    {
        fscanf(fd,"%s %s %s",dlabel,dopcode,doperand);
        if(strcmp(dopcode,"MEND")==0)
        {
            break;
        }
        if(doperand[0]=='?')//positional notation checking
        {
            arg_num = doperand[1]-'0';
            argtab_finder(farg1,arg_num); //function to reterive positional arguments from argtab from the positional values
            strcpy(doperand,arg_return);
            fprintf(fop,"%s %s %s\n",dlabel,dopcode,doperand);
        }
    }
    fclose(farg1);
}
void argtab_finder(FILE *fp,int argpos)
{
    int flag=0,i=1;
    char arg_name[30];
    fseek(fp,0,SEEK_SET);
    while(!feof(fp))
    {   
        fscanf(fp,"%s",arg_name);
        if(i==argpos)
        {
            break;
        }  
        i++;
    }
    strcpy(arg_return,arg_name);
}
void macro_define(FILE *fn,FILE *fd,FILE *fin)
{
    char stringpos[10];
    int pos=1;
    fprintf(fn,"%s\n",label);
    fprintf(fd,"%s %s %s\n",label,opcode,operand);
    while(strcmp(opcode,"MEND")!=0)
    {
        fscanf(fin,"%s%s%s",label,opcode,operand);
        if(strcmp(opcode,"MEND")==0)
        {
            break;
        }
        if(operand[0]=='&')
        {
            stringpos[0] = '?';
            stringpos[1] = pos+'0';
            stringpos[2] = '\0';
            pos++;
        }
        fprintf(fd,"%s %s %s\n",label,opcode,stringpos);
    }
    fprintf(fd,"%s %s %s\n",label,opcode,operand);
}
int nametab_checker(FILE *fp,char *m_name)
{
    fseek(fp,0,SEEK_SET);
    int flag=0;
    char name[30];
    while(!feof(fp))
    {
        fscanf(fp,"%s",name);
        if(strcmp(name,m_name)==0)
        {
            flag=1;
            break;
        }
    }
    if(flag==1)
    {
        return 1;
    }
    return 0;
}