#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "queue.h"

typedef struct{
	int tiempo_llegada;
	int prioridad;
	int tiempo_del_procesador;
	int impresora;
	int scanners;
	int modems;
	int dvd;
}PCB;




int main(int argc, char* argv[]){

	FILE *lista;
	int length,i,b;
	char a[21];
	Queue real_time,priority_1,priority_2,priority_3;
	lista = fopen(argv[1],"r");

	queueInit(&real_time, sizeof(PCB));
	queueInit(&priority_1, sizeof(PCB));
	queueInit(&priority_2, sizeof(PCB));
	queueInit(&priority_3, sizeof(PCB));
	
	while(!feof(lista)){
		fgets(a,21,lista);
		printf("%c, %c, %c, %c, %c, %c, %c\n",a[0],a[3],a[6],a[9],a[12],a[15],a[18]);
		PCB *proceso = malloc(sizeof(PCB));
		proceso->tiempo_llegada = atoi(&a[0]);
		proceso->prioridad = atoi(&a[3]);
		proceso->tiempo_del_procesador = atoi(&a[6]); 
		proceso->impresora = atoi(&a[9]); 
		proceso->scanners = atoi(&a[12]); 
		proceso->modems = atoi(&a[15]); 
		proceso->dvd = atoi(&a[18]); 

		if(proceso->prioridad == 0){
			enqueue(&real_time,&proceso);
			printf("Enchufado\n");
		}else if(proceso->prioridad == 1){
			enqueue(&priority_1,&proceso);
			printf("Clase Alta\n");
		}else if(proceso->prioridad == 2){
			enqueue(&priority_2,&proceso);
			printf("Clase Media\n");
		}else if(proceso->prioridad == 3){
			enqueue(&priority_3,&proceso);
			printf("Clase baja\n");
		}
	}




	fclose(lista);
	return 0;
}