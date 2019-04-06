#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 100
#define LEN 40
//proces tworzy baze danych studentow [imie,nazwisko,indeks] ktora nastepnie zapisuje do pliku tekstowego
//uzytkownika ma 2 opcje utworzenie i zapis bazy danych [s] lub zaladowanie bazy z pliku [l]
//wpisanie 0 w polu imie zakancza wprowadzanie danych
//wszystko odbywa sie przy dynamicznej alokacji pamieci
//przyklad:
//Jaka operacje chcialbys wykonac?(s/l)): s
//Podaj nazwe pliku: plik
//wpisz dane: Jan,Kowalski,111111
//wpisz dane: Andrzej,Muras,222222
//wpisz dane: 0
//Plik zapisany
struct student_t
{
    char *name;
    char *surname;
    int index;
};
int fill(char *t)
{
	scanf("%c",t);
	if(*t=='0')
	return 2;
	if(*t!=' ')
	t++;
	
	while(*(t-1)!='\n')
	{
		scanf("%c",t++);
		if(*(t-1)==',')
		{
			*(t-1)='\0';
			return 1;
		}
		
	} 
	*(t-1)='\0';
	return 0;
}
void destroy(struct student_t **s);
struct student_t* read(int *err_code);
int save_student_b(const struct student_t *s, FILE *file);
struct student_t* load_student_b(FILE *file);
void show(const struct student_t* p);
int main()
{
	

	char op;
	printf("Jaka operacje chcialbys wykonac?(s/l)): ");
	scanf("%c",&op);
	getchar();
	char *filename=(char*)malloc(sizeof(char)*LEN);
		if(filename==NULL)
		{
			printf("Failed to allocate memory");
			return 0;
		}
	if(op=='l'||op=='L')
	{
		printf("Podaj nazwe pliku: ");
		fgets(filename,LEN,stdin);
		*(filename+strlen(filename)-1)='\0';
		FILE *file=fopen(filename,"rb");
		if(file==NULL)
		{
			printf("Plik nie istnieje.");
			return 2;
		}
		while(1==1)
		{
		struct student_t *student=load_student_b(file);
		if(student==NULL)
		{
			free(filename);
			printf("error");
			return 0;
		}
		
		
		
		if(*student->name=='0')
		{
			free(student);
			free(student->name);
			break;
		}
		show(student);		
		destroy(&student);
		}
		
		free(filename);
		
		return 0;
	}else if(op=='s'||op=='S')
	{
		printf("Podaj nazwe pliku: ");
		fgets(filename,LEN,stdin);
		*(filename+strlen(filename)-1)='\0';
		FILE *file=fopen(filename,"wb");
		if(file==NULL)
		{
			printf("Nie mozna stworzyc pliku.");
			return 2;
		}
		int err_code;
		while(err_code!=4)
		{
			struct student_t *student = read(&err_code);
		if(err_code!=0&&err_code!=4)
		{
			printf("Err_code %d",err_code);
			free(filename);
			fclose(file);
			return err_code;
		}	
		getchar();
		if(err_code==4)
		{
			
			free(student->name);
			free(student);
			break;
		}
		int result = save_student_b(student,file);
		destroy(&student);

		}
		
		printf("Plik zapisany");
		free(filename);			
		fclose(file);
		return 0;
	}else
	{
		printf("Niepoprawne wejscie (s/l))");
		return 0;
	}
	
	
}
void show(const struct student_t* p)
{
	if(p!=NULL)
	printf("%s %s, %d\n",p->name,p->surname,p->index);
	
}
int save_student_b(const struct student_t *s, FILE *file)
{
	if(s==NULL)
	{
		return 1;
	}
	if(file==NULL)
	{
		return 2;
	}
	int len=(int)strlen(s->name);
	fwrite(&len,sizeof(int),1,file);
	fwrite(s->name,sizeof(char),len,file);
	len=(int)strlen(s->surname);
	fwrite(&len,sizeof(int),1,file);
	fwrite(s->surname,sizeof(char),len,file);
	fwrite(&s->index,sizeof(int),1,file);
	return 0;
}
struct student_t* load_student_b(FILE *file)
{
	if(file==NULL)
	{
		return NULL;
	}
	if(file==NULL)
	{
		return NULL;
	}
	struct student_t *student=(struct student_t*)malloc(sizeof(struct student_t));
	
	if(student==NULL)
	{
		fclose(file);
		return NULL;
	}
	unsigned int len=1;
	//wczytanie dlugosci imienia. Czy ta dlugosc wlicza terminator?
	fread(&len,sizeof(int),1,file);
	student->name=(char*)calloc(len+1,sizeof(char));
	if(student->name==NULL)
	{
		
		free(student);
		fclose(file);
		return NULL;
	}

	if(fread(student->name,sizeof(char),len,file)<len)
	{
		printf("da name: %s\n",student->name);
		free(student->name);
		free(student);
		fclose(file);
		return NULL;
	}
		
	//wczytywanie dlugosci nazwiska i wpisywanie nazwiska z pliku do elem struk
	fread(&len,sizeof(int),1,file);
	student->surname=(char*)calloc(len+1,sizeof(char));
	if(student->surname==NULL)
	{
		free(student->name);
		free(student);
		fclose(file);
		return NULL;
	}
	if(fread(student->surname,sizeof(char),len,file)<len)
	{
		free(student->name);
		free(student->surname);
		free(student);
		fclose(file);
		return NULL;
	}
	if(fread(&student->index,sizeof(int),1,file)<=0)
	{
		free(student->name);
		free(student->surname);
		free(student);
		fclose(file);
		return NULL;
	}

	if(student->index<=0)
	{
		free(student->name);
		free(student->surname);
		free(student);
		fclose(file);
		return NULL;
	}
	return student;
	
}
struct student_t* read(int *err_code)
{
	if(err_code==NULL)
	{
		return NULL;
	}
	struct student_t* p;
	p=(struct student_t*)malloc(sizeof(struct student_t));
	if(p==NULL)
	{
		*err_code=1;
		return NULL;
	}
	p->name=(char*)malloc(sizeof(char)*(SIZE));		
		if(p->name==NULL)
	{
		*err_code=1;
		free(p);
		return NULL;
	}
	printf("wpisz dane: ");
	int outcome = fill(p->name);
		if(outcome==0)
	{
		*err_code=2;
		free(p->name);
		free(p);
		return NULL;
	}else if(outcome==2)
	{ 
		*err_code=4;
		return p;
	}
	
	p->surname=(char*)malloc(sizeof(char)*(SIZE));
	if(p->surname==NULL)
	{
		*err_code=1;
		free(p->name);
		free(p);
		return NULL;	
	}
		if(fill(p->surname)==0)
	{
		*err_code=2;
		free(p->name);
		free(p->surname);
		free(p);
		return NULL;
	}
		if(scanf("%d",&p->index)==0)
	{
		*err_code=3;
		free(p->name);
		free(p->surname);
		free(p);
		return NULL;	
	}
	else
	*err_code=0;
	
	return p;
}
void destroy(struct student_t **s)
{
	if(s!=NULL)
	{
		free((*s)->name);
		free((*s)->surname);	
		free(*s);	
	}
}
