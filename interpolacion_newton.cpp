#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Función auxiliar para multiplicar dos polinomios: P3 = P1 * P2
vector<double> multiplicarPolinomios(const vector<double>& p1, const vector<double>& p2) {
    vector<double> resultado(p1.size() + p2.size() - 1, 0.0);
    for (size_t i = 0; i < p1.size(); i++) {
        for (size_t j = 0; j < p2.size(); j++) {
            resultado[i + j] += p1[i] * p2[j];
        }
    }
    return resultado;
}

int main() {
    int n;
    cout << "--- Interpolación por Polinomio de Newton ---" << endl;
    cout << "Ingrese el número de puntos: ";
    cin >> n;

    if (n < 1) {
        cout << "Se necesita al menos 1 punto." << endl;
        return 1;
    }

    vector<double> x(n);
    vector<double> y(n);

    // INTERFAZ INTERACTIVA PARA LOS PUNTOS
    cout << "\n--- Ingreso de los Puntos (x, y) ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << "\n>>> PUNTO " << (i + 1) << " <<<" << endl;
        cout << "  x" << i << ": "; cin >> x[i];
        cout << "  y" << i << ": "; cin >> y[i];
    }
    cout << "-------------------------------------\n" << endl;

    // Matriz para la tabla de diferencias divididas
    // La primera columna [i][0] almacena los valores de y
    vector<vector<double>> f(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        f[i][0] = y[i];
    }

    // Cálculo de la tabla de diferencias divididas
    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            f[i][j] = (f[i + 1][j - 1] - f[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

    // El polinomio final tendrá como máximo grado (n-1)
    vector<double> polinomioFinal(n, 0.0);
    
    // El primer coeficiente de Newton (a0) es simplemente f[0][0]
    polinomioFinal[0] = f[0][0];

    // Construcción algebraica del polinomio término a término
    // terminoAcumulado representa el producto (x - x0)(x - x1)...
    vector<double> terminoAcumulado = {1.0}; 

    for (int i = 1; i < n; i++) {
        // Multiplicamos el acumulado por el nuevo binomio (x - x_{i-1})
        // En código, (x - x_k) se representa por el vector {-x_k, 1.0} -> 1*x^1 - x_k*x^0
        vector<double> binomio = {-x[i - 1], 1.0};
        terminoAcumulado = multiplicarPolinomios(terminoAcumulado, binomio);

        // El coeficiente ai es f[0][i]
        double ai = f[0][i];

        // Sumamos el término actual escalado por ai al polinomio final
        for (size_t k = 0; k < terminoAcumulado.size(); k++) {
            polinomioFinal[k] += terminoAcumulado[k] * ai;
        }
    }

    // MOSTRAR EL POLINOMIO FORMATEADO ESTÉTICAMENTE
    cout << "=== POLINOMIO DE NEWTON RESULTANTE ===" << endl;
    cout << "P(x) = ";
    
    bool primerTermino = true;
    // Se recorre desde la potencia más alta a la más baja
    for (int i = n - 1; i >= 0; i--) {
        // Ignorar coeficientes extremadamente cercanos a cero
        if (abs(polinomioFinal[i]) < 1e-7) continue;

        // Control de signos y espaciado intermedio
        if (!primerTermino && polinomioFinal[i] > 0) cout << " + ";
        if (polinomioFinal[i] < 0) {
            if (primerTermino) cout << "-";
            else cout << " - ";
        }

        // Imprimir el valor absoluto del coeficiente si no es 1 (o si es el término independiente x^0)
        double coef = abs(polinomioFinal[i]);
        if (coef != 1.0 || i == 0) {
            cout << fixed << setprecision(4) << coef;
        }

        // Imprimir la variable x y su exponente correspondiente
        if (i > 0) {
            cout << "x";
            if (i > 1) cout << "^" << i;
        }
        
        primerTermino = false;
    }
    
    if (primerTermino) cout << "0"; // Caso en que todos los coeficientes sean cero
    cout << "\n----------------------------------------" << endl;

    return 0;
}
