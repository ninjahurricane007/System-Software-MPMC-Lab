#include<stdio.h>
void main()
{
    char record,pname[10];
    int paddr,plen,taddr,tlen,ocode,i;
    FILE *fp1;
    FILE *fp2;
    fp1=fopen("input.txt","r");
    fp2=fopen("output.txt","w");
    while(!feof(fp1))
    {
        fscanf(fp1,"%c",&record);
        if(record=='H')
        {
            fscanf(fp1,"%s%x%x",pname,&paddr,&plen);
        }
        else if(record=='T')
        {
            fscanf(fp1,"%x%x",&taddr,&tlen);
            for(i=0;i<tlen;i++)
            {
                fscanf(fp1,"%2x",&ocode);
                fprintf(fp2,"%x:%02x\n",taddr,ocode);
                taddr++;
            }
        }
        else if(record=='E')
        {
            break;
        }
    }
    fclose(fp1);
    fclose(fp2);
}
