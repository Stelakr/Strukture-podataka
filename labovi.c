#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define IME 128
#define MAX 1024


typedef struct {
	char ime[IME];
	char prezime[IME];
	double bodovi;
}Student;

int Brojstudenata(char* datoteka) {
	FILE* fp = NULL;
	int brojac = 0;
	char buffer[MAX] = { 0 };
	fp = fopen(datoteka, "r");

	if (!fp) {
		printf("Greska!");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		brojac++;
	}
	fclose(fp);
	
	return brojac;
}


Student* Alokacija(int brojStudenata, char* datoteka) {

	int brojac = 0;
	FILE* fp = NULL;
	Student* student = NULL;

	student = (Student*)malloc(brojStudenata * sizeof(Student));
	fp = fopen(datoteka, "r");

	if (!fp) {
		printf("Greska alociranja memorije.");
		free(student);
		return NULL;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s %s %lf", student[brojac].ime, student[brojac].prezime, &student[brojac].bodovi);
		brojac++;
	}

	fclose(fp);
	return student;
}



int Ispis(Student* student, int brojStudenata) {

	double Relativni = 0;
	int Bodmax = 0;

	for (int i = 1; i < brojStudenata; i++) {
		if (student[i].bodovi > student[Bodmax].bodovi)
			Bodmax = i;
	}



	for (int i = 0; i < brojStudenata; i++) {

		Relativni = (double) (student[i].bodovi / student[Bodmax].bodovi) * 100;
		printf("%s ", student[i].ime);
		printf("%s ", student[i].prezime);
		printf("%lf ", student[i].bodovi);
		printf("%lf", Relativni);
		printf("\n");

		Relativni = 0;
	}

	return 0;
}


int main()
{
	char* datoteka = "Source.txt";
	int brojStudenata = 0;
	Student* student;


	brojStudenata = Brojstudenata(datoteka);
	printf("%d", brojStudenata);

	student = Alokacija(brojStudenata, datoteka);

	Ispis(student, brojStudenata);




	return 0;
}