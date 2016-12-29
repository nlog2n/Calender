#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <string.h>
#include "cal.h"




void main(int argc,char* args[])
{
   
    char curday[10],curtime[10];int y,m,d;
    char trastr[128]="";
    
	BOOL b=FALSE;
	DATE datesolar,datelunar;

    if(argc==1/*no param*/){
    
      /* Display current date and time. */
      _strdate(curday);
      m=atoi(curday);
      d=atoi((curday+3));
      y=atoi((curday+6));
      if(y<50)y+=2000;
      else y+=1900;
      _strtime(curtime);
      printf( "现在日期:%d,%d,%d %s\n",y,m,d,curtime);

       //convert
      datesolar.year=y;datesolar.month=m;datesolar.day=d;
      SolarToLunar(datesolar,datelunar,b);
      if(b)
         printf( "    阴历:%d,%d,%d",//[闰月]",
              datelunar.year,datelunar.month,datelunar.day);
      else
         printf( "    阴历:%d,%d,%d",
              datelunar.year,datelunar.month,datelunar.day);
      GetTraditionalInfo(datesolar,trastr);
      printf("%s",trastr);
      y=atoi(curtime);
      trastr[0]='\0';trastr[1]='\0';
      GetTraditionalTime(y,trastr);
      printf("%s",trastr);

      return;

    }      

    if(argc==4){/*solar to lunar*/
       y=atoi(args[1]);m=atoi(args[2]);d=atoi(args[3]);
       if( y<=0||m<=0||d<=0 )//fail param
         {
          printf("illegal usage! please type <calender /?>to look help");   
          return;
         }
       int cy;
       if(m>12){printf("illegal month!");return;}
       if( y<100){
           printf("你是指%d年还是%d年?[%d]:",y,y+1900,y);
           scanf("%d",&cy);
           if(cy==(y+1900)) y+=1900;
       }           
       if(d>GetSolarMonthDays(y,m)){
          printf("illegal day!");return;
       }          
       printf(    "阳历:%d,%d,%d\n",y,m,d);
 

        //convert
        datesolar.year=y;datesolar.month=m;datesolar.day=d;
        SolarToLunar(datesolar,datelunar,b);
        if(b)
           printf( "阴历:%d,%d,%d", //[闰月]",
              datelunar.year,datelunar.month,datelunar.day);
        else
           printf( "阴历:%d,%d,%d",
              datelunar.year,datelunar.month,datelunar.day);

        GetTraditionalInfo(datesolar,trastr);
        printf("%s",trastr);
        return;
         
     }     

    if(argc==5){
       if( !(strcmp(args[1],"-s")==0
             ||strcmp(args[1],"-S")==0))
       {
          printf("illegal usage! please type <calender /?>to look help");   
          return;
       }

       y=atoi(args[2]);m=atoi(args[3]);d=atoi(args[4]);
       if( y<=0||m<=0||d<=0 )//fail param
         {printf("illegal paramter!\n");  return;}
       int cy;
       if(m>12){printf("illegal month!");return;}
       if( y<100){
           printf("你是指%d年还是%d年?[%d]:",y,y+1900,y);
           scanf("%d",&cy);
           if(cy==(y+1900)) y+=1900;
       }           
       if(d>GetLunarMonthDays(y,m)){
          printf("illegal day!");return;
       }       

        datelunar.year=y;datelunar.month=m;datelunar.day=d;
          //ask if it is leak month to decide
        if(y>=startYear&&y<=endYear){
          int mes,leakmonth;
          mes=GetLunarYearMonthes(y,leakmonth);        
          if(leakmonth==m){
            printf("该月要取闰月吗?[Y or N]:");
            char ch;
            scanf("%c",&ch);
            if(ch=='Y'||ch=='y')b=TRUE;
            //default N
          }
        }  

        if(b)printf("阴历:%d,%d,%d",y,m,d);//leak month
        else printf("阴历:%d,%d,%d",y,m,d);

       
        //convert
        LunarToSolar(datelunar,datesolar,b);
        GetTraditionalInfo(datesolar,trastr);
        printf("%s\n",trastr);
        printf( "阳历:%d,%d,%d",
              datesolar.year,datesolar.month,datesolar.day);

        return;
         
     } 
       
              
    if(strcmp(args[1],"/?")==0){
      printf("Copyright: 方晖<quasi@263.net>  1998.10.5");
      printf("\nUsage:   calender                      ;display current date");
      printf("\n         calender year month day       ;convert to lunar date");
      printf("\n         calender -s year month day    ;convert lunar to solar date");
      printf("\n         calender /?                   ;display help information");
      printf("\nSample:  calender 1997 12 23<enter>");
      return;
    }      
   printf("illegal usage! please type <calender /?>to look help");   
   
}

	

