#include <stdio.h>
#include <math.h>
#include "cal.h"



int IsSolarLeakYear(int year)
{
 if(year<0)return -1;
 if(year%400==0)return 1;
 if(year%4==0&&year%100!=0)return 1;
 return 0;
}

int GetSolarMonthDays(int year,int month)
{
 if(IsSolarLeakYear(year)&&month==2) return 29;
 if(!IsSolarLeakYear(year)&&month==2) return 28;
 return ((month<=7&&month%2==1)
         ||(month>7&&month%2==0) )?31:30;
}

int GetSolarYearDays(int year)
{
 int i,days;
 days=0;
 for(i=1;i<=12;i++)
    days+=GetSolarMonthDays(year,i);
 return days;
 }
 
////////////////////////////////////
int GetLunarMonthDays(int year,int month,BOOL leak)
{
 unsigned int m,l,tmp;
 if(year<startYear||year>endYear)return -1;
 if(month<1||month>12)return -1;

 l=Lunar[year-startYear][0];
 m=Lunar[year-startYear][1];
 if(l)
 {
   if(month<(int)l ||
      (month==(int)l&&!leak)) tmp=month;
   else tmp=month+1;
 }  
 else tmp=month;

 //shift the m by tmp
 return ((m>>(16-tmp))&0x0001)?30:29;
}

int GetLunarYearMonthes(int year,int& leakm)
{
  
 leakm=Lunar[year-startYear][0];
 return(leakm?13:12);
}

int GetLunarYearDays(int year)
{
 int m,leakm,days=0;
 for(int i=1;i<=12;i++)days+=GetLunarMonthDays(year,i,FALSE);
 m=GetLunarYearMonthes(year,leakm);
 if(leakm) days+=GetLunarMonthDays(year,leakm,TRUE);
 return days;
}

///////////////////////////////////////
void  SolarToLunarRef(DATE s,DATE& l,BOOL& bLeak)
//only between 1899.1.1 to 2018.12,31
{
 
 if(s.year<startYear ||s.year>endYear) return;
 int i,j,y;
 long days;
 days=0;
 for(y=startYear;y<s.year;y++){
      days+=GetSolarYearDays(y);
    }  
 if(y==s.year){
    for(i=1;i<s.month;i++)
      days+=GetSolarMonthDays(y,i);
    days+=s.day;
 }

//solar1899.1.1   lunar1898.11,20
//solar1899.1.12  lunar1898.12.1
//solar1899.2.10  lunar1899.1.1
//
 int ly,lm,ld,tmp,leakmonth;BOOL b;
 ly=startYear-1;lm=11;ld=20-1;
 if(days<12) ld+=days;
 if(days>=12&&days<41){ lm=12;ld=days-11;}
 if(days>=41){
    ly=startYear;
    lm=1;ld=1;
    days-=41;

    j=0;b=FALSE;
    while(days>0){
       j=1;b=FALSE;
       tmp=GetLunarYearMonthes(ly,leakmonth);
       while(days>=GetLunarMonthDays(ly,j,b)
              && j<=12)
       {      
        
        days-=GetLunarMonthDays(ly,j,b);
        j++;
        if(j==(leakmonth+1)&&!b) {b=TRUE;j--;}
        else b=FALSE;
       }
       if(j==13){ly++; continue;}
       bLeak=b;//if true,stop at leak month
       lm=j;    
       ld+=days;break;
    }   
  }
  
  l.year=ly;l.month=lm;l.day=ld;

}




void SolarToLunar(DATE solar,DATE& l,BOOL& b)
{
  if(solar.year>=startYear&&solar.year<=endYear){
       SolarToLunarRef(solar,l,b);
       return;
  }     
  DATE s=solar;
  
  double  exactYear,exactMonth,MonthesEachYear;
  exactYear=365.24218968-0.0000000616*((s.year+refYear)/2-2000);
  exactMonth=29.53058885+0.0000000022*((s.year+refYear)/2-2000);
  MonthesEachYear=exactYear/exactMonth;

  double  monthes,days,temp;
  monthes=MonthesEachYear*(refYear-s.year);
  //By refYear,pass monthes
  monthes-=floor(monthes);
  //the little monthes
  days=monthes*exactMonth;
  //must distract days

  s.year=refYear;
  if(s.day<=days) {
      s.month--;
      if(s.month==0){s.year--;s.month=12;
                    }
      temp=s.day+GetSolarMonthDays(s.year,s.month)
               -days;
      
  }
  else{
      temp=s.day-days;
       }
  s.day=(temp-floor(temp)>=0.5)?(int)ceil(temp):
                              (int)floor(temp);
           
  //look up according to referenced year
  SolarToLunarRef( s,l,b);
  l.year=solar.year-(s.year-l.year);
  
  /* check the differ between solar and lunar over
    21--1,20days or not.
  */
  double tmpdays,std1,std2;
  std1=21;std2=20+1*exactMonth;
  tmpdays=((solar.month-l.month+12)%12)*exactMonth
           +(solar.day-l.day);
  if(tmpdays>std2){
      s.year+=s.month/12; 
      s.month++;
      if(s.month==13)s.month=1;
      //again look up refYear calender
      SolarToLunarRef(s,l,b);
      l.year=solar.year-(s.year-l.year);
  }
  if(tmpdays<std1){
      s.month--;
      if(s.month==0){s.month=12;s.year--;}
     //again look up refYear calender
      SolarToLunarRef(s,l,b);
      l.year=solar.year-(s.year-l.year);
   } 

}


void LunarToSolarRef(DATE dl,DATE& ds,BOOL bLeak)
{
 
 if(dl.year<startYear ||dl.year>endYear) return;
 int i,y;
 int leakmonth;
 long days;
 days=0;
 for(y=startYear;y<dl.year;y++){
      days+=GetLunarYearDays(y);
    }  

 
  for(i=1;i<dl.month;i++)
      days+=GetLunarMonthDays(y,i);
  days+=dl.day;

  i=GetLunarYearMonthes(dl.year,leakmonth);
  if(leakmonth){
    if(leakmonth<dl.month)
       days+=GetLunarMonthDays(y,leakmonth,TRUE);
    if(leakmonth==dl.month&&bLeak)
       days+=GetLunarMonthDays(y,leakmonth,FALSE);
  }

//solar1899.1.1   lunar1898.11,20
//solar1899.1.12  lunar1898.12.1
//solar1899.2.10  lunar1899.1.1
 int sy,sm,sd,tmp;
 sy=startYear;sm=2;sd=1;days+=10-2;
 
 tmp=sm;
 while(days>=GetSolarMonthDays(sy,tmp)){
     days-=GetSolarMonthDays(sy,tmp);tmp++;
     if(tmp==13){sy++;tmp=1;}
 }

 sm=tmp;sd+=days;
 ds.year=sy;ds.month=sm;ds.day=sd;
}

void LunarToSolar(DATE dl,DATE& ds,BOOL b)
{
  if(dl.year>=startYear&&dl.year<=endYear){
     LunarToSolarRef(dl,ds,b);
     return;
  }   
  DATE ld=dl;//BOOL b;
  
  double  exactYear,exactMonth,MonthesEachYear;
  exactYear=365.24218968-0.0000000616*((ld.year+refYear)/2-2000);
  exactMonth=29.53058885+0.0000000022*((ld.year+refYear)/2-2000);
  MonthesEachYear=exactYear/exactMonth;

  double  monthes,days,temp;
  monthes=MonthesEachYear*(refYear-ld.year);
  monthes-=floor(monthes);
  days=monthes*exactMonth;

  ld.year=refYear;
  //then add days to ld
  temp=ld.day+days; 
  if(temp>GetSolarMonthDays(ld.year,ld.month)){
     temp-=GetSolarMonthDays(ld.year,ld.month);
     ld.month++;}
  ld.day=(temp-floor(temp)>=0.5)?(int)ceil(temp):
                              (int)floor(temp);
   
           
  //look up according to referenced year
  LunarToSolarRef(ld,ds);
  ds.year=dl.year+(ds.year-ld.year);

  /* check the differ between solar and lunar over
    21--1,20days or not.
  */
  double tmpdays,std1,std2;
  std1=21;std2=20+1*exactMonth;
  tmpdays=((ds.month-dl.month+12)%12)*exactMonth
           +(ds.day-dl.day);
      
  if(tmpdays>std2){
      ld.month--;
      if(ld.month==0){ ld.month=12;ld.year--;}
      //again look up
      LunarToSolarRef(ds,ld,b);
      ds.year=dl.year+(ds.year-ld.year);
  }
  if(tmpdays<std1){
      ld.year+=ld.month/12; 
      ld.month++;
      if(ld.month==13)ld.month=1;
      //again look up
      LunarToSolarRef(ds,ld,b);
      ds.year=dl.year+(ds.year-ld.year);
   } 
  
}

void GetTraditionalTime(int t,char* str)
{
 if(t<0||t>24)return;
 int index=-1;
 switch(t){
 case 23:
 case 24:
 case 0 :index=0;break;
 case 1 :
 case 2 :index=1;break;
 case 3 :
 case 4 :index=2;break;
 case 5 :
 case 6 :index=3;break;
 case 7 :
 case 8 :index=4;break;
 case 9 :
 case 10:index=5;break;
 case 11:
 case 12:index=6;break;
 case 13:
 case 14:index=7;break;
 case 15:
 case 16:index=8;break;
 case 17:
 case 18:index=9;break;
 case 19:
 case 20:index=10;break;
 case 21:
 case 22:index=11;break;
 default     :break;
 }

 strcat(str,dizhi[index]);
 strcat(str,"Ê±");
 return;
}
 

void GetTraditionalInfo(DATE sod,char* str)
{
 //according to refYear
 //solar1995.1.1  
 //equals lunar1994.12,1 jiaxu,dingchou,renchen
 int yt,yd,mt,md,dt,dd;
 yt=0;yd=10;mt=3;md=1;dt=8-1;dd=4-1;

 //count years elapsed
 DATE  lud;BOOL b;
 SolarToLunar(sod,lud,b);
 yt=(yt+(lud.year-1994))%10;yt=(yt+10)%10;
 yd=(yd+(lud.year-1994))%12;yd=(yd+12)%12;

 //count monthes elapsed
 //leak month has no this
 int monthes,i,j;monthes=0;
 if(!b)/*not leak month*/
 {
  if(lud.year>=1994)
  {
   for(i=1994;i<lud.year;i++)
    monthes+=12;//GetLunarYearMonthes(i,tmp);
   monthes+=lud.month;
   monthes--;monthes--;
  }
  else{
    monthes+=12-lud.month;
    for(j=lud.year+1;j<=1994;j++)
      monthes+=12;//GetLunarYearMonthes(j,tmp);
    monthes=-monthes;   
  }
  mt=(mt+monthes)%10;mt=(mt+10)%10;md=(lud.month+1)%12;
 }
 
 //count days elapsed
 long days=0;
 if(sod.year>=refYear)
 {
  for( i=refYear;i<sod.year;i++)
    days+=GetSolarYearDays(i);
  for(i=1;i<sod.month;i++)
    days+=GetSolarMonthDays(sod.year,i);
  days+=sod.day;
 }
 else{
   days+=GetSolarMonthDays(sod.year,sod.month)-sod.day;
   for( j=sod.month+1;j<=12;j++)
      days+=GetSolarMonthDays(sod.year,j);
   for(j=sod.year+1;j<refYear;j++)
     days+=GetSolarYearDays(j);
   days=-days;  
 }

dt=(dt+days)%10;dd=(dd+days)%12;
dt=(dt+10)%10;dd=(dd+12)%12;

strcat(str,dizhiBanimal[yd]);strcat(str,"  ");
strcat(str,tiangan[yt]);strcat(str,dizhi[yd]);
if(b){strcat(str,"ÈòÔÂ");}//strcat(str,"NONO");}
else{strcat(str,tiangan[mt]);strcat(str,dizhi[md]);}
strcat(str,tiangan[dt]);strcat(str,dizhi[dd]);


}





 

