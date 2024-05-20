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

struct estructura {
    string nombre;
    int puntos;
};

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

void guardarJugador(const estructura& jugador) {
    ofstream archivoJugadores("jugadores.txt", ios::app);

    if (!archivoJugadores.is_open()) {
        cerr << "Error al abrir el archivo de jugadores." << endl;
        return;
    }

    archivoJugadores << "Nombre: " << jugador.nombre << ", Puntos: " << jugador.puntos << endl;
    archivoJugadores.close();
}


void letrastopalabras() {
    int vidas = 3;
    int puntos = 0;
    int intentos = 0;
    string nombre;
    const int tiempolimite = 40;
    const int tiempoRespuesta = 10;
    vector<vector<char>> matrizletras;
    vector<vector<string>> palabras;

    cargarDatos(matrizletras, palabras);

    cout << "Bienvenido al juego de encuentra las palabras - Puedes encontrar como máximo 8 palabras" << endl;
    cout << "Ingresa tu nombre - ";
    cin >> nombre;
    estructura jugador;
      
    
    srand(time(0));

    steady_clock::time_point tiempoInicio = steady_clock::now();

    while (vidas > 0 && intentos <= 8) {
        int ronda = rand() % matrizletras.size();

        cout << "Basado en estas letras tienes 10 segundos para adivinar la mayor cantidad de palabras: ";
        for (auto &&i : matrizletras[ronda]) {
            cout << i << ' ';
        }
        cout << endl;

        while (duration_cast<seconds>(steady_clock::now() - tiempoInicio).count() < tiempolimite && vidas > 0 ) {
            steady_clock::time_point tiempoPregunta = steady_clock::now();
            string varaux;
            cout << endl << "Ingresa una palabra. Tienes 10 segundos para ingresarla ==> ";
            cin >> varaux;
            intentos += 1;

            steady_clock::time_point tiempoRespuestaUsuario = steady_clock::now();

            if (duration_cast<seconds>(tiempoRespuestaUsuario - tiempoPregunta).count() > tiempoRespuesta) {
                cout << "Te has demorado más de " << tiempoRespuesta << " segundos. La respuesta se considera incorrecta." << endl << endl;
                vidas -= 1;
                cout << "Puntos: " << puntos << endl;
                cout << "Vidas: " << vidas << endl;
                continue;
            }

            transform(varaux.begin(), varaux.end(), varaux.begin(), ::toupper);

            vector<string> palabrasusu = palabras[ronda];
            auto it = find(palabrasusu.begin(), palabrasusu.end(), varaux);
            if (it != palabrasusu.end()) {
                cout << "La palabra" << varaux << "está correcta. Ganaste 50 puntos" << endl << endl;
                puntos += 50;
            } else {
                cout << "La palabra \"" << varaux << "\" no está correcta. Perdiste 1 vida" << endl << endl;
                vidas -= 1;
            }
            cout << "Puntos: " << puntos << endl;
            cout << "Vidas: " << vidas << endl;
            cout << "Llevas " << intentos << " intentos." << endl;

            this_thread::sleep_for(seconds(2));
        }
    }
    jugador.nombre = nombre;
    jugador.puntos = puntos;

    if (vidas == 0) {
        cout << endl << "Te quedaron 0 vidas" << endl << "Perdiste" << endl;
        cout << "Tuviste " << puntos << " puntos" << endl;
    } else {
        cout << "TUVISTE  " << puntos << " PUNTOS " << endl;
    }
    cout << "Gracias por jugar" << endl;

    guardarJugador(jugador);
}

void countingSort(vector<estructura>& jugadores, int exp) {
    int n = jugadores.size();
    vector<estructura> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(jugadores[i].puntos / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(jugadores[i].puntos / exp) % 10] - 1] = jugadores[i];
        count[(jugadores[i].puntos / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        jugadores[i] = output[i];
}

void radixsort(vector<estructura>& jugadores) {
    int maxPuntos = jugadores[0].puntos;
    int n = jugadores.size();

    for (int i = 1; i < n; i++) {
        if (jugadores[i].puntos > maxPuntos)
            maxPuntos = jugadores[i].puntos;
    }

    for (int exp = 1; maxPuntos / exp > 0; exp *= 10)
        countingSort(jugadores, exp);
}

void crearArchivoResumen() {
    vector<estructura> jugadores;
    ifstream archivoJugadores("jugadores.txt");

    if (!archivoJugadores.is_open()) {
        cerr << "Error al abrir el archivo de jugadores." << endl;
        return;
    }

    string linea;
    while (getline(archivoJugadores, linea)) {
        estructura jugador;
        stringstream ss(linea);
        string temp;
        ss >> temp; // Ignorar "Nombre:"
        ss >> jugador.nombre; // Leer el nombre del jugador
        ss >> temp; // Ignorar "Puntos:"
        ss >> jugador.puntos; // Leer los puntos del jugador
        jugadores.push_back(jugador);
    }

    archivoJugadores.close();

    radixsort(jugadores); // Ordenar los jugadores por puntos

    ofstream archivoResumen("resumen.txt");
    if (!archivoResumen.is_open()) {
        cerr << "Error al crear el archivo de resumen." << endl;
        return;
    }

    for (const auto& jugador : jugadores) {
        archivoResumen << "Nombre: " << jugador.nombre << ", Puntos: " << jugador.puntos << endl;
    }

    archivoResumen.close();
}

int main() {
    letrastopalabras();
    crearArchivoResumen();
    return 0;
}

