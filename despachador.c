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

/*//---Recursos Clave---
RECURSO Recursos;
Recursos->impresora = 2;
Recursos->scanner = 1;
Recursos->modem = 1;       //no quiere compilar >:c (Para llevar la cuenta de los recursos que se esten usando)
Recursos->DVD = 2;
//---Recursos Clave---*/

int main(int argc, char* argv[]){

	cad[0] = "./child";

	FILE *lista;                                      // Archivo .in
	int length,i,b;
	char a[21];
	Queue real_time,priority_1,priority_2,priority_3; // Las Colas
	lista = fopen(argv[1],"r");

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
			//Iniciar T-20seg
			//verificar que no hayan procesos en ejecucion
				//Si es true se verifica tiempo de inicio con tiempo del procesador
				//Se pasa a ejecutar la cosa
			//cad[1] = a[6]; -> para pasar la cantidad de segundos al child.c pero esta sacando un warning ahi
			cad[1] = "4";
			p_id=fork();
			if (p_id == 0) {
				printf( "PID: %d, Prioridad: 0, T-restante: ?\n",getpid() );
				execlp(cad[0], "child", cad[1], NULL);    //Se ejecuta el child e imprime los segundos PERO no se devuelve al codigo
				//Aqui como que se mata o algo ?? 
				printf("Proceso finalizado\n");             //good caso :v
				dequeue(&real_time,&proceso);
				exit(0);
			} else if (p_id < 0){
			exit(0);
			}
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


	fclose(lista);
	return 0;
}