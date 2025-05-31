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
    //int x; //Posición en x
    //int y; //Posición en y
	bool Valido; //Es valido?
	int cota_positiva; //Menor distancia hasta el destino
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
}
*/

// USO DE RECURSIÓN ITERATIVA
// Arrays para movernos en las 8 direcciones posibles del laberinto
const int mov_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; // Cambios en la coordenada x
const int mov_y[8] = { 0,  1, 1, 1, 0, -1, -1, -1}; // Cambios en la coordenada y

const int directions[] = {4, 3, 5, 2, 6, 8, 1, 7};  // Números que identifican cada dirección (para guardar el camino)

// Función recursiva que implementa ramificación y poda para encontrar camino mínimo en el laberinto
bool maze_bb(int pos_x, int pos_y, vector<vector<Nodo> > &matriz, int coste, vector<int> &pasos){
    
    // Si la posición actual no es válida, incrementamos contador de nodos no factibles y devolvemos false
    if(!matriz[pos_x][pos_y].Valido) {
        no_factibles++;
        return false;
    }

    // Caso base: si hemos llegado a la esquina inferior derecha (final)
    if(pos_x == dim_x - 1 && pos_y == dim_y - 1) {
        hojas++; // Contamos que hemos llegado a una hoja (posible solución)

        // Si el coste actual más el valor de esta posición es mejor que el mejor peso encontrado hasta ahora
        if(coste + matriz[pos_x][pos_y].valor < mejorPeso) {
            mejorPeso = coste + matriz[pos_x][pos_y].valor; // Actualizamos el mejor coste
            mejorPasos = pasos; // Guardamos el camino que llevó a ese coste

            soluciones_actualizadas++; // Contamos que actualizamos solución óptima
        }

        // Si el mejor peso mejora la cota pesimista, actualizamos también la cota y contador
        if (mejorPeso < cota_pesimista) {
            cota_pesimista = mejorPeso;
            soluciones_cpesimista++;
        }
        return true; // Hemos encontrado una solución válida
    }

    visitados++; // Contamos que estamos visitando este nodo
    matriz[pos_x][pos_y].visitado = true; // Marcamos la posición actual como visitada para no revisarla de nuevo en esta rama

    // Exploramos las 8 direcciones posibles
    for(int i = 0; i < 8; i++) {
        int new_x = pos_x + mov_x[directions[i] - 1]; // Nueva coordenada x según dirección
        int new_y = pos_y + mov_y[directions[i] - 1]; // Nueva coordenada y según dirección

        // Comprobamos que la nueva posición está dentro del laberinto y no ha sido visitada antes
        if(new_x < 0 || new_x >= dim_x || new_y < 0 || new_y >= dim_y || matriz[new_x][new_y].visitado) {
            continue; // Si no cumple, pasamos a la siguiente dirección
        }

        // Si la nueva posición no es válida
        if(!matriz[new_x][new_y].Valido) {
            no_factibles++; // Contamos nodos no factibles
            continue; // Saltamos a siguiente dirección
        }

        // Si el coste acumulado más el valor de la nueva posición y su cota positiva es mejor que el mejor peso actual
        if (matriz[new_x][new_y].cota_positiva + coste + matriz[new_x][new_y].valor < mejorPeso) {
            explorados++; // Contamos que exploramos este nodo

            pasos.push_back(directions[i]); // Añadimos la dirección al vector de pasos que llevamos

            // Llamada recursiva a maze_bb con la nueva posición y coste actualizado
            maze_bb(new_x, new_y, matriz, coste + matriz[new_x][new_y].valor, pasos);

            pasos.pop_back(); // Deshacemos el último paso añadido (backtracking)
        } else {
            no_prometedores++; // Si no es prometedor, aumentamos contador de nodos descartados
        }
    }

    matriz[pos_x][pos_y].visitado = false; // Al terminar todas las exploraciones desde este nodo, lo marcamos como no visitado (backtracking)

    return false; // Devolvemos false porque no garantizamos que encontramos una solución en esta rama (solo en caso base se devuelve true)
}


// USO DE DICCIONARIO PARA EVITAR REPETICIONES
/*
#include <unordered_map>
#include <utility> // std::pair
#include <functional> // std::hash

// Definición de un struct para calcular el hash de un par (int, int),
// necesario para usar std::pair<int,int> como clave en unordered_map.
struct pair_hash {
    size_t operator() (const std::pair<int,int>& p) const {
        // Obtener el hash para la primera parte del par (p.first)
        auto h1 = std::hash<int>{}(p.first);
        // Obtener el hash para la segunda parte del par (p.second)
        auto h2 = std::hash<int>{}(p.second);
        // Combinar los dos hashes con un desplazamiento para evitar colisiones simples
        return h1 ^ (h2 << 1);
    }
};

// Movimientos posibles en el laberinto en 8 direcciones (orden específico)
const int mov_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int mov_y[8] = { 0,  1, 1, 1, 0, -1, -1, -1};

// Dirección numérica asociada a cada movimiento para registrar pasos
const int directions[] = {4, 3, 5, 2, 6, 8, 1, 7};

// Función principal que implementa ramificación y poda para encontrar
// el camino de coste mínimo desde (pos_x, pos_y) hasta el destino mediante el uso de diccionario.
// Parámetros:
// - pos_x, pos_y: posición actual en la matriz
// - mapa: diccionario con nodos (posiciones) y su información (visitados, valores, cotas, etc.)
// - coste: coste acumulado hasta la posición actual
// - pasos: vector donde se registran las direcciones tomadas para llegar a la solución
bool maze_bb(int pos_x, int pos_y, unordered_map<pair<int,int>, Nodo, pair_hash> &mapa, int coste, vector<int> &pasos){

    // Crear la clave para el nodo actual con la posición (pos_x, pos_y)
    pair<int, int> key_actual = std::make_pair(pos_x, pos_y);
    // Obtener la referencia al nodo actual desde el mapa
    Nodo &nodo_actual = mapa[key_actual];

    // Caso base: si llegamos a la posición objetivo (esquina inferior derecha)
    if(pos_x == dim_x - 1 && pos_y == dim_y - 1) {

        hojas++; // Incrementar contador de nodos hoja

        // Si el coste actual + el valor del nodo es mejor que el mejor coste encontrado hasta ahora
        if(coste + nodo_actual.valor < mejorPeso) {

            mejorPeso = coste + nodo_actual.valor; // Actualizar mejor coste
            mejorPasos = pasos; // Guardar los pasos que llevaron a esta solución

            soluciones_actualizadas++; // Contador de soluciones encontradas o actualizadas

            // Actualizar cota pesimista (mejor solución encontrada) si es mejor
            if (mejorPeso < cota_pesimista) {
                cota_pesimista = mejorPeso;
                soluciones_cpesimista++;
            }
        }
        return true; // Se encontró una solución
    }

    visitados++; // Incrementar contador de nodos visitados
    nodo_actual.visitado = true; // Marcar el nodo actual como visitado para evitar ciclos
    bool descartado_prometedor = false; // Variable para indicar si algún nodo hijo fue prometedor y se exploró

    // Iterar sobre las 8 direcciones posibles de movimiento
    for(int i = 0; i < 8; i++) {
        int new_x = pos_x + mov_x[directions[i] - 1]; // Calcular nueva posición en X
        int new_y = pos_y + mov_y[directions[i] - 1]; // Calcular nueva posición en Y

        // Comprobar que la nueva posición está dentro de los límites
        if(new_x < 0 || new_x >= dim_x || new_y < 0 || new_y >= dim_y) {
            continue; // Saltar esta iteración si la posición es inválida
        }

        // Crear la clave para la posición siguiente
        pair<int, int> key_siguiente = std::make_pair(new_x, new_y);
        // Buscar el nodo siguiente en el mapa
        auto it = mapa.find(key_siguiente);
        if(it == mapa.end()) continue;  // Si no existe el nodo, saltar esta iteración

        Nodo &nodo_siguiente = it->second; // Referencia al nodo siguiente

        // Si el nodo siguiente ya fue visitado, saltar para evitar ciclos
        if(nodo_siguiente.visitado) {
            continue;
        }

        // Si el nodo siguiente no es válido
        if(!nodo_siguiente.Valido) {
            no_factibles++;
            continue;
        }

        // Si la cota optimista + coste actual + valor del nodo siguiente
        // es menor que el mejor coste encontrado hasta ahora, es prometedor explorar
        if (nodo_siguiente.cota_positiva + coste + nodo_siguiente.valor < mejorPeso) {

            explorados++; // Incrementar contador de nodos explorados

            pasos.push_back(directions[i]); // Añadir la dirección actual a la ruta de pasos

            // Recursivamente llamar a maze_bb para el nodo siguiente y acumular el resultado
            descartado_prometedor = maze_bb(new_x, new_y, mapa, coste + nodo_siguiente.valor, pasos) || descartado_prometedor;
            
            pasos.pop_back(); // Deshacer el paso tras volver de la recursión
        } else {
            no_prometedores++; // Incrementar contador de nodos descartados por no ser prometedores
        }
    }

    // Si ningún hijo fue prometedor, incrementamos contador de poda
    if(!descartado_prometedor) {
        prometedor_descartado++;
    }

    nodo_actual.visitado = false; // Desmarcar el nodo actual para que pueda explorarse en otro camino

    return false; // Devuelve false indicando que no se encontró camino prometedor desde aquí
}
*/
#include <queue>
#include <tuple> // Para std::tuple
void calcular_cota_positiva(vector<vector<Nodo> >& matriz) {
    int dim_x = (int)matriz.size();
    int dim_y = (int)matriz[0].size();

    // Inicializar cota_optimista con infinito
    for (int i = 0; i < dim_x; ++i)
        for (int j = 0; j < dim_y; ++j)
            matriz[i][j].cota_positiva = MAX_VALOR;

    // Cola de prioridad: {coste acumulado, x, y}
    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater< > > pq;

    int destino_x = dim_x - 1;
    int destino_y = dim_y - 1;

    matriz[destino_x][destino_y].cota_positiva = 0;
    pq.push({0, {destino_x, destino_y}});

    while (!pq.empty()) {
        auto actual = pq.top(); pq.pop();
        int coste = actual.first;
        int x = actual.second.first;
        int y = actual.second.second;

        if (coste > matriz[x][y].cota_positiva) continue;

        for (int dir = 0; dir < 8; ++dir) {
            int nx = x + mov_x[dir];
            int ny = y + mov_y[dir];

            if (nx < 0 || ny < 0 || nx >= dim_x || ny >= dim_y) continue;
            if (!matriz[nx][ny].Valido) continue;

            int nuevo_coste = coste + matriz[nx][ny].valor;
            if (nuevo_coste < matriz[nx][ny].cota_positiva) {
                matriz[nx][ny].cota_positiva = nuevo_coste;
                pq.push({nuevo_coste, {nx, ny}});
            }
        }
    }
}

void abrir_fichero(const string file_name){

    ifstream file(file_name.c_str());

    if(file.is_open()){
        file >> dim_x;
        file >> dim_y;

        //vector< vector<int>> matriz(dim_x,vector<int>(dim_y))
        vector<vector<Nodo> > matriz(dim_x, vector<Nodo>(dim_y));

        for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                int valor;
                file >> valor;

                Nodo nodo;
                //nodo.x = i;
                //nodo.y = j;
                nodo.visitado = false;
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
        calcular_cota_positiva(matriz);

        maze_bb(pos_init_x, pos_init_y, matriz, coste_inicial, pasos);
        clock_t end_time = clock();
        printf("Mejor peso: %d\n", mejorPeso);
        
        cout<<(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC<<endl;
        cout<<"análisis del laberinto:"<<endl;

        if(mejorPeso!=0){
            if(p2D){
                cout<<"Mejor camino: "<<endl;
                mejorcamino(matriz);
                cout<<"Mejor camino: "<<endl;
                dibujar_p2D(matriz);
            }
        }
        if(p){
            if(mejorPasos.size()==0){
                cout<<"<0>"<<endl;
            }
            else{
                mostrar_pasos();
            }
        }
        
        /*file >> dim_x;
        file >> dim_y;

        //vector< vector<int>> matriz(dim_x,vector<int>(dim_y))
        //vector<vector<Nodo> > matriz(dim_x, vector<Nodo>(dim_y));
        unordered_map<pair<int,int>, Nodo, pair_hash> mapa;

        for(int i=0;i<dim_x;i++){
            for(int j=0;j<dim_y;j++){ 
                int valor;
                file >> valor;
                if(valor != 0){
                    cota_pesimista += valor;
                
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

                //cota_pesimista += nodo.Valido ? nodo.valor : 0;

                //matriz[i][j] = nodo;
                mapa[make_pair(i, j)] = nodo;
                }
            }
        }
        int pos_init_x=0;
        int pos_init_y=0;
        int coste_inicial = 0;
        vector<int> pasos;
        vector<pair<int, int> > camino;
        clock_t start_time = clock();
        if(!mapa[make_pair(0, 0)].Valido || !mapa[make_pair(dim_x-1, dim_y-1)].Valido){
            mejorPeso = 0;
        }
        maze_bb(pos_init_x, pos_init_y, mapa, coste_inicial, pasos);
        clock_t end_time = clock();
        printf("Mejor peso: %d\n", mejorPeso);
        cout<<(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC<<endl;

        file.close();
        */
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