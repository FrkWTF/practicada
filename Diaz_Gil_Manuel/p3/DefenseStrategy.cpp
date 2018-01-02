// ###### Config options ################



// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
#include <queue>
#include "math.h"

using namespace Asedio; 

struct celda
{
    int row,col;
    float valor;
    celda():row(0),col(0),valor(0){}
    celda(int x,int y,float v):row(x),col(y),valor(v){}
    

};
bool operator < (const celda& a,const celda& b){return a.valor<b.valor;}    

bool factible(int row,int col,int nCellsWidth, int nCellsHeight, float mapWidth,float mapHeight,std::list<Object*> obstacles,std::list<Defense*> defenses,std::list<Defense*>::iterator current){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    float x,y,distanciapuntos=0;
    x=row*cellWidth+cellWidth*0.5f;
    y=col*cellHeight+cellHeight*0.5f;
    std::list<Object*>::iterator obsts=obstacles.begin();
    while(obsts != obstacles.end()){
        distanciapuntos=sqrt(abs(x-(*obsts)->position.x)*abs(x-(*obsts)->position.x) +abs(y-(*obsts)->position.y)*abs(y-(*obsts)->position.y));
        if(((*current)->radio+(*obsts)->radio)>distanciapuntos)return false;
        ++obsts;
    }
    
    std::list<Defense*>::iterator defs=defenses.begin();
    
    while(defs != defenses.end()){
        distanciapuntos=sqrt(abs(x-(*defs)->position.x)*abs(x-(*defs)->position.x)+abs(y-(*defs)->position.y)*abs(y-(*defs)->position.y));
        if(((*current)->radio+(*defs)->radio)>distanciapuntos)return false;
        ++defs;
    }

    if((x-(*current)->radio)<0 || (x+(*current)->radio)>mapWidth){
        return false;
    }
    if((y-(*current)->radio)<0 || (y+(*current)->radio)>mapHeight){
        return false;
    }
    
    return true;

}    

float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
    , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    	
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    
    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);
    	
    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}

void sinOrdenacion(std::vector<cell>& c){
    for(int i=0;i<c.size()-1;++i){
    for(int j=i+1;j<c.size();++j){    
        if(c[i]<c[j]){
        std::swap(c[i],c[j]);
        }
        }
    }
}

void ordenacionInsercion(std::vector<cell>& c, int i, int j){
    cell x;
    for(int k=i+1;k<j;++k){
        x=c[k];
        while(k>i && x>c[k-1]){
            c[k]=c[k-1];
            --k;
        }
        c[k]=x;
    }
}

void fusion(std::vector<cell>& c,int i,int k,int j){
    int n = j-i;
    std::vector<cell> aux;
    int iter=i;
    int iter2=k;
    for (int l = 0; l < n; ++l)
    {
        if(iter<=k && (iter2>j-1 || c[iter]<=c[iter2])){
            aux.push_back(c[iter]);
            ++iter;
        }
        else{
            aux.push_back(c[iter2]);
            ++iter2;
        }       
    }
    for (int l = 0; l < n; ++l)
    {
        c[l+i-1]=aux[l];
    }
}

void ordenacionFusion(std::vector<cell>& c,int i, int j){
    int n = j-i;
    if (n<3){
        ordenacionInsercion(c,i,j);
    }
    else{
        int k=j-((j-i)/2);
        ordenacionFusion(c,i,k);
        ordenacionFusion(c,k,j);
        fusion(c,i,k,j);    
    }

}

int pivote(std::vector<cell>& c,int i, int j){
    
}

void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    bool colocado=false;
    celda cactual;
    std::priority_queue<celda> mceldas;

    int maxAttemps = 10000;
	cronometro c;
    long int r = 0;
    c.activar();
    do {	
		List<Defense*>::iterator currentDefense = defenses.begin();
		while(currentDefense != defenses.end() && maxAttemps > 0) {

			(*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
			(*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
			(*currentDefense)->position.z = 0; 
			++currentDefense;
		}
		
		++r;
    } while(c.tiempo() < 1.0);
    c.parar();
    std::cout << (nCellsWidth * nCellsHeight) << '\t' << c.tiempo() / r << '\t' << c.tiempo()*2 / r << '\t' << c.tiempo()*3 / r << '\t' << c.tiempo()*4 / r << std::endl;
}
