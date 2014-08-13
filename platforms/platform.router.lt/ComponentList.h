/********************************************************************************
	MPSoCBench Benchmark Suite
	Authors: Liana Duenha
	Supervisor: Rodolfo Azevedo
	Date: July-2012
	www.archc.org/benchs/mpsocbench

	Computer Systems Laboratory (LSC)
	IC-UNICAMP
	http://www.lsc.ic.unicamp.br/


	This source code is part of the MPSoCBench Benchmark Suite, which is a free
	source-code benchmark for evaluation of Electronic Systemc Level designs.
	This benchmark is distributed with hope that it will be useful, but
	without any warranty.

*********************************************************************************/

#ifndef _COMPONENTLIST_H
#define _COMPONENTLIST_H

#include <iostream>
#include <cstring>
#include <stdio.h>
// #include <cstdlib>


using std::cout;
using std::endl;
// using std::itoa;


class ComponentType
{
	public:

		ComponentType()
		{
			name = new char[100];
			cName = new char [100];
			kind = new char [100];
			strcpy(name,"noName");
			strcpy(kind,"noKind");
			strcpy(cName, "noName");
			interface = 0;

		}
		ComponentType(const char *name, const char *kind)
		{
			this->name = new char[100];
			this->cName = new char [100];
			this->kind = new char[100];
			strcpy(this->name, name);
			strcpy(this->cName,name);
			convertName();
			strcpy(this->kind,kind);
			interface = 0;

		}

		ComponentType(const ComponentType& t)
		{
			name = new char[100];
			kind = new char[100];
			cName = new char [100];
			strcpy(name,t.name);
			strcpy(kind,t.kind);
			strcpy(cName,t.cName);
			interface = t.interface;

		}
		ComponentType& operator =(const ComponentType& t)
		{
			strcpy(name,t.name);
			strcpy(kind,t.kind);
			strcpy(cName,t.cName);
			interface = t.interface;
			return *this;
		}

		~ComponentType()
		{
			delete []name;
			delete []kind;
			delete []cName;

		}
		bool operator == (const ComponentType& t) const
		{
			if(!strcmp(name,t.name)) return true; 
			return false;
		}
		void print() const
		{
			// puts(name);
			cout << "name: " << name << "  kind: " << kind << "  interface: " << interface;
			
		}
		long int getInterface()
		{
			return interface;
		}
		void convertName()
		{
			for(int i=0; i<strlen(this->cName); i++)
			{
				if(cName[i]=='.')
					cName[i]='_';

			}
			// cout << "\n\nConverteu " << this->name << " para " << this->cName;

		}
		char *name;
		char *cName;
		char *kind;
		long unsigned int interface;

};

class ComponentNode
{
	public:
		ComponentNode()
		{
			next = prev = 0;
			headOfChildrenList = 0;
			nTargetRelations = 0;
			targetRelations = new ComponentNode* [100];
		
		}

		ComponentNode(const ComponentType& el, ComponentNode* ptr=0, ComponentNode* ptr2=0, ComponentNode* ptr3=0):
		info(el)
		{
			next = ptr;
			prev = ptr2;
			headOfChildrenList = ptr3;
			targetRelations = new ComponentNode* [100];
			nTargetRelations = 0;
		
		}
	
		/*void alocTargetRelation (int nMaxNodes)
		{
			
			targetRelation = new ComponentNode* [nMaxNodes];
			for (int i=0; i<nMaxNodes-1; i++)
			{
				targetRelation[i]=0;
			}
		}*/

		ComponentType info;
		ComponentNode* next;
		ComponentNode* prev;
		ComponentNode *headOfChildrenList;
		ComponentNode **targetRelations;
		int nTargetRelations;
		
};


class ComponentList
{
	public:

	    ComponentList()
		{
			//cout << "\nA lista foi instanciada!\n";			
			head = tail = 0;
			numberOfComponents = 0;


			// ABRINDO ARQUIVO DE SAIDA - DOT SUPORT
			out = fopen("ESLDiagram.dot", "w");
			if(out == NULL)
			{
				cout << "\nError opening file dotsuport.dot\n";
			        exit(101);
			}
			fileHeader();
		}

	       ~ComponentList();

		int isEmpty() const
		{
			return head==0;
	    }
		bool contains(const ComponentType& element) const
		{
			return isInList(element)!=0;
		}
		void add(const ComponentType&);
	 	void addHead(const ComponentType&);
		void addTail(const ComponentType&);
		bool deleteHead();   
		bool deleteTail();   
		bool deleteNode(const ComponentType&);
		void print() const;
		int size() const;
		void incNumberOfComponents();

        void printAtFile() const;
		void fileHeader() const;
        ComponentNode *getHead()
        {
        	return head;

        }
        ComponentNode *getTail()
        {

        	return tail;
        }
        ComponentNode* isInList(const ComponentType&) const;

        void findNodesWithSameInterface (ComponentNode*, long int);
        void excRepetedInterfaces();
        void captureRelations();
	
        FILE *out;
	private:
		ComponentNode *head, *tail;
		int numberOfComponents;

};

inline
ComponentList::~ComponentList()
{
	ComponentNode *p;

	while (!isEmpty())
	{
      	p = head->next;
		delete head;
		head = p;
		
	}
	fclose(out);

}


inline
void
ComponentList::add(const ComponentType& newElement)
{
	addTail(newElement);
}


inline
void
ComponentList::addHead (const ComponentType& newElement)
{
	ComponentNode* newNode = new ComponentNode(newElement, head);

	if (newNode) 
	{
		if(isEmpty())
			tail = head = newNode;
		else
		{
			head->prev = newNode;
			head = newNode;
		}
		numberOfComponents++;
	}
}

inline
void
ComponentList::addTail (const ComponentType& newElement)
{

	ComponentNode* newNode = new ComponentNode (newElement,0,tail);

	if (newNode)   //== 0) throw DoubleListException(0);
	{
		if(isEmpty())
			tail = head = newNode;
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
		numberOfComponents++;
	}
}


inline
bool
ComponentList::deleteHead ()
{
	if (isEmpty()) return false;
			
	ComponentNode* tmp = head;

	head = head->next;
	if (head) 
		head->prev = 0;
	else 
		tail = 0;

	numberOfComponents--;
	delete tmp;
	return true;
	
	
}

inline
bool
ComponentList::deleteTail ()
{
	if (isEmpty()) return false;
	
	ComponentNode* tmp = tail;	

	tail = tail->prev;
	if (tail)
		tail->next = 0;
	else
		head = 0;

	numberOfComponents--;
	delete tmp;
	return true;
	
}

inline
bool
ComponentList::deleteNode (const ComponentType& element)
{
	ComponentNode* found = isInList(element);

	if(!found) return false; 
	
	if (found->prev)
		found->prev->next = found->next;
	else
		head = found->next;
	if (found->next)
		found->next->prev = found->prev;
	else
		tail = found->prev;
	this->numberOfComponents--;
	delete found;
	return true;
		
	
}

inline
ComponentNode*
ComponentList::isInList (const ComponentType& element) const
{
	ComponentNode* tmp;
	for(tmp = head; tmp!=0 && !(tmp->info==element); tmp=tmp->next);
	return tmp;
}


// Print method used for debug
// file out.txt

inline
void
ComponentList::print() const
{
	ComponentNode* tmp;
	ComponentNode* tmpChild;
	FILE *out_temp;
	
	out_temp = fopen("out.txt", "w");
	if(out_temp == NULL)
	{
		cout << "\nError opening file out.txt\n";
	        exit(101);
	}


	for(tmp = head; tmp!=0 ; tmp=tmp->next)
	{
 		cout << "\n";
		

 		fprintf(out_temp, "\nName: %s  Kind: %s Interface: %ld", (tmp->info).name, (tmp->info).kind, (tmp->info).getInterface() );

		
		if(tmp->targetRelations)
		{
			int i=0;
			fprintf(out_temp, "\nhas %d relations.", tmp->nTargetRelations);
			while (i < tmp->nTargetRelations)
			{
				fprintf(out_temp,"\n\tRelated to %s",((tmp->targetRelations[i])->info).name);
				i++;				
			}

		}
        	
		tmpChild = tmp->headOfChildrenList;
		while (tmpChild != 0)
		{

			fprintf(out_temp, "\nName: %s Kind: %s Interface: %ld", (tmpChild->info).name, (tmpChild->info).kind, (tmpChild->info).getInterface());
			if(tmpChild->targetRelations)	
			{
				int i=0;
				fprintf(out_temp, "\n\thas %ld relations.", tmpChild->nTargetRelations);

				while (i < tmpChild->nTargetRelations)
				{
					fprintf(out_temp,"\n\tRelated to %s",((tmpChild->targetRelations[i])->info).name);

					i++;				
				}

			}

			tmpChild = tmpChild->next;
		}
	}
	fclose (out_temp);
}


inline
void
ComponentList::printAtFile() const
{
	ComponentNode* tmp;
	ComponentNode* tmp1;
	int num=0;


	// Impressão dos nós



	for(tmp = head; tmp!=0 ; tmp=tmp->next)
	{

		fputs("\nsubgraph ",out);
		fprintf(out,"cluster%d",num);
		num++;
		fputs("{\n",out);
		fputs("node[shape=box,style=filled,color=white];\n", out);
		fputs("style=filled;\n", out);
		fputs("color=lightgrey;\n", out);

		if (tmp->headOfChildrenList != 0)
		{
			for (tmp1 = tmp->headOfChildrenList; tmp1 !=0; tmp1 = tmp1->next)
			{
				fputs((tmp1->info).cName, out);
				fputs(" [label=\"",out);
				fputs((tmp1->info).cName,out);
				fputs("\\n(",out);
				fputs((tmp1->info).kind,out);
				fputs(")",out);
				fputs("\"];\n",out);

			}
		}
		fputs("label=\"", out);
		fputs((tmp->info).cName,out);
		fputs("\";",out);
		fputs("\n",out);
		fputs("}",out);
	}

	// Impressão das arestas
	int i;
	for(tmp = head; tmp!=0 ; tmp=tmp->next)
	{
		i = 0;
		if(i < tmp->nTargetRelations)
		{
			fputs("\n",out);
			fputs( (tmp->info).cName, out);
			fputs( " -> ",out );			
			fputs( ((tmp->targetRelations[i])->info).cName, out);
			i++;		
		}
		
		
		for (tmp1 = tmp->headOfChildrenList; tmp1 !=0; tmp1 = tmp1->next)
		{
			i=0;
			while(i < tmp1->nTargetRelations)
			{
				fputs("\n",out);
				fputs( (tmp1->info).cName, out);
				fputs( " -> ",out );			
				fputs( ((tmp1->targetRelations[i])->info).cName, out);
				i++;		
			}
		}	
		
	}


	fputs("\n",out);
	fputs("}",out);
}

inline
int ComponentList::size() const
{
	return numberOfComponents;
}

inline
void ComponentList::fileHeader () const
{
	fputs("digraph {", out);
	fputs("\nranksep=1.5;",out);

}
inline
void ComponentList::incNumberOfComponents()
{
	numberOfComponents++;
}

inline
void ComponentList::findNodesWithSameInterface (ComponentNode* self, long int interface_)
{

	ComponentNode* tmp;
	ComponentNode* tmpChild;


	for(tmp = getHead(); tmp!=0 && tmp != self ; tmp=tmp->next)
	{

		if ( interface_ == (tmp->info).getInterface() )
		{	

			if ( (strcmp( (tmp->info).kind, "sc_export") == 0 )||(strcmp( (tmp->info).kind, "sc_clock") == 0 )||(strcmp( (tmp->info).kind,"sc_interface") == 0 ))
			{ 
				self->targetRelations[self->nTargetRelations++] = tmp;
			}
		}
	    tmpChild = tmp->headOfChildrenList;
		while (tmpChild != 0 && tmpChild != self)
		{

			if (interface_ == (tmpChild->info).getInterface())
			{	

				if ( (strcmp( (tmpChild->info).kind, "sc_export") == 0 )||(strcmp( (tmpChild->info).kind, "sc_clock") == 0 ) || (strcmp( (tmpChild->info).kind, "sc_interface") ==0 ) )
				{ 
					self->targetRelations[self->nTargetRelations++] = tmpChild;
				}

			}
			tmpChild = tmpChild->next;
		}
	}


	// apenas para confirmar
	

}

inline
void
ComponentList::excRepetedInterfaces()
{

			ComponentNode* tmp;
			ComponentNode* tmpChild;
			ComponentNode* auxChild;
			ComponentNode* prevChild;

            // identifica e remove nós que representem a mesma interface "sc_export" de entrada
			// de componente da plataforma com o objetivo de melhorar a "aparencia" do diagrama
			// a ser gerado (exclui possiveis arestas repetidas no diagrama)

			for(tmp = getHead(); tmp!=0 ; tmp=tmp->next)
			{

					 ComponentNode *tmpChild = tmp->headOfChildrenList;

					 while ( (tmpChild != 0) && (strcmp( (tmpChild->info).kind,"sc_export") != 0 ) )
					 {
						 tmpChild = tmpChild -> next;
					 }
					 if (tmpChild != 0)
					 {
						 // há uma sc_export na lista de filhos do módulo
						 prevChild = tmpChild;
						 auxChild = tmpChild->next;

						 while ( auxChild != 0)
						 {
							 if ( (auxChild -> info).getInterface() == (tmpChild->info).getInterface() )  // ( (auxChild -> info).getInterface() == (tmpChild->info).getInterface() )) )
							 {
								 prevChild -> next = auxChild -> next;
								 delete auxChild;
								 auxChild = prevChild -> next;

							 }
							 else
						     {
								 prevChild = auxChild;
								 auxChild = auxChild ->next;
						     }

						 }
					 }
		    }


			// identifica componentes "pais" que não tenham "filhos" sc_export
			// cria um componente iface para representar a interface de entrada
			// do componente pai
			// Isso é necessário porque "pai" será mapeado em um cluster da ferramenta
			// dot e um cluster não possui arestas (ligações)
			// As ligações (arestas) que chegam ao componente pai serão ligadas ao
			// componetne iface criado

			for(tmp = getHead(); tmp!=0 ; tmp=tmp->next)
			{

					 //cout << "\nEntrando na lista de filhos do módulo " << (tmp->info).name;
					 ComponentNode *tmpChild = tmp->headOfChildrenList;


					 while ( (tmpChild != 0) && (strcmp( (tmpChild->info).kind,"sc_export") != 0 ) )
					 {
						 tmpChild = tmpChild -> next;
					 }
					 if (tmpChild == 0)
					 {
						 char *compoundName = new char[100];
						 strcpy(compoundName, (tmp->info).name);
						 strcat(compoundName, "_iface");

						 ComponentType t (compoundName,"sc_interface");
						 t.interface = (tmp->info).interface;

						 ComponentNode* aux = new ComponentNode(t);

						 (tmp->info).interface = 0;
						 aux->next = tmp->headOfChildrenList;
						 tmp->headOfChildrenList = aux;


	 				 }
					 else
				     {
						 (tmp->info).interface = 0;
				     }

		    }
}


inline
void ComponentList::captureRelations()
{

	ComponentNode* tmp;
	ComponentNode* tmpChild;
	ComponentNode** tmp2;

	for(tmp = getHead(); tmp!=0 ; tmp=tmp->next)
	{

		if ( ((tmp->info).getInterface()!=0) && ( ( strcmp ((tmp->info).kind,"sc_port") == 0) || ( strcmp ((tmp->info).kind,"sc_in") == 0)))
		{
			findNodesWithSameInterface (tmp, (tmp->info).getInterface());

		}
		tmpChild = tmp->headOfChildrenList;

		while (tmpChild != 0)
		{

			if  (((tmpChild->info).getInterface()!=0) && ( strcmp( (tmpChild->info).kind,"sc_port") == 0 || ( strcmp ((tmpChild->info).kind,"sc_in") == 0)))
			{
				findNodesWithSameInterface (tmpChild,(tmpChild->info).getInterface());

			}
			tmpChild = tmpChild->next;
		}//while exists children
	}// for
}// captureRelations


#endif
