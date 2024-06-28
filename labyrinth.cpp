#include <iostream> // Incluye la biblioteca para entrada y salida estándar
#include <vector> // Incluye la biblioteca para usar vectores
#include <stack> // Incluye la biblioteca para usar pilas (stack)
#include <cstdlib> // Incluye la biblioteca para funciones de C estándar (rand, srand)
#include <ctime> // Incluye la biblioteca para manejo de tiempo (time)

using namespace std; // Permite usar el espacio de nombres estándar

class Celda {
public:
    int x, y; // Coordenadas de la celda en el laberinto
    bool visitado; // Indica si la celda ha sido visitada
    bool pared_superior, pared_inferior, pared_izquierda, pared_derecha; // Paredes de la celda
    bool en_camino; // Indica si la celda está en el camino de la solución

    // Constructor que inicializa una celda con sus coordenadas y todas las paredes intactas
    Celda(int x, int y) : x(x), y(y), visitado(false), pared_superior(true), pared_inferior(true), pared_izquierda(true), pared_derecha(true), en_camino(false) {}
};

// Inicializa el laberinto con celdas no visitadas y todas las paredes intactas
vector<vector<Celda>> inicializar_laberinto(int ancho, int alto) {
    vector<vector<Celda>> laberinto; // Vector bidimensional para almacenar el laberinto
    for (int y = 0; y < alto; ++y) {
        vector<Celda> fila; // Vector para almacenar una fila del laberinto
        for (int x = 0; x < ancho; ++x) {
            fila.emplace_back(x, y); // Añade una nueva celda a la fila
        }
        laberinto.push_back(fila); // Añade la fila al laberinto
    }
    return laberinto; // Devuelve el laberinto inicializado
}

// Verifica si una celda vecina es válida (dentro del laberinto y no visitada)
bool es_vecino_valido(int x, int y, const vector<vector<Celda>>& laberinto, int ancho, int alto) {
    return x >= 0 && x < ancho && y >= 0 && y < alto && !laberinto[y][x].visitado;
}

// Obtiene los vecinos válidos de una celda
vector<Celda*> obtener_vecinos(Celda& celda, vector<vector<Celda>>& laberinto, int ancho, int alto) {
    vector<Celda*> vecinos; // Vector para almacenar los punteros a los vecinos válidos
    int x = celda.x;
    int y = celda.y;

    // Verifica y añade los vecinos válidos (superior, derecha, inferior, izquierda)
    if (es_vecino_valido(x, y - 1, laberinto, ancho, alto)) // Superior
        vecinos.push_back(&laberinto[y - 1][x]);
    if (es_vecino_valido(x + 1, y, laberinto, ancho, alto)) // Derecha
        vecinos.push_back(&laberinto[y][x + 1]);
    if (es_vecino_valido(x, y + 1, laberinto, ancho, alto)) // Inferior
        vecinos.push_back(&laberinto[y + 1][x]);
    if (es_vecino_valido(x - 1, y, laberinto, ancho, alto)) // Izquierda
        vecinos.push_back(&laberinto[y][x - 1]);

    return vecinos; // Devuelve el vector de vecinos válidos
}

// Genera el laberinto utilizando el algoritmo de backtracking iterativo
void generar_laberinto(vector<vector<Celda>>& laberinto, int ancho, int alto) {
    stack<Celda*> pila; // Pila de celdas para seguir el camino actual
    Celda* actual = &laberinto[0][0]; // Iniciar desde la celda superior izquierda
    actual->visitado = true; // Marcar la celda actual como visitada
    pila.push(actual); // Agregar la celda actual a la pila

    while (!pila.empty()) {
        actual = pila.top(); // Obtener la celda en la cima de la pila
        vector<Celda*> vecinos = obtener_vecinos(*actual, laberinto, ancho, alto); // Obtener los vecinos válidos de la celda actual

        if (!vecinos.empty()) { // Si hay vecinos no visitados
            Celda* siguiente_celda = vecinos[rand() % vecinos.size()]; // Elegir un vecino aleatorio
            siguiente_celda->visitado = true; // Marcar el vecino elegido como visitado

            // Eliminar la pared entre la celda actual y la siguiente celda
            if (siguiente_celda->x == actual->x) { // Si están en la misma columna
                if (siguiente_celda->y < actual->y) {
                    actual->pared_superior = false; // Elimina la pared superior de la celda actual
                    siguiente_celda->pared_inferior = false; // Elimina la pared inferior de la siguiente celda
                } else {
                    actual->pared_inferior = false; // Elimina la pared inferior de la celda actual
                    siguiente_celda->pared_superior = false; // Elimina la pared superior de la siguiente celda
                }
            } else { // Si están en la misma fila
                if (siguiente_celda->x < actual->x) {
                    actual->pared_izquierda = false; // Elimina la pared izquierda de la celda actual
                    siguiente_celda->pared_derecha = false; // Elimina la pared derecha de la siguiente celda
                } else {
                    actual->pared_derecha = false; // Elimina la pared derecha de la celda actual
                    siguiente_celda->pared_izquierda = false; // Elimina la pared izquierda de la siguiente celda
                }
            }

            pila.push(siguiente_celda); // Agregar la siguiente celda a la pila
        } else { // Si no hay vecinos válidos, hacer backtracking
            pila.pop(); // Sacar la celda actual de la pila
        }
    }
}

// Encuentra un camino en el laberinto desde la celda actual hasta la salida utilizando DFS recursivo
bool encontrar_camino(vector<vector<Celda>>& laberinto, Celda* actual, int salida_x, int salida_y, int ancho, int alto) {
    if (actual->x == salida_x && actual->y == salida_y) { // Si la celda actual es la salida
        actual->en_camino = true; // Marcar la celda como parte del camino
        return true; // Camino encontrado
    }

    actual->visitado = true; // Marcar la celda actual como visitada
    vector<Celda*> vecinos;

    // Verifica y añade los vecinos accesibles (superior, derecha, inferior, izquierda)
    if (actual->y > 0 && !laberinto[actual->y - 1][actual->x].visitado && !actual->pared_superior)
        vecinos.push_back(&laberinto[actual->y - 1][actual->x]); // Superior
    if (actual->x < ancho - 1 && !laberinto[actual->y][actual->x + 1].visitado && !actual->pared_derecha)
        vecinos.push_back(&laberinto[actual->y][actual->x + 1]); // Derecha
    if (actual->y < alto - 1 && !laberinto[actual->y + 1][actual->x].visitado && !actual->pared_inferior)
        vecinos.push_back(&laberinto[actual->y + 1][actual->x]); // Inferior
    if (actual->x > 0 && !laberinto[actual->y][actual->x - 1].visitado && !actual->pared_izquierda)
        vecinos.push_back(&laberinto[actual->y][actual->x - 1]); // Izquierda

    // Recorre los vecinos para encontrar un camino
    for (Celda* vecino : vecinos) {
        if (encontrar_camino(laberinto, vecino, salida_x, salida_y, ancho, alto)) {
            actual->en_camino = true; // Marcar la celda actual como parte del camino
            return true; // Camino encontrado
        }
    }

    return false; // No se encontró camino
}

// Resetea el estado visitado de todas las celdas del laberinto
void resetear_visitados(vector<vector<Celda>>& laberinto) {
    for (auto& fila : laberinto) {
        for (auto& celda : fila) {
            celda.visitado = false; // Marcar todas las celdas como no visitadas
            celda.en_camino = false; // Marcar todas las celdas como no parte del camino
        }
    }
}

// Imprime el laberinto en consola
void imprimir_laberinto(vector<vector<Celda>>& laberinto, int entrada_x, int salida_x, int ancho, int alto) {
    for (int y = 0; y < alto; ++y) {
        // Imprimir paredes superiores
        for (int x = 0; x < ancho; ++x) {
            cout << (laberinto[y][x].pared_superior ? "+---" : "+   ");
        }
        cout << "+" << endl;

        // Imprimir paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            if (y == 0 && x == entrada_x)
                cout << "  "; // Entrada del laberinto
            else
                cout << (laberinto[y][x].pared_izquierda ? "| " : "  ");

            if (laberinto[y][x].en_camino)
                cout << "* "; // Parte del camino
            else
                cout << "  "; // Celda vacía
        }
        cout << "|" << endl;
    }

    // Imprimir paredes inferiores de la última fila
    for (int x = 0; x < ancho; ++x) {
        cout << (laberinto[alto - 1][x].pared_inferior ? "+---" : "+   ");
    }
    cout << "+" << endl;
}

// Programa principal
int main() {
    srand(time(0)); // Inicializar la semilla para números aleatorios

    int ancho, alto; // Variables para el ancho y alto del laberinto
    cout << "Ingrese el ancho del laberinto: ";
    cin >> ancho; // Leer ancho del laberinto
    cout << "Ingrese el alto del laberinto: ";
    cin >> alto; // Leer alto del laberinto

    vector<vector<Celda>> laberinto = inicializar_laberinto(ancho, alto); // Inicializar el laberinto

    generar_laberinto(laberinto, ancho, alto); // Generar el laberinto

    int entrada_x = 0; // Coordenada X de la entrada
    int salida_x = ancho - 1; // Coordenada X de la salida

    resetear_visitados(laberinto); // Resetear el estado visitado de las celdas
    encontrar_camino(laberinto, &laberinto[0][entrada_x], salida_x, alto - 1, ancho, alto); // Encontrar un camino

    imprimir_laberinto(laberinto, entrada_x, salida_x, ancho, alto); // Imprimir el laberinto con el camino

    return 0; // Finalizar el programa
}
