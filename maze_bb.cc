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

int visitados = 0;
int explorados = 0;
int hojas = 0;
int no_factibles = 0;
int no_prometedores = 0;
int prometedor_descartado = 0;
int soluciones_actualizadas = 0;
int soluciones_cpesimista = 0;

int cota_pesimista = 0;
int a = 0;

struct Nodo{
    int valor; //Valor del nodo
    int x; //Posición en x
    int y; //Posición en y
    //int coste; //Coste mínimo hasta este nodo
	bool Valido; //Es valido?
	int cota_positiva; //Menor distancia hasta el destino
    //bool mejor; //Es el mejor nodo hasta ahora
    bool visitado; //Ha sido visitado?
};

int mejorPeso = MAX_VALOR; 
vector<int> mejorPasos;
vector<Nodo> pasos;

void mostrarmatriz(vector<vector<Nodo> > &matriz){
    for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                cout<<matriz[i][j].valor<<" ";
            }
            cout<<endl;
        }
    
}

bool maze_bb(int pos_x, int pos_y, vector<vector<Nodo> > &matriz, int coste){

    //Nodo n_actual = matriz[pos_x][pos_y];
    
    visitados++;

    if((pos_x == 0 && pos_y == 0 && !matriz[pos_x][pos_y].Valido) || 
       (pos_x == dim_x-1 && pos_y == dim_y-1 && !matriz[pos_x][pos_y].Valido)){
        no_factibles++;
        return false;
    }

    if(!matriz[pos_x][pos_y].Valido){
        no_factibles++;
        return false;
    }
    if(matriz[pos_x][pos_y].cota_positiva + coste + matriz[pos_x][pos_y].valor >= mejorPeso){
        no_prometedores++;
        return false;
    }
    if(pos_x==dim_x-1 && pos_y==dim_y-1 && matriz[pos_x][pos_y].Valido){
        //explorados++;
        //soluciones++;
        hojas++;
        if(coste + matriz[pos_x][pos_y].valor < mejorPeso){
            
            mejorPeso = coste + matriz[pos_x][pos_y].valor;
            //cota_pesimista = mejorPeso;
            /*mejorPasos.clear();
            for(const auto& paso : pasos){
                mejorPasos.push_back(paso.valor);
            }
            mejorPasos.push_back(matriz[pos_x][pos_y].valor);*/
            soluciones_actualizadas++;
        }
        if (mejorPeso < cota_pesimista) {
            cota_pesimista = mejorPeso;
            soluciones_cpesimista++; // <--- nbest-solution-updated-from-pessimistic-bound
        }
        return true;
    }

    
    //cout<<"Cota pesimista: "<<cota_pesimista<<endl;
    cout<<"Visitando nodo ("<<pos_x<<", "<<pos_y<<") con coste "<<coste<<endl;
    matriz[pos_x][pos_y].visitado = true;
    coste += matriz[pos_x][pos_y].valor;
    bool llega_al_final = false;;

    if(pos_x < dim_x-1 && pos_y < dim_y-1 && !matriz[pos_x+1][pos_y].visitado && matriz[pos_x+1][pos_y+1].Valido){
        //Mover hacia abajo y a la derecha
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x+1, pos_y+1, matriz, coste);
    }
    if(pos_x < dim_x-1 && !matriz[pos_x+1][pos_y].visitado && matriz[pos_x+1][pos_y].Valido){
        //Mover hacia abajo
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x+1, pos_y, matriz, coste);
    }
    if(pos_y < dim_y-1 && !matriz[pos_x][pos_y+1].visitado && matriz[pos_x][pos_y+1].Valido){
        //Mover hacia la derecha
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x, pos_y+1, matriz, coste);
    }
    if(pos_x > 0 && pos_y < dim_y-1 && !matriz[pos_x-1][pos_y+1].visitado && matriz[pos_x-1][pos_y+1].Valido){
        //Mover hacia arriba y a la derecha
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x-1, pos_y+1, matriz, coste);
    }
    if(pos_y > 0 && pos_x < dim_x-1 && !matriz[pos_x+1][pos_y-1].visitado && matriz[pos_x+1][pos_y-1].Valido){
        //Mover hacia abajo y a la izquierda
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x+1, pos_y-1, matriz, coste);
    }
    if(pos_x > 0 && !matriz[pos_x-1][pos_y].visitado && matriz[pos_x-1][pos_y].Valido){
        //Mover hacia arriba
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x-1, pos_y, matriz, coste);
    }
    if(pos_y > 0 && !matriz[pos_x][pos_y-1].visitado && matriz[pos_x][pos_y-1].Valido){
        //Mover hacia la izquierda
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x, pos_y-1, matriz, coste);
    }
    if(pos_x > 0 && pos_y > 0 && !matriz[pos_x-1][pos_y-1].visitado && matriz[pos_x-1][pos_y-1].Valido){
        //Mover hacia arriba y a la izquierda
        explorados++;
        llega_al_final = llega_al_final || maze_bb(pos_x-1, pos_y-1, matriz, coste);
    }

    matriz[pos_x][pos_y].visitado = false;

    if(!llega_al_final){
        prometedor_descartado++;
    }
    //n_actual.coste = 0;
    //n_actual.cota_positiva = max(dim_x-1-pos_init_x, dim_y-1-pos_init_y);

    //cout<<n_actual.cota_positiva<<endl;

    //visitados++;
    //explorados++;

    //if(n_actual.cota_positiva==0){
        //soluciones++;
        //mejorPeso = min(mejorPeso, n_actual.coste);
    //}
    
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
                nodo.x = i;
                nodo.y = j;
                nodo.visitado = false;
                //nodo.mejor = false;
                nodo.valor = valor;
                nodo.Valido = (valor != 0);
                //nodo.coste = MAX_VALOR;;

                nodo.cota_positiva = max(dim_x-1-i, dim_y-1-j);

                cota_pesimista += nodo.Valido ? 1 : 0;

                matriz[i][j] = nodo;

            }
        }

        file.close();

        mostrarmatriz(matriz);

        int pos_init_x=0;
        int pos_init_y=0;
        int coste_inicial = 0;//matriz[pos_init_x][pos_init_y].valor;
        maze_bb(pos_init_x, pos_init_y, matriz, coste_inicial);
        printf("Mejor peso: %d\n", mejorPeso);
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