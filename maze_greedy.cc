//MARTÍN AZNAR GARCÍA, 51797315G

#include<iostream>
#include<fstream>
#include<vector>
#include<math.h> //Para min
#include <climits> // Para INT_MAX

// x =v^; y= <>

#define MAX_VALOR 9999

using namespace std;

int dim_n;
int dim_m;

bool p2D=false;
bool b_file=false;

void dibujar_camino(vector< vector<int>> &mapa,vector< vector<int>> &camino){
    //comprobar_camino(camino);
    for(int i=0;i<dim_n;i++){
        for(int j=0;j<dim_m;j++){
            if(camino[i][j]){
                cout<<"*";
            }
            else{
                cout<<mapa[i][j];
            }
        }
        cout<<endl;
    }
}

int maze_greedy(int x, int y, vector< vector<int>> &mapa,int coste,vector< vector<int>> &camino){
    
    int coste_perpendicular=MAX_VALOR;
    int coste_abajo=MAX_VALOR;
    int coste_der=MAX_VALOR;
    int coste_aux=0;

    if(mapa[0][0]==0){
        return 0;
    }
    else{
        camino[0][0]=1;
    }

    if(x==dim_n-1 && y==dim_m-1){
        //cout<<"LLEGAMOS"<<endl;
        return mapa[x][y];
    }

    bool perpendicular=(x+1<dim_n && y+1<dim_m && mapa[x+1][y+1]);
    bool abajo=(x+1<dim_n && y<dim_m && mapa[x+1][y]);
    bool der=(x<dim_n && y+1<dim_m && mapa[x][y+1]);

    if(perpendicular){ //Calculo posición perpendicular

        coste_perpendicular=maze_greedy(x+1,y+1,mapa,coste,camino);
        camino[x+1][y+1]=1;
    }
    else if(abajo){ //Calculo posición arriba
        coste_abajo=maze_greedy(x+1,y,mapa,coste,camino);
        camino[x+1][y]=1;
    }
    else if(der){ //Calculo posición izquierda
        //cout<<"V"<<endl;
        coste_der=maze_greedy(x,y+1,mapa,coste,camino);
        //cout<<d
        camino[x][y+1]=1;
        //cout<<"K"<<endl;
    }
    else{ //Caso ningun movimiento posible
        return MAX_VALOR;
    }
    //cout<<x<<" "<<y<<" p:"<<coste_perpendicular<<" a:"<<coste_abajo<<" d:"<<coste_der<<endl;
    coste_aux+=min(coste_der, min(coste_perpendicular,coste_abajo)); //Minimo coste
    //cout<<coste_aux<<endl;
    if(coste_aux<MAX_VALOR){ //Si no hay camino posible 0
        coste=coste_aux+mapa[x][y];
        return coste;
    }
    else if(x==0 && y==0 && coste_aux==MAX_VALOR){
        return 0;
    }
    else{
        return MAX_VALOR;
    }

    
    //return coste;

}

void abrir_fichero(string file_name){

    ifstream file(file_name);

    if(file.is_open()){
        int num;

        file >> dim_n;
        file >> dim_m;

        vector< vector<int>> matriz(dim_n,vector<int>(dim_m));

        for(int i=0;i<dim_n;i++){
            for(int j=0;j<dim_m;j++){
                file>>matriz[i][j];
            }
        }

        int pos_init_x=0;
        int pos_init_y=0;

        //dibujar_mapa(matriz);

        //vector< vector<int>> tabla_memo(dim_n,vector<int>(dim_m));
        //vector< vector<int>> tabla_it_matriz(dim_n,vector<int>(dim_m));
        vector< vector<int>> camino(dim_n,vector<int>(dim_m));
        
        cout<<maze_greedy(pos_init_x,pos_init_y,matriz,matriz[0][0],camino)<<endl;
        if(p2D){
            dibujar_camino(matriz,camino);
        }
        
    }
    else{
        cerr<<"ERROR: can't open file: "<<file_name<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
    }
}

int main(int argn,char* argv[]){

    string file;

    if(argn==1){
        cerr<<"Usage:"<<endl
            <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
        return -1;
    }

    for(int i=1;i<argn;i++){
        string arg=argv[i];

        if(arg=="--p2D"){
            p2D=true;
        }
        else if(arg=="-f"){
            if(i!=argn-1){
                file=argv[++i];
                b_file=true;
            }
            else{
               // cerr<<"Error de argumento, falta [Nombre_archivo]: -f [Nombre_archivo]"<<endl;
               cerr<<"ERROR: missing filename."<<endl
                     <<"Usage:"<<endl
                     <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
                return -1;
            }
        }
        else{
            cerr<<"ERROR: unknow option "<<argv[i]<<"."<<endl
                <<"Usage:"<<endl
                <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
                return -1;
        }

    }
    if(!b_file){
        cerr<<"ERROR: missing filename."<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
        return -1;
    }

    abrir_fichero(file);

    return 0;
}