//Martín Aznar García, 51797315G

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility> // Para std::pair
#include <ctime>
#include <cmath> // Para sqrt


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

struct Nodo{
    int valor; //Valor del nodo
    int x; //Posición en x
    int y; //Posición en y
    //int coste; //Coste mínimo hasta este nodo
	bool Valido; //Es valido?
	int cota_positiva; //Menor distancia hasta el destino
    bool mejor; //Es el mejor nodo hasta ahora
    bool visitado; //Ha sido visitado?
};

int mejorPeso = MAX_VALOR; 
vector<int> mejorPasos;
vector<pair<int, int> > mejorCamino;

void mostrarmatriz(const vector<vector<Nodo> > &matriz){
    for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                cout<<matriz[i][j].valor<<" ";
            }
            cout<<endl;
        }
    
}
void mejorcamino(vector<vector<Nodo> > &matriz){
    //cout<<"a :"<<mejorPeso<<" "<<mejorPasos.size()<<endl;
    int pos_ini_x = 0;
    int pos_ini_y = 0;
    if(mejorPeso != MAX_VALOR){
        matriz[pos_ini_x][pos_ini_y].visitado = true;
        //cout<<"Mejor camino: ";
        for(int i=0;i<mejorPasos.size();i++){
            //cout<<"("<<pos_ini_x<<", "<<pos_ini_y<<") "<<endl;
            //cout<<mejorPasos[i]<<" "<<endl;
            switch (mejorPasos[i]) {
                case 1: //arriba
                    matriz[--pos_ini_x][pos_ini_y].visitado = true;
                    break;
                case 2: //arriba derecha
                    matriz[--pos_ini_x][++pos_ini_y].visitado = true;
                    break;
                case 3: //derecha
                    matriz[pos_ini_x][++pos_ini_y].visitado = true;
                    break;
                case 4: //abajo derecha
                    matriz[++pos_ini_x][++pos_ini_y].visitado = true;
                    break;
                case 5: //abajo
                    matriz[++pos_ini_x][pos_ini_y].visitado = true;
                    break;
                case 6: //abajo izquierda
                    matriz[++pos_ini_x][--pos_ini_y].visitado = true;
                    break;
                case 7: //izquierda
                    matriz[pos_ini_x][--pos_ini_y].visitado = true;
                    break;
                case 8: //arriba izquierda
                    matriz[--pos_ini_x][--pos_ini_y].visitado = true;
                    break;
            }
        }  
    }
}

void dibujar_p2D(const vector<vector<Nodo> > &matriz){
    for(int i=0;i<dim_x;i++){
        for(int j=0;j<dim_y;j++){
            if(matriz[i][j].visitado){
                cout<<"*";
            }
            else{
                cout<<matriz[i][j].valor;
            }
            
        }
        cout<<endl;
    }
}

void mostrar_pasos(){
    cout<<"<";
    for(int i=0;i<mejorPasos.size();i++){
        cout<<mejorPasos[i];
    }
    cout<<">"<<endl;
}
//USO DE RECURSIÓN PURA
/*bool maze_bb(int pos_x, int pos_y, vector<vector< Nodo> > &matriz, int coste, vector<int> &pasos){


    visitados++;

    if(!matriz[pos_x][pos_y].Valido){
        no_factibles++;
        return false;
    }
    if(matriz[pos_x][pos_y].cota_positiva + coste>= mejorPeso){
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
            mejorPasos.clear();
            mejorPasos = pasos;
            mejorCamino.clear();
            //mejorCamino = camino;
            mejorCamino.push_back(make_pair(pos_x, pos_y));
            soluciones_actualizadas++;
        }
        if (mejorPeso < cota_pesimista) {
            cota_pesimista = mejorPeso;
            soluciones_cpesimista++;
        }
        return true;
    }

    matriz[pos_x][pos_y].visitado = true;
    coste += matriz[pos_x][pos_y].valor;
    bool llega_al_final = false;;

    if(pos_x < dim_x-1 && pos_y < dim_y-1 && !matriz[pos_x+1][pos_y+1].visitado && matriz[pos_x+1][pos_y+1].Valido){
        //Mover hacia abajo y a la derecha
        explorados++;
        pasos.push_back(4);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x+1, pos_y+1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_x < dim_x-1 && !matriz[pos_x+1][pos_y].visitado && matriz[pos_x+1][pos_y].Valido){
        //Mover hacia abajo
        explorados++;
        pasos.push_back(5);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x+1, pos_y, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_y < dim_y-1 && !matriz[pos_x][pos_y+1].visitado && matriz[pos_x][pos_y+1].Valido){
        //Mover hacia la derecha
        explorados++;
        pasos.push_back(3);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x, pos_y+1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_x > 0 && pos_y < dim_y-1 && !matriz[pos_x-1][pos_y+1].visitado && matriz[pos_x-1][pos_y+1].Valido){
        //Mover hacia arriba y a la derecha
        explorados++;
        pasos.push_back(2);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x-1, pos_y+1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_y > 0 && pos_x < dim_x-1 && !matriz[pos_x+1][pos_y-1].visitado && matriz[pos_x+1][pos_y-1].Valido){
        //Mover hacia abajo y a la izquierda
        explorados++;
        pasos.push_back(6);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x+1, pos_y-1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_x > 0 && !matriz[pos_x-1][pos_y].visitado && matriz[pos_x-1][pos_y].Valido){
        //Mover hacia arriba
        explorados++;
        pasos.push_back(1);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x-1, pos_y, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_y > 0 && !matriz[pos_x][pos_y-1].visitado && matriz[pos_x][pos_y-1].Valido){
        //Mover hacia la izquierda
        explorados++;
        pasos.push_back(7);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x, pos_y-1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }
    if(pos_x > 0 && pos_y > 0 && !matriz[pos_x-1][pos_y-1].visitado && matriz[pos_x-1][pos_y-1].Valido){
        //Mover hacia arriba y a la izquierda
        explorados++;
        pasos.push_back(8);
        //camino.push_back(make_pair(pos_x, pos_y));
        llega_al_final = maze_bb(pos_x-1, pos_y-1, matriz, coste, pasos) || llega_al_final;
        //camino.pop_back();
        pasos.pop_back(); // Deshacemos el último paso
    }

    matriz[pos_x][pos_y].visitado = false;

    if(!llega_al_final){
        coste -= matriz[pos_x][pos_y].valor;
        prometedor_descartado++;
    }

    return llega_al_final;
}*/

// USO DE RECURSIÓN ITERATIVA
/*const int mov_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int mov_y[8] = { 0,  1, 1, 1, 0, -1, -1, -1};
const int directions[] = {4, 3, 5, 2, 6, 8, 1, 7}; // Direcciones correspondientes a los cambios

bool maze_bb(int pos_x, int pos_y, vector<vector<Nodo> > &matriz, int coste, vector<int> &pasos){
    if(!matriz[pos_x][pos_y].Valido) {
        no_factibles++;
        return false;
    }
    if(pos_x == dim_x - 1 && pos_y == dim_y - 1) {
        hojas++;
        if(coste + matriz[pos_x][pos_y].valor < mejorPeso) {
            mejorPeso = coste + matriz[pos_x][pos_y].valor;
            mejorPasos = pasos;
            soluciones_actualizadas++;
        }
        if (mejorPeso < cota_pesimista) {
            cota_pesimista = mejorPeso;
            soluciones_cpesimista++;
        }
        return true;
    }

    visitados++;
    matriz[pos_x][pos_y].visitado = true;

    for(int i = 0; i < 8; i++) {
        int new_x = pos_x + mov_x[directions[i] - 1];
        int new_y = pos_y + mov_y[directions[i] - 1];
        bool sigue = false;
        
        if(new_x < 0 || new_x >= dim_x || new_y < 0 || new_y >= dim_y || matriz[new_x][new_y].visitado) {
            continue;
        }
        if(!matriz[new_x][new_y].Valido) {
            no_factibles++;
            continue;
        }

        if (matriz[new_x][new_y].cota_positiva + coste + matriz[new_x][new_y].valor < mejorPeso) {
            explorados++;
            
            pasos.push_back(directions[i]);

            maze_bb(new_x, new_y, matriz, coste + matriz[new_x][new_y].valor, pasos);

            matriz[new_x][new_y].visitado = false;
            pasos.pop_back();
        } else {
            no_prometedores++;
        }
    }
    matriz[pos_x][pos_y].visitado = false;
    return false;
}*/


void abrir_fichero(const string file_name){

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
                nodo.mejor = false;
                nodo.valor = valor;
                nodo.Valido = (valor != 0);
                //nodo.coste = MAX_VALOR;;

                nodo.cota_positiva = max(dim_x-1-i, dim_y-1-j); //Heuristica
                //nodo.cota_positiva = sqrt((dim_x-1-i)*(dim_x-1-i) + (dim_y-1-j)*(dim_y-1-j)); //Euclidia

                cota_pesimista += nodo.Valido ? nodo.valor : 0;

                matriz[i][j] = nodo;

            }
        }

        file.close();

        //mostrarmatriz(matriz);

        int pos_init_x=0;
        int pos_init_y=0;
        int coste_inicial = 0;
        vector<int> pasos;
        vector<pair<int, int> > camino;
        clock_t start_time = clock();
        if(!matriz[0][0].Valido || !matriz[dim_x-1][dim_y-1].Valido){
            mejorPeso = 0;
        }
        maze_bb(pos_init_x, pos_init_y, matriz, coste_inicial, pasos/*, camino*/);
        clock_t end_time = clock();
        printf("Mejor peso: %d\n", mejorPeso);
        cout<<(end_time - start_time) * 1000.00 / (double)CLOCKS_PER_SEC << " seconds" << endl;
        //vector< vector<int>> camino(dim_n,vector<int>(dim_m));
        //vector< vector<int>> optimista(dim_n,vector<int>(dim_m));

        //double tiempo = maze_bt(matriz);
        
        //cout<<mejorPeso<<endl;

        //cout<<visitados<<" "<<explorados<<" "<<hojas_visitadas<<" "<<descartados_no_factibles<<" "<<descartados_no_prometedores<<endl;

        //cout<<tiempo<<endl;
        cout<<"análisis del laberinto:"<<endl;
        if(p){
            if(mejorPasos.size()==0){
                cout<<"<0>"<<endl;
            }
            else{
                mostrar_pasos();
            }
        }

        if(mejorPeso!=0){
            if(p2D){
                cout<<"Mejor camino: "<<endl;
                mejorcamino(matriz);
                cout<<"Mejor camino: "<<endl;
                dibujar_p2D(matriz);
            }
        }
        
        
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