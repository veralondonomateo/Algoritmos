#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

void cargarDatos(vector<vector<char>>& matrizletras, vector<vector<string>>& palabras) {
    ifstream archivoLetras("matrizletras.txt");
    ifstream archivoPalabras("palabras.txt");

    if (!archivoLetras.is_open() || !archivoPalabras.is_open()) {
        cerr << "Error al abrir los archivos de datos." << endl;
        exit(EXIT_FAILURE);
    }

    string linea;
    while (getline(archivoLetras, linea)) {
        vector<char> filaLetras;
        for (char letra : linea) {
            if (letra != ' ') {
                filaLetras.push_back(letra);
            }
        }
        matrizletras.push_back(filaLetras);
    }

    while (getline(archivoPalabras, linea)) {
        vector<string> filaPalabras;
        string palabra;
        istringstream streamLinea(linea);
        while (streamLinea >> palabra) {
            filaPalabras.push_back(palabra);
        }
        palabras.push_back(filaPalabras);
    }

    archivoLetras.close();
    archivoPalabras.close();
}

void letrastopalabras() {
    int vidas = 3;
    int puntos = 0;
    int intentos = 0;
    const int tiempolimite = 40;
    const int tiempoRespuesta = 10; // Tiempo máximo de respuesta permitido en segundos
    vector<vector<char>> matrizletras;
    vector<vector<string>> palabras;

    // Cargar datos desde los archivos
    cargarDatos(matrizletras, palabras);

    cout << "Bienvenido al juego de encuentra las palabras - Puedes encontrar como máximo 8 palabras" << endl;

    // Inicializar la semilla para el generador de números aleatorios
    srand(time(0));

    steady_clock::time_point tiempoInicio = steady_clock::now();

    while (vidas > 0 && intentos <= 8) {
        int ronda = rand() % matrizletras.size(); // Seleccionar una fila al azar

        cout << "Basado en estas letras tienes 10 segundos para adivinar la mayor cantidad de palabras: ";
        for (auto &&i : matrizletras[ronda]) {
            cout << i << ' ';
        }
        cout << endl;

        while (duration_cast<seconds>(steady_clock::now() - tiempoInicio).count() < tiempolimite && vidas > 0 ) {
            // Tomar el tiempo justo antes de pedir la entrada del usuario
            steady_clock::time_point tiempoPregunta = steady_clock::now();
            string varaux;
            cout << endl << "Ingresa una palabra. Tienes 10 segundos para ingresarla ==> ";
            cin >> varaux;
            intentos += 1;

            // Tomar el tiempo justo después de recibir la entrada del usuario
            steady_clock::time_point tiempoRespuestaUsuario = steady_clock::now();

            // Verificar si se ha excedido el tiempo de respuesta
            if (duration_cast<seconds>(tiempoRespuestaUsuario - tiempoPregunta).count() > tiempoRespuesta) {
                cout << "Te has demorado más de " << tiempoRespuesta << " segundos. La respuesta se considera incorrecta." << endl << endl;
                vidas -= 1;
                cout << "Puntos: " << puntos << endl;
                cout << "Vidas: " << vidas << endl;
                continue;
            }

            // Convertir la entrada del usuario a mayúsculas para la comparación
            transform(varaux.begin(), varaux.end(), varaux.begin(), ::toupper);

            vector<string> palabrasusu = palabras[ronda];
            auto it = find(palabrasusu.begin(), palabrasusu.end(), varaux);

            if (it != palabrasusu.end()) {
                cout << "La palabra \"" << varaux << "\" está correcta. Ganaste 50 puntos" << endl << endl;
                puntos += 50;
            } else {
                cout << "La palabra \"" << varaux << "\" no está correcta. Perdiste 1 vida" << endl << endl;
                vidas -= 1;
            }
            cout << "Puntos: " << puntos << endl;
            cout << "Vidas: " << vidas << endl;
            cout << "Llevas " << intentos << " intentos." << endl; // mostramos en pantalla cuantos intentos llevas.

            // Esperar un segundo antes de la siguiente iteración
            this_thread::sleep_for(seconds(2));
        }
    }

    if (vidas == 0) {
        cout << endl << "Te quedaron 0 vidas" << endl << "Perdiste" << endl;
        cout << "Tuviste " << puntos << " puntos" << endl;
    } else {
        cout << "TUVISTE  " << puntos << " PUNTOS " << endl;
    }
}

int main() {
    letrastopalabras();
    return 0;
}