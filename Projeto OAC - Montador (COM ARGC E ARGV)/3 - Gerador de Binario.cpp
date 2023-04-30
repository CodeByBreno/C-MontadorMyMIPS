char* complementar (char* vet, int T){
	int i;
	for (i = 0; i < T-1; i++){
			if (vet[i] == '0') vet[i] = '1';
			else vet[i] = '0';
	}
	return vet;
}

char* adicionar_um (char* vet, int T){
	int i;
	for (i = T-2; i >= 0; i--){
		if (vet[i] == '1'){
			vet[i] = '0';
		}
		else{
			vet[i] = '1';
			break;
		}
	}
	return (vet);
}

char* complemento_de_dois (char* vet, int T){
	return (adicionar_um (complementar(vet, T), T));
}

char* converter_bin (int n, char* vet, int T){
	vet = (char *) malloc (T);
	int i, controle;

	if (n>=0) controle = n;
	else controle = -n;
 
	for (i = 0; i < T-1; i++, controle /= 2){
		if (controle%2 == 0)
			vet[T-2 - i] = '0';
		else 
			vet[T-2 - i] = '1';
	}
	vet[T-1] = '\0';

	if (n >= 0)
		return vet;
	else{
		return(complemento_de_dois(vet, T));
	}
}

