// Transformador de datos en CSV a formato plano separado por espacios, especialmente pensado para leer con ifstream de cpp
// Versió 1.0.0 por Rafael Gras

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string leerCasillaCSV(ifstream& cinf, bool& fin_del_archivo, bool& fin_de_linea ) {
    string cas = "";
    bool s=false;
    if (!fin_del_archivo) {
        int i;
        fin_del_archivo = (i=cinf.get())==EOF; // Leemeos el primer caracter
        if ( i=='\"' and !fin_del_archivo) {s=true; i=cinf.get() ;}; // salta el primer carácter ( " ) si la casilla es string

        if (s) {
            while (i!='\"' and !fin_del_archivo) {
                if (i=='\\') {i=cinf.get();}; // Cuando encuentra una \ la elimina y guarda el siguiente carácter, sin interpretar
                if (i==' ') {i='_';}; // convertir los espacios en _
                cas = cas + char(i);
                fin_del_archivo = (i=cinf.get())==EOF;
            }
            if (!fin_del_archivo) fin_del_archivo = (i=cinf.get())==EOF; // Saltar la coma
        } else {
            while (i!=',' and i!='\n' and !fin_del_archivo) {
                if (i==' ') {i='_';}; // convertir los espacios en _
                cas = cas + char(i);
                fin_del_archivo = (i=cinf.get())==EOF;
            }
        }
        fin_de_linea = i=='\n'; // fin de linea si el ultimo caracter leido es un salto de linea
    }
    return cas;
}

const bool eliminar_columnas = true;
const int N1 = 4;
const int columnes[N1] = {3,4,13,14}; // columnas a eliminar
int main() {
    ifstream entrada("2024_4t_denuncies_sancions_transit_detall.csv"); // ES MUY IMPORTANTE QUE NO EXISTAN FILAS VACIAS (a veces al final del archivo hay unos \n sobrantes)
    ofstream salida("2024_4t_denuncies_sancions_transit_detall.txt");
    bool fin_de_archivo = false, fin_de_linea=false;
    string casilla="";
    int columna=1;
    bool saltar_columna = false;
    bool columna_int = false;

    while (!fin_de_archivo) {
        casilla = leerCasillaCSV(entrada,fin_de_archivo,fin_de_linea);
        saltar_columna=false;
        columna_int=false;
        for (int i=0; i<N1;i++) {
            if (columna==columnes[i]) saltar_columna=true;
        }

        if (!saltar_columna) { // si se tiene que saltar la columna no escribe nada
            if (casilla!="" and casilla!="__" and casilla!="______")    salida << casilla;
            else                                    salida << "0"; // en casillas vacías dejar un cero
        }
        if (fin_de_linea)   {salida << "\n";columna=1;}
        else                { if (!saltar_columna) salida << " ";columna++;};
    }

}
