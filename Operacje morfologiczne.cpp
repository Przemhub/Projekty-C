#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 40
#define MAX2 44
//odczytuje plik zdjeciowy [jpg/png] i wykonuje operacje aytmetyczne na bitach pikseli ktore
//daja efekt odwrocenia horyzontalnie lub wertykalnie obraz lub zanegowanie kolorow
//zapisuje zdjecie do pliku z nazwa zmodyfikowana
struct image_t {
  char type[3];
  int **ptr;
  int width;
  int height;
};
struct image_t* image_flip_horizontal(const struct image_t *m1);
struct image_t* image_flip_vertical(const struct image_t *m1);
struct image_t* image_negate(const struct image_t *m1);
struct image_t* load_image_t(const char *filename, int *err_code);
int save_image_t(const char * filename, const struct image_t *m1);
void destroy_image(struct image_t **m);
int main()
{
	 struct image_t *arr;
	 struct image_t *arr2;
	 int err_code=0;
	 char *filename=(char*)malloc(sizeof(char)*MAX);
	 if(filename==NULL)
	 {
	 	printf("Failed to allocate memory");
	 	return 4;
	 }
	 
	 
	printf("Podaj nazwe pliku: ");
	fgets(filename,MAX,stdin);
	*(filename+strlen(filename)-1)='\0';
	
	arr=load_image_t(filename,&err_code);
	if(arr==NULL)
	{
		if(err_code==4)
		{
			printf("Failed to allocate memory");
			free(filename);
			return err_code;	
		}else if(err_code==2)
		{
			printf("File not found");
			free(filename);
			return  err_code;
		}else if(err_code==3)
		{
			printf("File corrupted");;
		free(filename);
			return err_code;
		}
		
	}
	
	arr2=image_flip_horizontal(arr);
	if(arr2==NULL)
	{
			printf("Failed to allocate memory");
			destroy_image(&arr);
		free(filename);
			return 4;	
	}
	char *modFilename=(char*)malloc(sizeof(char)*MAX2);
		if(modFilename==NULL)
	 {
	 	printf("Failed to allocate memory");
	 	destroy_image(&arr);
		destroy_image(&arr2);
		free(filename);
		free(modFilename);
	 	return 4;
	 }
	const char* prefx1="mirror_h_";
	strcpy(modFilename,prefx1);
	strcat(modFilename,filename);
	int result=save_image_t(modFilename,arr2);
	if(result==2)
	{
		printf("Couldn't create file\n");
	}else
	printf("File saved\n");
	destroy_image(&arr2);
	arr2=image_flip_vertical(arr);
	if(arr2==NULL)
	{
			printf("Failed to allocate memory");
			destroy_image(&arr);
		free(filename);
		free(modFilename);
			return 4;		
	}
		
	const char *prefx2="mirror_v_";
	strcpy(modFilename,prefx2);
	strcat(modFilename,filename);
	result=save_image_t(modFilename,arr2);
	if(result==2)
	{
		printf("Couldn't create file\n");
	}else
	printf("File saved\n");
	destroy_image(&arr2);
	arr2=image_negate(arr);
	if(arr2==NULL)
	{
		printf("Failed to allocate memory");
			destroy_image(&arr);
		free(filename);
		free(modFilename);
			return 4;
	}	
	
	const char *prefx3="invert_";	
	strcpy(modFilename,prefx3);
	strcat(modFilename,filename);
	result=save_image_t(modFilename,arr2);
	if(result==2)
	{
		printf("Couldn't create file");
	}else
	printf("File saved");

	destroy_image(&arr);
	destroy_image(&arr2);
	free(filename);
	free(modFilename);
	return 0;
}
struct image_t* load_image_t(const char *filename, int *err_code)
{
	if(err_code!=NULL)
	{
		if(filename==NULL)
		{
		*err_code=1;
		return NULL;
		}
	FILE *file;
	file=fopen(filename,"r");
	if(file==NULL)
	{
		*err_code=2;
		return NULL;
	}
	struct image_t *img;
	img=(struct image_t*)malloc(sizeof(struct image_t));
	if(img==NULL)
	{
		fclose(file);
		*err_code=4;
		return NULL;
	}

	

	if(feof(file)||fscanf(file,"%3s",img->type)==-1)
	{
	
		*err_code=3;
		fclose(file);
			free(img);
		return NULL;
	}
	if(feof(file)||fscanf(file,"%d",&img->width)==-1||img->width<=0)
	{
		
		*err_code=3;
			fclose(file);
		free(img);
		return NULL;
	}
	if(feof(file)||fscanf(file,"%d",&img->height)==-1||img->height<=0)
	{
		
		*err_code=3;
		free(img);
			fclose(file);
		return NULL;
	}
	img->ptr=(int**)malloc(sizeof(int*)*img->height);
	if(img->ptr==NULL)
	{
		*err_code=4;
		free(img);
			fclose(file);
		return NULL;
	}
	for(int i=0;i<img->height;i++)
	{
		*(img->ptr+i)=(int*)malloc(sizeof(int)*img->width);
		if(*(img->ptr+i)==NULL)
		{
			while(i>0)
			{
				i--;
				free(*(img->ptr+i));		
			}
			free(img->ptr);	
			img->ptr=NULL;
			free(img);
			img=NULL;
				fclose(file);
			*err_code=4;
			return NULL;
		}	
	}
	int number;
	if(feof(file)||fscanf(file,"%d",&number)==-1||number<=0)
	{
		*err_code=3;
		destroy_image(&img);
			fclose(file);
		return NULL;
	}
	if(feof(file))
	{
		
		*err_code=3;
		destroy_image(&img);
			fclose(file);
		return NULL;
	}
		if(*(img->type+1)=='3')
	{
		*err_code=3;
		destroy_image(&img);
			fclose(file);
		return NULL;
	}else if(*(img->type+1)=='2'&&number!=255)
	{
		*err_code=3;
	destroy_image(&img);
			fclose(file);
		return NULL;
	}else if(*(img->type+1)=='1'&&number!=8)
	{
		*err_code=3;
	destroy_image(&img);
			fclose(file);
		return NULL;
	}
	
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				if(fscanf(file,"%d",(*(img->ptr+i)+j))<=0)
				{
					*err_code=3;
					for(int i=0;i<img->height;i++)
					{
						free(*(img->ptr+i));
					}
					free(img->ptr);	
					img->ptr=NULL;
					free(img);
					img=NULL;
						fclose(file);
					return NULL;
				}
			}
		} 
	*err_code=0;
	fclose(file);
	return img;
	}else
	{
	if(filename==NULL)
	{
		return NULL;
	}
	FILE *file;
	file=fopen(filename,"r");
	if(file==NULL)
	{
		return NULL;
	}
	fseek(file,0,0);
	struct image_t *img;
	img=(struct image_t*)malloc(sizeof(struct image_t));
	if(img==NULL)
	{
		fclose(file);
		return NULL;
	}
	if(feof(file)||fscanf(file,"%3s",img->type)==-1)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}
	if(feof(file)||fscanf(file,"%d",&img->width)==-1||img->width<=0)
	{
		free(img);
		img=NULL;
		fclose(file);
		return NULL;
	}
	if(feof(file)||fscanf(file,"%d",&img->height)==-1||img->height<=0)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}
	int number;
	if(feof(file)||fscanf(file,"%d",&number)==-1||number<=0)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}
	if(feof(file))
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;	
	}
	if(*(img->type+1)=='3')
	{
		free(img);
			fclose(file);
		return NULL;
	}else if(*(img->type+1)=='2'&&number!=255)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}else if(*(img->type+1)=='1'&&number!=8)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}
	img->ptr=(int**)malloc(sizeof(int*)*img->height);
	if(img->ptr==NULL)
	{
		free(img);
		img=NULL;
			fclose(file);
		return NULL;
	}
	for(int i=0;i<img->height;i++)
	{
		*(img->ptr+i)=(int*)malloc(sizeof(int)*img->width);
		if(*(img->ptr+i)==NULL)
		{
			while(i>0)
			{
				i--;
				free(*(img->ptr+i));		
			}
			free(img->ptr);	
			free(img);
				fclose(file);
			return NULL;
		}	
	}
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				if(fscanf(file,"%d",(*(img->ptr+i)+j))<=0)
				{
					for(int i=0;i<img->height;i++)
					{
						free(*(img->ptr+i));
					}
					free(img->ptr);	
					free(img);
						fclose(file);
					return NULL;
				}
			}
		} 
	fclose(file);
	return img;	
	}
	
}

int save_image_t(const char * filename, const struct image_t *m1)
{
	if(filename==NULL||m1==NULL||m1->ptr==NULL||m1->height<=0||m1->width<=0)
	{
		return 1;
	}
	FILE *file;
	
	file=fopen(filename,"w");
	if(file==NULL)
	{
		return 2;
	}
	fprintf(file,"%s\n",m1->type);
	fprintf(file,"%d %d\n",m1->width,m1->height);
	fprintf(file,"255\n");
	for(int i=0;i<m1->height;i++)
	{
		for(int j=0;j<m1->width;j++)
		{
			fprintf(file,"%d ",*(*(m1->ptr+i)+j));
		}
			fprintf(file,"\n");
	}
	fclose(file);
	return 0;
}
void destroy_image(struct image_t **m)
{
	if(*m!=NULL)
	{
		if(((*m)->ptr)!=NULL)
		{
			for(int i=0;i<(*m)->height;i++)
			{
				free(*((*m)->ptr+i));
			}
			free((*m)->ptr);
		} 
		free(*m);
	}
	
}
struct image_t* image_flip_horizontal(const struct image_t *m1)
{
	if(m1==NULL||m1->ptr==NULL||m1->height<=0||m1->width<=0)
			return NULL;
		struct image_t *img=(struct image_t*)malloc(sizeof(struct image_t));
		if(img==NULL)
		{
			return NULL;
		}
		img->height=m1->height;
		img->width=m1->width;
		strcpy(img->type,m1->type);
		img->ptr=(int**)malloc(sizeof(int*)*m1->height);
		if(img->ptr==NULL)
		{
			free(img);
			return NULL;
		}
		for(int i=0;i<img->height;i++)
		{
			*(img->ptr+i)=(int*)malloc(sizeof(int)*m1->width);
			if(*(img->ptr+i)==NULL)
			{
				while(i>0)
				{
					i--;
					free(*(img->ptr+i));		
				}
				free(img->ptr);	
				free(img);
				return NULL;
			}	
		}
	for(int i=0;i<m1->height;i++)
	{
		for(int j=0;j<m1->width;j++)
		{
			*(*(img->ptr+i)+j)=*(*(m1->ptr+m1->height-i-1)+j);
		}
	}
	return img;
}
struct image_t* image_flip_vertical(const struct image_t *m1)
{
		if(m1==NULL||m1->ptr==NULL||m1->height<=0||m1->width<=0)
			return NULL;
		struct image_t *img=(struct image_t*)malloc(sizeof(struct image_t));
		if(img==NULL)
		{
			return NULL;
		}
		img->height=m1->height;
		img->width=m1->width;
		strcpy(img->type,m1->type);
		img->ptr=(int**)malloc(sizeof(int*)*m1->height);
		if(img->ptr==NULL)
		{
			free(img);
			return NULL;
		}
		for(int i=0;i<img->height;i++)
		{
			*(img->ptr+i)=(int*)malloc(sizeof(int)*m1->width);
			if(*(img->ptr+i)==NULL)
			{
				while(i>0)
				{
					i--;
					free(*(img->ptr+i));		
				}
				free(img->ptr);	
				free(img);
				return NULL;
			}	
		}
	
		for(int i=0;i<m1->height;i++)
		{
			for(int j=0;j<m1->width;j++)
			{
				*(*(img->ptr+i)+j)=*(*(m1->ptr+i)+m1->width-j-1);
			}
		}
		return img;
}
struct image_t* image_negate(const struct image_t *m1)
{
	
		if(m1==NULL||m1->ptr==NULL||m1->height<=0||m1->width<=0)
			return NULL;
		struct image_t *img=(struct image_t*)malloc(sizeof(struct image_t));
		if(img==NULL)
		{
			
			return NULL;
		}
		img->height=m1->height;
		img->width=m1->width;
		strcpy(img->type,m1->type);
		img->ptr=(int**)malloc(sizeof(int*)*m1->height);
		if(img->ptr==NULL)
		{
			free(img);
			return NULL;
		}
		for(int i=0;i<img->height;i++)
		{
			*(img->ptr+i)=(int*)malloc(sizeof(int)*m1->width);
			if(*(img->ptr+i)==NULL)
			{
				while(i>0)
				{
					i--;
					free(*(img->ptr+i));		
				}
				free(img->ptr);	
				free(img);
				return NULL;
			}	
		}
	
		for(int i=0;i<m1->height;i++)
		{
			for(int j=0;j<m1->width;j++)
			{
				*(*(img->ptr+i)+j)=255-*(*(m1->ptr+i)+j);
			
			}
		}
		return img;
}
