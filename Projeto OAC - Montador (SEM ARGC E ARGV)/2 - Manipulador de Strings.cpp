void strcpy (char* vet1, char* vet2){ //SOMENTE PARA COM O FIM DE VET2
	int i;
	for (i = 0; vet2[i] != '\0'; i++){
		vet1[i] = vet2[i];
	}
	vet1[i] = '\0';
	return;
}
int strlen (char* vet){
	int i;
	for (i = 0; vet[i] != '\0'; i++);
	return i;
}
int strcmp (char* vet1, char* vet2){
	int i;
	for (i = 0; vet1[i] != '\0' && vet2[i] != '\0'; i++)
		if (vet1[i] != vet2[i]) return 0;
	if (vet1[i] != vet2[i]) return 0;	
	return 1;
}

char* concatenar (char* vet1, char* vet2){
	int i, c;
	char* resultado;
	resultado = (char *) malloc (strlen(vet1)+strlen(vet2));
	for (i = 0; vet1[i] != '\0'; i++) resultado[i] = vet1[i];
	for (i = i, c = 0; vet2[c] != '\0'; i++, c++) resultado[i] = vet2[c];
	resultado[i] = '\0';
	return resultado;
}

char* cortar(char* vet1, int a, int b){
	char* resultado;
	int i, c;
	if (a >= b) return NULL;
	if (a >= strlen(vet1)) return NULL;
	resultado = (char *) malloc (b-a);
	for (i = a, c = 0; i < b; i++) resultado[c++] = vet1[i];
	resultado[c] = '\0';
	return resultado;
}	

int compareIgnoreCase (char* vet1, char* vet2){
	int i;
	for (i = 0; vet1[i] != '\0' && vet2[i] != '\0'; i++){
		if (!(vet1[i] == vet2[i] || vet1[i] == vet2[i] + 32 || vet1[i] == vet2[i] - 32)) return (0);
	}
	if (vet1[i] == vet2[i]) return (1);
	else return 0;
}

