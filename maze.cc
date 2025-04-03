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

bool ignore_naive=false;
bool t=false;
bool p2D=false;
bool b_file=false;

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

void dibujar_memo_tabla(vector< vector<int>> &tabla_memo){
    for(int i=0;i<dim_n;i++){
        for(int j=0;j<dim_m;j++){
            if(tabla_memo[i][j]){
                if(tabla_memo[i][j]==-1){
                    cout<<" X";
                    //cout<<tabla_memo[i][j]<<" ";
                }
                else{
                    cout<<" "<<tabla_memo[i][j];
                   
                }
            }
            else{
                cout<<" -";
                //cout<<tabla_memo[i][j]<<" ";
            }
        }
        cout<<endl;
    }
}
void dibujar_it_tabla(vector< vector<int>> &tabla_iter){
    for (int i = 0; i < dim_n; i++) {
        for (int j = 0; j < dim_m; j++) {
            if(tabla_iter[i][j] == INT_MAX-1){
                cout<<" X";
            }
            else{
                cout<<" "<<tabla_iter[i][j];
            }
        }
        cout<<endl;
    }
}

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

int maze_naive(int x,int y, vector< vector<int>> &mapa,int coste){
    int coste_perpendicular=INT_MAX;
    int coste_arriba=INT_MAX;
    int coste_izquierda=INT_MAX;

    if(mapa[0][0]==0){
        return 0;
    }
    
    if(x==0 && y==0){
        return coste+mapa[x][y];
    }

    bool perpendicular=(x-1>=0 && y-1>=0 && mapa[x-1][y-1]);
    bool arriba=(x-1>=0 && y>=0 && mapa[x-1][y]);
    bool izquierda=(x>=0 && y-1>=0 && mapa[x][y-1]);

    if(perpendicular){ //Calculo posición perpendicular
        coste_perpendicular=maze_naive(x-1,y-1,mapa,coste+mapa[x][y]);
    }
    if(arriba){ //Calculo posición arriba
        coste_arriba=maze_naive(x-1,y,mapa,coste+mapa[x][y]);
    }
    if(izquierda){ //Calculo posición izquierda
        coste_izquierda=maze_naive(x,y-1,mapa,coste+mapa[x][y]);
    }
    if(!perpendicular && !arriba && !izquierda){ //Caso ningun movimiento posible
        return INT_MAX;
    }
    
    coste=min(coste_izquierda, min(coste_perpendicular,coste_arriba)); //Minimo coste
    if(x==dim_n-1 && y==dim_m-1 && coste==INT_MAX){ //Si no hay camino posible 0
        return 0;
    }
    return coste;
}

int maze_memo(int x,int y,vector< vector<int>> &mapa,vector< vector<int>> &tabla_coste,vector< vector<int>> &camino){
    
    int coste_perpendicular=INT_MAX;
    int coste_arriba=INT_MAX;
    int coste_izquierda=INT_MAX;

    if(mapa[0][0]==0){
        for(int i=0;i<dim_n;i++){
            for(int j=0;j<dim_m;j++){
                tabla_coste[i][j]=-1;
                
            }
        }
        return 0;
    }

    if(x==0 && y==0){ //
        return tabla_coste[x][y]=mapa[x][y];
        return mapa[x][y];
    }

    if(mapa[x][y]==0){
        tabla_coste[x][y]=-1;
        return INT_MAX;
    }
    
    bool perpendicular= (x-1>=0 && y-1>=0);
    bool arriba= (x-1>=0);
    bool izquierda= (y-1>=0);

    if(perpendicular){
        coste_perpendicular=maze_memo(x-1,y-1,mapa,tabla_coste,camino);
    }
    if(arriba){
        coste_arriba=maze_memo(x-1,y,mapa,tabla_coste,camino);
    }
    if(izquierda){
        coste_izquierda=maze_memo(x,y-1,mapa,tabla_coste,camino);
    }

    int coste=min(coste_perpendicular,min(coste_arriba,coste_izquierda));
    if(coste!=INT_MAX){
        if(coste==coste_perpendicular && perpendicular){
            camino[x-1][y-1]=1;
        }
        else if(coste==coste_arriba && arriba){
            camino[x-1][y]=1;
        }
        else if(coste==coste_izquierda && izquierda){
            camino[x][y-1]=1;
        }
    }

    if(x==dim_n-1 && y==dim_m-1 && coste==INT_MAX){
        tabla_coste[x][y]=-1;
        return 0;
    }
    if(coste==INT_MAX){
        tabla_coste[x][y]=-1;
        return INT_MAX;
    }

    return tabla_coste[x][y]=coste+mapa[x][y];
}

int maze_it_matrix(int x,int y,vector< vector<int>> &mapa,vector< vector<int>> &tabla_coste){
    

    int coste=0;

    for (int i = 0; i < dim_n; i++) {
        for (int j = 0; j < dim_m; j++) {
            tabla_coste[i][j] = INT_MAX-1;
        }
    }

    if (mapa[0][0] != 0) {
        tabla_coste[0][0] = mapa[0][0];
    } else {
        return 0;
    }

    for(int i=0;i<=x;i++){
        for(int j=0;j<=y;j++){

            int coste_perpendicular=INT_MAX;
            int coste_arriba=INT_MAX;
            int coste_izquierda=INT_MAX;

            if (mapa[i][j] == 0) 
                continue;

            if(j==0 && i>0 && tabla_coste[i-1][j]==INT_MAX-1){
                continue;
            }
            
            if(i>0 && j>0){
                coste_perpendicular = min(tabla_coste[i][j], tabla_coste[i-1][j-1]+mapa[i][j]);
            }
            if(i>0){
                coste_arriba=min(tabla_coste[i][j], tabla_coste[i-1][j]+mapa[i][j]);
            }
            if(j>0){
                coste_izquierda=min(tabla_coste[i][j], tabla_coste[i][j-1]+mapa[i][j]);
            }

            int min_coste = min(coste_perpendicular, min(coste_arriba, coste_izquierda));
            
            if (min_coste < tabla_coste[i][j]) {
                tabla_coste[i][j] = min_coste;
            }
            //cout<<"pos_i: "<<i<<" pos_j: "<<j<<" p: "<<coste_perpendicular<<" a: "<<coste_arriba<<" i:"<<coste_izquierda<<endl<<min_coste<<endl;
        }
    }
    if(tabla_coste[x][y]==INT_MAX-1){
        return 0;
    }
    return tabla_coste[x][y];
}

int maze_vector(int x, int y, vector<vector<int>> &mapa) {
    
    if(mapa[0][0]==0){
        return 0;
    }

    vector<int> previo(dim_m, MAX_VALOR-1), actual(dim_m, MAX_VALOR-1); 

    previo[0] = mapa[0][0];
    actual[0] = mapa[0][0];

    for (int i = 0; i <= x; i++) {
        for (int j = 0; j <= y; j++) {
            if (mapa[i][j] == 0) {
                actual[j] = MAX_VALOR-1;
                continue;
            }

            int coste_perpendicular = MAX_VALOR;
            int coste_arriba = MAX_VALOR;
            int coste_izquierda = MAX_VALOR;

            if(i>0 && j>0){
                coste_perpendicular = previo[j-1]+mapa[i][j];
            }
            if(i>0){
                coste_arriba=previo[j]+mapa[i][j];
            }
            if(j>0){
                coste_izquierda=actual[j-1]+mapa[i][j];
            }
            if(i>0 || j>0){
                actual[j] = min(coste_perpendicular, min(coste_arriba, coste_izquierda));
            }

            //cout<<"pos_i: "<<i<<" pos_j: "<<j<<" p: "<<coste_perpendicular<<" a: "<<coste_arriba<<" i:"<<coste_izquierda<<endl<<actual[j]<<" "<<previo[j]<<endl;
        }

        previo = actual; // Avanzamos a la siguiente fila
    }

    return (previo[y] >= MAX_VALOR-1) ? 0 : previo[y]; 
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

        int pos_init_x=dim_n-1;
        int pos_init_y=dim_m-1;

        //dibujar_mapa(matriz);

        vector< vector<int>> tabla_memo(dim_n,vector<int>(dim_m));
        vector< vector<int>> tabla_it_matriz(dim_n,vector<int>(dim_m));
        vector< vector<int>> camino(dim_n,vector<int>(dim_m));
        
        camino[0][0]=1;
        camino[dim_n-1][dim_m-1]=1;

        if(ignore_naive){
            cout<<"- ";
        }
        else{
            cout<<maze_naive(pos_init_x,pos_init_y,matriz,0)<<" ";
        }
        int coste_memo=maze_memo(pos_init_x,pos_init_y,matriz,tabla_memo,camino);
        cout<<coste_memo<<" ";
        int coste_it_matriz=maze_it_matrix(pos_init_x,pos_init_y,matriz,tabla_it_matriz);
        cout<<coste_it_matriz<<" ";
        cout<<maze_vector(pos_init_x,pos_init_y,matriz)<<endl;
        if(p2D){
            //cout<<'?'<<endl;
            if(coste_memo==0){
                cout<<0<<endl;
            }
            else{
                dibujar_camino(matriz,camino);
            }
        }
        if(t){
            //cout<<'?'<<endl;
            cout<<"Memoization table:"<<endl;
            dibujar_memo_tabla(tabla_memo);
            cout<<"Iterative table:"<<endl;
            dibujar_it_tabla(tabla_it_matriz);
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

    if(argn<3){
        cerr<<"Usage:"<<endl
            <<"maze [--p2D] [-t] [--ignore-naive] -f file"<<endl;
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