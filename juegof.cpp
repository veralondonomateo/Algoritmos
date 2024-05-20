#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void letrastopalabras() {
    int vidas = 3;
    int puntos = 0;
    const int tiempolimite = 40;
    const int tiempoRespuesta = 5; // Tiempo máximo de respuesta permitido en segundos
    vector<vector<char>> matrizletras = {
        {'L', 'E', 'A', 'R', 'T', 'H', 'D'},
        {'O', 'M', 'P', 'S', 'I', 'N', 'E'},
        {'R', 'A', 'C', 'T', 'E', 'I', 'V'},
        {'S', 'U', 'N', 'I', 'O', 'T', 'A'},
        {'D', 'E', 'R', 'F', 'A', 'T', 'I'},
        {'G', 'O', 'L', 'N', 'A', 'I', 'B'},
        {'H', 'U', 'M', 'A', 'N', 'E', 'D'}
    };
    vector<vector<string>> palabras = {
        {"real", "hora", "arte", "dar"},
        {"mes", "pino", "sol", "piso"},
        {"Carro", "Ver", "tierra", "via"},
        {"Sol", "Tú", "Uno", "tino"},
        {"arte", "Red", "Fiar", "Tarde"},
        {"lago", "gallo", "agil", "bailon"},
        {"mano", "luna", "humo", "amen"}
    };

    cout << "Bienvenido al juego de encuentra las palabras" << endl;

    steady_clock::time_point tiempoInicio = steady_clock::now();

    while (vidas > 0 && puntos < 100) {
        cout << "Basado en estas letras tienes 10 segundos para adivinar la mayor cantidad de palabras: ";
        for (auto &&i : matrizletras[0]) {
            cout << i << ',';
        }

        while (duration_cast<seconds>(steady_clock::now() - tiempoInicio).count() < tiempolimite && vidas > 0 && puntos < 100) {
            string varaux;
            cout << endl << "Ingresa una palabra. Tienes 5 segundos para ingresarla ==> ";
            cin >> varaux;

            vector<string> palabrasusu = palabras[0];
            auto it = find(palabrasusu.begin(), palabrasusu.end(), varaux);

            // Verificar si se ha excedido el tiempo de respuesta, si es así
            // con el continue vuelve y empieza sabiendo que el usuario tiene una
            // vida menos
            if (duration_cast<seconds>(steady_clock::now() - tiempoInicio).count() >= tiempoRespuesta) {
                cout << "Te has demorado más de " << tiempoRespuesta << " segundos. La respuesta se considera incorrecta." << endl << endl;
                vidas -= 1;
                continue;
            }

            if (it != palabrasusu.end()) {
                cout << "La palabra \"" << varaux << "\" está correcta. Ganaste 50 puntos" << endl << endl;
                puntos += 50;
            } else {
                cout << "La palabra \"" << varaux << "\" no está correcta. Perdiste 1 vida" << endl << endl;
                vidas -= 1;
            }

            // Esperar un segundo antes de la siguiente iteración
            this_thread::sleep_for(seconds(1));
        }
    }

    if (vidas == 0) {
        cout << "Te quedaron 0 vidas. Perdiste" << endl;
        cout << "Tuviste " << puntos << " puntos";
    } else {
        cout << "Ganaste ";
        cout << "Tuviste " << puntos << " puntos";
    }
}
;
int main() {
    letrastopalabras();
    return 0;
};
