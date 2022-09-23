#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct node
{
    char label[30];
    int opaddress;
    int saddress;
    struct node *next;
}*start=NULL,*last=NULL,*temp,*prev,*newnode;
char label[30],opcode[30],operand[30],slabel[30],saddress[30],oopcode[10],ohex[10],opnd_add[30],obj_code[30];
int symtab_checker(FILE *fp,char *symbol);
int optab_checker(FILE *fp,char *opc);
void insert_link_list(struct node *new);
void display_link_list(struct node *st);
int retval=0;
int main()
{
    int start_add,locctr=0,inc=0,i,const_op;
    char locstring[30],record,pgmname[30],length[30],fullline;
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    fp1 = fopen("input.txt","r");
    fp2 = fopen("optab.txt","r");
    fp3 = fopen("symtab.txt","w+");
    fp5 = fopen("symtab1.txt","w+");
    fp4 = fopen("obj.txt","w");

    while(!feof(fp1))
    {
        fscanf(fp1,"%s %s %s",label,opcode,operand);
        printf("\n%s %s %s",label,opcode,operand);
        if(strcmp(opcode,"END")==0)
        {
            break;
        }
        if(strcmp(opcode,"START")==0)
        {
            start_add = atoi(operand);
            strcpy(pgmname,label);
            locctr = start_add;
            fprintf(fp4,"%c^%s^00%s^%s\n",'H',label,operand,"000000");
            fprintf(fp4,"%c^00%d",'T',locctr);
        }
        else
        {
            //step 1 if there is symbol in label field
            if(strcmp(label,"*")!=0)
            {
                retval = symtab_checker(fp3,label);
                if(retval==0)
                {
                    fprintf(fp3,"%s %d\n",label,locctr);
                }
                else
                {
                    if(strcmp(saddress,"*")==0)
                    {
                        //replace * with locctr
                        fseek(fp3,0,SEEK_END);
                        fprintf(fp3,"%s %d\n",label,locctr);
                        //solve the link list and create text records
                        temp = start;
                        while(temp!=NULL)
                        {
                            if(strcmp(temp->label,slabel)==0)
                            {
                                temp->saddress = locctr;
                            }
                            temp = temp->next;
                        }
                    }
                }
            }
            //step 2 check opcode and operand
            retval = optab_checker(fp2,opcode);
            if(retval==1)
            {
                retval = symtab_checker(fp3,operand);
                if(retval==1)
                {
                    if(strcmp(saddress,"*")!=0)
                    {
                        strcpy(opnd_add,saddress);//saving operand address
                    }
                    else//if label address is * in symtab
                    {
                        strcpy(opnd_add,"0000");
                        //add to link list the locctr+1
                        newnode = (struct node *)malloc(sizeof(struct node));
                        newnode->next = NULL;
                        strcpy(newnode->label,slabel);
                        newnode->opaddress=locctr+1;
                        newnode->saddress=0;
                        insert_link_list(newnode);
                    }
                }
                else//if symbol not found in symtab save 0000 as operand address and add to link list and insert (label,*) to symtab 
                {
                    strcpy(opnd_add,"0000");
                    fprintf(fp3,"%s %s\n",operand,"*");
                    newnode = (struct node *)malloc(sizeof(struct node));
                    newnode->next = NULL;
                    strcpy(newnode->label,operand);
                    newnode->opaddress=locctr+1;
                    insert_link_list(newnode);
                }
                //generate objcode
                strcpy(obj_code,ohex);
                strcat(obj_code,opnd_add);
                fprintf(fp4,"^%s",obj_code);
                locctr+=3;
            }
            else if(strcmp(opcode,"WORD")==0)
            {
                //convert constant to object code
                const_op = atoi(operand);
                printf("\nThe constant obj code is %02d",const_op);
                fprintf(fp4,"^0000%02d",const_op);
                locctr+=3;

            }
            else if(strcmp(opcode,"RESW")==0)
            {
                locctr+=3*atoi(operand);
            }
            else if(strcmp(opcode,"RESB")==0)
            {
                locctr+=atoi(operand);
            }
        }

    }
    //solving all forward references by creating text records for it
    temp = start;
    while(temp!=NULL)
    {
        fprintf(fp4,"\n");
        fprintf(fp4,"%c^%d^02^%d",'T',temp->opaddress,temp->saddress);
        temp = temp->next;
    }
    //end record insertion
    fprintf(fp4,"\n");
    fprintf(fp4,"%c^00%d",'E',start_add);
    fseek(fp4,0,SEEK_SET);
    fprintf(fp4,"%c^%s^%d^00%d  ",'H',pgmname,start_add,locctr);
    display_link_list(start);


    return 0;
}
void display_link_list(struct node *st)
{
    temp = start;
    while(temp!=NULL)
    {
        printf("\nlabel : %s\naddres to modify : %d",temp->label,temp->opaddress);
        temp = temp->next;
    }
}
void insert_link_list(struct node *new)
{
    if(start==NULL)
    {
        start = new;
        last = new;
    }
    else
    {
        last->next = new;
        last = last->next;
    }
}
int symtab_checker(FILE *fp,char *symbol)
{
    fseek(fp,0,SEEK_SET);
    char label[30],addr[30];
    int flag=0;
    while(!feof(fp))
    {
        fscanf(fp,"%s %s",label,addr);
        if(strcmp(label,symbol)==0)
        {
            flag=1;
            strcpy(slabel,label);
            strcpy(saddress,addr);
        }
    }
    if(flag==1)
    {
        return 1;
    }
    return 0;
}
int optab_checker(FILE *fp,char *opc)
{
    fseek(fp,0,SEEK_SET);
    char opcode[30],hexval[30];
    int flag=0;
    while(!feof(fp))
    {
        fscanf(fp,"%s %s",opcode,hexval);
        if(strcmp(opcode,opc)==0)
        {
            flag=1;
            strcpy(oopcode,opcode);
            strcpy(ohex,hexval);
            break;
        }
    }
    if(flag==1)
    {
        return 1;
    }
    return 0;
}