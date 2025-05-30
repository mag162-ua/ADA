//Martín Aznar García, 51797315G

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define MAX_VALOR 9999
//#define orden []

int dim_x;
int dim_y;

bool p2D=false;
bool b_file=false;
bool p=false;

int visitados;
int explorados;
int hojas;
int no_factibles;
int no_prometedores;
int prometedor_descartado;
int soluciones;
int soluciones_cpesimista;

int cota_pesimista = 0;

struct Nodo{
    int valor;
	//int x; //Coordenada x
	//int y; //Coordenada y
	int coste; //Coste mínimo hasta este nodo
	bool Valido; //Es valido?
	//int cota_pesimista; //Mayor distancia hasta el destinno
	int cota_positiva; //Menor distancia hasta el destino
};

/*void maze_bb(Nodo n_actual, ){

}*/
void mostrarmatriz(vector<vector<Nodo> > &matriz){
    for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                cout<<matriz[i][j].valor<<" ";
            }
            cout<<endl;
        }
    
}

void abrir_fichero(string file_name){

    ifstream file(file_name.c_str());

    if(file.is_open()){
        int num;

        file >> dim_x;
        file >> dim_y;

        //vector< vector<int>> matriz(dim_x,vector<int>(dim_y))
        vector<vector<Nodo> > matriz(dim_x, vector<Nodo>(dim_y));

        for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                int valor;
                file >> valor;

                Nodo nodo;
                nodo.valor = valor;
                nodo.Valido = (valor != 0);
                nodo.coste = 0;
                //nodo.cota_pesimista = 0;
                nodo.cota_positiva = max(dim_x-1-i, dim_y-1-j);

                cota_pesimista += nodo.Valido ? 1 : 0;

                matriz[i][j] = nodo;

            }
        }

        mostrarmatriz(matriz);

        int pos_init_x=0;
        int pos_init_y=0;

        //vector< vector<int>> camino(dim_n,vector<int>(dim_m));
        //vector< vector<int>> optimista(dim_n,vector<int>(dim_m));

        //double tiempo = maze_bt(matriz);
        
        //cout<<mejorPeso<<endl;

        //cout<<visitados<<" "<<explorados<<" "<<hojas_visitadas<<" "<<descartados_no_factibles<<" "<<descartados_no_prometedores<<endl;

        //cout<<tiempo<<endl;
        
        /*if(mejorPeso!=0){
            if(p2D){
                dibujar_p2D(matriz);
            }
        }*/
        
        /*if(p){
            if(mejorPeso==0){
                cout<<"<0>"<<endl;
            }
            else{
                mostrar_pasos();
            }
        }*/
    }
    else{
        cerr<<"ERROR: can't open file: "<<file_name<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
    }
}

int main(int argn,char* argv[]){

    string file;

    if(argn==1){
        cerr<<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
        return -1;
    }

    for(int i=1;i<argn;i++){
        string arg=argv[i];

        if(arg=="--p2D"){
            p2D=true;
        }
        else if(arg=="-p"){
            p=true;
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
                     <<"maze [--p2D] [-p] -f file"<<endl;
                return -1;
            }
        }
        else{
            cerr<<"ERROR: unknow option "<<argv[i]<<"."<<endl
                <<"Usage:"<<endl
                <<"maze [--p2D] [-p] -f file"<<endl;
                return -1;
        }

    }
    if(!b_file){
        cerr<<"ERROR: missing filename."<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
        return -1;
    }

    abrir_fichero(file);

    return 0;
}