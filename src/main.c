/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* define pid_t */
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h> /* fork() */


int primo=0;


/*Esta fun~]ap retornará 0 ou 1; 
 0 se o numero não for primo.
 1 se for primo;
*/
int verifica_primo(long int vet){
	int i=0;int result=0;
	long int l=0;
	if(vet==2){ //dois é primo;
		return 1;
	}else if(vet==1){//1 não é primo;
		return 0;
	}else if(vet % 2 ==0){//se numero é par e maior que dois não é primo*/
		return 0;
	}else{//aqui se o numero é diferente de 1 ou 2, e não é par sendo maior que dois verificamos manualmente
			for (l = 2; l<= vet/2; l++) {
				if (vet % l == 0) {//verificaca se o numeor tem difisor
				result++;//se sim resul é incrementado
				break; 		
				}
			}
 		if (result == 0) {
			return 1;//se não encontrou divisor retorna 1 (numero é primo);
 		}else {
			return 0;// se encontra divisor retorna 0 numero não é primo;
 		}
	}
		
}

int main() {

  long int x[60];
  char caracter='\0';
  int num_processos=0;
  int i;
  int primos;//ira armazenar a quantidade total de primos.
  int entradas=0;
  int encerrados=0;
  int *mem;//irá conter os resultados calculados nos processos filhos
  

/*inicializar o vetor de entrada em -1, será critério de parada do loop que percerrerá a entrada*/
  for (i=0;i<60;i++){
    x[i]=-1;
  }
  
//trata de paramentros do mmap
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  
  mem = (int*) mmap(NULL, sizeof(int)*100, protection, visibility, 0, 0);
  /*inicializar mem em -1*/
  for (i=0;i<100;i++){
    mem[i]=-1;
  }

  /*leitura dos valores*/
	i=0;
	while(caracter!='\n'){
		/*aqui realiza a leitura da entrada*/
			scanf("%li%c", &x[i],&caracter);
			i++;
			entradas++;
	}

	/*define processos*/
	pid_t pid;

	i=0;
	while(x[i]!=-1){//aqui -1 significa que todas as entradas já foram avaliadas
		if(num_processos<4){//se numero de processos é menor que 4, ou seja, mo maximo 3, cria um processo
			pid=fork();//abre um novo processo;
			num_processos++;
			if(pid==0){
				//*realiza no processo filho a verificação se um numero é primo*/
				mem[i]=verifica_primo(x[i]);
				exit(0);//sai do processo ao terminar uma verificação
			}
			i++;//aqui se incrementa i, passando a leitura da proxima entrada;
		}
		else{
			//se o numero de processos aberto já é 4, então ele aguarda ate um processo terminar
			waitpid(0, NULL, 0);
			num_processos--;//decrementa o numero de processos
		}
	}
	
	
	i=0;
	/*Certifice-se que todos os processos estão concluidos*/
	while(1){
		for (i=0;i<entradas;i++){
			if(mem[i]>=0){
				encerrados++;//conta se todas as entradas foram avaliadas ou se o processo de alguma delas ainda não terminou
			}
		}	
		if(encerrados==entradas){//se totas foram avaliadas saio do loop
			break;
		}
		encerrados=0;
	}
		

	primos =0;
	
	i=0;
	/*Percorre o mean somando todos os numero primos*/
	while(mem[i]!=-1){
		primos=primos+mem[i];
		i++;
	}
	//imprime o resultado
	printf("%d\n",primos);


  return 0;
}
