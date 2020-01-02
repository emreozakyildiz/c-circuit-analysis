#include <stdio.h>
#include <stdlib.h>

struct direnc{
	float direncDegeri;
	struct direnc *sonraki;
};

typedef struct direnc direnc;

direnc *bas, *yeni;

void direncEkle(float veri){
	if(bas==NULL){
		yeni=(direnc*)malloc(sizeof(direnc));
		yeni->direncDegeri = veri;
		bas = yeni;
	}
	else{
		yeni->sonraki=(direnc*)malloc(sizeof(direnc));
		yeni=yeni->sonraki;
		yeni->direncDegeri = veri;
	}
	yeni->sonraki=NULL;
}

int direncSayisiBul(){
	int counter=0;
	direnc *p;
	p=bas;
	while(p!=NULL){
		counter++;
		p=p->sonraki;
	}
	return counter;
}

float esdegerDirencHesapla(direnc *p,int direncSayisi,float esdegerDirenc,int islemKodu){
	
	if(islemKodu==100){
		p=bas;
		direncSayisi= direncSayisiBul();
	
		if(direncSayisi%2==0){
			esdegerDirenc = p->direncDegeri + p->sonraki->direncDegeri;
		}
		else if(direncSayisi==1){
			return p->direncDegeri;
		}
		else if(direncSayisi%2==1){
			esdegerDirenc = (p->direncDegeri * p->sonraki->direncDegeri)/(p->direncDegeri + p->sonraki->direncDegeri);
		}
		direncSayisi -=1;
		p=p->sonraki->sonraki;
		esdegerDirencHesapla(p,direncSayisi,esdegerDirenc,101);
	}
	else if(islemKodu==101){
		if(p==NULL){
			return esdegerDirenc;
		}
		else{
			if(direncSayisi%2==0){
				esdegerDirenc +=p->direncDegeri;
			}
			else{
				esdegerDirenc = (esdegerDirenc * p->direncDegeri)/(esdegerDirenc + p->direncDegeri);
			}
			direncSayisi -=1;
			p=p->sonraki;			
			esdegerDirencHesapla(p,direncSayisi,esdegerDirenc,101);
		}
	}
}

void direnclerUzerineDusenGerilim(direnc *p, float akim, int islemKodu){
	float simdikiDirenc,simdikiEsdeger,simdikiAkim;

	if(p->sonraki == NULL){
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",p->direncDegeri,p->direncDegeri*akim);
		return;
	}

	if(islemKodu==100){
		direnc *q=bas;
		p=p->sonraki;
		while(p->sonraki !=NULL){
			p=p->sonraki;
			q=q->sonraki;
		}
		simdikiDirenc = p->direncDegeri;
		free(p);
		q->sonraki=NULL;
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",simdikiDirenc,simdikiDirenc*akim);
		direnclerUzerineDusenGerilim(bas,akim,101);
	}
	else if(islemKodu==101){
		direnc *q=bas;
		p=p->sonraki;
		while(p->sonraki !=NULL){
			p=p->sonraki;
			q=q->sonraki;
		}
		simdikiDirenc = p->direncDegeri;
		free(p);
		q->sonraki =NULL;
		simdikiEsdeger=esdegerDirencHesapla(bas,0,0,100);
		simdikiAkim= akim* (simdikiEsdeger / (simdikiEsdeger + simdikiDirenc));
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",simdikiDirenc,simdikiDirenc*simdikiAkim);
		direnclerUzerineDusenGerilim(bas,akim-simdikiAkim,100);
	}
}

int main(){
	float veri;
	float rEs,gerilim,anakolAkimi;

	printf("Gerilimi girin:");
	scanf("%f",&gerilim);

	int counter=0;
	printf("İlk direnci girin:");
	scanf("%f",&veri);
	while(veri !=0){
		direncEkle(veri);
		printf("%.2f, basariyla eklendi. Sonraki direnci girin:",veri);
		scanf("%f",&veri);
	}
	rEs=esdegerDirencHesapla(bas,0,0,100);
	anakolAkimi = gerilim / rEs;
	printf("Esdeger Direnc= %.2f R\nAnakol Akimi = %.2f Amper\n",rEs, anakolAkimi);
	direnclerUzerineDusenGerilim(bas,anakolAkimi,100);
}
