#include "tm4c123gh6pm.h"

#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
#define TRAFFIC_LIGHTS			(*((volatile unsigned long *)0x400050FC))
#define PEDESTRIAN_LIGHTS		(*((volatile unsigned long *)0x40025028))
#define SENSORS							(*((volatile unsigned long *)0x4002401C))
 

void ports_Init(void){
  unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x32; 
  delay = SYSCTL_RCGC2_R; 
	// Port B
  GPIO_PORTB_LOCK_R = 0x4C4F434B; 
  GPIO_PORTB_CR_R = 0x3F;          
	GPIO_PORTB_PCTL_R = 0x00000000; 
  GPIO_PORTB_AMSEL_R &= ~0x3F;    
  GPIO_PORTB_AFSEL_R &= ~0x3F;   
  GPIO_PORTB_DEN_R |= 0x3F;      
	GPIO_PORTB_DIR_R |= 0x3F;    
	// Port E
  GPIO_PORTE_LOCK_R = 0x4C4F434B;  
  GPIO_PORTE_CR_R = 0x07;           
	GPIO_PORTE_PCTL_R = 0x00000000;  
  GPIO_PORTE_AMSEL_R &= ~0x07;     
  GPIO_PORTE_AFSEL_R &= ~0x07;   
  GPIO_PORTE_PUR_R &= ~0x07;      
  GPIO_PORTE_DEN_R |= 0x07;        
	GPIO_PORTE_DIR_R &= ~0x07;    
	// Port F
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   
  GPIO_PORTF_CR_R = 0x0A;          
	GPIO_PORTF_PCTL_R = 0x00000000;   
  GPIO_PORTF_AMSEL_R &= ~0x0A;      
  GPIO_PORTF_AFSEL_R &= ~0x0A;     
  GPIO_PORTF_DEN_R |= 0x0A;        
	GPIO_PORTF_DIR_R |= 0x0A;      
}

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               
  NVIC_ST_CTRL_R = 0x00000005;     
}

void SysTick_Wait10ms(){
	
  NVIC_ST_RELOAD_R = 8000000 - 1; 
  NVIC_ST_CURRENT_R = 0;      
  while((NVIC_ST_CTRL_R&0x00010000)==0){ 
  }
}
void SysTick_Wait(unsigned long delay){
	unsigned long i;
	for(i=0; i < delay; i++)
		SysTick_Wait10ms();
}

typedef struct Stype{

	unsigned long TrafficOut;
	unsigned long WalkOut;
	unsigned long Time; 
	unsigned long Next[8];
} SType;

int main(void){ 
	unsigned long S = 0; 

		SType FSM[11]={
		{0x0C,0x02,5,{0,0,1,1,1,1,1,1}},
		{0x14,0x02,7,{1,0,2,2,4,4,2,2}},
		{0x21,0x02,5,{2,3,2,3,3,3,3,3}},
		{0x22,0x02,7,{3,0,2,0,4,0,4,4}},
		{0x24,0x08,5,{4,5,5,5,4,5,5,5}},
		{0x24,0x00,1,{4,6,6,6,4,6,6,6}},
		{0x24,0x02,1,{4,7,7,7,4,7,7,7}},
		{0x24,0x00,1,{4,8,8,8,4,8,8,8}},
		{0x24,0x02,1,{4,9,9,9,4,9,9,9}},
		{0x24,0x00,1,{4,10,10,10,4,10,10,10}},
		{0x24,0x02,1,{5,0,2,0,4,0,2,0}}
	};
 
	ports_Init(); 
	SysTick_Init(); 

  while(1){
    TRAFFIC_LIGHTS = FSM[S].TrafficOut; 
		PEDESTRIAN_LIGHTS = FSM[S].WalkOut;
    SysTick_Wait(FSM[S].Time);
    S = FSM[S].Next[SENSORS];
  }
}

