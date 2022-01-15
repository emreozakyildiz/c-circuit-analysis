#include <stdio.h>
#include <stdlib.h>

struct resistor{
	float value;
	struct resistor *next;
};

typedef struct resistor resistor;

resistor *head, *add;

void addResistor(float data){
	if(head==NULL){
		add=(resistor*)malloc(sizeof(resistor));
		add->value = data;
		head = add;
	}
	else{
		add->next=(resistor*)malloc(sizeof(resistor));
		add=add->next;
		add->value = data;
	}
	add->next=NULL;
}

int countResistors(){
	int counter=0;
	resistor *p;
	p=head;
	while(p!=NULL){
		counter++;
		p=p->next;
	}
	return counter;
}

float calculateEquivalentResistor(resistor *p,int countedResistors,float equivalentResistance,int processCode){
	
	if(processCode==100){
		p=head;
		countedResistors= countResistors();
	
		if(countedResistors%2==0){
			equivalentResistance = p->value + p->next->value;
		}
		else if(countedResistors==1){
			return p->value;
		}
		else if(countedResistors%2==1){
			equivalentResistance = (p->value * p->next->value)/(p->value + p->next->value);
		}
		countedResistors -=1;
		p=p->next->next;
		calculateEquivalentResistor(p,countedResistors,equivalentResistance,101);
	}
	else if(processCode==101){
		if(p==NULL){
			return equivalentResistance;
		}
		else{
			if(countedResistors%2==0){
				equivalentResistance +=p->value;
			}
			else{
				equivalentResistance = (equivalentResistance * p->value)/(equivalentResistance + p->value);
			}
			countedResistors -=1;
			p=p->next;			
			calculateEquivalentResistor(p,countedResistors,equivalentResistance,101);
		}
	}
}

void voltageAcrossResistor(resistor *p, float current, int processCode){
	float nowResistor,nowEquivalentResistance,nowCurrent;

	if(p->next == NULL){
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",p->value,p->value*current);
		return;
	}

	if(processCode==100){
		resistor *q=head;
		p=p->next;
		while(p->next !=NULL){
			p=p->next;
			q=q->next;
		}
		nowResistor = p->value;
		free(p);
		q->next=NULL;
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",nowResistor,nowResistor*current);
		voltageAcrossResistor(head,current,101);
	}
	else if(processCode==101){
		resistor *q=head;
		p=p->next;
		while(p->next !=NULL){
			p=p->next;
			q=q->next;
		}
		nowResistor = p->value;
		free(p);
		q->next =NULL;
		nowEquivalentResistance=calculateEquivalentResistor(head,0,0,100);
		nowCurrent= current* (nowEquivalentResistance / (nowEquivalentResistance + nowResistor));
		printf("%.2f R uzerine dusen gerilim = %.2f V\n",nowResistor,nowResistor*nowCurrent);
		voltageAcrossResistor(head,current-nowCurrent,100);
	}
}

int main(){
	float data;
	float equivalentResistance,voltage,mainFlow;

	printf("Gerilimi girin:");
	scanf("%f",&voltage);

	int counter=0;
	printf("İlk direnci girin:");
	scanf("%f",&data);
	while(data !=0){
		addResistor(data);
		printf("%.2f, basariyla eklendi. Sonraki direnci girin:",data);
		scanf("%f",&data);
	}
	equivalentResistance=calculateEquivalentResistor(head,0,0,100);
	mainFlow = voltage / equivalentResistance;
	printf("Esdeger Direnc= %.2f R\nAnakol Akimi = %.2f Amper\n",equivalentResistance, mainFlow);
	voltageAcrossResistor(head,mainFlow,100);
}
