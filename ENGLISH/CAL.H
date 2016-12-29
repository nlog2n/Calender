#include <string.h>
#ifndef  BOOL
#define  BOOL int
#define  FALSE 0
#define  TRUE  1
#endif

#define  startYear  1899
#define  endYear    2018
#define  refYear    1995

struct DATE{
          int year,month,day;
           };

        
static unsigned int  Lunar[][2]={
                      //from 1899 year
                                 {0,0xab50},{8,0x4b68},
//1901                      
{0,0x4ae0},{0,0xa570},{5,0x52d0},{0,0xd260},{0,0xd950},
{4,0x6aa8},{0,0x56a0},{0,0x9ad0},{2,0x4ae8},{0,0x4ae0},

//1911
{6,0xa4d8},{0,0xa2d0},{0,0xd250},{5,0xd528},{0,0xb540},
{0,0xd650},{2,0x96d0},{0,0x95b0},{7,0x49b8},{0,0x4970},

//1921
{0,0xa4b0},{5,0xb258},{0,0x6a50},{0,0x6d40},{4,0xada8},
{0,0x2b60},{0,0x9570},{2,0x4978},{0,0x4970},{6,0x64b0},

//1931
{0,0xd4a0},{0,0xea50},{5,0x6d48},{0,0x5ad0},{0,0x2b60},
{3,0x9370},{0,0x92e0},{7,0xc968},{0,0xc950},{0,0xd4a0},

//1941
{6,0xda50},{0,0xb550},{0,0x56a0},{4,0xaad8},{0,0x25d0},
{0,0x92d0},{2,0xc958},{0,0xa950},{7,0xb4a8},{0,0x6ca0},

//1951
{0,0xb550},{5,0x55a8},{0,0x4da0},{0,0xa5b0},{3,0x52b8},
{0,0x52b0},{8,0xa950},{0,0xe950},{0,0x6aa0},{6,0xad50},

//1961
{0,0xab50},{0,0x4b60},{4,0xa570},{0,0xa570},{0,0x5260},
{3,0xe930},{0,0xd950},{7,0x5aa8},{0,0x56a0},{0,0x96d0},

//1971
{5,0x4ae8},{0,0x4ad0},{0,0xa4d0},{4,0xd238},{0,0xd250},
{8,0xd528},{0,0xb540},{0,0xb6a0},{6,0x96d0},{0,0x95b0},

//1981
{0,0x49b0},{4,0xa4b8},{0,0xa4b0},{10,0xb258},{0,0x6a50},
{0,0x6d40},{6,0xada0},{0,0xab60},{0,0x9570},{5,0x4978},

//1991
{0,0x4970},{0,0x64b0},{3,0x6a50},{0,0xea50},{8,0x6b28},
{0,0x5ac0},{0,0xab60},{5,0x9368},{0,0x92e0},{0,0x6960},

//2001
{4,0xd4a8},{0,0xd4a0},{0,0xda50},{2,0x5aa8},{0,0x56a0},
{7,0xaad8},{0,0x25d0},{0,0x92d0},{5,0x6958},{0,0xa950},

//2011
{0,0xb4a0},{4,0xb550},{0,0xad50},{9,0x55a8},{0,0x4ba0},
{0,0xa5b0},{6,0x52b8},{0,0x52b0}/*end 2018 year*/

};


static char *tiangan[10]={"Jia","Yi","Bing","Ding","Wu",
                     "Ji","Geng","Xin","Ren","Gui"},
      *dizhi[12]={"Zi","Chou","Yin","Mou","Chen","Si",
                   "Wu","Wei","Shen","You","Xu","Hai"},
      *dizhiBanimal[12]={"Mouse","Ox","Tiger","Rabbit","Dragon","Snake",
                   "Pony","Lamb","Monkey","Cock","Dog","Pig"};





int IsSolarLeakYear(int year);
int GetSolarMonthDays(int year,int month);
int GetSolarYearDays(int year);

int GetLunarMonthDays(int year,int month,BOOL leak=FALSE);
int GetLunarYearMonthes(int year,int& leakm);
int GetLunarYearDays(int year);

void  SolarToLunarRef(DATE s,DATE& l,BOOL& bLeak);
//only between 1899.1.1 to 2018.12,31
void SolarToLunar(DATE date,DATE& date4lunar,BOOL& b);

void LunarToSolarRef(DATE dl,DATE& ds,BOOL bLeak=FALSE);
void LunarToSolar(DATE dl,DATE& ds,BOOL b=FALSE);


void GetTraditionalTime(int t,char* str);
void GetTraditionalInfo(DATE sod,char* str);
