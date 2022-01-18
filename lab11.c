#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <hash_map>


#define MAX 30
#define MAX_HASH 11

struct _TreeRoot;
typedef struct _TreeRoot* TreePosition;
typedef struct _TreeRoot {
	char City[MAX];
	int population;
	TreePosition left;
	TreePosition right;
} TreeRoot;

struct _ListRoot;
typedef struct _ListRoot* ListPosition;
typedef struct _ListRoot {
	char state[MAX];

	TreePosition Cities;

	ListPosition next;
} ListRoot;

struct _HashT;
typedef struct _HashT* HashTab;
typedef struct _HashT {
	ListPosition* Head;
} HashT;

ListPosition CreateMember();
ListPosition FindState(HashTab, char*);
TreePosition CreateTreeMember();

int AddStateInHashTab(HashTab, char*, char*);
int ReadStateFile(HashTab, char*);
int AddStateInTheList(ListPosition, char*, char*);
TreePosition AddCityToTheTree(TreePosition, char*, int);
TreePosition ReadFileCities(TreePosition, char*);
int PrintStateCities(ListPosition);
int PrintTreeInOrder(TreePosition);

int Choose(HashTab);
int FindCityPopulationsBiggerThanX(TreePosition, int);
HashTab CreateHashTab();




int main() {

	HashTab hashTab = NULL;

	hashTab = CreateHashTab();

	ReadStateFile(hashTab, "States.txt");

	Choose(hashTab);

	printf("\nPrint eneter key to exit the programm.");
	fseek(stdin, 0, SEEK_END);
	getchar();
	return EXIT_SUCCESS;
}


int Choose(HashTab hashTab) {
	ListPosition q = NULL;
	char tmp1[MAX];
	int tmp2;
	printf(" Choose name of the state you want to search: ");
	scanf(" %s", tmp1);
	printf("Find all cities that have more than x inhabitants : ");
	scanf("%d", &tmp2);
	q = FindState(hashTab, tmp1);
	if (!q) {
		printf("Given state does not exist!");
		return EXIT_FAILURE;
	}
	FindCityPopulationsBiggerThanX(q - Cities, tmp2);
	return EXIT_SUCCESS;
}

int FindCityPopulationsBiggerThanX(TreePosition p, int populationX) {
	if (p == NULL) {
		return EXIT_SUCCESS;
	}

	FindCityPopulationsBiggerThanX(p->left, populationX);

	if (p->population > populationX) {
		printf("%s %d, ", p->City, p->population);
	}
	FindCityPopulationsBiggerThanX(p->right, populationX);

	return EXIT_SUCCESS;
}

ListPosition CreateMember() {
	ListPosition q = malloc(sizeof(ListRoot));
	if (q == NULL) {
		printf("Can't allocate memory");
		return NULL;
	}
	strcpy(q->state, "");
	q - Cities = NULL;
	q->next = NULL;
	return q;
}

TreePosition CreateTreeMember(char* City, int population) {
	TreePosition q = malloc(sizeof(Tree));
	if (q == NULL) {
		printf("Can't allocate memory");
		return NULL;
	}
	strcpy(q->City, City);
	q->population = population;
	q->left = NULL;
	q->right = NULL;
	return q;
}

int PrintTreeInOrder(TreePosition p) {
	if (p == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		PrintTreeInOrder(p->left);
		printf("%s %d, ", p->City, p->population);
		PrintTreeInOrder(p->right);
	}
	return EXIT_SUCCESS;
}

int ReadStateFile(HashTab hashTab, char* FileName) {
	char StateName[MAX] = "", FileCityName[MAX] = "";

	FILE* dat = NULL;
	dat = fopen(FileName, "r");
	if (!dat) {
		printf("Can't open the file.");
		return EXIT_FAILURE;
	}

	while (!feof(dat)) {
		fscanf(dat, "%s %s", StateName, FileCityName);
		AddStateInHashTab(hashTab, StateName, FileCityName);
	}
	fclose(dat);
	return EXIT_SUCCESS;
}

int AddStateInTheList(ListPosition Head, char* state, char* FileCityName) {
	ListPosition tmp = NULL;
	char tmp1[MAX], tmp2[MAX];

	tmp = CreateMember();
	strcpy(tmp->state, state);

	tmp - Cities = ReadFileCities(tmp - Cities, FileCityName);

	strcpy(tmp1, state);

	while (Head->next != NULL) {
		strcpy(tmp2, Head->next->state);
		if (strcmp(strupr(tmp1), strupr(tmp2)) > 0) {
			Head = Head->next;
			continue;
		}
		break;
	}

	tmp->next = Head->next;
	Head->next = tmp;
}

TreePosition ReadFileCities(TreePosition headTree, char* FileName) {
	char CityName[MAX] = "";
	int Citypopulation = 0;
	FILE* dat = NULL;

	dat = fopen(FileName, "r");
	if (!dat) {
		printf("Can't open the file.");
		return NULL;
	}

	while (!feof(dat)) {
		fscanf(dat, "%s %d", CityName, &Citypopulation);
		headTree = AddCityToTheTree(headTree, CityName, Citypopulation);
	}
	fclose(dat);
	return headTree;
}

TreePosition AddCityToTheTree(TreePosition p, char* City, int population) {
	if (p == NULL) {
		p = CreateTreeMember(City, population);
	}
	else if (population == p->population) {
		if (strcmp(City, p->City) < 0) {
			p->left = AddCityToTheTree(p->left, City, population);
		}
		if (strcmp(City, p->City) > 0) {
			p->right = AddCityToTheTree(p->right, City, population);
		}
	}
	else {
		if (population < p->population) {
			p->left = AddCityToTheTree(p->left, City, population);
		}
		if (population > p->population) {
			p->right = AddCityToTheTree(p->right, City, population);
		}
	}
	return p;
}

ListPosition FindState(HashTab hashTab, char* state) {
	int index = HashFunction(state);
	ListPosition Head = hashTab->Head[index];

	while (Head->next != NULL) {
		if (strcmp(state, Head->next->state) == 0) {
			return Head->next;
		}
		else {
			Head = Head->next;
		}
	}
	return NULL;



	int HashFunction(char* nameState) {
		int rez = 0;
		for (int i = 0; i < strlen(nameState) || i < 5; i++) {
			rez += nameState[i];
		}
		return rez % MAX_HASH;
	}

	HashTab CreateHashTab() {
		HashTab hash = NULL;
		int i = 0;

		hash = (HashTab)malloc(sizeof(HashT));
		if (hash == NULL) {
			printf("Can't allocate memory");
			return NULL;
		}
		hash->Head = (ListPosition*)malloc(sizeof(ListPosition) * MAX_HASH);
		if (hash->Head == NULL) {
			printf("Can't allocate memory");
			return NULL;
		}

		for (i = 0; i < MAX_HASH; i++) {
			hash->Head[i] = CreateMember();
		}
		return hash;
	}

	int AddStateInHashTab(HashTab hashTab, char* StateName, char* FileCityName) {
		int  index = 0;
		index = HashFunction(StateName);

		AddStateInTheList(hashTab->Head[index], StateName, FileCityName);
	}