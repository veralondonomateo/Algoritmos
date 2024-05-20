#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

void letrastopalabras() {
    int vidas = 3;
    int puntos = 0;
    int intentos = 0;
    const int tiempolimite = 40;
    const int tiempoRespuesta = 10; // Tiempo máximo de respuesta permitido en segundos
    vector<vector<char>> matrizletras = {
        {'L', 'E', 'A', 'R', 'T', 'H', 'D', 'G', 'O', 'N'}, //se añadir mas letras para mayor poder construir mas palabras
        {'O', 'M', 'P', 'S', 'I', 'N', 'E', 'T', 'A', 'R'},
        {'R', 'A', 'C', 'T', 'E', 'I', 'V', 'L', 'D', 'U'},
        {'S', 'U', 'N', 'I', 'O', 'T', 'A', 'R', 'M', 'E'},
        {'D', 'E', 'R', 'F', 'A', 'T', 'I', 'L', 'O', 'P'},
        {'G', 'O', 'L', 'N', 'A', 'I', 'B', 'S', 'C', 'E'},
        {'H', 'U', 'M', 'A', 'N', 'E', 'D', 'T', 'R', 'L'}
    };
    vector<vector<string>> palabras = {
        {"REAL", "HORA", "ARTE", "DAR", "GATO", "LEON", "TARDE", "LARGO"},
        {"MES", "PINO", "SOL", "PISO", "MESA", "SIRENA", "TRAMPA", "ROMPE"},
        {"CARRO", "VER", "TIERRA", "VIA", "CIELO", "LUCERO", "CARTA", "ACUERDO"},
        {"SOL", "TU", "UNO", "TINO", "MUSA", "SANTO", "SOMAR", "NUEVO"},
        {"ARTE", "RED", "FIAR", "TARDE", "PEDIR", "DELITO", "PORTAR", "DIFERIR"},
        {"LAGO", "GALLO", "AGIL", "BAILON", "CINE", "SILLA", "BOTAR", "LOGICO"},
        {"MANO", "LUNA", "HUMO", "AMEN", "TERMA", "RUTINA", "LENTE", "DUELO"}
    };

    cout << "Bienvenido al juego de encuentra las palabras - Puedes encontrar como máximo 8 palabras" << endl;

    // Inicializar la semilla para el generador de números aleatorios
    srand(time(0));

    steady_clock::time_point tiempoInicio = steady_clock::now();

    while (vidas > 0 && intentos <= 8) {  //eliminamos el parametro de puntos debido a que el jugador podrá seguir jugando hasta que no tenga vidas.
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
                cout<<"puntos : "<<puntos<<endl;
                cout<<"vidas : "<<vidas<<endl;
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
            cout <<"llevas "<<intentos<<" intentos."<<endl; // mostramos en pantalla cuantos intentos llevas.

            // Esperar un segundo antes de la siguiente iteración
            this_thread::sleep_for(seconds(2));
        }
    }

    if (vidas == 0) {
        cout <<endl<< "Te quedaron 0 vidas"<<endl<< "Perdiste" << endl;
        cout << "Tuviste " << puntos << " puntos" << endl;
    } else {
        
        cout << "TUVISTE  " << puntos << " PUNTOS " << endl;
    }
}

int main() {
    letrastopalabras();
    return 0;
}