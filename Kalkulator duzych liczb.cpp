#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define SIZE 500
//program wykonujacy arytmetyczne obliczenia dodawania odejmowania i mnozenia na liczbach wielkosci nawet 10^50
//przyklad wejscia programu:
//wprowadz wyrazenie: 45531*21232+1252-32122
//wynik: 966683322
int add(const char* number1, const char* number2, char** result);
int validate(const char * number);
int compare(const char* number1, const char* number2);
int subtract_inside(const char* number1, const char* number2, char* result,int size);
int subtract(const char* number1, const char* number2, char **result);
int add_inside(const char* number1, const char* number2, char* result, int size);
int multiply_inside(const char*,const char*,char*);
int multiply(const char* number1, const char* number2, char **result);
int validate_expression(const char *expr);
void delete_zeros(char*,char*);
void delet_zeros(char *number1);
int calculate(const char* expr, char **result);
int main()
{
	char *expr=(char*)malloc(SIZE*sizeof(char));
	if(expr==NULL)
	{
		printf("Failed to allocate memory");
		return 3;
	}
	printf("Podaj wyrazenie: ");
	
	fgets(expr,SIZE,stdin);
	*(expr+strlen(expr)-1)='\0';
		char *result;
	int outcome = calculate(expr,&result);
	if(outcome==3)
	{
		printf("Failed to allocate memory");
		free(expr);
		return 3;
	}
	if(outcome==1||outcome==2)
	{
		printf("Incorrect input");
	
	free(expr);
		return 2;
	}
	printf("wynik: %s",result);
	free(result);
	free(expr);
    return 0;		
}

int calculate(const char* expr, char **result)
{
	if(expr==NULL||result==NULL)
	return 1;
	if(validate_expression(expr)==1)
	{
		return 2;
	}
	int len=0;
	char sym;
	if(*(expr+len)=='-')
	len++;
	for(;*(expr+len)>='0'&&*(expr+len)<='9';len++);//dlugosc number1
	char *number1=(char*)calloc((len+1),sizeof(char));
	if(number1==NULL)
	{
		return 3;
	}
	for(int i=0;i<len;i++)
	{
		*(number1+i)=*(expr+i);
	}	
	if(*(expr+len)=='\0')
	{
		*result=(char*)malloc(sizeof(char)*(len+1));
		if(*result==NULL)
		{
			free(number1);
			return 3;
		}
		strcpy(*result,number1);
		free(number1);
		return 0;
	}
	
	//printf("number1 %s\n",number1);
	int i=len+1;//czytanie tekstu przesuniete o dlugosc number1
	len=0;//dlugosc number2
	sym=*(expr+i-1);
//	printf("%c",sym);
//int count=0;
	char *number2;
	while(1)
	{
		
		if(((*(expr+i)=='+'||*(expr+i)=='-'||*(expr+i)=='*')&&(*(expr+i-1)>='0'&&*(expr+i-1)<='9'))||*(expr+i)=='\0')
		{	
		//printf("%c ",*(expr+i));
	
				number2=(char*)calloc((len+1),sizeof(char));
				if(number2==NULL)
				{
					free(number1);
					return 3;
				}
				if(*(expr+i)=='\0')
				{
					//printf("Koncowka\n");
				}
				//printf("%c\n",*(expr+i-len));
				for(int j=i-len,k=0;k<len;j++,k++)
				{
					
					*(number2+k)=*(expr+j);
				//	printf("%c\n",*(expr+j));
				}	
				//printf("number2 %s\n",number2);
				//printf("%c\n",*(expr+i));
					switch(sym)
					{
						case '+':
						//	printf("%s+%s\n",number1,number2);
							if(add(number1,number2,result)==2)
							{
								return 3;	
							}
								
							break;
						case '-':
						//	printf("%s-%s\n",number1,number2);
							if(subtract(number1,number2,result)==2)
							{
								return 3;
							}
							
							break;
						case '*':
						//	printf("%s*%s\n",number1,number2);
							if(multiply(number1,number2,result)==2)
							{
								return 3;
							}
							break;
						default:
							printf("Error?");
							break;
					}
				sym=*(expr+i);	
				len=-1;
				free(number2);
				free(number1);
				number1=(char*)calloc(strlen(*result)+2,sizeof(char));
				if(number1==NULL)
				{
					free(*result);
					return 3;
				}
				for(int j=0;j<(int)strlen(*result);j++)
				{
					*(number1+j)=*(*result+j);
				}
				
				if(*(expr+i)!='\0')
				{
					
					free(*result);	
					//count++;
				}
				
				if(*(expr+i)=='\0')
				{
					break;
				}
				
				
		}
		i++;
		len++;
		
	}

	free(number1);
	return 0;
}

int validate_expression(const char *expr)
{
	if(expr==NULL)
	return 2;
	if((*expr<'0'||*expr>'9')&&*expr!='-')
	return 1;
	while(*expr!='\0')
	{
		if(*expr<'0'||*expr>'9')
		{
			if(*expr=='-'||*expr=='+'||*expr=='*')
			{
				if(*(expr+1)!='-'&&(*(expr+1)<'0'||*(expr+1)>'9'))
				{
					return 1;
				}
			}else
			return 1;
		}
		expr++;
	}
	return 0;
}
int validate(const char * number)
{
	if(number==NULL)
	return 2;
	if(*number=='\0'||*number=='\n')
	return 1;
	if(*number=='-')
	number++;
	if(*number=='\0'||*number=='\n')
	return 1;
	while(*number!='\0'&&*number!='\n')
	{
		if(*number<'0'||*number>'9')
		return 1;
		number++;
	}
	if(*number=='\n')
	{
		return 1;	
	}
	return 0;
}
int add(const char* number1, const char* number2, char** result)
{   
   if(number1==NULL||number2==NULL||result==NULL)
    {
        return 1;
    }
    if(validate(number1)==1||validate(number2)==1)
    {
        return 2;
    }
        int size=0;
        int s1=0,s2=0;
 	s1=(int)strlen(number1);
 	s2=(int)strlen(number2);
	if(s1>s2)
	size=s1;
	else
	size=s2;
    *result=(char*)malloc(sizeof(char)*(size+4));
    if(*result==NULL)
    {
        return 3;
    }
    if(*number1=='-')
    {
        if(*number2=='-')
        {
           number1++;
           number2++;
           if(*number1=='\0'||*number2=='\0')
           {
           	**result='0';
           	*(*result+1)='\0';
           	return 1;
		   }
           
            while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
           // printf("number1 == - number2==-\n");
            add_inside(number1,number2,*result,size+1);      
            for(int i=(int)strlen(*result)+1;i>0;i--)
            {
                *(*(result)+i)=*(*(result)+i-1);
            }
            *(*result)='-';
        }else
        {
        	number1++;
            while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
            //printf("number1 == - number2==+\n");
            if(compare(number1,number2)==1)
                {
                //   printf("number1>number2");
                subtract_inside(number1,number2,*result,size+2);
              //  printf("po odjeciu number-number2\n");
                 for(int i=(int)strlen(*result)+1;i>0;i--)
                            {
                            *(*(result)+i)=*(*(result)+i-1);    
                            //printf("przesuwa result\n");
                            }
                          //  printf("po dodaniu minusa\n");
                        *(*result)='-';
                }
                else if(compare(number1,number2)==-1)
                {
                //	printf("%s\n\n%s\n",number1,number2);
                //	printf("number2>number1\n");
                    subtract_inside(number2,number1,*result,size+1); 
                       
                }else
                {
                //	printf("number2==number1");
                            *(*(result))='0';
                            *(*(result)+1)='\0';                                
                }
            }       
    }else
    {
        if(*number2=='-')
        {
            number2++;
            while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
           // printf("number1 == + number2==-\n");
            if(compare(number1,number2)==-1)
                {
                    
                subtract_inside(number2,number1,*result,size+1);
                for(int i=(int)strlen(*result)+1;i>0;i--)
                            {
                            *(*(result)+i)=*(*(result)+i-1);    
                            }
                        *(*result)='-';
                }
                else if(compare(number1,number2)==1)
                {
                    subtract_inside(number1,number2,*result,size+1);
                        
                }else
                {
                            *(*(result))='0';
                            *(*(result)+1)='\0';                                
                }
            
        }else
        {
        //	printf("number1 == + number2==+\n");
        	 while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
            add_inside(number1,number2,*result,size+1);
            
        }
	}
	
    return 0;
}
int subtract(const char* number1, const char* number2, char **result)
{
   if(number1==NULL||number2==NULL||result==NULL)
    {
        return 1;
    }
    if(validate(number1)==1||validate(number2)==1)
    {
        return 2;
    }  
        int size=0;
        int s1=0,s2=0;
    s1=(int)strlen(number1);
    s2=(int)strlen(number2);
	if(s1>s2)
	{
		size=s1;
	}else
	size=s2;
    *result=(char*)calloc(size+4,sizeof(char));
    if(*result==NULL)
    return 3;
    if(*number1=='-')
    {
        if(*number2!='-')
        {
        	if(*number2=='0'&&*(number2+1)=='\0')
        	{
        		strcpy(*result,number1);
        		return 0;
        	}
            number1++;  
             while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
            add_inside(number1,number2,*result,size+1);
            for(int i=(int)strlen(*result)+1;i>0;i--)
            {
                *(*(result)+i)=*(*(result)+i-1);
            }
            **(result)='-';
        }
        if(*number2=='-')
        {
        	  if(*number1=='\0'||*number2=='\0')
           {
           	**result='0';
           	*(*result+1)='\0';
           	return 2;
		   }
        	number1++;
        	number2++;
        	 while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
            if(compare(number1,number2)==-1)
                {
                   
            subtract_inside(number2,number1,*result,size+1);
                
                }
                else if(compare(number1,number2)==1)
                {
                    subtract_inside(number1,number2,*result,size+1);
                    for(int i=(int)strlen(*result)+1;i>0;i--)
                {
                    *(*(result)+i)=*(*(result)+i-1);    
                }
                **(result)='-';
                    
                    }else
                    {
                        *(*(result))='0';
                        *(*(result)+1)='\0';
                    }
               }
                
        }
        else 
        {
        if(*number2!='-')
        { 
        	if(*number2=='0'&&*(number2+1)=='\0')
        	{
        		strcpy(*result,number1);
        		return 0;
        	}
        	if(*number1=='0'&&*(number1+1)=='\0')
        	{
        		strcpy(*result,number2);
        		for(int i=(int)strlen(*result)+1;i>0;i--)
                {
                    *(*(result)+i)=*(*(result)+i-1);    
                }
                *(*result)='-';
        		return 0;
        	}
        	while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
                if(compare(number1,number2)==-1)
                {
                 
           	 subtract_inside(number2,number1,*result,size+1);    
                for(int i=(int)strlen(*result)+1;i>0;i--)
                {
                    *(*(result)+i)=*(*(result)+i-1);    
                }
                *(*result)='-';
                }else if(compare(number1,number2)==1)
                {
                    
                    subtract_inside(number1,number2,*result,size+1);
                }else
                {
                    *(*(result))='0';
                    *(*(result)+1)='\0';
                }
        }else 
        {
        		if(*number1=='0'&&*(number1+1)=='\0')
        	{
        		number2++;
        		strcpy(*result,number2);
        		return 0;
        	}
        	number2++;
        	 while(*number1=='0')
            number1++;
            while(*number2=='0')
            number2++;
				add_inside(number1,number2,*result,size+1);    
        }
    }
    return 0;
}
int multiply(const char* number1, const char* number2, char **result)
{
	 if(number1==NULL||number2==NULL||result==NULL)
    {
        return 1;
    }
    if(validate(number1)==1||validate(number2)==1)
    {
        return 2;
    }  
        int size=0;
        int s1=0,s2=0;
    s1=(int)strlen(number1);
    s2=(int)strlen(number2);
	if(s1>s2)
	{
		size=s1;
	}else
	size=s2;
    *result=(char*)calloc((size*2+1),sizeof(char));
    if(*result==NULL)
    return 3;
    if(*number1=='0'&&*(number1+1)=='\0')
    	{
    		**result='0';
    		*(*result+1)='\0';
    		return 0;
		}
		if(*number2=='0'&&*(number2+1)=='\0')
    	{
    		**result='0';
    		*(*result+1)='\0';
    		return 0;
		}
    if(*number1=='-'&&*number2!='-')
    {
    	
    	number1++;
    	while(*number1=='0')
    	number1++;
    	while(*number2=='0')
    	number2++;
    	
    	multiply_inside(number1,number2,*result);
    	 for(int i=(int)strlen(*result)+1;i>0;i--)
           {
               *(*(result)+i)=*(*(result)+i-1);    
           }
	      *(*result)='-';
    	
	}else if(*number1!='-'&&*number2=='-')
	{
		
		number2++;
    	while(*number1=='0')
    	number1++;
    	while(*number2=='0')
    	number2++;
    	multiply_inside(number1,number2,*result);
    	 for(int i=(int)strlen(*result)+1;i>0;i--)
           {
               *(*(result)+i)=*(*(result)+i-1);    
           }
	      *(*result)='-';
	}else
	{
		if(*number1=='-')
		number1++;
		if(*number2=='-')
		number2++;
		while(*number1=='0')
    	number1++;
    	while(*number2=='0')
    	number2++;
    	multiply_inside(number1,number2,*result);
    	//printf("%s*%s=%s\n",number1,number2,*result);
	}
	
	return 0;
}
int subtract_inside(const char* number1, const char* number2, char* result,int size)
{
	if(size<=(int)strlen(number1)||size<=(int)strlen(number2))
	{
		return 0;
	}
	if(strlen(number1)>strlen(number2))
	size=strlen(number1);
	else
	size=strlen(number2);
	char memory=0;
	const char *wsk1=number1;
	const char *wsk2=number2;
		while(*number1)
		number1++;
		number1--;
		while(*number2)
		number2++;
		number2--;
		
	for(int i=0;i<size;i++)
	{
		*result=0;
		
		if((number1<wsk1&&number2<wsk2)&&memory==1)
		{
			*result-=(memory+48);
		}		
		else 
		{
		*result-=memory;	
		}
		
		 memory=0;
		if(number1>=wsk1&&number2>=wsk2)
		{
			*result+=(*number1-*number2);
			*result+=48;
		}else
		if(number1>=wsk1&&number2<wsk2)
		{
			*result+=(*number1);
		
		}else
		if(number1<wsk1&&number2>=wsk2)
		{
			*result+=(*number2);
		}
		
		if(*result<48)	
		{
			*result+=10;
			memory=1;
		}
		//printf("%c %c %c\n",*number1,*number2,*result);
		result++;
		number1--;
		number2--;	
		
	}
	if(*(result-1)==48)
	*(result-1)='\0';
	else
	*result='\0';
	result-=size; 
	int n=0;
	while(*(result+n)!='\0')
	n++;
	char stor;
	int j=n-1;
	int i =0;
	
	while(i<j)
	{	
		stor=*(result+i);
		*(result+i)=*(result+j);
		*(result+j)=stor;
		i++;j--;
	}
		if(*(result)=='0')
	{
		delet_zeros(result);	
	}
	if(*result=='\0')
    {
        *result='0';
        *(result+1)='\0';   
    }
    delet_zeros(result);
	return 1;
}
int compare(const char* number1, const char* number2)
{
	if(number1==NULL||number2==NULL)
	return 2;
//	printf("%s %s\n",number1,number2);
	if(validate(number1)==2||validate(number1)==1||validate(number2)==2||validate(number2)==1)
	return 3;
	if(*number1=='0'||*number2=='0')
	return 3;
	if(strlen(number1)>strlen(number2))
	{
	//	printf("strlen n1>n2\n");
		return 1;
	}else if(strlen(number2)>strlen(number1))
	{
	//	printf("strlen n1<n2\n");
		return -1;
	}
	
	if(*number1=='-')
	{
		if(*number2!='-')
		return -1;
		else
		{
			number1++;
			number2++;
			while(*number1!='\0'&&*number2!='\0')
			{
				if(*number1>*number2)
				return -1;
				else if(*number1<*number2) 
				return 1;
				
				number1++;
				number2++;
			}
		}
	}else
	{
		if(*number2!='-')
		{
			while(*number1!='\0'&&*number2!='\0')
			{
				if(*number1>*number2)
				return 1;
				else if(*number1<*number2) 
				return -1;
				number1++;
				number2++;
			}
		}
		else
		{
			return 1;			
		}
	}
	
	//printf("%lld %lld\n",num1,num2);
	return 0;
	
}
int add_inside(const char* number1, const char* number2, char* result, int size)
{
    char memory=0;
    if(size<=(int)strlen(number1)||size<=(int)strlen(number2))
    {
        return 0;
    }
    const char *wsk1=number1;
    const char *wsk2=number2;
        while(*number1)
        number1++;
        number1--;
        while(*number2)
        number2++;
        number2--;
    for(int i=0;i<size;i++)
    {
        *result=0;
        if((number1<wsk1&&number2<wsk2)&&memory==1)
        {
            *result+=(memory+48);
        }       
        else 
        {
        *result+=memory;    
        }
        
         memory=0;
        if(number1>=wsk1&&number2>=wsk2)
        {
            *result+=(*number1+*number2);
            *result-=48;
        }else
        if(number1>=wsk1&&number2<wsk2)
        {
            *result+=(*number1);
            
        }else
        if(number1<wsk1&&number2>=wsk2)
        {
            *result+=(*number2);
        }
        if(*result>57)  
        {
            *result-=10;
            memory=1;
        }
    
        result++;
        number1--;
        number2--;  
    }
   // printf("%s+%s=%s\n",number1,number2,result);
    *result='\0';
    result-=size; 
    int n=0;
    while(*(result+n)!='\0')
    n++;
    char stor;
    int j=n-1;
    int i =0;
    

    while(i<j)
    {   
        stor=*(result+i);
        *(result+i)=*(result+j);
        *(result+j)=stor;
        i++;j--;
    }
     
   
    if(*result=='\0')
    {
        *result='0';
        *(result+1)='\0';   
    }
    delet_zeros(result);
    return 1;
}
int multiply_inside(const char* number1, const char* number2, char* result)
{
   	int n1 = strlen(number1);
    int n2 = strlen(number2);   
	int left_shift=0;
	int i;
    for(i = n2-1; i >= 0; i--)
    {
        int carry = 0, k = left_shift;
        for(int j = n1-1; j >= 0; j--)
        {
            int partial_result = (*(number1+j) - '0')*(*(number2+i) - '0') + carry;
            if(*(result+k))
                partial_result += *(result+k) - '0';
            *(result+k) = partial_result%10 + '0';
            k++;
            carry = partial_result/10;  
        }
        if(carry > 0)
            *(result+k) = carry +'0'; 
        left_shift++;
    }
    
	int n=0;
    while(*(result+n)!='\0')
    n++;
    char stor;
    int j=n-1;
    i =0;
	  while(i<j)
    {   
        stor=*(result+i);
        *(result+i)=*(result+j);
        *(result+j)=stor;
        i++;j--;
    }
   
    if(*result=='\0')
    {
        *result='0';
        *(result+1)='\0';   
    }
    delet_zeros(result);
    return 1;
}




void delet_zeros(char *number1)
{
	int i=0;
	if(*number1=='-')
	{
		number1++;
	
	}
		while(*(number1+i)=='0')
		{
			i++;
		}
		int j=0;
		while(*(number1+j)!='\0')
		{
			*(number1+j)=*(number1+i+j);
			j++;
		}
		*(number1+j)='\0';
		
}

