#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>  // Para trabajar con archivos de texto
#include <sstream>  // Para trabajar con cadenas de texto

using namespace std;
using namespace std::chrono;

struct estructura {
    string nombre;
    int puntos;
};

// Función para cargar los datos desde los archivos de texto
void cargarDatos(vector<vector<char>>& matrizletras, vector<vector<string>>& palabras) {
    ifstream archivoLetras("matrizletras.txt");    // Abre el archivo de letras
    ifstream archivoPalabras("palabras.txt");      // Abre el archivo de palabras

    if (!archivoLetras.is_open() || !archivoPalabras.is_open()) {  // Verifica si los archivos se abrieron correctamente
        cerr << "Error al abrir los archivos de datos." << endl;    // Mensaje de error si no se pudieron abrir los archivos
        exit(EXIT_FAILURE);  // Sale del programa con un estado de fallo
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

    archivoLetras.close();   // Cierra el archivo de letras
    archivoPalabras.close(); // Cierra el archivo de palabras
}

// Función para escribir la información del jugador en un archivo
void guardarJugador(const estructura& jugador) {
    ofstream archivoJugadores("jugadores.txt", ios::app); // Abre el archivo de jugadores en modo de escritura, agregando al final

    if (!archivoJugadores.is_open()) {  // Verifica si el archivo se abrió correctamente
        cerr << "Error al abrir el archivo de jugadores." << endl; // Mensaje de error si no se pudo abrir el archivo
        return;
    }

    archivoJugadores << "Nombre: " << jugador.nombre << ", Puntos: " << jugador.puntos << endl; // Escribe la información del jugador en el archivo
    archivoJugadores.close(); // Cierra el archivo de jugadores
}

// Función principal del juego
void letrastopalabras() {
    int vidas = 3;
    int puntos = 0;
    int intentos = 0;
    string nombre;
    const int tiempolimite = 40;
    const int tiempoRespuesta = 10; // Tiempo máximo de respuesta permitido en segundos
    vector<vector<char>> matrizletras;
    vector<vector<string>> palabras;

    // Cargar datos desde los archivos
    cargarDatos(matrizletras, palabras);

    cout << "Bienvenido al juego de encuentra las palabras - Puedes encontrar como máximo 8 palabras" << endl;
    cout << "Ingresa tu nombre - ";
    cin >> nombre;
    estructura jugador;
    jugador.nombre = nombre;
    jugador.puntos = puntos;    
    
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
                cout << "La palabra