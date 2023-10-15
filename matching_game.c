#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX 15   // kullanýlacak max sayi

void drawBoard(int matris[][MAX], int boyut);
void readFromFile(int matris[][MAX], char *fileName);
void randomMatrix(int matris[][MAX],int boyut);
int random(int range);
void move(int ind1,int ind2, int number,int matris[][MAX],int boyut);
void undo(int ind1,int ind2,int matris[][MAX]);
void playAuto(int matris[][MAX], int boyut);
int playManual(int matris[][MAX],int boyut);
int matrixSize(char* fileName);
int getNearestEqualNumber(int rowIndex, int colIndex, int matris[][MAX], int boyut);
int calculateScore(int randomMatrixCounter,int undoCounter,int manualCounter,double sure,int boyut);
void resetMatrix(int matris[][MAX], int boyut);

int main(){
    int i,j,matris[MAX][MAX]={{0}}; 
    int boyut; // kullanicidan aliniz
    int cikis=0,bitis=1,menu=5;
    char fileName[20];	
    char userNames[5][MAX];//kullanýcý adý
	int scores[5]={0}; // kullanýcý skoru
	clock_t bas,son;
	double sure;
	int k=0,randomMatrixCounter=0,manualCounter=0,undoCounter=0;

	/* buradan itibaren menü vb kodlarý olmalidir. dosya okumak için asagidaki kod verilmistir */

	do{
		int score,user,finish=5;
		printf("1.Rasgele Matris Olustur \n2.Dosyadan Matris Olustur \n3.Kullanicilarin Skorlarini Goster \nCikmak icin 4'e basiniz.\n");
		scanf("%d",&menu);
		switch(menu){
			case 1: //RASTGELE MATRÝS OLUÞTURMA VE OYUN KISMI
				randomMatrixCounter++;
				printf("Kullanici adinizi giriniz. ");
				scanf("%s",userNames[k]);
				while(finish!=1){
					printf("1.Manuel Modda Oyna \n2.Oto Modda Oyna \n3.Ana Menuye Don\n");
					scanf("%d",&menu);
					switch(menu){
						case 1:
							manualCounter++;
							printf("Lutfen matris boyutunu giriniz. ");
							scanf("%d",&boyut);
							randomMatrix(matris,boyut);
							bas=clock();
							undoCounter=playManual(matris,boyut);
							son=clock();
							sure=(double)(son-bas)/CLOCKS_PER_SEC;
							break;//manual mod
						case 2:
							printf("Lutfen matris boyutunu giriniz. ");
							scanf("%d",&boyut);
							randomMatrix(matris,boyut);
							playAuto(matris,boyut);
							break;//oto mod
						case 3:
							finish=1;
							resetMatrix(matris,boyut);
							break;// ana menüye dön
				}
				if(menu!=3){
					printf("Eger bir tur daha oynamak istemiyorsaniz lutfen 1 e basiniz. Aksi taktirde herhangi bir rakama basiniz.  ");
					scanf("%d",&finish);	
					resetMatrix(matris,boyut);
				}
		
			}
				break;// Rastgele Matris Oluþtur
				
			case 2:// DOSYADAN MATRÝS OKUMA VE OYUN KISMI
				printf("Kullanici adinizi giriniz. ");
				scanf("%s",userNames[k]);
				while(finish!=1){
					printf("1.Manuel Modda Oyna \n2.Oto Modda Oyna \n3.Ana Menuye Don\n");
					scanf("%d",&menu);				
  					switch(menu){
						case 1:
							manualCounter++;
							printf("Dosya Adini Giriniz: ");
	  						scanf("%s",fileName);
  							readFromFile(matris, fileName);
							undoCounter=playManual(matris,matrixSize(fileName));
							break;//manual mod
						case 2:
							printf("Dosya Adini Giriniz: ");
  							scanf("%s",fileName);
  							readFromFile(matris, fileName);	
							playAuto(matris,matrixSize(fileName));					
							break;//oto mod
						case 3:
							finish=1;
							resetMatrix(matris,boyut);
							break;// ana menüye dön
					}
					if(menu!=3){
						printf("Eger bir tur daha oynamak istemiyorsaniz lutfen 1 e basiniz. Aksi taktirde herhangi bir rakama basiniz.  ");
						scanf("%d",&finish);	
						resetMatrix(matris,boyut);
					}
				}
  				break;//Dosyadan Matris Oluþtur
  				
			case 3: //KULLANICI SKORLARINI GÖSTERME KISMI
				for(i=0;i<k;i++){
				printf("%s adli kullaninin skoru: %d\n",userNames[i],scores[i]);
				}
				break;//Kullanýcýlarýn skorlarýný göster
				
			case 4: // ÇIKIÞ KISMI
				cikis=4;
				break;
				
		}
		score=calculateScore(randomMatrixCounter,undoCounter,manualCounter,sure,boyut);
		scores[k]=score;
		k++;// yeni kullanýcý indisine geçiþ yapmak için 
		
	}while(cikis!=4);
	
  return 0;
}

void randomMatrix(int matris[][MAX],int boyut) {
	int i,j;
	int rand1, rand2;
	srand(time(NULL));
	resetMatrix(matris,boyut);
	for (j = 0; j < 2; j++) {
		for (i = 1; i <= boyut;i++) {
			int rand1=(rand()%boyut); 
			int rand2=(rand()%boyut);
			if(matris[rand1][rand2]==0){
				matris[rand1][rand2] = i;	
			}
			else{
				//kontrol ettiðin yer doluysa tekrar yeni bir noktaya sayýyý yerleþtirme iþlemi.
				int found=0;
				while(!found){
					rand1=rand()% boyut;
					rand2 = rand()% boyut;
					if(matris[rand1][rand2]==0){
						matris[rand1][rand2]=i;
						found=1;
					}
				}
			}// matriste rastgele noktalara ikiþer tane olacak þekilde 1 den boyuta kadar sayýlarý yerleþtiriyor.
		}
	}
	
}
void move(int ind1,int ind2, int number,int matris[][MAX],int boyut){
	if(matris[ind1][ind2]==0){
		matris[ind1][ind2]=number;
	}
	else if(ind1>=boyut || ind2 >= boyut){
		printf("Girdiginiz indisler matris boyutunu asiyor lutfen duzeltin. ");
	}
	else{
		printf("Hareket etmek istediginiz alan dolu.");
	}

}
void undo(int ind1,int ind2,int matris[][MAX]){
	if(matris[ind1][ind2]!=0){
		matris[ind1][ind2]=0;
	}
}

int playManual(int matris[][MAX],int boyut){
	int bitis=1,undoCounter=0;
	while(bitis!=0){
	int i,ind1,ind2,number,undoCall;
	drawBoard(matris,boyut);
	printf("\nLutfen birlestirmek istediginiz noktanin bulundugu ilk ve ikinci indisi giriniz.\nOrnek: Once satir sonra sutun olacak sekilde (0,1) noktasinda ise 0 1 yaziniz. ");
	scanf("%d %d",&ind1,&ind2);
	printf("Lutfen yerlestirmek istediginiz sayiyi giriniz. ");
	scanf("%d",&number);
	move(ind1,ind2,number,matris,boyut);
	drawBoard(matris,boyut); 
	printf("Eger bir adim geri gitmek istiyorsaniz lutfen 1 e basiniz. Devam etmek icin herhangi bir rakama basiniz. ");
	scanf("%d",&undoCall);
	if(undoCall==1){
		undoCounter++;
		undo(ind1,ind2,matris);
		drawBoard(matris,boyut);
	}
	printf("Eger oyunu tamamladiysaniz 0 a basiniz. Devam etmek icin herhangi bir rakama basiniz.  ");
	scanf("%d",&bitis);
}
	return undoCounter;
}

void readFromFile(int matris[][MAX], char *fileName){
	int i,j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("Dosya Acilamadi!");
		return;
    }
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matris[i][j]=temp; 
    }  
  	fclose(data); 
   // Buraya kadar olan bolumde dosyadan okuma yapiliyor. Kodu degistirmeyiniz.
}

/* Matrisi ekrana yazdirma fonksiyonu
 * Bu fonksiyonu yapiyi bozmayacak sekilde degistirebilirsiniz.
 */
 
void drawBoard(int matris[][MAX], int boyut){
    int i,j,k;
    for(i=0;i<boyut;i++){
        printf("\n");
        for (k=0;k<boyut;k++)
            printf("-------");
        printf("\n");
        for(j=0;j<boyut;j++)
            if (matris[i][j]!=0)
                printf("  %d   |",matris[i][j]);
            else
                 printf("      |",matris[i][j]);
    }
    printf("\n");
}

int matrixSize(char* fileName){
	FILE *data = fopen(fileName,"r");
	char karakter;
    int satir_sayisi = 1;
	if (data == NULL) {
	        printf("Dosya açma hatasý!");
	        exit(1);
	    }
	
	    while ((karakter = fgetc(data)) != EOF) {
	        if (karakter == '\n') {
	            satir_sayisi++;
	        }
	    }
	    fclose(data);
	    return satir_sayisi/2;
}

void playAuto(int matris[][MAX], int boyut) {
    int i, j;
	drawBoard(matris,boyut);
    for (i = 0; i < boyut; i++) {
        for (j = 0; j < boyut; j++) {
            if (matris[i][j] == 0) {
                int number = getNearestEqualNumber(i, j, matris, boyut);

                if (number != 0) {
                    move(i, j, number, matris, boyut);
                    drawBoard(matris, boyut);
                }
            }
        }
    }
}

int getNearestEqualNumber(int rowIndex, int colIndex, int matris[][MAX], int boyut) {
    int i, j;
    int number = 0;
    int minDistance = boyut * 2;

    for (i = 0; i < boyut; i++) {
        for (j = 0; j < boyut; j++) {
            if (matris[i][j] != 0 && matris[i][j] %boyut == matris[rowIndex][colIndex] %boyut) {
                int distance = abs(i - rowIndex) + abs(j - colIndex);

                if (distance < minDistance) {
                    minDistance = distance;
                    number = matris[i][j];
                }
            }
        }
    }

    if (number == 0) {
        for (i = 1; i <= boyut; i++) {
            if (i != matris[rowIndex][colIndex]) {
                number = i;
                break;
            }
        }
    }

    return number;
}
int calculateScore(int randomMatrixCounter,int undoCounter,int manualCounter,double sure,int boyut){
	int score=0;
	score=(2*randomMatrixCounter)-undoCounter+(4*manualCounter)-(sure/10)+boyut;
	return score;
}
void resetMatrix(int matris[][MAX], int boyut) {
	int i, j;
	for (i = 0; i < boyut; i++) {
		for (j = 0; j < boyut; j++) {
			matris[i][j] = 0;
		}
	}
}
