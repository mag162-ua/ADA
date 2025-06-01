// Autor: Martín Aznar García, 51797315G

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <ctime>
#include <algorithm>
#include <climits>

using namespace std;

constexpr int COSTE_MAXIMO = 9999;    // Valor alto inicial para costes

int ancho, alto;                      // Dimensiones del laberinto

// Opciones del programa pasadas por línea de comandos
bool mostrarCamino2D = false;
bool mostrarPasos = false;
bool archivoEspecificado = false;

// Estadísticas del algoritmo
int totalVisitados = 0;
int totalExplorados = 0;
int totalHojas = 0;
int totalNoFactibles = 0;
int totalNoPrometedores = 0;
int totalPrometedoresDescartados = 0;
int actualizacionesSolucion = 0;
int actualizacionesCotaPesimista = 0; // Contador de actualizaciones de cota pesimista

int cotaPesimistaInicial = 0;  // Suma inicial para cota pesimista

// Representación de cada celda en el laberinto
struct Celda {
    int fila;
    int columna;
    int costeEntrada;
    bool transitable;
    int heuristica;     // Estimación heurística (distancia restante)
    int costeAcumulado; // Mejor coste conocido hasta esta celda
    bool enCaminoOptimo;
};

// Mejor solución encontrada globalmente
int mejorCosteEncontrado = COSTE_MAXIMO;
vector<int> rutaOptima;

// Movimientos en 8 direcciones (N, NE, E, SE, S, SW, W, NW)
const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
const int codigosDireccion[8] = {4, 3, 5, 2, 6, 8, 1, 7}; // Como teclado numérico

// Función para mostrar la matriz de costes (debug)
void imprimirMatrizCostes(const vector<vector<Celda>>& laberinto) {
    for (int i = 0; i < ancho; i++) {
        for (int j = 0; j < alto; j++) {
            cout << laberinto[i][j].costeEntrada << " ";
        }
        cout << "\n";
    }
}

// Marca en la matriz las celdas del camino óptimo para visualización
void marcarCaminoOptimo(vector<vector<Celda>>& laberinto) {
    int x = 0, y = 0;
    if (mejorCosteEncontrado == COSTE_MAXIMO) return;

    laberinto[x][y].enCaminoOptimo = true;
    for (int paso : rutaOptima) {
        switch (paso) {
            case 1: y--; break;
            case 2: x--; y++; break;
            case 3: x--; break;
            case 4: x--; y--; break;
            case 5: y++; break;
            case 6: x++; y--; break;
            case 7: x++; break;
            case 8: x++; y++; break;
            default: break;
        }
        laberinto[x][y].enCaminoOptimo = true;
    }
}

// Imprime la matriz con el camino óptimo marcado con '*'
void mostrarLaberintoConCamino(const vector<vector<Celda>>& laberinto) {
    for (int i = 0; i < ancho; i++) {
        for (int j = 0; j < alto; j++) {
            if (laberinto[i][j].enCaminoOptimo)
                cout << '*';
            else
                cout << static_cast<char>(laberinto[i][j].costeEntrada + '0');
        }
        cout << "\n";
    }
}

// Muestra la secuencia de pasos guardada en la ruta óptima
void imprimirSecuenciaPasos() {
    cout << "<";
    for (int paso : rutaOptima) cout << paso;
    cout << ">\n";
}

// Estructura que guarda el estado para la cola de prioridad A*
struct EstadoBusqueda {
    int fila, columna;
    int costeHastaAhora;
    int costeEstimadoTotal; // costeHastaAhora + heurística
    vector<int> pasosRealizados;

    bool operator>(const EstadoBusqueda& otro) const {
        return costeEstimadoTotal > otro.costeEstimadoTotal;
    }
};

// Algoritmo A* para encontrar el camino mínimo con ramificación y poda
void maze_bb(vector<vector<Celda>>& laberinto) {
    priority_queue<EstadoBusqueda, vector<EstadoBusqueda>, greater<EstadoBusqueda>> pq;

    pq.push({0, 0, laberinto[0][0].costeEntrada, laberinto[0][0].costeEntrada + laberinto[0][0].heuristica, {}});

    while (!pq.empty()) {
        EstadoBusqueda actual = pq.top();
        pq.pop();
        totalVisitados++;

        int f = actual.fila;
        int c = actual.columna;

        if (actual.costeHastaAhora >= laberinto[f][c].costeAcumulado) {
            totalNoPrometedores++;
            continue;
        }

        laberinto[f][c].costeAcumulado = actual.costeHastaAhora;

        if (f == ancho - 1 && c == alto - 1) {
            totalHojas++;
            if (actual.costeHastaAhora <= mejorCosteEncontrado) {
                mejorCosteEncontrado = actual.costeHastaAhora;
                rutaOptima = actual.pasosRealizados;
                actualizacionesSolucion++;
            }
            if (mejorCosteEncontrado < cotaPesimistaInicial) {
                cotaPesimistaInicial = mejorCosteEncontrado;
                actualizacionesCotaPesimista++;
            }
            continue;
        }
        bool prometedorDescartado = false;
        for (int i = 0; i < 8; i++) {
            int nx = f + dx[codigosDireccion[i] - 1];
            int ny = c + dy[codigosDireccion[i] - 1];

            if (nx < 0 || ny < 0 || nx >= ancho || ny >= alto) continue;
            if (!laberinto[nx][ny].transitable) {
                totalNoFactibles++;
                continue;
            }

            int nuevoCoste = actual.costeHastaAhora + laberinto[nx][ny].costeEntrada;
            int estimacion = nuevoCoste + laberinto[nx][ny].heuristica;

            if (estimacion >= mejorCosteEncontrado) {
                totalNoPrometedores++;
                continue;
            }

            if (laberinto[nx][ny].costeAcumulado <= nuevoCoste) {
                totalNoPrometedores++;
                continue;
            }

            prometedorDescartado = true || prometedorDescartado;
            totalExplorados++;

            vector<int> pasosActualizados = actual.pasosRealizados;
            pasosActualizados.push_back(codigosDireccion[i]);
            
            pq.push({nx, ny, nuevoCoste, estimacion, pasosActualizados});
        }
        if(!prometedorDescartado) {
            totalPrometedoresDescartados++;
        }
    }
}

// Lee el archivo y prepara el laberinto para el algoritmo
void procesarArchivo(const string& nombreArchivo) {
    ifstream fichero(nombreArchivo);
    if (!fichero) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\n";
        cerr << "Uso: maze [--p2D] [-p] -f archivo\n";
        return;
    }

    fichero >> ancho >> alto;
    vector<vector<Celda>> laberinto(ancho, vector<Celda>(alto));

    cotaPesimistaInicial = 0;
    for (int i = 0; i < ancho; i++) {
        for (int j = 0; j < alto; j++) {
            int valor;
            fichero >> valor;

            Celda celda;
            celda.fila = i;
            celda.columna = j;
            celda.costeEntrada = valor;
            celda.transitable = (valor != 0);
            celda.enCaminoOptimo = false;
            celda.costeAcumulado = COSTE_MAXIMO;
            celda.heuristica = max(ancho - 1 - i, alto - 1 - j);

            if (celda.transitable)
                cotaPesimistaInicial += valor;

            laberinto[i][j] = celda;
        }
    }
    fichero.close();

    if (!laberinto[0][0].transitable || !laberinto[ancho-1][alto-1].transitable) {
        mejorCosteEncontrado = 0;
        return;
    }

    clock_t inicio = clock();
    maze_bb(laberinto);
    clock_t fin = clock();

    cout << mejorCosteEncontrado << "\n";
    cout <<  totalVisitados << " " << totalExplorados << " " << totalHojas << " " << totalNoFactibles << " " << totalNoPrometedores << " " << totalPrometedoresDescartados << " " << actualizacionesSolucion << " " << actualizacionesCotaPesimista << "\n";
    cout << (fin - inicio) * 1000.0 / CLOCKS_PER_SEC << " ms\n";

    if (mejorCosteEncontrado != 0 && mostrarCamino2D) {
        marcarCaminoOptimo(laberinto);
        cout << "Camino óptimo:\n";
        mostrarLaberintoConCamino(laberinto);
    }

    if (mostrarPasos) {
        if (rutaOptima.empty()) cout << "<0>\n";
        else imprimirSecuenciaPasos();
    }
}

int main(int argc, char* argv[]) {
    string archivoEntrada;

    if (argc == 1) {
        cerr << "Uso:\nmaze [--p2D] [-p] -f archivo\n";
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--p2D") {
            mostrarCamino2D = true;
        } else if (arg == "-p") {
            mostrarPasos = true;
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                archivoEntrada = argv[++i];
                archivoEspecificado = true;
            } else {
                cerr << "Error: Falta el nombre del archivo.\nUso: maze [--p2D] [-p] -f archivo\n";
                return -1;
            }
        } else {
            cerr << "Argumento desconocido: " << arg << "\n";
            cerr << "Uso: maze [--p2D] [-p] -f archivo\n";
            return -1;
        }
    }

    if (!archivoEspecificado) {
        cerr << "Error: Se requiere el parámetro -f para especificar archivo.\n";
        cerr << "Uso: maze [--p2D] [-p] -f archivo\n";
        return -1;
    }

    procesarArchivo(archivoEntrada);

    return 0;
}
