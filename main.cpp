#include <bits/stdc++.h>
#include "Instancia.h"
#include "generador.h"
#include "validacion.h"

using namespace std;

void guardarInstancia(
    const Instancia& casa,
    int numeroInstancia
) {

    string nombreArchivo =
        "instancia_"
        + casa.tipo
        + "_"
        + to_string(numeroInstancia)
        + ".txt";

    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo "
             << nombreArchivo << endl;
        return;
    }

    // ============================================================
    // DATOS GENERALES
    // ============================================================

    archivo << "==============================" << endl;
    archivo << "DATOS DE LA INSTANCIA" << endl;
    archivo << "==============================" << endl;

    archivo << "Tipo: " << casa.tipo << endl;
    archivo << "Semilla: " << casa.semilla << endl;

    archivo << "\nDIMENSIONES" << endl;

    archivo << "Largo: " << casa.largo << " m" << endl;
    archivo << "Ancho: " << casa.ancho << " m" << endl;
    archivo << "Altura: " << casa.altura << " m" << endl;
    archivo << "Superficie: " << casa.superficie << " m2" << endl;
    archivo << "Volumen: " << casa.volumen << " m3" << endl;
    archivo << "Umax: " << casa.Umax << endl;

    archivo << "Cantidad total de elementos: "
            << casa.elementos.size()
            << endl;


    // ============================================================
    // OBTENER CATALOGOS POR CATEGORIA
    // ============================================================

    map<string, vector<Alternativa>> catalogos;

    for (const Elemento& elemento : casa.elementos) {

        if (!elemento.fijo &&
            catalogos.find(elemento.categoria) == catalogos.end()) {

            catalogos[elemento.categoria] =
                elemento.alternativas;
        }
    }


    // ============================================================
    // GUARDAR CATALOGOS UNA SOLA VEZ
    // ============================================================

    archivo << "\n==============================" << endl;
    archivo << "CATALOGOS POR CATEGORIA" << endl;
    archivo << "==============================" << endl;

    for (const auto& par : catalogos) {

        const string& categoria = par.first;
        const vector<Alternativa>& alternativas = par.second;

        archivo << "\n------------------------------" << endl;
        archivo << "Categoria: " << categoria << endl;

        archivo << "Cantidad de alternativas: "
                << alternativas.size()
                << endl;

        for (const Alternativa& alternativa : alternativas) {

            archivo
                << "  "
                << alternativa.nombre
                << " | U: "
                << alternativa.U
                << " | Costo/m2: $"
                << alternativa.costoM2
                << endl;
        }
    }


    // ============================================================
    // ELEMENTOS
    // ============================================================

    int cantidadFijos = 0;
    int cantidadOptimizables = 0;

    set<string> categorias;

    archivo << "\n==============================" << endl;
    archivo << "ELEMENTOS" << endl;
    archivo << "==============================" << endl;

    for (const Elemento& elemento : casa.elementos) {

        categorias.insert(elemento.categoria);

        archivo << "\n------------------------------" << endl;

        archivo << "Nombre: "
                << elemento.nombre
                << endl;

        archivo << "Categoria: "
                << elemento.categoria
                << endl;

        archivo << "Area: "
                << elemento.area
                << " m2"
                << endl;


        if (elemento.fijo) {

            cantidadFijos++;

            archivo << "Tipo: FIJO" << endl;

            archivo << "U fijo: "
                    << elemento.Ufijo
                    << endl;

            archivo << "Costo fijo/m2: $"
                    << elemento.costoFijoM2
                    << endl;
        }

        else {

            cantidadOptimizables++;

            archivo << "Tipo: OPTIMIZABLE" << endl;

            archivo << "Catalogo utilizado: "
                    << elemento.categoria
                    << endl;

            archivo << "Cantidad de alternativas: "
                    << elemento.alternativas.size()
                    << endl;
        }
    }


    // ============================================================
    // RESUMEN
    // ============================================================

    archivo << "\n==============================" << endl;
    archivo << "RESUMEN" << endl;
    archivo << "==============================" << endl;

    archivo << "Categorias: "
            << categorias.size()
            << endl;

    archivo << "Catalogos generados: "
            << catalogos.size()
            << endl;

    archivo << "Elementos totales: "
            << casa.elementos.size()
            << endl;

    archivo << "Elementos fijos: "
            << cantidadFijos
            << endl;

    archivo << "Elementos optimizables: "
            << cantidadOptimizables
            << endl;

    archivo << "Umax: "
            << casa.Umax
            << endl;

    archivo << "Superficie: "
            << casa.superficie
            << " m2"
            << endl;

    archivo.close();

    cout << "Archivo guardado: "
         << nombreArchivo
         << endl;
}


// ============================================================
// MAIN
// ============================================================

int main() {

    vector<string> tipos = {
        "pequena",
        "mediana",
        "grande"
    };

    int semilla = 101;

    int totalValidas = 0;
    int totalDescartadas = 0;


    for (const string& tipo : tipos) {

        int validasTipo = 0;

        cout << "\n========================================" << endl;
        cout << "GENERANDO INSTANCIAS TIPO: "
             << tipo
             << endl;
        cout << "========================================" << endl;


        while (validasTipo < 5) {

            Instancia casa =
                generarInstancia(
                    tipo,
                    semilla
                );

            cout << "\nProbando semilla: "
                 << semilla
                 << endl;


            if (!validarInstancia(casa)) {

                cout << "Resultado: INVALIDA - descartada"
                     << endl;

                totalDescartadas++;

                semilla++;

                continue;
            }


            cout << "Resultado: VALIDA" << endl;

            validasTipo++;
            totalValidas++;

            guardarInstancia(
                casa,
                validasTipo
            );

            semilla++;
        }
    }


    cout << "\n========================================" << endl;
    cout << "RESUMEN FINAL" << endl;
    cout << "========================================" << endl;

    cout << "Instancias validas generadas: "
         << totalValidas
         << endl;

    cout << "Instancias descartadas: "
         << totalDescartadas
         << endl;

    cout << "Resultado: "
         << "5 pequenas + 5 medianas + 5 grandes"
         << endl;

    return 0;
}
