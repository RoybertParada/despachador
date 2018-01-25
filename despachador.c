#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "queue.h"

pid_t p_id;
char *cad[2];       //cadena para el exec()

typedef struct{
	int tiempo_llegada;
	int prioridad;
	char tiempo_del_procesador;
	int impresora;
	int scanners;
	int modems;
	int dvd;
}PCB;


typedef struct{
	int impresora;
	int scanner;
	int modem;
	int DVD;
}RECURSO;


int main(int argc, char* argv[]){

	cad[0] = "./child";

	FILE *lista;                                      // Archivo .in
	int length,i,b;
	char a[21];
	
	lista = fopen(argv[1],"r");

	//---Recursos Clave---
		RECURSO *Recursos =  malloc(sizeof(RECURSO));
		Recursos->impresora = 2;
		Recursos->scanner = 1;
		Recursos->modem = 1;       
		Recursos->DVD = 2;
	//---Recursos Clave---

	Queue real_time,priority_1,priority_2,priority_3; // Las Colas
	//Se inicializan las colas -------------
		queueInit(&real_time, sizeof(PCB));
		queueInit(&priority_1, sizeof(PCB));
		queueInit(&priority_2, sizeof(PCB));
		queueInit(&priority_3, sizeof(PCB));
	//--------------------------------------



//---------------------Se leen los procesos linea por linea---------------------------
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
//------------------------------------------------------------------------------------
while((getQueueSize(&priority_3)>0) || (getQueueSize(&priority_2)>0) || (getQueueSize(&priority_1)>0) || (getQueueSize(&real_time)>0)){         // Mientras allan procesos en alguna de las colas se ejecutara
	if(getQueueSize(&real_time)>0){			// Si la cola de tiempo real esta llena se ejecuta primero siempre
		/*SE HACE PROCESO DE FORK Y VAINA LOCA*/
		dequeue(&real_time);				// Luego ejecutado se saca de la cola
		printf("Proceso Real\n");
		continue;							// Se continua en el inicio del ciclo por que tiene que evaluar que no hayan mas en tiempo real
	}else if(getQueueSize(&priority_1)>0){  
		dequeue(&priority_1);
		printf("Prioridad 1\n");			// Misma Logica para todas las demas colas
		continue;
	}else if(getQueueSize(&priority_2)>0){
		dequeue(&priority_2);
		printf("Prioridad 2\n");
		continue;
	}else if(getQueueSize(&priority_3)>0){
		dequeue(&priority_3);
		printf("Prioridad 3\n");
		continue;
	}
}

	fclose(lista);
	return 0;
}