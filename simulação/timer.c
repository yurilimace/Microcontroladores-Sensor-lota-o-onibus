/* 
 * File:   timer.c
 * Author: yuri
 *
 * Created on 21 de Setembro de 2016, 13:42
 */

#include <p18f4550.h>
#include<timers.h>
#include<delays.h>

//defines do sensor
#define Trigger PORTCbits.RC0
#define Echo PORTCbits.RC1

//defines do lcd
#define ID PORTCbits.RC6
#define Enable PORTCbits.RC7


char i=0;
int Vetor[8];

// delay de 10 microssegundos para o pino trigger 
void Delay_10us()
{
    _asm
    nop
    nop
    nop
    nop
    nop
    nop
    _endasm

}


// delay para recepção de instrução ou dado no LCD
void delay_1ms(void)
{
  Delay100TCYx(9);
  Delay10TCYx(9);
  Delay1TCY();
  Delay1TCY();
  Delay1TCY();
  Delay1TCY();
}








void delay()
{
    int i = 0;
    while(i<100)
    {
        i++;
    }

}


 // Funções do LCD

//Envia o Dado para o LCD de Forma paralela
void Envia(int num)
{
    int p,n;
    n=num;
    for(p=0;p<8;p++)
    {
        Vetor[p]=n%2;
        n=n/2;
    }
    PORTDbits.RD0=Vetor[0];
    PORTDbits.RD1=Vetor[1];
    PORTDbits.RD2=Vetor[2];
    PORTDbits.RD3=Vetor[3];
    PORTDbits.RD4=Vetor[4];
    PORTDbits.RD5=Vetor[5];
    PORTDbits.RD6=Vetor[6];
    PORTDbits.RD7=Vetor[7];
}

// Chegada do dado no LCD na subida do clock
void Recebe(void)
{
    delay_1ms();
    Enable = 0;
    delay_1ms();
    Enable = 1;

}

//configurações iniciais do LCD
void Configure_LCD(void)
{
    ID = 0;
    Envia(56);
    Recebe();
    Envia(56);
    Recebe();
    Envia(6);
    Recebe();
    Envia(14);
    Recebe();
    Envia(1);
    Recebe();
}

//Coloca o cursor na segunda linha do LCD
void Quebra_Linha(void)
{
    ID = 0 ;
    Envia(0xC0);
    Recebe();
    ID = 1;
}

//Desloca o Cursor Para Direita
void Desloca_Direita(void)
{
    ID = 0 ;
    Envia(0x14);
    Recebe();
    ID = 1;

}

// Organiza as palavras no lcd
void Alinhar()
{
        Quebra_Linha();
        Desloca_Direita();
        Desloca_Direita();
        Desloca_Direita();
        Desloca_Direita();
        Desloca_Direita();
        Desloca_Direita();
        Desloca_Direita();
}

//escreve a palavra distância no LCD
void Setar_distancia()
{
    ID = 1 ;
    Envia(68);
    Recebe();
    Envia(105);
    Recebe();
    Envia(115);
    Recebe();
    Envia(116);
    Recebe();
    Envia(97);
    Recebe();
    Envia(110);
    Recebe();
    Envia(99);
    Recebe();
    Envia(105);
    Recebe();
    Envia(97);
    Recebe();
    Envia(58);
    Recebe();
}


// Escreve a virgula no lcd
void Set_Ponto()
{
    Envia(44);
    Recebe();
}

//Escreve cm no LCD
void Set_cm()
{
     Envia(32);
     Recebe();
     Envia(99);
     Recebe();
     Envia(109);
     Recebe();
}



// Função de Enviar a distância marcada ao lcd
void Send_Data(long long int distancia)
{
    int aux,centena,unidade,dezena,Casa1,Casa2;
    //caso seja maior < 100
    if(distancia >= 10000)
    {
        centena = distancia/10000; //quebra o numero obtendo o algarismo da centena
        aux = distancia%10000;   //divide o numero para que possa obter o algarismo da dezena
        
        dezena = aux / 1000 ;  //divide o numero obtendo a dezena
        aux = aux % 1000;     //divide o numero para que possa obter o algarismo da unidade
        
        unidade = aux/100 ;    //divide o numero obtendo a unidade
        aux = aux % 100;       //divide o numero para que possa obter a primeira casa decimal
        
        Casa1 = aux / 10;    //divide e se obtem a primeira casa decimal
        Casa2 = aux % 10;    //divide e se obtem a segunda casa decimal
        
        centena = centena +48; //Transformação dos algarismos em ascii  
        dezena = dezena + 48 ;
        unidade = unidade + 48;
        Casa1 = Casa1 + 48;
        Casa2 = Casa2 + 48;
        
        Alinhar(); //Envio ao LCD
        Envia(centena);
        Recebe();
        Envia(dezena);
        Recebe();
        Envia(unidade);
        Recebe();
        Set_Ponto();
        Envia(Casa1);
        Recebe();
        Envia(Casa2);
        Recebe();
        Set_cm();
    }
    
    //caso seja < 100
    else
    {
        dezena=distancia/1000;   //mesmo procidemento da quebra do numero feito acima
        aux = distancia % 1000; //so que sem o algarismo da centena
        unidade=aux / 100;
        aux = aux % 100;
        Casa1 = aux / 10;
        Casa2 = aux % 10 ;
        
        dezena = dezena + 48;    //transformação em ascii
        unidade = unidade + 48;
        Casa1 = Casa1 + 48;
        Casa2 = Casa2 + 48;
        
        Alinhar(); //envio ao lcd
        Envia(dezena);
        Recebe();
        Envia(unidade);
        Recebe();
        Set_Ponto();
        Envia(Casa1);
        Recebe();
        Envia(Casa2);
        Recebe();
        Set_cm();
    }
    

}

// Limpa o lcd
void Clean()
{
     ID = 0;
     Envia(1);
     Recebe();
}

//Delay para reiniciar todo o processo
void Atualiza_LCD()
{
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);
    Delay10KTCYx(9);

}



void Config_PIC()
{
  ADCON1=0b00001111;
  TRISC=0b00111110;
  TRISA=0b11111110;
  TRISD=0b00000000;
}

void main()
{
    long long int  Tempo;
    long long int  distancia;
    //Inicialização das configurações do PIC e LCD
    Config_PIC();
    Configure_LCD();
     
    Trigger = 0;
    delay();
    while(1)
    {
        
        //codigo sensor
       //Inicialização do Sensor 
        Trigger = 1 ;
        Delay_10us();
        Trigger = 0;
        OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1); //Inicialização do Timer
        while(!Echo){};
        WriteTimer0(0);
        while(Echo){};
        Tempo = ReadTimer0(); //Recebendo o tempo em que o pino echo ficou em nivel alto
        distancia= (Tempo*100)/58; //calculando a distancia formula dada no datasheet do sensor multiplica por 100 para se obter duas casas decimais
        Clean();
        Setar_distancia();
        Send_Data(distancia);
        Atualiza_LCD();
        
        
        
    }
}



