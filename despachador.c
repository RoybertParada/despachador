#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include <sys/wait.h>

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
	FILE *lista;                                      // Archivo .in
	int tiempo_proc = 0;                              // Tiempo del procesador
	int ejecutando = 0;                               // Verificador de proceso en ejecucion
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

	Queue real_time,priority_1,priority_2,priority_3,super_cola,cola_no_listos; // Las Colas
	//Se inicializan las colas -------------
		queueInit(&super_cola, sizeof(PCB));
		queueInit(&cola_no_listos, sizeof(PCB));
		queueInit(&real_time, sizeof(PCB));
		queueInit(&priority_1, sizeof(PCB));
		queueInit(&priority_2, sizeof(PCB));
		queueInit(&priority_3, sizeof(PCB));
	//-------------------------------------


//---------------------Se leen los procesos linea por linea---------------------------
	while(!feof(lista)){
		fgets(a,21,lista);
		printf("%c, %c, %c, %c, %c, %c, %c\n",a[0],a[3],a[6],a[9],a[12],a[15],a[18]);
		PCB *proceso = malloc(sizeof(PCB));
		proceso->tiempo_llegada = atoi(&a[0]);
		proceso->prioridad = atoi(&a[3]);
		proceso->tiempo_del_procesador = a[6]; 
		proceso->impresora = atoi(&a[9]); 
		proceso->scanners = atoi(&a[12]); 
		proceso->modems = atoi(&a[15]); 
		proceso->dvd = atoi(&a[18]); 

		enqueue(&super_cola, &proceso);        //Se encolan todos los procesos por tiempo de llegada
	}
//------------------------------------------------------------------------------------

	char *asd[2];
	asd[0] = "./child";     //nombre del ejecutable               
	asd[2] = NULL;

	while((getQueueSize(&priority_3)>0) || (getQueueSize(&priority_2)>0) || (getQueueSize(&priority_1)>0) || (getQueueSize(&real_time)>0) || (getQueueSize(&super_cola)>0) || (getQueueSize(&cola_no_listos)>0)){         // Mientras hayan procesos en alguna de las colas se ejecutara - se comienza en tiempo_proc=1
		tiempo_proc++;
		PCB *primer = malloc(sizeof(PCB));
		queuePeek(&super_cola,&primer);
		

		printf("%d = %d \n", primer->tiempo_llegada, tiempo_proc);
		if( primer->tiempo_llegada == tiempo_proc ){      //Si el proceso nuevo llega en el tiempo por el que va el procesador
			if ( primer->prioridad == 0){
				if( ejecutando != 1 ){
					ejecutando = 1;
					p_id=fork();
					
	        //----------Si es el hijo -----------//
	        		if (p_id == 0) {            
	        		asd[1] = &(primer->tiempo_del_procesador);  //numero de segundos que durara el proceso                                                                
	          		printf( "PID: %d, Prioridad: 0, Tiempo restante del procesador: %s, Ningun recurso \n",getpid(),asd[1]);
	          		execvp( asd[0], asd);
	          		perror("execl() failure!\n\n");
	        //---Si hay error al crear el fork---//
	        		} else if (p_id < 0){   
	        			printf("Error Proceso\n");          
	          			exit(1);
	        //----------Si es el padre ----------//
	        		} else if (p_id != 0 ){  
	        			printf("esperando al hijo\n");  
	          			wait(NULL);                 //Se espera a que termine el proceso
	          			ejecutando = 0;
	          			dequeue(&super_cola,&primer);
	          			printf("ejecuto el hijo\n");
	        		}
				}
			} else {                                //Si la prioridad es 1 2 o 3
				
				printf("break\n");
			}
			printf("que pasa aqui\n");
		}else{

			dequeue(&super_cola,&primer);
		}

		//printf("Tiempo %c\n", primer->tiempo_del_procesador);
		//dequeue(&super_cola,primer);
	}		
	fclose(lista);
	return 0;
}