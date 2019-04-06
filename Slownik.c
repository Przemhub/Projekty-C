#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#define SIZE 40
#define SIZE2 30
//program otwiera plik tekstowy (.txt), z zawartosci tworzy slownik z iloscia wystapien
//slow posortowanych w kolejnosci alfabetycznej i zapisuje go do pliku tekstowego ze zmodyfikowana nazwa
	struct word_count_t {
	  char *word;
	  int counter;
	};
	struct dictionary_t
	{
	  int size;
	  int capacity;
	  struct word_count_t *wc;
	};
void delete_char(char *tab)
{
	int i=0;
	for(;*(tab+i)!='\0';i++)
	{
		if(!((*(tab+i)>=97&&*(tab+i)<=122)||(*(tab+i)<=90)&&*(tab+i)>=65))
		{
			int j=i;
			while(*(tab+j+1)!='\0')
			{
				*(tab+j)=*(tab+j+1);
				j++;
			}
			*(tab+j)='\0';	
		}
	}
}
 struct dictionary_t* create_dictionary(int N);
 void destroy_dictionary(struct dictionary_t** d);
 struct word_count_t* dictionary_find_word(const struct dictionary_t *d, const char *word);
  void dictionary_display(const struct dictionary_t *d);
  int dictionary_add_word(struct dictionary_t *d, const char *word);
  int dictionary_sort_occurence(struct dictionary_t *d);
  int dictionary_sort_alphabetically(struct dictionary_t *d);
   struct dictionary_t* load_dictionary_b(const char *filename, int *err_code);
   int save_dictionary_b(const struct dictionary_t *d, const char *filename);
   
   int main()
   {
   		FILE *file;
	char *word=(char*)malloc(sizeof(char)*SIZE);
	   //dictionary_display(arr);
	if(word==NULL)
	{
		printf("Failed to allocate memory");
		return 4;	
	}
	char *filename=(char*)malloc(sizeof(char)*SIZE);
	if(filename==NULL)
	{
		free(word);
		printf("Failed to allocate memory");
		return 4;
	}
	printf("Podaj nazwe pliku: ");
	fgets(filename,SIZE,stdin);
	*(filename+strlen(filename)-1)='\0';
	file =fopen(filename,"r");
	if(file==NULL)
	{
		printf("File not found");
		free(word);
		free(filename);
		return 2;
	}
	int count=0;
	while(fscanf(file,"%30s",word)>0)
	{
		count++; 
	}
	
	struct dictionary_t *d1=create_dictionary(count);
	
	if(d1==NULL)
	{
		printf("Failed to allocate memory");
		free(word);
		free(filename); 
		fclose(file);
		return 4;
	}
	
	
	printf("Co chcesz zrobic?: ");
	int choice=0;
	scanf("%d",&choice);
	fseek(file,0,0);
	if(choice==0)
	{
		int err_code=0;
		d1=load_dictionary_b(filename,&err_code);
		if(d1==NULL)
		{
			if(err_code==3)
			{
				printf("Error z danymi z pliku");
				return 1;
			}
		}
	}else if(choice==1)
	{
	while(fscanf(file,"%30s",word)!=-1)
	{  
		delete_char(word);
		int flag=dictionary_add_word(d1,word);
		if(flag==2)
		{
			printf("Failed to allocate memory");
			free(word);
			free(filename); 
			destroy_dictionary(&d1);
			fclose(file);
			return 4;
		}
		free(word);
		fclose(file);
	}
	}else
	{
		printf("Incorrect input");
			free(word);
			free(filename); 
			destroy_dictionary(&d1);
			fclose(file);
		return 1;
	}
	//sortuje slownik
	int len=strlen(filename);
	dictionary_sort_alphabetically(d1);
	
	//tworzy i alokuje pamiec do nazwy pliku modFilename
	char *modFilename=(char*)malloc(sizeof(char)*(len+6));
	if(modFilename==NULL)
	{
			printf("Failed to allocate memory");
			free(filename); 
			destroy_dictionary(&d1);
			return 4;
	}
	
	*(filename+strlen(filename)-4)='\0';//aby miec nazwe pliku bez koncowki .bin
	const char *sufx= "_alpha.bin";
	strcpy(modFilename,filename);
	strcat(modFilename,sufx);

	//zapisuje posortowany alfabetycznie d1 do pliku
	if(save_dictionary_b(d1,modFilename)==2)
	{
		printf("Couldn't create file");
		return 2;
	}
	printf("File saved\n");
	
	dictionary_sort_occurence(d1);
	//zmienia nazwe pliku modFilname dla drugiego sortowania
	const char *sufx2= "_occ.bin";
	strcpy(modFilename,filename);
	strcat(modFilename,sufx2);
	
	if(save_dictionary_b(d1,modFilename)==2)
	{
		printf("Couldn't create file");
		return 2;
	}
	printf("File saved");
	
	
	
	free(modFilename);
	free(filename); 
	destroy_dictionary(&d1);
	return 0;
   }
   
   int save_dictionary_b(const struct dictionary_t *d, const char *filename)
   {
		if(filename==NULL||d==NULL||d->capacity<=0||d->size<0||d->size>d->capacity||d->wc==NULL)
		{
				return 1;
		}
			FILE* file=fopen(filename,"wb");
			if(file==NULL)
			{
				return 2;
			}
		fwrite(&d->size,sizeof(int),1,file);
		int i=0;
	 	for(;i<d->size;i++)
	 	{
	 		//z terminatorem, a moze trzeba bez??
	 		int len=strlen((d->wc+i)->word);
	 		fwrite(&len,sizeof(int),1,file);
	 		fwrite((d->wc+i)->word,sizeof(char),len,file);
	 		fwrite(&(d->wc+i)->counter,sizeof(int),1,file);
		 }
		 fclose(file);
		 return 0;
   }
   
   struct dictionary_t* load_dictionary_b(const char *filename, int *err_code)
   {
   		if(err_code==NULL)
   		{
   			return NULL;
		}
		if(filename==NULL)
		{
			*err_code=1;
			return NULL;
		}
		FILE* file=fopen(filename,"rb");
		if(file==NULL)
		{
			*err_code=2;
			return NULL;
		}
		int cap;	
		if(fread(&cap,sizeof(int),1,file)<=0)
		{
			*err_code = 3;
			fclose(file);
			return NULL;
		}
		struct dictionary_t *d=create_dictionary(cap);
		if(d==NULL)
		{
			*err_code==4;
			fclose(file);
			return NULL;
		}
		
		int countr=0;
		char *word;
		while(!feof(file))
		{
			int len=1;
		
			if(fread(&len,sizeof(int),1,file)<=0)
			{
				if(feof(file))
				break;
				*err_code=3;
				destroy_dictionary(&d);
				fclose(file);
				if(word!=NULL)
				free(word);
				return NULL;
			}
			word=(char*)calloc(len+1,sizeof(char));
			if(fread(word,sizeof(char),len,file)<len)
			{
				*err_code=3;
				destroy_dictionary(&d);
				free(word);
				fclose(file);
				return NULL;
			}
			
			if(fread(&countr,sizeof(int),1,file)<=0)
			{
				*err_code=3;
				destroy_dictionary(&d);
				fclose(file);
				free(word);
				return NULL;
			}
			while(countr>0)
			{
				dictionary_add_word(d,word);
				countr--;
			}
			
		}
		fclose(file);
		return d;
   }

   int dictionary_sort_alphabetically(struct dictionary_t *d)
   {
   			if(d==NULL||d->capacity<=0||d->size<0||d->wc==NULL||d->size>d->capacity)
			{
				return 1;
			}
   		int n=0;
   		do
   		{
   			n=0;
   			int i=0;
   			for(;i<d->size-1;i++)
   		{
   			int j=0;
   			while(*((d->wc+i)->word+j)!='\0'&&*((d->wc+i+1)->word+j)!='\0')
			{
				
			   	if(*((d->wc+i)->word+j)>*((d->wc+i+1)->word+j))
			   	{
			   		struct word_count_t storage=*(d->wc+i+1);
			   		*(d->wc+i+1)=*(d->wc+i);
			   		*(d->wc+i)=storage;
			   		n++;
			   		break;
			   		
				 }else if(*((d->wc+i)->word+j)<*((d->wc+i+1)->word+j))
				 break;
			
				 j++;
			}
		}
	
		   }while(n!=0);
   		
   		return 0;
   }
   int dictionary_sort_occurence(struct dictionary_t *d)
   {
   		if(d==NULL||d->capacity<=0||d->size<0||d->wc==NULL||d->size>d->capacity)
		{
			return 1;
		}
		int n=0;
   		do
   		{
   			n=0;
   			int i=0;
   			for(;i<d->size-1;i++)
   		{
			   	if((d->wc+i)->counter>(d->wc+i+1)->counter)
			   	{
			   		struct word_count_t storage=*(d->wc+i+1);
			   		*(d->wc+i+1)=*(d->wc+i);
			   		*(d->wc+i)=storage;
			   		n++;
			   		break;
				 }		
		}
		  }while(n!=0);
		return 0;
   }
   void dictionary_display(const struct dictionary_t *d)
{
	if(d!=NULL&&d->capacity>=1&&d->size>=0&&d->size<=d->capacity&&d->wc!=NULL)
	{
		int i=0;
		for(;i<d->size;i++)
		{
			printf("%s %d\n",(d->wc+i)->word,(d->wc+i)->counter);
		 }
	}
}
void destroy_dictionary(struct dictionary_t** d)
{
	if(d!=NULL&&*d!=NULL)
	{
		if((*d)->wc!=NULL)
		{
			int i=0;
		for(;i<(*d)->size;i++)
		{
			free(((*d)->wc+i)->word);
		}
		
		free((*d)->wc);
		(*d)->wc=NULL;
		}
		free(*d);
		*d=NULL;
	}
}
struct word_count_t* dictionary_find_word(const struct dictionary_t *d, const char *word)
{
	if(word==NULL||d==NULL||d->capacity<=0||d->size<0||d->wc==NULL||d->size>d->capacity)
	{
		return NULL;
	}
//	printf("%s\nw petli\n",word);
int i=0;
	for(;i<d->size;i++)
	{
	//	printf("%s\n",(d->wc+i)->word);
		//printf("%d ",i);
	//	printf("tu");
		if(strcmp((d->wc+i)->word,word)==0)
		{
		//	printf("\n");
			return (d->wc+i);
		}
	//	printf("tu2");
	}
	//printf("tu3\n");
	//printf("\n");
	return NULL;
}
struct dictionary_t* create_dictionary(int N)
{
	if(N<=0)
	{
		return NULL;
	}
	struct dictionary_t *dict=(struct dictionary_t*)malloc(sizeof(struct dictionary_t));
	if(dict==NULL)
	{
		return NULL;
	}
	dict->size=0;
	dict->capacity=N;
	dict->wc=(struct word_count_t*)malloc(sizeof(struct word_count_t)*N);
	if(dict->wc==NULL)
	{
		free(dict);
		return NULL;
	}	
	int i=0;
	for(;i<dict->capacity;i++)
	{
		(dict->wc+i)->counter=0;
	}
	return dict;
}
int dictionary_add_word(struct dictionary_t *d, const char *word)
{
	if(word==NULL||d==NULL||d->capacity<=0||d->size<0||d->wc==NULL)
	return 1;
//	printf("tu1");
	if(dictionary_find_word(d,word)==NULL)
	{
	//	printf("tu2");
		if(d->size==d->capacity)
		{
			struct word_count_t *wc=(struct word_count_t*)malloc(sizeof(struct word_count_t)*d->capacity);
			if(wc==NULL)
			{
				return 2;
			}
			free(wc);
			d->capacity*=2;
			d->wc=(struct word_count_t*)realloc(d->wc,sizeof(struct word_count_t)*d->capacity);					
		}
	//	printf("tu3");
		int len=(int)strlen(word)+1;
		(d->wc+d->size)->word=(char*)calloc(len,sizeof(char));
//		printf("tu4");
		if((d->wc+d->size)->word==NULL)
		{
			free((d->wc+d->size)->word);
			return 2;
		}
	//	printf("tu5");
		strcpy((d->wc+d->size)->word,word);
		((d->wc+d->size)->counter)=1;
		d->size++;	
	}else
	{
	//		printf("tu6\n");
	struct dictionary_t d2=*d;
		
		d2.wc=dictionary_find_word(d,word);
		int j=0;
		for(;strcmp((d->wc+j)->word,d2.wc->word)!=0;j++);
		(d->wc+j)->counter++;
	}
//	printf("tu7\n");
	return 0;
}
