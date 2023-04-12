#include <stdio.h>
#include <string.h>
#include <iostream>
#include <malloc.h>
using namespace std;


char* MySwap(const char* text, const char symbol) {
	char* res = nullptr;
	size_t textLength;
	size_t firstSymbolIndex;
	size_t lastSymbolIndex;

	__asm {
		//-------------------------------------------------------------------------------
		mov eax, text
		push eax
		call strlen
		add esp, 4
		mov textLength, eax
		inc eax

		push eax
		call malloc
		add esp, 4
		mov res, eax
		//--------------------------------------------------------------------------------
		// ищет первое вхождение символа
		mov ecx, text

		Cycle :                   
		mov dl, [ecx]            
			mov byte ptr[eax], dl   
			cmp dl, 0                
			je EXIT                  
									 
			cmp dl, symbol           
			je RememberFirstSymbol   
			inc eax                  
			inc ecx                  
			jmp Cycle                

			RememberFirstSymbol :     
		sub ecx, text            
			mov firstSymbolIndex, ecx

		//------------------------------------------------------------------------------------------
		// делает то же, что и первый, только для последнего вхождения символа, так же попутно копирует строку, только с конца
			mov ecx, text
			add ecx, textLength
			dec ecx

			mov eax, res
			add eax, textLength
			dec eax

			Cycle2 :
		mov dl, [ecx]
			mov[eax], dl
			cmp ecx, text
			je ExCycle
			cmp dl, symbol
			je RememberLastSymbol
			dec eax
			dec ecx
			jmp Cycle2

			RememberLastSymbol :
		sub ecx, text
			mov lastSymbolIndex, ecx
			//--------------------------------------------------------------------------------------------
			ExCycle :

		mov eax, firstSymbolIndex 
			cmp eax, lastSymbolIndex  
			je EXIT                

			mov eax, res              
			add eax, firstSymbolIndex 

			mov ecx, text             
			add ecx, lastSymbolIndex  

			mov ebx, text             
			add ebx, firstSymbolIndex 

			Cycle3 :                   
		mov dh, [ecx]             
			mov[eax], dh             
			cmp ecx, ebx             
			je EXIT                  
			dec ecx                   
			inc eax                   
			jmp Cycle3                
			//---------------------------------------------------------------------------------------------
			EXIT :
		mov eax, res              
			mov ecx, textLength      
			mov[eax + ecx], 0        
	}
}
int main() {
	const char enterText[] = "Please enter text:\n";
	const char enterSymbol[] = "Please enter symbol:\n";
	const char outputResult[] = "Get the result:\n";
	const char repeatEnter[] = "\nWant to re-enter?\n"
		"(Enter)-repeat\n";
	const char scanfStr[] = "%[^\n]s";

	char inputText[100];
	char inputSymbol[2];
	char keepWorkingOrNot[1];
	__asm {
	REPEATE:
		lea eax, enterText           
			push eax                     
			call printf_s                
			add esp, 4                   

			mov eax, 100                 
			push    eax                  
			lea     eax, inputText       
			push	eax                  
			call    gets_s               
			add esp, 8                   

			lea eax, enterSymbol         
			push eax                     
			call printf_s                
			add esp, 4                   

			mov eax, 2                   
			push    eax                  
			lea     eax, inputSymbol     
			push	eax                  
			call    gets_s               
			add esp, 8                   

			lea eax, outputResult        
			push eax                     
			call printf_s                
			add esp, 4                   

			mov dl, byte ptr[inputSymbol]
			push edx                     
			lea eax, inputText           
			push eax                     
			call MySwap                  
			add esp, 8                   

			push eax                     
			call printf_s                
			add esp, 4                   

			lea eax, repeatEnter         
			push eax                     
			call printf_s                
			add esp, 4                   

			mov eax, 1                   
			push eax                     
			lea eax, keepWorkingOrNot    
			push eax                     
			call gets_s                 
			add esp, 8

			mov dh, [keepWorkingOrNot]  
			cmp dh, 0                   
			je REPEATE
			jne EXIT
			EXIT :
	}
}
