#include "daviGPIO.h"
//CONFIGURAR UN PIN
void pinMode(int pin, int mode ){
	if (pin > 19||pin<0) return;
	
	if(pin>=0&&pin<=7){
		if(mode==OUTPUT){
			DDRD |=(1<<pin);
			}else{
			DDRD &=~(1<<pin);
		}
		}else if(pin>=8&&pin<=13){
		uint8_t bit = pin - 8;
		if(mode==OUTPUT){
			DDRB |=(1<<bit);
			}else{
			DDRB  &=~(1<<bit);
		}
		}else{
		uint8_t bit = pin - 14;
		if(mode==OUTPUT){
			
			DDRC |=(1<<bit);
			}else{
			DDRC &=~(1<<bit);
		}
	}

	
	
	
}

//CONFIGURAR UN PUERTO COMO ENTRADA O SALIDA
void portMode(char port, int mode){
	uint8_t val = (mode == OUTPUT) ? 0b11111111 : 0b00000000;//asignamos el valor que tendran los bits del puerto a una
	//variable auxiliar que sirve para asignar el modo
	if(port=='B'){//asigna el valor de entrada o salida al puerto correspondiente
		DDRB=val;
		}else if(port=='C'){
		DDRC=val;
		}else{
		DDRD=val;
	}
}

//LEER EL ESTADO DE UN PIN
uint8_t digitalRead(int pin){
	if (pin >= 19||pin<0) return LOW;
	if(pin<=7){
		return (PIND&(1<<pin))? HIGH: LOW;
		}else if(pin>=8&&pin<=13){
		uint8_t bit=pin-8;
		return (PINB&(1<<bit))? HIGH: LOW;
		}else{
		uint8_t bit=pin-14;
		return (PINC&(1<<bit))? HIGH: LOW;
	}
	
	return LOW;
}
//ESCRIBIR EL ESTADO DE UN PIN
void digitalWrite(int pin, int state){
	if (pin > 19||pin<0) return;
	if(pin<=7){
		if(state== HIGH){
			PORTD|=(1<<pin);
			}else{
			PORTD&=~(1<<pin);
		}
		}else if(pin>=8&&pin<=13){
		uint8_t bit=pin-8;
		if(state== HIGH){
			PORTB|=(1<<bit);
			}else{
			PORTB&=~(1<<bit);
		}
		}else{
		uint8_t bit=pin-14;
		if(state== HIGH){
			PORTC|=(1<<bit);
			}else{
			PORTC&=~(1<<bit);
		}
	}
	
}
//LEER EL ESTADO DE UN PUERTO
uint8_t portRead(char port){
	if (port !='B' && port !='C' && port!='D') return 0x00;
	
	switch(port){
		case 'C':
		return PINC;
		break;
		case 'B':
		return PINB;
		break;
		case 'D':
		return PIND;
		break;
	}
	
	return 0x00;
}


//ESCRIBIR EL ESTADO DE UN PUERTO
void portWrite(char port, int state){
	uint8_t val = (state==HIGH)?0xff:0x00;
	if (port == 'B') PORTB = val;
	else if (port == 'C') PORTC = val;
	else if (port == 'D') PORTD = val;
}