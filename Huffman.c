#include <stdio.h>
#include <stdlib.h>

typedef struct node *Pnode; /* wskaznik na wezel drzewa  */
typedef struct node{
	char S;
	int Q;
	Pnode left, right, p, next, prev;
}Tnode;

typedef struct k
{
	char chain[20];
	int L;
}code;

typedef struct fcr
{
	char S;
	int p;
}fcret;

Pnode WL;
code codebook[128];

void ReadCode(Pnode w)
{
	char chain[20];
	int L=0;
	int t;
	
	Pnode tmp=w;
	while(tmp!=WL)
	{
		if(tmp->p!=WL && tmp==tmp->p->right)
		{
			chain[L]='1';
			L++;
		}
		if(tmp->p!=WL && tmp==tmp->p->left)
		{
			chain[L]='0';
			L++;
		}
		tmp=tmp->p;	
	}
	
	//Przepisanie
	codebook[w->S].L=L;
	t=0;
	while(t < codebook[w->S].L)
	{
		L--;
		codebook[w->S].chain[t]=chain[L];
		t++;
	}
}

void InOrder(Pnode w)
{
	if(w->left!=NULL)
	{
		InOrder(w->left);
	}
	
	if(w->left==NULL && w->right==NULL)
	{
		ReadCode(w);
	}
	
	if(w->right!=NULL)
	{
		InOrder(w->right);
	}
}

void Insert(Pnode N)
{
	N->next=WL->next;
	N->prev=WL;
	WL->next->prev=N;
	WL->next=N;
}

Pnode ExtractMin()
{
	int min=100000;
	Pnode M;
	
	Pnode tmp=WL->next;
	while(tmp!=WL)
	{
		if(tmp->Q < min)
		{
			min=tmp->Q;
			M=tmp;
		}
		tmp=tmp->next;
	}
	
	M->prev->next=M->next;
	M->next->prev=M->prev;
	M->next=NULL;
	M->prev=NULL;
	return M;
}

fcret FindChar(char ctab[], int p)
{
	Pnode tmp=WL->next;
	while(1)
	{
		if(ctab[p]=='1')
		{
			tmp=tmp->right;
		}
		if(ctab[p]=='0')
		{
			tmp=tmp->left;
		}
		p++;
	
		if(tmp->right==NULL && tmp->left==NULL)
		{
			break;
		}
	} 
	fcret r;
	r.S=tmp->S;
	r.p=p;
	return r;
}

void DeCode()
{
	char tab[2000];
	int len=0;
	FILE *plik;
	plik = fopen("code.txt", "r");
	while (feof(plik)==0)
	{
		fscanf (plik, "%c", &tab[len]);
		len++;
	}
	len--;
	fclose(plik);
	
	int I=0;
	int P=0;
	int i;
	
	fcret r;
	
	while(P<len)
	{
		r=FindChar(tab,P);
		
		tab[I]=r.S;
		P=r.p;
		
		I++;	
	}
	
	for(i=0;i<I;i++)
	{
		printf("%c", tab[i]);
	}
}

void Code(char tab[], int Len)
{
	FILE *wynik;
	wynik = fopen("code.txt", "w");
	int i;
	int j;
	for(i=0;i<Len;i++)
	{
		
		for(j=0;j<codebook[tab[i]].L;j++)
		{
			fputc(codebook[tab[i]].chain[j], wynik);
		}		
	}
	fclose(wynik);
}

int main()
{
	int Len=0;
	int Alphabet=0;
	
	char str[500];
	FILE *plik;
	plik = fopen("huff.txt", "r");
	if(plik==NULL)
	{
		printf("Blad otwarcia pliku z kodem\n");
	}
	else
	{
	while (feof(plik)==0)
	{
		fscanf (plik, "%c", &str[Len]);
		Len++;
	}
	Len--;
	fclose(plik);
	
	int quan[128];
	int i;
	for (i=0;i<128;i++)//Zerowanie tablicy quantity
	{
		quan[i]=0;
	}
	for (i=0;i<Len;i++)//Wprowadzenie ilosci
	{
		quan[str[i]]++;
	}
	
	WL=(Pnode)malloc(sizeof(Tnode)); //Utworzenie listy i dodanie wezlow
	WL->next=WL;
	WL->prev=WL;
	
	for (i=0;i<128;i++)
	{
		if( quan[i]>0 )
		{
			Pnode N=(Pnode)malloc(sizeof(Tnode));
			N->left=NULL;
			N->right=NULL;
			N->Q=quan[i];
			N->S=i;
			Insert(N);
			
			Alphabet++;//Zwiekszenie rozmiaru alfabetu
		}
	}
	
	while(WL->next->next!=WL)//Formowanie drzewa
	{
		Pnode X=ExtractMin();
		Pnode Y=ExtractMin();
		Pnode Z=(Pnode)malloc(sizeof(Tnode));
		
		Z->left=X;
		X->p=Z;
		
		Z->right=Y;
		Y->p=Z;
		
		Z->p=WL;
		
		Z->Q=X->Q+Y->Q;
		Insert(Z);	
	}
	
	//Zapelnienie ksiegi kodowej
	InOrder(WL->next);
	//Wypisanie ksiegi kodowej
	for (i=0;i<128;i++)
	{
		if( quan[i]>0 )
		{
			printf("%c %d ",i,codebook[i].L);
			
			int m=0;
			while(m<codebook[i].L)
			{
				printf("%c",codebook[i].chain[m]);
				m++;
			}
			printf("\n");
		}
	}
	
	//Porownanie
	int OriginRoom=0;
	int CodedRoom=0;
	
	if(Alphabet<=2){OriginRoom=Len;}
	if(Alphabet>2 && Alphabet <=4){OriginRoom=Len*2;}
	if(Alphabet>4 && Alphabet <=8){OriginRoom=Len*3;}
	if(Alphabet>8 && Alphabet <=16){OriginRoom=Len*4;}
	if(Alphabet>16 && Alphabet <=32){OriginRoom=Len*5;}
	if(Alphabet>32 && Alphabet <=64){OriginRoom=Len*6;}
	if(Alphabet>64 && Alphabet <=128){OriginRoom=Len*7;}
	
	for (i=0;i<128;i++)
	{
		if( quan[i]>0 )
		{
			CodedRoom=CodedRoom + quan[i]*codebook[i].L;
		}
	}
	printf("Dlugosc: %d\n", Len);
	printf("Rozmiar alfabetu: %d\n", Alphabet);
	printf("Oryginalne miejsce: %d\n",OriginRoom);
	printf("Kodowane miejsce: %d\n",CodedRoom);
	
	Code(str,Len);
	DeCode();
	}
	getchar();
	return 0;
}
