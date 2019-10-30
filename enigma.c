/***
* @author Andrés Moreno Bernal
* @version 1.0 june 2019
*/


#include<stdio.h>
#include<stdlib.h>
#define N 26

int posnueva(int r[N]);
int mensajevalido(char a[]);
void cifrar(int r1[N],int r2[N],int r3[N],int *pos1,int *pos2, int *pos3);
void descifrar (int r1[N],int r2[N],int r3[N],int *pos1,int *pos2, int *pos3);
void arreglar_cadena (char cadena[]);
void configurar(int r1[N],int r2[N],int r3[N], char *rot1, char *rot2, char *rot3, int *pos1,int *pos2,int *pos3);
void igual(int x[N], int y[N]);
void reestablecer(int r1[N],int r2[N],int r3[N],char rot1, char rot2, char rot3, int pos1,int pos2,int pos3);
void girar(int r1[N],int r2[N],int r3[N], int i);
void imprimerotor(int r[N]);
void inirotor(int pos, int r[N], int a[N]); //inicializa el rotor con la posicion elegida y devuelve tambien esa posicion
void volver(int r1[N],int r2[N],int r3[N],char rot1, char rot2, char rot3,int pos1,int pos2,int pos3);
int buscarconect (int r[N],int x); //devuelve posicion de un conector de entrada en el rotor (busca un elemento en el vector y devuelve su posicion)

//ROTORES REPRESENTADOS POR VECTORES DE ENTEROS, EL INDICE ES EL CONECTOR DE ENTRADA, EL ELEMENTO EL DE SALIDA

//IMPORTANTE ENTENDER QUE LAS POSICIONES VAN DEL 0 AL 25 Y CONECTORES DEL 1 AL 26

	int A[N]={5,11,13,6,12,7,4,17,22,26,14,20,15,23,25
						,8,24,21,19,16,1,9,2,18,3,10 };
				 
	int B[N]={1,10,4,11,19,9,18,21,24,2,12,8,23,20,13,3,17,
					7,26,14,16,25,6,22,15,5};
		
	int C[N]={2,4,6,8,10,12,3,16,18,20,24,22,26,14,25,5,9,
					23,7,1,11,13,21,19,17,15};
				 
	int D[N]={5,19,15,22,16,26,10,1,25,17,21,9,18,8,24,12,
					14,6,20,7,11,4,3,13,23,2};
		
	int E[N]={22,26,2,18,7,9,20,25,21,16,19,4,14,8,12,24,
					1,23,13,10,17,15,6,5,3,11};
				 
	int R[N]={6,22,16,10,9,1,15,25,5,4,18,26,24,23,7,3,20,
					11,21,17,19,2,14,13,8,12};
				 

//PROGRAMA PRINCIPAL

int main()
{
	int op;
	int config=0; 
	char rot1,rot2,rot3; //guardaran los rotores elegidos
	int pos1,pos2,pos3; //estas variables van guardando las configuraciones antes de cada vez que se cifra 
	//para volver a ellas cuando se vaya a descifrar y poder hacer este proceso infinitas veces correctamente
	int ini1,ini2,ini3; //estas variables guardan la configuracion inicial para cuando quiera restaurar
	int r1[N];
	int r2[N];
	int r3[N];
	do{	
		printf("Seleccione una opcion: \n\n");
		//printf("\t0-Mostrar rotores\n");
		printf("\t1-Configurar Enigma\n");
		printf("\t2-Cifrar mensaje\n");
		printf("\t3-Descifrar mensaje\n");
		printf("\t4-Reestablecer \n");
		printf("\t5-Salir\n");
		fflush(stdin);
		scanf("%i",&op);
		switch(op)
		{
			//case 0: imprimerotor(r1); imprimerotor(r2); imprimerotor(r3); printf("\n\n"); break;
			case 1: configurar(r1,r2,r3,&rot1,&rot2,&rot3,&ini1,&ini2,&ini3); 
					pos1=ini1; pos2=ini2; pos3=ini3; config=1; printf("\n\n"); break;
			//case 6: volver(r1,r2,r3,rot1,rot2,rot3,pos1,pos2,pos3); imprimerotor(r1); imprimerotor(r2); imprimerotor(r3); break;
			case 2: 
				if(config==1) //Me aseguro de que no se realicen estas opciones si la maquina no esta configurada
				{
						
						cifrar(r1,r2,r3,&pos1,&pos2,&pos3); 
						printf("\n\n");
						break;
				}
				else
					printf("Configure la maquina primero\n"); printf("\n\n"); break;
			case 3: 
				if(config==1)
				{
					volver(r1,r2,r3,rot1,rot2,rot3,pos1,pos2,pos3);  descifrar(r1,r2,r3,&pos1,&pos2,&pos3); printf("\n\n"); break;
				}
				else
					printf("Configure la maquina primero\n"); printf("\n\n"); break;
				
			case 4: 
				if(config==1)
				{
					reestablecer(r1,r2,r3,rot1,rot2,rot3,ini1,ini2,ini3); 
					printf("\nLos rotores se han reestablecido a sus posiciones iniciales \n\n\n");
					break;
				}
			else
				printf("Configure la maquina primero\n"); printf("\n\n"); break;
		}
		
	}while(op!=5);
	
	printf("\nHa salido de la maquina\n");
	return 0;
}

//FUNCIONES

void cifrar (int r1[N],int r2[N],int r3[N],int *pos1,int *pos2, int *pos3)
{
	
	char mensaje[100]; 
	char cifrado[100]; //vector donde voy a ir metiendo el mensaje cifrado
	int i,j,aux,cod,pos;
	int letras=0; //contador de letras que vamos metiendo
	
	*pos1=posnueva(r1); //IMPORTANTE guardo la posicion de los rotores antes de modificarlos para despues partir de esta al descifrar
	*pos2=posnueva(r2);
	*pos3=posnueva(r3);
	
	
	
	do 
	{
		printf("Introduce un mensaje: \n");
		fflush(stdin);
		fgets(mensaje,100,stdin);
		arreglar_cadena(mensaje);
	}while(mensajevalido(mensaje)==0); //le sigo pidiendo el mensaje hasta que sea valido

	i=0;
	printf("El mensaje introducido es: %s\n", mensaje);

	while(mensaje[i]!='\0') //hasta que acabe la palabra la recorro letra por letra
	{
		aux=mensaje[i]-65; //si es A, sera un 0, si es B sera un 1 etc, pasar de char a int
		cod=r1[aux]; //me situo en el conector del primer rotor conectado a dicha letra
		cod=r2[cod-1]; //si cod (conector de salida de r1) es 5, deberé irme al conector de entrada 5 del rotor 2
		//que el conector 5 estara en el indice 4 del vector,POR ESO ES EL -1
		cod=r3[cod-1]; //hago lo mismo, si el contenido de lo anterior (contenido=conector de salida) es 6, 
		//tengo que ir al conector de entrada 6 (indice 5 del vector, por eso el -1)
		cod=R[cod-1]; //exactamente lo mismo pero ahora en el reflector, el conector de salida del reflector, es el conector de r3 al que
		//me tengo que conectar para comenzar la vuelta del mensaje
		//Proceso de vuelta del mensaje: si dicho anterior conector de r3 es el 7, como es de salida alq me tengo que conectar (ver dibujo del enunciado)
		//voy a buscar enq POSICION del rotor (en que conector de entrada) esta el 7 (conector de salida),   
		aux=buscarconect(r3,cod); 
		cod=aux+1; //ahora al reves, como tengo la posicion para saber que CONECTOR de entrada es, le sumo 1
		aux=buscarconect(r2,cod); //Lo mismo, busco aq posicion de r2 llega el anterior
		cod=aux+1; //19
		aux=buscarconect(r1,cod); //Lo mismo, aqui ya el mensaje ha vuelto al rotor 1 al conector de entrada, y sabremos con q letra va a cifrar 
		//sumandole 65 para pasarlo a char
		cifrado[i]=aux+65;
		letras++; //incremento las letras ya que dependiendo las letras que se metan girará los rotores de una manera
		i++; //para que siga avanzando a la siguiente letra 
		girar(r1,r2,r3,letras); //giro los rotores para que la maquina cambie por dentro y codifique distinto 
	}
	j=0;
	printf("\nEl mensaje cifrado es: ");
	while(j<i)
	{
		printf("%c",cifrado[j]);
		
		j++;
	}
	
	
	printf("\n");
}


//se diferencia de cifrar en el mensaje de salida y en que cifrar, guarda la posicion de los rotores antes de cifrar y modificarlos, LO DEMAS IGUAL
void descifrar (int r1[N],int r2[N],int r3[N],int *pos1,int *pos2,int *pos3) //funciona exactamente igual que cifrar 
{
	char mensaje[100];
	char cifrado[100];
	
	int i,j,aux,cod,pos;
	int letras=0;
	
	do
	{
	printf("Introduce un mensaje a descifrar: \n");
	fflush(stdin);
	fgets(mensaje,100,stdin);
	arreglar_cadena(mensaje);
	}while(mensajevalido(mensaje)==0);
	i=0;
	printf("El mensaje introducido es: %s\n", mensaje);
	while(mensaje[i]!='\0')
	{
		aux=mensaje[i]-65; //conector de esa letra //0
		cod=r1[aux]; //5
		cod=r2[cod-1];
		cod=r3[cod-1]; 
		cod=R[cod-1];
		aux=buscarconect(r3,cod); 
		cod=aux+1; //26
		aux=buscarconect(r2,cod); //18
		cod=aux+1; //19
		aux=buscarconect(r1,cod); //20
		
		cifrado[i]=aux+65;
		letras++;
		i++;
		girar(r1,r2,r3,letras);
	}
	
	j=0;
	printf("\nEl mensaje descifrado es: ");
	while(j<i)
	{
		printf("%c",cifrado[j]);
		
		j++;
	}
	
	
	printf("\n");
}

int buscarconect (int r[N],int x) //devuelve posicion de un conector de entrada en el rotor (busca un elemento en el vector y devuelve su posicion)
{
	int i=0;
	int res;
	while(i<N )
	{
		if(r[i]==x)
			res=i;
		i++;
	}
	return res;
}


void girar(int r1[N],int r2[N],int r3[N], int i) //recibe el numero de letras leidas y gira el rotor adecuado
{
	int x[N];
	int j;
	if(i<=26) //girar rotor 1
	{
		igual(x,r1);
		for(j=0; j<N; j++)
		{
			r1[j]=x[(j+1)%N];
		}
		
	}
	
	if(i>=26 && i<=52) //girar rotor 2
	{
		
		igual(x,r2);
		
		for(j=0; j<N; j++)
		{
			r2[j]=x[(j+1)%N];
			
		}
	}
	
	if(i>=52) //girar rotor 3
	{
		igual(x,r3);
	
		for(j=0; j<N; j++)
		{
			r3[j]=x[(j+1)%N];
		}
		i=0;//han vuelto a su posicion inicial y reinicio
	}
	
	
	//printf("\nEl rotor girado es: \n");
	//imprimerotor(r1);
	//imprimerotor(r2);
	//imprimerotor(r3);
}

int mensajevalido(char a[]) //devuelve 1 si el mensaje es valido y 0 si no lo es
{
	int valido=1;
	int i=0;
	while (a[i]!='\0' && valido==1)
	{
		if(a[i]>90 || a[i] < 65) //no haya minusculas ni Ñ
			valido=0;
		i++;
	}
	return valido;
}

//ARREGLAR CADENA
//Esta funcion quita el \n que coloca en la cadena la funcion fgets y asi evitamos problemas al recorrer luego la cadena
 void arreglar_cadena (char cadena[]){
    int i = 0;

    while (cadena[i]!='\n'){
        i++;
    }
    cadena[i]='\0';
}

int posnueva(int r[N]) //devuelve primer conector de salida de un rotor 
{
	return r[0];
}

void inirotor(int pos, int r[N], int a[N]) //copia un rotor en otro vacio a partir de la posicion que le llegue como parametro
{
	
	int p=pos;
	 p--; //asi si ej: me mete el conector 1, pongo primero el que esta en la posicion 0, que van del 0 al 25
	int i=0;
	int j=0;
	for(i=0; i<N; i++)
	{
			r[i]=a[p];
			p=(p+1)%N; //esto es para que siga dando la vuelta cuando llegue al final, % es division que devuelve resto
		
	}
	//printf("\nEl rotor restaurado es: \n");
	//imprimerotor(r);
	
}

void imprimerotor(int r[N]) //funcion para comprobar los rotores, no pedida por el tranajo
{
	int j;
	printf("\n");
			for(j=0; j<N; j++)
			{
				printf("%i ",r[j]);
			}
			printf("\n\n");
		
}

//configura los rotores y sus posiciones iniciales
void configurar(int r1[N],int r2[N],int r3[N], char *rot1, char *rot2, char *rot3, int *pos1, int *pos2,int *pos3)
{
		
	printf("Rotores a elegir: A B C D E (EN MAYUSCULAS) \n");
	do
	{
		printf("Introduce el primer rotor (EN MAYUSCULAS): \n");
		fflush(stdin);
		scanf("%c",rot1);
	}while(*rot1<65 ||*rot1 >69); //para que solo meta uno de los validos
	
	fflush(stdin);
	
	do
	{
		printf("Introduce su posicion inicial (que conector de entrada desea que vaya primero, el primero es el 1 y el ultimo el 26): \n");
		fflush(stdin);
		scanf("%i",pos1); 

	}while(*pos1 < 1||*pos1 >26);

	switch(*rot1) //segun elq introduzca rellenara el rotor con él a partir del conector introducido (inirotor) 
	{
		case 'A': inirotor(*pos1,r1,A); break;
		case 'B': inirotor(*pos1,r1,B); break;
		case 'C': inirotor(*pos1,r1,C); break;
		case 'D': inirotor(*pos1,r1,D); break; 
		case 'E': inirotor(*pos1,r1,E);break;
		default: break;
	}

	printf("\n");
	
	do
	{
		printf("Introduce el segundo rotor A B C D E (Uno no usado antes): \n");
		fflush(stdin);
		scanf("%c",rot2);
		fflush(stdin);

	}while(*rot1==*rot2 || *rot2<65 ||*rot2 >69); //para que no meta rotores repetidos
	
	do
	{
		printf("Introduce su posicion inicial (que conector de entrada desea que vaya primero, el primero es el 1 y el ultimo el 26): \n");
		fflush(stdin);
		scanf("%i",pos2); // ya que el vector va de 0 a 25 y los conectores de 1 a 25

	}while(*pos2 <1 ||*pos2 >26);

		switch(*rot2)
		{
			case 'A': inirotor(*pos2,r2,A); break;
			case 'B': inirotor(*pos2,r2,B); break;
			case 'C': inirotor(*pos2,r2,C); break;
			case 'D': inirotor(*pos2,r2,D); break; 
			case 'E': inirotor(*pos2,r2,E); break;
			default: break;
		}
		
		do
		{
			printf("Introduce el tercer rotor A B C D E (Uno no usado antes): \n");
			fflush(stdin);
			scanf("%c",rot3);
		}while(*rot2==*rot3 || *rot3==*rot1 || *rot3>69 || *rot3<65); //para que no meta rotores repetidos
		
		do
		{
			printf("Introduce su posicion inicial (que conector de entrada desea que vaya primero, el primero es el 1 y el ultimo el 26): \n");
			fflush(stdin);
			scanf("%i",pos3); // ya que el vector va de 0 a 25 y los conectores de 1 a 25

		}while(*pos3 <1 || *pos3 >26);

		switch(*rot3)
		{
			case 'A': inirotor(*pos3,r3,A); break;
			case 'B': inirotor(*pos3,r3,B); break;
			case 'C': inirotor(*pos3,r3,C); break;
			case 'D': inirotor(*pos3,r3,D); break; 
			case 'E': inirotor(*pos3,r3,E); break;
			default: break;
		}
				
}

void igual(int x[N], int y[N]) //copia un rotor en otro
{
	int i,j;
	for(i=0; i<N; i++)
	{
		x[i]=y[i];
	}
}

//para cada rotor llama a inipos con las posiciones elegidas al principio
//lo que hace que la maquina (los tres rotores) vuelva al estado inicial
void reestablecer(int r1[N],int r2[N],int r3[N],char rot1, char rot2, char rot3,int pos1,int pos2,int pos3)
{
	
	switch(rot1)
			{
				case 'A': inirotor(pos1,r1,A); break;
				case 'B': inirotor(pos1,r1,B); break;
				case 'C': inirotor(pos1,r1,C); break;
				case 'D': inirotor(pos1,r1,D); break; 
				case 'E': inirotor(pos1,r1,E); break;
				default: break;
			}
	
	switch(rot2)
			{
				case 'A': inirotor(pos2,r2,A); break;
				case 'B': inirotor(pos2,r2,B); break;
				case 'C': inirotor(pos2,r2,C); break;
				case 'D': inirotor(pos2,r2,D); break; 
				case 'E': inirotor(pos2,r2,E); break;
				default :break;
			}
			
	switch(rot3)
			{
				case 'A': inirotor(pos3,r3,A); break;
				case 'B': inirotor(pos3,r3,B); break;
				case 'C': inirotor(pos3,r3,C); break;
				case 'D': inirotor(pos3,r3,D); break; 
				case 'E': inirotor(pos3,r3,E); break;
				default: break;
			}
		
}

//me sirve para volver a poner los rotores como antes de el ultimo cifrado, asi como he dicho antes,
//podre descifrar correctamente partiendo de la misma configuracion de los rotores, ya que si solo usase reestablecer,
//solo podria cifrar y descifrar 1 vez sin reestablecer
void volver(int r1[N],int r2[N],int r3[N],char rot1, char rot2, char rot3,int pos1,int pos2,int pos3)
{
//voy por los 3 rotores y segun el que se haya elegido lo restauro a partir de la correspondiente variable posx
	int x;
	switch(rot1)
			{
				case 'A':
					 //como buscarconect me devuelve la POSICION (del 0 al 25) le tengo que sumar 1, porque inirotor 
					 //trabaja con CONECTORES (del 1 al 26)
					 x=buscarconect(A,pos1)+1; 
					 inirotor(x,r1,A); break;
				case 'B': 
					 x=buscarconect(B,pos1)+1; 
					 inirotor(x,r1,B); break;
				case 'C': 
					x=buscarconect(C,pos1)+1; 
					 inirotor(x,r1,C); break;
				case 'D': 
					 x=buscarconect(D,pos1)+1; 
					 inirotor(x,r1,D); break;
				case 'E': 
					 x=buscarconect(E,pos1)+1; 
					 inirotor(x,r1,E); break;
				default: break;
			}
	
	switch(rot2)
			{
				case 'A':
					 x=buscarconect(A,pos2)+1; 
					 inirotor(x,r2,A); break;
				case 'B': 
					 x=buscarconect(B,pos2)+1; 
					 inirotor(x,r2,B); break;
				case 'C': 
					x=buscarconect(C,pos2)+1; 
					 inirotor(x,r2,C); break;
				case 'D': 
					 x=buscarconect(D,pos2)+1; 
					 inirotor(x,r2,D); break;
				case 'E': 
					 x=buscarconect(E,pos2)+1; 
					 inirotor(x,r2,E); break;
				default: break;
			}
			
	switch(rot3)
			{
				case 'A':
					 x=buscarconect(A,pos3)+1; 
					 inirotor(x,r3,A); break;
				case 'B': 
					 x=buscarconect(B,pos3)+1; 
					 inirotor(x,r3,B); break;
				case 'C': 
					x=buscarconect(C,pos3)+1; 
					 inirotor(x,r3,C); break;
				case 'D': 
					 x=buscarconect(D,pos3)+1; 
					 inirotor(x,r3,D); break;
				case 'E': 
					 x=buscarconect(E,pos3)+1; 
					 inirotor(x,r3,E); break;
				default: break;
			}
		
}

