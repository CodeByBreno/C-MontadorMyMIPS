#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "2 - Manipulador de Strings.cpp"
#include "3 - Gerador de Binario.cpp"

//declaro a estrutura que será usada
typedef struct{
	char opcode[7] = "######";
	int rs;
	int rt;
	int rd;
	int shamt;
	char funct[7] = "######";
	char imm[17] = "################";
	char offset[27] = "##########################";
} instruction;

typedef struct{
	char* lb;
	int numero_da_linha = 0;
} label ;

//definindo os dados base
int linha_atual = 0;
instruction tabela;

label* lista_de_labels;
int numero_de_labels = 0;

void erro(int i){
	switch (i){
		case 1:{
			printf ("Erro na linha %d: Instrução com Formato inadequado", linha_atual - 1);
			exit(2);
		}
		case 2:{
			printf ("Erro na linha %d: Operação não existe", linha_atual - 1);
			exit (3);
		}
		case 3: {
			printf ("Erro na linha %d: Registrador de formato desconhecido", linha_atual - 1);
			exit(4);
		}
		case 4: {
			printf ("Erro na linha %d: 'Nenhum' não é um label válido'", linha_atual - 1);
			exit(5);
		}
		case 5:{
			printf ("Erro na linha %d: Label destino não existe!", linha_atual - 1);
			exit(6);
		}
	}
}

void add_label (char* vet){
	if (numero_de_labels++ == 0) lista_de_labels = (label *) malloc(sizeof(label));
	else lista_de_labels = (label *) realloc (lista_de_labels, numero_de_labels);

	lista_de_labels[numero_de_labels - 1].lb = (char *) malloc (strlen(vet));
	strcpy (lista_de_labels[numero_de_labels - 1].lb, vet);
	lista_de_labels[numero_de_labels - 1].numero_da_linha = linha_atual - 1;
}

void calcula_offset(char* vet){
	int i;
	char* aux;
	if (strcmp (vet, "nenhum")) erro(4);
	for (i = 0; i < numero_de_labels; i++){
		if (strcmp(lista_de_labels[i].lb, vet)){
			strcpy (tabela.offset, converter_bin(lista_de_labels[i].numero_da_linha, aux, 27));
			return;
		}
	}
	erro(5);
}

void calcula_imm(char* vet){
	int i;
	char* aux;
	if (strcmp (vet, "nenhum")) erro(4);
	for (i = 0; i < numero_de_labels; i++){
		if (strcmp(lista_de_labels[i].lb, vet)){
			strcpy (tabela.imm, converter_bin (lista_de_labels[i].numero_da_linha - linha_atual + 1, aux, 27));
			return;
		}
	}
	erro(5);
}
	
void inicializar(){
	char vet[7] = "nenhum";
	strcpy(tabela.opcode, vet);
	tabela.rs = -1;
	tabela.rt = -1;
	tabela.rd = -1;
	tabela.shamt = -1;
	strcpy(tabela.funct, vet);
	strcpy(tabela.imm, vet);
	strcpy(tabela.offset, vet);
}

void apresentar_linha(){
	printf ("OPCODE = %s\n", tabela.opcode);	
	printf ("RS = %d\n", tabela.rs);
	printf ("RT = %d\n", tabela.rt);
	printf ("RD = %d\n", tabela.rd);
	printf ("SHAMT = %d\n", tabela.shamt);
	printf ("FUNCT = %s\n", tabela.funct);
	printf ("IMM = %s\n", tabela.imm);
	printf ("OFFSET = %s\n", tabela.offset);
}

int identificar_op(char* vet){
	//1 - formato registrador, registrador, registrador
	//2 - formato registrador, registrador, numero
	//3 - formato registrador, registrador, label
	//4 - formato label
	//5 - formato registrador
	//6 - formato registrador, offset(registrador)	

	//formatos R
	if (compareIgnoreCase (vet, "Add")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "100000");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "Sub")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "100010");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "And")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "100100");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "Or")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "100101");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "Xor")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "100110");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "Slt")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "101010");
		tabela.shamt = 0;
		return 1;
	}
	if (compareIgnoreCase (vet, "Jr")){
		strcpy(tabela.opcode, "000000");
		strcpy(tabela.funct, "001000");
		tabela.shamt = 0;
		return 1;
	}

	//formato I
	if (compareIgnoreCase (vet, "Addi")){
		strcpy(tabela.opcode, "001000");
		return 2;
	}
	if (compareIgnoreCase (vet, "Beq")){
		strcpy(tabela.opcode, "000100");
		return 3;
	}	
	if (compareIgnoreCase (vet, "Bne")){
		strcpy(tabela.opcode, "000101");
		return 3;
	}
	if (compareIgnoreCase (vet, "Lw")){
		strcpy(tabela.opcode, "100011");
		return 6;
	}
	if (compareIgnoreCase (vet, "Sw")){
		strcpy(tabela.opcode, "101011");
		return 6;
	}

	//formato J
	if (compareIgnoreCase (vet, "J")){
		strcpy(tabela.opcode, "000010");
		return 4;
	}
	if (compareIgnoreCase (vet, "Jal")){
		strcpy(tabela.opcode, "000011");
		return 4;
	}

	erro(1);
}

int identificar_reg (char* vet){
	if (compareIgnoreCase (vet, "$zero")) return 0; //registrador $zero
	
	if (vet[3] != '\0') erro(3); //se tiver um registrador com mais de 3 elementos, além do zero
	if (compareIgnoreCase (vet, "$at")) return 1; //registrador $at

	if (vet[1] == 'v' || vet[1] == 'V'){ //registradores $v
		if ((vet[2] < '0' || vet[2] > '1')) erro(3);
		return (vet[2] - '0' + 2);
	}

	if (vet[1] == 'a' || vet[1] == 'A'){ //registradores $a
		if ((vet[2] < '0' || vet[2] > '3')) erro(3);
		return (vet[2] - '0' + 4);
	}

	if (vet[1] == 't' || vet[1] == 'T'){ //registradores $t, $t8 e $t9
		if (vet[2] == '8' || vet[2] == '9')
			return (vet[2] - '0' + 24);
		if ((vet[2] < '0' || vet[2] > '7')) erro(3);
		return (vet[2] - '0' + 8);
	}

	if (vet[1] == 's' || vet[1] == 'S'){ //registradores $s e $sp
		if (vet[2] != 'p' && vet[2] != 'P'){
			if ((vet[2] < '0' || vet[2] > '7')) erro(3);
			return (vet[2] - '0' + 16);
		}
		else return 29;
	}

	if (vet[1] == 'k' || vet[1] == 'K'){ //registradores $v
		if ((vet[2] < '0' || vet[2] > '1')) erro(3);
		return (vet[2] - '0' + 26);
	}

	if (compareIgnoreCase (vet, "$gp")) return 28;
	if (compareIgnoreCase (vet, "$fp")) return 30;
	if (compareIgnoreCase (vet, "$ra")) return 31;
}

int tabelar(char* linha){
	int i, j, c = 0;
	int flag = 0, controle = 0, tam = strlen(linha);
	char operation[5];
	char registrador[20];

	//verifico se a linha tem código (algo além de espaços e comentários)
	for (i = 0; linha[i] != '\n' ; i++){
		if (linha[i] == ':'){
			controle = 1;
			if (flag == 1) break;
		}
		if (linha[i] == '#') break;
		if (linha[i] != ' ') flag = 1;
	}
	if (flag == 0) return 0; //caso a linha seja vazia ou só tenha comentários

	//coloca na tabela os dados informados daqui em diante
	linha_atual++;

	for (i = 0; i < tam; i++){

		//printf ("CONTROLE = %d || FLAG = %d\n", controle, flag);
		//leio a label, flag vale 1
		if (flag == 1 && controle != 1) flag = 2;
		if (linha[i] == ':' && controle == 1 && flag == 1){
			flag = 2;
			if (i == 0) erro(1);
			for (j = 0; j < i; j++){
				registrador[j] = linha[j];
			}
			registrador[i++] = '\0';
			add_label(registrador);
			//puts (lista_de_labels[numero_de_labels - 1].lb);
			j = 0;
		}
		
		//leio o operador, flag vale 2
		if (linha[i] != ' ' && flag == 2){
			if (linha[i] == '$') erro(1);
			//finalizador da operação : ocorre quando um ' ' é encontrado novamente ou quando
			//o maior tamanho possível para o identificado foi contado
			for (j = i, c = 0; j < i + 4 && linha[j] != ' ' && linha[j] != '\n'; j++){
				if (linha[j] == '\n') erro(1);
				operation[c++] = linha[j];
			}
			operation[c] = '\0';
			flag = 3;
			i += c;
			j = 0;
			//puts (operation);
		}

		//verifico se haverá algum registrador com base na operação a ser realizada
		if (flag == 3){
			flag = 4;
			controle = identificar_op(operation);
		}

		//1 - formato registrador, registrador, registrador
		//2 - formato registrador, registrador, numero
		//3 - formato registrador, registrador, label
		//4 - formato label
		//5 - formato registrador
		//6 - formato registrador, offset(registrador)

		// com base na operação, salvo os registradores e labels
		if (flag >= 4){
			//se tiver o formato reg,reg, alguma_coisa ou reg simplismente
			if (controle != 4 && controle != 6){ 
				if (flag != 9){ //serve para obter os dois primeiros registradores
					if (flag % 2 == 1 && linha[i] == '$') erro(1); //caso surja algo como $te$te, sem virgula
					
					if (flag % 2 == 0 && linha[i] != ' '){ //detecto o inicio de um registrador
						//puts ("teste");
						if (linha[i] != '$' && flag != 8) erro(1);
						flag++;
						j = i;
					}
	
					if (flag % 2 == 1 && (linha[i] == ' ' || linha[i] == ',') && flag != 9){ //mecanismo decodificador de registrador
						for (j = j, c = 0; j < i; j++) registrador[c++] = linha[j];
						registrador[c] = '\0';

						//------------------------------- primeiro registrador detectado
						if (flag == 5){
							//puts (registrador);
							if (controle == 5){
								tabela.rs = identificar_reg(registrador);
								return 1;
							}
							if (controle == 1) //instrução do tipo R
								tabela.rd = identificar_reg(registrador);
							if (controle == 2) //estrutura rt, rs, imm
								tabela.rt = identificar_reg(registrador);
							if (controle == 3) //estrutura rs, rt. offset
								tabela.rs = identificar_reg(registrador);
							
						}
						 //------------------------------ segundo registrador detectado
						if (flag == 7){
							//puts (registrador);
							if (controle == 1 || controle == 2)
								tabela.rs = identificar_reg(registrador);
							if (controle == 3)
								tabela.rt = identificar_reg(registrador);
						}		
						flag++;
					}
				}
				//------------------------------ terceiro registrador detectado
				else{
					if (linha[i] == ' ' || linha[i] == ',' || i == tam - 1){
						for (j = j, c = 0; j < i; j++) registrador[c++] = linha[j]; //na verdade pode ser um número
						registrador[c] = '\0';
						//puts(registrador);
						if (controle == 1){
							tabela.rt = identificar_reg(registrador);
							return 1;
						}		
						if (controle == 2){
							strcpy (tabela.imm, registrador);
							return 1;
						}
						if (controle == 3){
							calcula_imm(registrador);
							return 1;
						}
					}
				}
			}
			if (controle == 4){ // se não tiver registrador
				if (linha[i] != ' '){
					for (j = i, c = 0; linha[j] != ' ' && linha[j] != '\n' && linha[j] != '#'; j++)
						registrador[c++] = linha[j];
					registrador[c] = '\0';
					calcula_offset (registrador);
					return 1;
					}
			}
			if (controle == 6){ //formato r, offset(r)
				//detecto e guardo o primeiro registrador
				if (flag % 2 == 0 && linha[i] != ' ' ){ //detecto o inicio de algo diferente de espaço vazio
						flag++;
						if (linha[i] == '(') flag--;
						j = i;
				}
				if ((flag%2 == 1) && (linha[i] == ' ' || linha[i] == ',' || linha[i] == ')' || linha[i] == '(')){ 
					for (j = j, c = 0; j < i; j++) registrador[c++] = linha[j];
					registrador[c] = '\0';
					//puts (registrador);
					if (flag == 5) tabela.rt = identificar_reg(registrador);
					if (flag == 7){
						int m;
						char* pressa;
						sscanf (registrador, "%d ", &m);
						strcpy (tabela.imm, converter_bin(m, pressa, 18));
					}
					if (flag == 9){
						tabela.rs = identificar_reg(registrador);
						return 1;
					}
					flag++;
				}
			}
		}
	}
}

void escrever (char * vet, FILE * s){
	int i;
	//puts(vet);
	vet[9] = '\0';
	for (i = 0; vet[i] != '\0'; i++){
		putc (vet[i], s);
	}	
	putc('\n', s);
}

void gerar_bin(FILE * s){
	char* aux;
	char* rs, *rt, *rd, *shamt;
	if (strcmp (tabela.opcode, "000000")){ // instrução de formato R
		rs = converter_bin(tabela.rs, aux, 6);
		rt = converter_bin(tabela.rt, aux, 6);
		rd = converter_bin(tabela.rd, aux, 6);
		shamt = converter_bin(tabela.shamt, aux, 6);
		
		aux = cortar(shamt, 3, 5);
		aux = concatenar(aux, tabela.funct); //8 bits, logo n preciso alocar espaço novo pra aux
		escrever(aux, s);

		strcpy (aux, rd);
		aux = concatenar(aux, cortar(shamt, 0, 3));
		escrever(aux, s);
		
		aux = cortar(rs, 2, 5);
		aux = concatenar (aux, rt);
		escrever(aux, s);

		strcpy (aux, tabela.opcode);
		aux = concatenar (aux, cortar(rs, 0, 2));
		escrever(aux, s);
		return;
	}
	else if (strcmp (tabela.opcode, "000010") || strcmp (tabela.opcode, "000011")){ //tipo J
		aux = cortar (tabela.offset, 18, 26);
		escrever(aux, s);
		
		aux = cortar (tabela.offset, 10, 18);
		escrever(aux, s);

		aux = cortar (tabela.offset, 2, 10);
		escrever(aux, s);

		strcpy (aux, tabela.opcode);
		aux = concatenar (aux, cortar(tabela.offset, 0, 2));
		escrever(aux, s);
		return;
	}	
	else { //tipo I
		rs = converter_bin(tabela.rs, aux, 6);
		rt = converter_bin(tabela.rt, aux, 6);

		aux = cortar (tabela.imm, 8, 16);
		escrever(aux, s);
	
		aux = cortar (tabela.imm, 0, 8);
		escrever(aux, s);

		aux = cortar(rs, 2, 5);
		aux = concatenar (aux, rt);
		escrever(aux, s);

		strcpy (aux, tabela.opcode);
		aux = concatenar (aux, cortar (rs, 0, 2));
		escrever(aux, s);
		return;
	}
}

char* ler_linha (FILE * entrada){
	char * resultado, c;
	int i;
	resultado = (char *) malloc(50);
	for (c = getc(entrada), i = 0; c != '\n' && c != EOF; i++, c = getc(entrada)){
		resultado[i] = (char) c;
	}
	resultado[i] = '\0';
	return resultado;
}

main(int argc, char* argv[]){
	char* linha;
	FILE *p;
	FILE *s;
	
	setlocale(LC_ALL, "portuguese");
	p = fopen(argv[0], "r");
	if (argc >= 2) s = fopen(argv[2], "wb");
	else s = fopen("memoria.dat", "wb");

	if (!p){
		printf ("Ocorreu um erro ao abrir o arquivo\n");
		exit (1);
	}

	while (!feof(p)){
		linha = ler_linha(p);
		int a = strlen(linha);
		linha[a] = '\n';	
		linha[a + 1] = '\0';
	
		inicializar();
		if (tabelar(linha)) {
			apresentar_linha();
			gerar_bin(s);
		}
	}
}
