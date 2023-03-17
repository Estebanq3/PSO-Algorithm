#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 

/*Las particulas tienen los siguientes componentes: 
  Initial positions, velocity, and best positions of all particles.
	Posiciones Iniciales
	FitnesLocal
	Velocidad
	Mejores Posiciones
	Mejor Local Fitness
 
*/
struct Particula {
   
   double velocidad[2];
   double posiciones[2];
   double actualLocalFitness;
   
   double mejorPosicion[2];
   double mejorLocalFitness;
   
   
};


/*
	Funcion Objetivo
	@param
	Recibe como parametros la posicion de una particula, es decir el eje x y el eje y.
	@return
	La evaluacion de la particula, de acuerdo a sus posiciones. Es decir el fitness.

*/
double funcionObjetivo(double x, double y){
	
	double evaluated = (sin((x/180)-4))/(((pow(x,2))/(100000))+1)+(pow(x,2))/(500000)+(cos(((y)/(100))-1))/(((pow(y,2))/(1000000))+1)+(pow(y,2))/(500000);
	
	return evaluated;
}


/*
	Asignacion de los parametros iniciales de cada particula
	@param
	Recibe como parametros un vector de particulas, asimismo
	maxNumber y minimum number son el dominio del posicionamiento de una particula
	vMax y vMin son el dominio de la velocidad de una particula para una asignacion inicial
	@return
	Al ser void no retorna nada

*/
void asignarParametrosIniciales(struct Particula particulas[], double maxNumber, double minimumNumber, double vMax, double vMin){
	for(int i = 0; i < 5; i++){
		//POSICION
		particulas[i].posiciones[0] = minimumNumber + (double)rand() * (maxNumber - minimumNumber) / (double)RAND_MAX;	//Posicion en x
		particulas[i].posiciones[1] = minimumNumber + (double)rand() * (maxNumber - minimumNumber) / (double)RAND_MAX;	//Posicion en y
		//VELOCIDAD
		particulas[i].velocidad[0] = vMin + (double)rand() * (vMax - vMin) / (double)RAND_MAX;		//Velocidad en x
		particulas[i].velocidad[1] = vMin + (double)rand() * (vMax - vMin) / (double)RAND_MAX;		//Velocidad en y
		//MEJOR POSICION
		particulas[i].mejorPosicion[0] = particulas[i].posiciones[0];
		particulas[i].mejorPosicion[1] = particulas[i].posiciones[1];
		//ACTUAL FITNESS
		particulas[i].actualLocalFitness = funcionObjetivo(particulas[i].posiciones[0],particulas[i].posiciones[1]);
		//MEJOR FITNESS
		particulas[i].mejorLocalFitness = funcionObjetivo(particulas[i].posiciones[0],particulas[i].posiciones[1]);
		
	}
	
}


/*
	Funcion exceso de limites para controlar que se respete el dominio de las posiciones x y y en las particulas
	@param
	Recibe como parametro una posicion determinada, ya sea en x o en y
	@return
	Al ser void no retorna nada

*/
void excesoDeLimitesDomino(double *xy){
	if(*xy > 1000 || *xy < -1000){
		while(*xy > 1000 || *xy < -1000){
			if(*xy > 1000){
				*xy -= 1000;
				*xy = -(*xy);
			}
			if(*xy < -1000){
				*xy += 1000;
				*xy = fabs(*xy);
			}
		}
		//printf("\n Posicion irrespeta dominio, cambia a posicion: %f \n",*xy);
	}
}


//Algoritmo PSO
/* 
 Criterio de parada, mientras que no se cumple hacer (10000 iteraciones del ciclo más externo, o cuando la función objetivo permanezca sin disminuir en 0.001 o más durante 50 iteraciones consecutivas):
		Imprimir # de iteración, posición de la mejor solución global G y su valor de la función, coordenadas de cada una de las partículas Xi con 5 decimales y valor de la función objetivo para esa partícula con 7 decimales
		Evaluar cada particula con la funcion objetivo
		Cada particula:
			Actualizar posicion y velocidad de cada particula
			    *Velocidad de cada particula x_i & y_i:
					velocidad x: velocida nueva = w* velocidadActual + C1 * randomNumber0-1(bestLocalPosition-posicionActual) + C2*randomNumber0-1(bestGlobalPosition-posicionActual)
					velocidad y: velocida nueva = w* velocidadActual + C1 * randomNumber0-1(bestLocalPosition-posicionActual) + C2*randomNumber0-1(bestGlobalPosition-posicionActual)
				
			    *Posicion de cada particula x_i & y_i (xi(t+1) = xi(t) + vi(t+1))
					posicion x = posicion nueva = posicionActualX + velocidadNuevaX
					posicion y = posicion nueva = posicionActualY + velocidadNuevaY
				*Rebotar posiciones en caso de irrespetar domino
				
				
			Evaluar NUEVA posicion y velocidad de cada particula
			Actualizar su mejor personal, f(G local) > f(nueva local)
			Actualizar el global del sistema, si f(G) > f(particula i), entonces f(G) = f(particula i)
 
 Dar el mejor global obtenido (Al detenerse, se debe imrimir el mensaje "Solucion final: " y las coordenadas y valor de la mejor solución encontrada hasta ese momento.)
*/


int main( ) {

   //Se trabaja con 5 particulas segun indicaciones
   struct Particula particulas[5];
   
   /* Variables estaticas */
   double w  = 0.3;
   double C1 = 0.8;
   double C2 = 1.5;
   
   
   //Asignando parametros iniciales a cada particula
   asignarParametrosIniciales(particulas,1000,-1000,10,-10);
   
	
   /*Variables auxiliares para el algoritmo PSO*/
   int iteracion = 0;
   //Fitness inicial
   double posicionesBestSolution[2] = {particulas[0].posiciones[0],particulas[0].posiciones[1]};
   double bestFitness = funcionObjetivo(particulas[0].posiciones[0],particulas[0].posiciones[1]);
   
   
   //Parametros para controlar que por mas de 50 iteraciones no haya una diferencia de al menos mas de 0.001
   double antiguoFitness = 0;
   int numeroRegistradasProhibido = 0;
   
	
	//Algoritmo PSO
	while(iteracion <= 10000 && numeroRegistradasProhibido < 50){
		//Imprimir informes sobre mejor resultado hasta el momento
		printf("Iter: %d   G = (%.5f,%.5f)   f(G): %.7f \n", iteracion+1,posicionesBestSolution[0], posicionesBestSolution[1],bestFitness);
		for(int i = 0; i < 5; i++){
			printf("x%d = (%.5f,%.5f)  f(x%d) = %.7f \n", i+1, particulas[i].posiciones[0],particulas[i].posiciones[1],i+1,particulas[i].actualLocalFitness);
		}
		
		
		//Evaluar cada particula
		for(int i = 0; i < 5; i++){
			particulas[i].actualLocalFitness = funcionObjetivo(particulas[i].posiciones[0],particulas[i].posiciones[1]);
		}
		
		
		//En cada particula:
		//Calculamos nueva VELOCIDAD y POSICION de cada particula:
		for(int i = 0; i < 5; i++){
			double r1 = (double)rand()/(double)RAND_MAX;
			double r2 = (double)rand()/(double)RAND_MAX;
			double r3 = (double)rand()/(double)RAND_MAX;
			double r4 = (double)rand()/(double)RAND_MAX;
			
			
			//velocidad x: velocida nueva = w* velocidadActual + C1 * randomNumber0-1(bestLocalPosition-posicionActual) + C2*randomNumber0-1(bestGlobalPosition-posicionActual)
			particulas[i].velocidad[0] = w*particulas[i].velocidad[0] + C1*r1*(particulas[i].mejorPosicion[0]-particulas[i].posiciones[0]) +  C2*r2*(posicionesBestSolution[0]-particulas[i].posiciones[0]);
			//velocidad y: velocida nueva = w* velocidadActual + C1 * randomNumber0-1(bestLocalPosition-posicionActual) + C2*randomNumber0-1(bestGlobalPosition-posicionActual)
			particulas[i].velocidad[1] = w*particulas[i].velocidad[1] + C1*r3*(particulas[i].mejorPosicion[1]-particulas[i].posiciones[1]) +  C2*r4*(posicionesBestSolution[1]-particulas[i].posiciones[1]);
			

			//posicion x = posicion nueva = posicionActualX + velocidadNuevaX
			particulas[i].posiciones[0] = particulas[i].posiciones[0] +  particulas[i].velocidad[0];
			//posicion y = posicion nueva = posicionActualY + velocidadNuevaY
			particulas[i].posiciones[1] = particulas[i].posiciones[1] +  particulas[i].velocidad[1];
			
			
			//Control de respeto de dominio
			excesoDeLimitesDomino(&particulas[i].posiciones[0]);
			excesoDeLimitesDomino(&particulas[i].posiciones[1]);
			
		}
		
		
		//Evaluar de nuevo cada particula despues del cambio, para actualiza datos locales
		for(int i = 0; i < 5; i++){
			particulas[i].actualLocalFitness = funcionObjetivo(particulas[i].posiciones[0],particulas[i].posiciones[1]);
		}
		
		
		//Actualizar su mejor personal de cada particula si f(G local) > f(nueva local) hacer f(G local) = f(nueva local)
		for(int i = 0; i < 5; i++){
			if(particulas[i].mejorLocalFitness > particulas[i].actualLocalFitness){
					particulas[i].mejorLocalFitness = particulas[i].actualLocalFitness;
					//Actualizando posiciones en caso de encontrar mejor solucion
					particulas[i].mejorPosicion[0] = particulas[i].posiciones[0];
					particulas[i].mejorPosicion[1] = particulas[i].posiciones[1];
			}
		}
		
		
		//Antiguo fitness guardar para revisar 0.001
		antiguoFitness = bestFitness;
		
		
		//Actualizar el global del sistema, si f(G) > f(particula i), entonces f(G) = f(particula i)
		for(int i = 0; i < 5; i++){
			if(bestFitness > particulas[i].mejorLocalFitness){
					bestFitness = particulas[i].mejorLocalFitness;
					//Actualizando posiciones en caso de encontrar mejor solucion
					posicionesBestSolution[0] = particulas[i].mejorPosicion[0];
					posicionesBestSolution[1] = particulas[i].mejorPosicion[1];
			}
		}
		
		
		//Comprobando registro de distancia entre soluciones mayor a 0.001
		if(fabs(fabs(bestFitness) - fabs(antiguoFitness)) <= 0.001){
			numeroRegistradasProhibido += 1;
			//printf("Suma de prohibida: %d \n", numeroRegistradasProhibido);
		}else{
			numeroRegistradasProhibido = 0;
		}
		
		iteracion += 1;
	
	}
   
   printf("\nSolucion Final: G = (%.5f,%.5f) f(G): %.7f \n", posicionesBestSolution[0], posicionesBestSolution[1],bestFitness);
   return 0;
}


