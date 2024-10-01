#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct cvor
{
	int** matrica;
	int dubina, br_sinova, br_epruveta, prva_epruveta, druga_epruveta;
	struct cvor** sinovi;
};
typedef struct cvor cvor;

cvor* pravljenje_cvora(int** m, int n, int iz, int u, int p) {
	cvor* novi_cvor;
	novi_cvor = malloc(sizeof(cvor));
	if (novi_cvor) {
		novi_cvor->matrica = m;
		novi_cvor->br_epruveta = n;
		novi_cvor->prva_epruveta = iz;
		novi_cvor->druga_epruveta = u;
		novi_cvor->br_sinova = 0;
		novi_cvor->dubina = p;
		novi_cvor->sinovi = malloc(n * (n - 1) * sizeof(cvor*));
		return novi_cvor;
	}
}

int** nova_matrica(int** m, int iz, int u, int n) {
	int** matr;
	int i, j;
	matr = malloc(4 * sizeof(int*));
	for (i = 0; i < 4; i++) {
		matr[i] = malloc(n * sizeof(int));
		for (j = 0; j < n; j++) {
			matr[i][j] = m[i][j];
		}
	}
	if (prebacivanje(matr, iz, u) == 1)return matr;
	else return NULL;
}

void pravljenje_sinova(cvor* trenutni, int lvl) {
	int i, j;
	int** nova;
	int brojac = 0;
	for (i = 0; i < trenutni->br_epruveta; i++) {
		for (j = 0; j < trenutni->br_epruveta; j++) {
			if (i != j) {
				nova = nova_matrica(trenutni->matrica, i, j, trenutni->br_epruveta);
				if (nova != NULL) {
					cvor* sledeci = pravljenje_cvora(nova, trenutni->br_epruveta, i, j, lvl);
					trenutni->sinovi[brojac] = sledeci;
					brojac++;
					trenutni->br_sinova++;
				}
			}
		}
	}
}

int boja(int** matrica, int epruveta) {
	int boja_na_vrhu, v, t;
	v = 0;
	t = 0;
	while (t < 4) {
		if (matrica[v][epruveta] != 0) {
			boja_na_vrhu = matrica[v][epruveta];
			break;
		}
		else {
			v++;
			t++;
			if (v == 3)boja_na_vrhu = matrica[v][epruveta];
		}
	}
	return boja_na_vrhu;
}

int br_boja(int** matrica, int epruveta) {
	int b, v, br;
	b = boja(matrica, epruveta);
	v = 0;
	br = 0;
	if (b != 0) {
		while (v < 4) {
			if (matrica[v][epruveta] == 0) {
				v++;
			}
			else {
				if (matrica[v][epruveta] == b) {
					br++;
					v++;
				}
				else break;
			}
		}
	}
	return br;
}

int br_slobodnih(int** matrica, int epruveta) {
	int br, v;
	v = 0;
	br = 0;
	while (v < 4) {
		if (matrica[v][epruveta] == 0) {
			br++;
			v++;
		}
		else break;
	}
	return br;
}

int prebacivanje(int** matrica, int epruveta1, int epruveta2) {
	int rez, boja1, boja2, br_boja1, br_boja2, prostor1, prostor2, i, t, v1, v2;
	boja1 = boja(matrica, epruveta1);
	boja2 = boja(matrica, epruveta2);
	br_boja1 = br_boja(matrica, epruveta1);
	br_boja2 = br_boja(matrica, epruveta2);
	prostor1 = br_slobodnih(matrica, epruveta1);
	prostor2 = br_slobodnih(matrica, epruveta2);
	v1 = prostor1;
	v2 = prostor2;

	if (prostor2 == 0)return 0;
	if ((boja1 == boja2 || boja2 == 0) && boja1 != 0) {
		if (prostor2 >= br_boja1) {
			for (i = 0; i < br_boja1; i++) {
				t = matrica[v1][epruveta1];
				matrica[v1][epruveta1] = matrica[v2 - 1][epruveta2];
				matrica[v2 - 1][epruveta2] = t;
				v1++;
				v2--;
			}
			return 1;
		}
		else {
			for (i = 0; i < prostor2; i++) {
				t = matrica[v1][epruveta1];
				matrica[v1][epruveta1] = matrica[v2 - 1][epruveta2];
				matrica[v2 - 1][epruveta2] = t;
				v1++;
				v2--;
			}
			return 1;
		}
	}
	else return 0;


}

int provera_stanja(int** matrica, int br_epruveta) {
	int i, j, brojac, vrh;
	brojac = 0;
	for (j = 0; j < br_epruveta; j++) {
		vrh = matrica[0][j];
		for (i = 1; i < 4; i++) {
			if (matrica[i][j] != vrh)return 0;
		}
	}
	return 1;

}

struct Stack {
	int top;
	unsigned capacity;
	cvor** array;
};

struct Stack* createStack(unsigned capacity)
{
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = malloc(stack->capacity * sizeof(cvor*));
	return stack;
}

int isFull(struct Stack* stack)
{
	return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack)
{
	return stack->top == -1;
}

void push(struct Stack* stack, cvor* item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
}

cvor* pop(struct Stack* stack)
{
	if (isEmpty(stack))
		return;
	return stack->array[stack->top--];
}
cvor* peek(struct Stack* stack)
{
	if (isEmpty(stack))
		return NULL;
	return stack->array[stack->top];
}

void stampanje(int** matrica, int n) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < n; j++) {
			printf("|%d| ", matrica[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

void preorder(cvor* koren) {
	int i;
	struct Stack* stack1 = createStack(1000);
	push(stack1, koren);
	cvor* trenutni;
	while (!isEmpty(stack1)) {
		trenutni = pop(stack1);
		if (trenutni->br_sinova != 0) {
			for (i = trenutni->br_sinova - 1; i > -1; i--) {
				push(stack1, trenutni->sinovi[i]);
			}
		}
		stampanje(trenutni->matrica, trenutni->br_epruveta);
		printf("%d\n", trenutni->dubina);
	}
}

int poredjenje(int** m1, int** m2, int n) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < n; j++) {
			if (m1[i][j] != m2[i][j])return 0;
		}
	}
	return 1;
}

int postoji_li_resenje(cvor* trenutni) {
	int i, j;
	struct Stack* stek = createStack(1000);
	cvor* sledeci;
	push(stek, trenutni);
	while (!isEmpty(stek)) {
		sledeci = pop(stek);
		if (provera_stanja(sledeci->matrica, sledeci->br_epruveta) == 1) {
			return 1;
		}
		else {
			for (j = 0; j < sledeci->br_sinova; j++) {
				push(stek, sledeci->sinovi[j]);
			}
		}
	}
	return 0;
}

void najbolji_potez(cvor* trenutni) {
	int i;
	for (i = 0; i < trenutni->br_sinova; i++) {
		if (postoji_li_resenje(trenutni->sinovi[i])) {
			printf("Potez koji vodi ka resenju je presipanje iz %d-te epruvete u %d-u epruvetu\n", trenutni->sinovi[i]->prva_epruveta, trenutni->sinovi[i]->druga_epruveta);
			break;
		}
	}
}

void put_ka_resenju(cvor* trenutni) {
	int i;
	cvor* novi = trenutni;
	while (provera_stanja(novi->matrica, novi->br_epruveta) != 1) {
		for (i = 0; i < novi->br_sinova; i++) {
			if (postoji_li_resenje(novi->sinovi[i])) {
				stampanje(novi->matrica, novi->br_epruveta);
				novi = novi->sinovi[i];
				break;
			}
		}
	}
	stampanje(novi->matrica, novi->br_epruveta);

}

int main() {

	int* fib, * p;
	int n, k, j, i, f, br_poteza, opcija, flag, prva, druga;

	int** matrica;
	int** korisnicka;

	time_t t;
	srand((unsigned)time(&t));

	printf("Unesite ukupan broj epruveta i broj praznih epruveta:");
	scanf("%d %d", &n, &k);
	printf("Unesite ukupan broj poteza do kraja igre:");
	scanf("%d", &br_poteza);

	//generisanje seed-a
	fib = (int*)malloc(1000 * sizeof(int));
	for (i = 0; i < 10; i++) {
		fib[i] = rand();
	}

	//generisanje pseudoslucajnih brojeva
	for (i = 10; i < 1000; i++) {
		fib[i] = (fib[i - 7] + fib[i - 10]) % (n - k) + 1;
	}

	//niz koji govori koliko se neka boja puta ponovila do sad odnosno da li se pojavila do 4 puta
	p = (int*)malloc((n - k) * sizeof(int));
	for (i = 1; i < (n - k) + 1; i++) {
		p[i] = 0;
	}


	//generisanje pocetne kombinacije
	f = 10;
	matrica = malloc(4 * sizeof(int*));
	for (i = 0; i < 4; i++) {
		matrica[i] = malloc(n * sizeof(int));
		for (j = 0; j < n - k; j++) {
			if (p[fib[f]] < 4) {
				matrica[i][j] = fib[f];
				p[fib[f]]++;
				f++;
			}
			else {
				j--;
				f++;
			}
		}
		for (j = n - k; j < n; j++) {
			matrica[i][j] = 0;
		}
	}


	//pocetni slucaj
	for (i = 0; i < 4; i++) {
		for (j = 0; j < n; j++) {
			printf("|%d| ", matrica[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');

	cvor* koren = pravljenje_cvora(matrica, n, 0, 0, 0);
	struct Stack* stack = createStack(1000);
	push(stack, koren);
	cvor* trenutni;
	while (!isEmpty(stack)) {
		trenutni = pop(stack);
		if (trenutni->dubina != br_poteza && provera_stanja(trenutni->matrica, trenutni->br_epruveta) == 0) {
			pravljenje_sinova(trenutni, (trenutni->dubina) + 1);
			for (i = 0; i < trenutni->br_sinova; i++) {
				push(stack, trenutni->sinovi[i]);

			}

		}
	}
	cvor* trenutno_stanje = koren;
	flag = 1;
	while (flag <= br_poteza) {
		if (postoji_li_resenje(trenutno_stanje) == 0) {
			printf("Izgubili ste :(");
			return 0;
		}
		printf("1.Odigrati potez\n");
		printf("2.Pomoc\n");
		printf("3.Resenje\n");
		printf("4.Preorder\n");
		printf("5.Izlaz\n");
		printf("Izaberite neku od opcija:");
		scanf("%d", &opcija);
		switch (opcija) {
		case 1:
			printf("Izaberite iz koje epruvete presipate u koju:");
			scanf("%d %d", &prva, &druga);
			if (prebacivanje(matrica, prva, druga) == 1) {
				for (i = 0; i < trenutno_stanje->br_sinova; i++) {
					if (poredjenje(trenutno_stanje->sinovi[i]->matrica, matrica, n) == 1) {
						trenutno_stanje = trenutno_stanje->sinovi[i];
						break;
					}
				}
			}
			stampanje(matrica, n);
			if (provera_stanja(matrica, n)) {
				printf("Igra je zavrsena. Pobedili ste :)");
				return 0;
			}
			flag++;
			break;
		case 2:
			najbolji_potez(trenutno_stanje);
			break;
		case 3:
			put_ka_resenju(trenutno_stanje);
			return 0;
			break;
		case 4:
			preorder(koren);
		case 5:
			return 0;
		default:
			printf("Lose uneta opcija.Pokusajte opet.\n");
			break;

		}
	}


	return 0;
}