#include<iostream>
#include<fstream>
#include<vector>
#include<math.h> //Para min
#include <climits> // Para INT_MAX

// x =v^; y= <>

using namespace std;

int dim_n;
int dim_m;

struct punto{
    int x=0;
    int y=0;
};

void derecha(punto d_p){
    d_p.y+=1;
}

void abajo(punto a_p){
    a_p.x+=1;
}

void abajo_y_derecha(punto a_d_p){
    a_d_p.y+=1;
    a_d_p.x+=1;
}

void dibujar_mapa(vector< vector<int>> &mapa){
    for(int i=0;i<dim_n;i++){
        for(int j=0;j<dim_m;j++){
            cout<<mapa[i][j]<<" ";
        }
        cout<<endl;
    }
}

int maze_naive(int x,int y, vector< vector<int>> &mapa,int coste){
    int coste_perpendicular=INT_MAX;
    int coste_arriba=INT_MAX;
    int coste_izquierda=INT_MAX;
    
    cout<<"b:"<<x<<" "<<y<<" "<<mapa[x][y]<<" "<<coste<<endl;
    if(x==0 && y==0){
        cout<<"a:"<<coste<<endl;
        return coste+mapa[x][y];
    }

    bool perpendicular=(x-1>=0 && y-1>=0 && mapa[x-1][y-1]);

    if(perpendicular){

        //cout<<"perpendicular_movimiento: "<<endl;
        coste_perpendicular=maze_naive(x-1,y-1,mapa,coste+mapa[x][y]);

    }

    bool arriba=(x-1>=0 && y>=0 && mapa[x-1][y]);

    if(arriba){

        //cout<<"arriba_movimiento: "<<endl;
        coste_arriba=maze_naive(x-1,y,mapa,coste+mapa[x][y]);

    }

    bool izquierda=(x>=0 && y-1>=0 && mapa[x][y-1]);

    if(izquierda){

        //cout<<"izquierda_movimiento: "<<endl;
        coste_izquierda=maze_naive(x,y-1,mapa,coste+mapa[x][y]);

    }
    if(!perpendicular && !arriba && !izquierda){
        return INT_MAX;
    }

    cout<<"x: "<<x<<" y: "<<y<<"   "<<coste_izquierda<<" "<<coste_arriba<<" "<<coste_perpendicular<<endl;
    
    coste=min(coste_izquierda, min(coste_perpendicular,coste_arriba));
    if(x==dim_n-1 && y==dim_m-1 && coste==INT_MAX){
        return 0;
    }
    return coste;
}

void abrir_fichero(string file_name){

    ifstream file(file_name);

    if(file.is_open()){
        int num;

        file >> dim_n;
        file >> dim_m;

        //int matriz[dim_n*dim_m];

        vector< vector<int>> matriz(dim_n,vector<int>(dim_m));

        for(int i=0;i<dim_n;i++){
            for(int j=0;j<dim_m;j++){
                file>>matriz[i][j];
            }
        }

        /*int i=0;
        int j=0;
        while(file >> num){
            if(j>=dim_n){
                j=0;
                i++;
            }
            matriz[i*dim_n+j];
        }*/

        //struct punto p;
        
        int pos_init_x=dim_n-1;
        int pos_init_y=dim_m-1;

        dibujar_mapa(matriz);
        cout<<maze_naive(pos_init_x,pos_init_y,matriz,0)<<endl;
        //matriz[dim_n-1][dim_m-1]
    }
    else{
        cout<<"Error apertura de fichero"<<endl;
    }
}

int main(int argn,char* argv[]){

    string file;
    bool ignore_naive=false;
    bool t=false;
    bool p2D=false;
    bool b_file=false;

    if(argn<3){
        cout<<"Error de argumento, faltan argumentos"<<endl;
        return -1;
    }

    for(int i=1;i<argn;i++){
        string arg=argv[i];

        if(arg=="--ignore-naive"){
            ignore_naive=true;
        }
        else if(arg=="-t"){
            t=true;
        }
        else if(arg=="--p2D"){
            p2D=true;
        }
        else if(arg=="-f"){
            if(i!=argn-1){
                file=argv[++i];
                b_file=true;
            }
            else{
                cout<<"Error de argumento, faltan nombre del archivo"<<endl;
                return -1;
            }
        }
        else{
            cout<<"Error de argumento, argumento incorrecto"<<endl;
        }

    }
    if(!b_file){
        cout<<"Error de argumento, faltan nombre del archivo"<<endl;
        return -1;
    }

    abrir_fichero(file);

    return 0;
}