// PROJECTE FINAL DE INFORMATICA ESEIAAT 2025-2026
// Rafael Gras, Mykola Stefanskyy, Albert Sabadell
// Versió 0.0.4 per Albert Sabadell
// Canvis:
// --> Añadir subprogramas Entidad 3 i comenzar Entidad 4

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct t_multa{
    int codi, import, punts_retirar;
    string desccurtaCA, desccurtaES, descllargaCA, descllargaES, norm;
    char tipus; //M (movimiento), E (estacionamiento)
};

struct t_imposicio {
    string concepte;
    string valor;
    string valordesc_ca;
    string valordesc_es;
};

struct t_comanda {
    string entitat;
    string accio;
    string info;
    string hora;
};

typedef vector<t_comanda> v_comandes;

typedef vector<t_imposicio> v_imposicio;

typedef vector<t_multa> v_multa;

const string FICHERO_CODIGO_SANCIONES = "2024_denuncies_sancions_transit_codis.csv";
const string FICHERO_SUBTIPUS_EXPEDIENT = "2024_denuncies_sancions_transit_conceptes_annex.csv"; // Entidad 3 y 4


const string MENU_PRINCIPAL =
"\nSelecciona que tipo de accion desea hacer:\n"
"1. Acciones básicas sobre la entidad \"1: infracciones/denuncias Barcelona\" \n"   // Entidad 1 --> Rafael Gras
"2. Acciones básicas sobre la entidad \"2: tipos de multas\" \n"                    // Entidad 2 --> Mykola Stefansky
"3. Acciones básicas sobre la entidad \"3: registro de uso\" \n"                    // Entidad 3 --> Albert Sabadell
"4. A. Específicas: Diagnóstico de denuncias en Barcelona \n"                       // Acción específica 1
"5. A. Específicas: Diagnóstico de denuncias en una zona específica \n"             // Acción específica 2
"6. A. Específicas: Validación de datos \n"                                         // Acción específica 3
"7. Salir del programa \n --> ";



const string MENU_ENTIDAD_2 =
"¿Qué quiere hacer con la entidad \"2: tipos de multas\"?\n"
"1. Consultar la información sobre un tipo de multa.\n"
"2. Añadir una nuevo tipo de multa al sistema.\n"
"3. Eliminar un tipo de multa del sistema.\n"
"4. Modificar un tipo de multa.\n -->";

//Subprogramas de Mykola Stefansky: Menu y Entidad 2 (multas)
bool Menu(v_multa& v2, v_imposicio& v3);
void LecturaFicheroE2(v_multa&v1);
void ModificarFicheroE2(const v_multa&v1);
void ConsultaInfoE2(const v_multa&v1);
void AnadirInfoE2(v_multa&v1);
void EliminarInfoE2(v_multa&v1);
void ModificarInfoE2(v_multa&v1);

//Subprogramas Albert Sabadell

bool MenuEntidad3(v_imposicio& v3);
void LecturaFicheroE3(v_imposicio& v3);
void ModificarFicheroE3(const v_imposicio& v3);
void ConsultaInfoE3(const v_imposicio& v3);
void AnadirInfoE3(v_imposicio& v3);
void EliminarInfoE3(v_imposicio& v3);
void ModificarInfoE3(v_imposicio& v3);

// Subprogramas Rafael Gras


int main(){
    setlocale(LC_ALL,""); // Esta función no estrictamente necesaria pero sirve para mostrar todos los carácteres del alfabeto español en la salido cout

    cout << "Inicializando Gestor-Denuncias-Barcelona..." << endl << endl;

    cout << "Leyendo fichero " << FICHERO_CODIGO_SANCIONES << endl;
    v_multa v2;
    LecturaFicheroE2(v2);

    cout << "Leyendo fichero " << FICHERO_SUBTIPUS_EXPEDIENT << endl;
    v_imposicio v3;
    LecturaFicheroE3(v3);

    while (!Menu(v2, v3)); // El programa se acaba cuando Menu() devuelve true (el usuario ha solicitado salir del programa)
}

bool Menu(v_multa& v2, v_imposicio& v3) { // Devuelve true cuando el programa debe finalizar
    int opcion, opcion2;
    cout << MENU_PRINCIPAL;
    cin >> opcion;

    switch (opcion) {
        case 1: {
            cout << "Pendiente de hacer..." << endl;
            break; }
        case 2: {
            cout << MENU_ENTIDAD_2;
            cin >> opcion2;
            switch (opcion2) {
                case 1 : {ConsultaInfoE2(v2); break;}
                case 2 : {AnadirInfoE2(v2); break;}
                case 3 : {EliminarInfoE2(v2); break;}
                case 4 : {ModificarInfoE2(v2); break;}
                default : {cout << "La opción introducida no es válida" << endl; break;}
                }
            break; }
        case 3: {
           while (!MenuEntidad3(v3)); // Menú específico de la Entidad 3
           break;
                  }
        case 4: {
            cout << "Pendiente de hacer..." << endl;
            break;}
        case 5: {
            cout << "Pendiente de hacer..." << endl;
            break;}
        case 6: {
            cout << "Pendiente de hacer..." << endl;
            break;}
        case 7: {
            cout << "¡Hasta pronto!" << endl;
            break;}
        default: {cout << "Introduce una opción válida" << endl; break; }
    }
    return opcion == 7;
}

void LecturaFicheroE2(v_multa&v1){

    ifstream cinf(FICHERO_CODIGO_SANCIONES);
    string titulo, postcoma;
    char coma;
    int k = 0;
    t_multa x;

    cinf >> titulo; //Extraer titulos de los datos (primera linea)
    while (cinf >> x.codi >> coma){ // Lectura y incorporacion al vector v1

        //Utilizamos la función getline para poder leer hasta la siguiente coma y así leer solo la siguiente casilla del csv
        getline(cinf, x.desccurtaCA,',');
        while (x.desccurtaCA[0] == '"' && x.desccurtaCA[x.desccurtaCA.size()-1] != '"'){ //Los campos que tienen comas tienen " en un .csv al comienzo y, por lo tanto, comprobamos este caso para evitar incorrecciones en las lecturas
            getline(cinf, postcoma, ',');
            x.desccurtaCA = x.desccurtaCA + "," + postcoma;}

        getline(cinf, x.desccurtaES,',');
        while (x.desccurtaES[0] == '"' && x.desccurtaES[x.desccurtaES.size()-1] != '"'){
            getline(cinf, postcoma, ',');
            x.desccurtaES = x.desccurtaES + "," + postcoma;}

        getline(cinf, x.descllargaCA,',');
        while (x.descllargaCA[0] == '"' && x.descllargaCA[x.descllargaCA.size()-1] != '"'){
            getline(cinf, postcoma, ',');
            x.descllargaCA = x.descllargaCA + "," + postcoma;}

        getline(cinf, x.descllargaES,',');
        while (x.descllargaES[0] == '"' && x.descllargaES[x.descllargaES.size()-1] != '"'){
            getline(cinf, postcoma, ',');
            x.descllargaES = x.descllargaES + "," + postcoma;}

        getline(cinf, x.norm,',');
        cinf >> x.import >> coma >> x.tipus >> coma >> x.punts_retirar;

        v1.push_back(x); //Introducimos los datos de la lectura en el vector
    }
}

void LecturaFicheroE3(v_imposicio& v3) {

    ifstream cinf(FICHERO_SUBTIPUS_EXPEDIENT);

    string linea, palabra;
    getline(cinf, linea);

    while (getline(cinf, linea)) {
        t_imposicio x;
        size_t pos = 0;
        int col = 0;

        while ((pos = linea.find(',')) != string::npos) {
            palabra = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            // eliminar posibles comillas
            if (!palabra.empty() && palabra[0] == '"') palabra = palabra.substr(1, palabra.size()-2);

            if (col == 0) x.concepte = palabra;
            else if (col == 1) x.valor = palabra;
            else if (col == 2) x.valordesc_ca = palabra;
            else if (col == 3) x.valordesc_es = palabra;

            col++;
        }

        if (!linea.empty()) x.valordesc_es = linea;

        v3.push_back(x);
    }
}

void ModificarFicheroE3(const v_imposicio& v3){
    ofstream fout(FICHERO_SUBTIPUS_EXPEDIENT);

    // Usamos esto para la Cabecera
    fout << "Concepte,Valor,Descripcio_CA,Descripcio_ES";

    for (int i = 0; i < v3.size(); i++){
        fout << endl;
        fout << v3[i].concepte << "," << v3[i].valor << ","
             << v3[i].valordesc_ca << "," << v3[i].valordesc_es;
    }
}

void AnadirInfoE3(v_imposicio& v3){
    t_imposicio nuevo;

    cout << "Introduce el concepto: ";
    cin.ignore();
    getline(cin, nuevo.concepte);

    cout << "Introduce el valor (código único): ";
    cin >> nuevo.valor;
    cin.ignore();

    cout << "Introduce la descripción en catalán: ";
    getline(cin, nuevo.valordesc_ca);

    cout << "Introduce la descripción en español: ";
    getline(cin, nuevo.valordesc_es);

    v3.push_back(nuevo);
    ModificarFicheroE3(v3);

    cout << "Subtipo añadido correctamente." << endl << endl;
}

void EliminarInfoE3(v_imposicio& v3){
    string codigo;
    bool encontrado = false;

    cout << "Introduce el valor (código) del subtipo a eliminar: ";
    cin >> codigo;

    for (int i = 0; i < v3.size() && !encontrado; i++){
        if (v3[i].valor == codigo){
            encontrado = true;
            v3.erase(v3.begin() + i);
            ModificarFicheroE3(v3);
            cout << "Subtipo eliminado correctamente." << endl << endl;
        }
    }
    if (!encontrado) cout << "No se encontró el subtipo." << endl << endl;
}

void ModificarInfoE3(v_imposicio& v3){
    string codigo;
    bool encontrado = false;

    cout << "Introduce el valor (código) del subtipo a modificar: ";
    cin >> codigo;
    cin.ignore();

    for (int i = 0; i < v3.size() && !encontrado; i++){
        if (v3[i].valor == codigo){
            encontrado = true;

            cout << "Nuevo concepto (actual: " << v3[i].concepte << "): ";
            getline(cin, v3[i].concepte);

            cout << "Nueva descripción en catalán (actual: " << v3[i].valordesc_ca << "): ";
            getline(cin, v3[i].valordesc_ca);

            cout << "Nueva descripción en español (actual: " << v3[i].valordesc_es << "): ";
            getline(cin, v3[i].valordesc_es);

            ModificarFicheroE3(v3);
            cout << "Subtipo modificado correctamente." << endl << endl;
        }
    }
    if (!encontrado) cout << "No se encontró el subtipo." << endl << endl;
}


void ConsultaInfoE3(const v_imposicio& v3) {
    string codigo;
    bool trobat = false;

    cout << "Introduce el código (Valor) a consultar: ";
    cin >> codigo;

    for (int i = 0; i < v3.size() && !trobat; i++) {
        if (v3[i].valor == codigo) {
            trobat = true;
            cout << "Concepto: " << v3[i].concepte << endl;
            cout << "Descripción (CA): " << v3[i].valordesc_ca << endl;
            cout << "Descripción (ES): " << v3[i].valordesc_es << endl << endl;
        }
    }
    if (!trobat) cout << "No se encontró información para el código " << codigo << endl << endl;
}

bool MenuEntidad3(v_imposicio& v3) {
    int opcion;
    cout << "\nMenu Entidad 3: Subtipos de expediente\n"
         << "1. Consultar un subtipo\n"
         << "2. Añadir un subtipo\n"
         << "3. Eliminar un subtipo\n"
         << "4. Modificar un subtipo\n"
         << "5. Salir\n--> ";
    cin >> opcion;

    switch(opcion) {
        case 1: ConsultaInfoE3(v3); break;
        case 2: AnadirInfoE3(v3); break;
        case 3: EliminarInfoE3(v3); break;
        case 4: ModificarInfoE3(v3); break;
        case 5: return true;
        default: cout << "Opción no válida" << endl; break;
        }
        return opcion == 5;
   }

void ModificarFicheroE2(const v_multa&v1){

    ofstream couth(FICHERO_CODIGO_SANCIONES);

    //Cabecera
    couth << "Infraccio_Codi,Infraccio_Desc_Curta_CA,Infraccio_Desc_Curta_ES,Infraccio_Desc_Llarga_CA,Infraccio_Desc_Llarga_ES,Normativa,Import_Nominal,Tipus_Infraccio,Punts_A_Retirar";

    for (int i = 0; i < v1.size(); i++){
        couth << endl;
        couth << v1[i].codi << "," << v1[i].desccurtaCA << "," << v1[i].desccurtaES << ",";
        couth << v1[i].descllargaCA << "," << v1[i].descllargaES << "," << v1[i].norm << ",";
        couth << v1[i].import << "," << v1[i].tipus << "," << v1[i].punts_retirar;
    }
}

void ConsultaInfoE2(const v_multa&v1){
        int leng = 1, n = 0;
        bool trobat = false;
        int codigo, opcionlenguaje;

        cout << "Introduce el código de la infracción a consultar: ";
        cin >> codigo;

        for (int i = 0; i < v1.size() && !trobat; i++){
            if (v1[i].codi == codigo) {
                    trobat = true;
                    n = i;}}

        if (!trobat) cout << "No se ha encontrado información." << endl << endl;
        else {

            while (leng != 0){
                cout << "¿Cómo quiere ver las descripciones de la infracción? (En catalán (0) o en español (1))";
                cin >> opcionlenguaje;
                if (opcionlenguaje == 0){
                cout << "DescCurtaCA: " << v1[n].desccurtaCA << endl;
                cout << "DescLlargaCA: " << v1[n].descllargaCA << endl;
                leng = 0;}
                else if (opcionlenguaje == 1){
                cout << "DescCurtaES: " << v1[n].desccurtaES << endl;
                cout << "DescLlargaES: " << v1[n].descllargaES << endl;
                leng = 0;}
                else cout << "Introduce una opción válida.";}

            cout << "Normativa: " << v1[n].norm << endl;
            cout << "Importe nominal: " << v1[n].import << " euros. " << endl;
            if (v1[n].tipus == 'E') cout << "Tipo: Estacionamiento" << endl;
            else cout << "Tipo: En movimiento " << endl;
            cout << "Puntos a retirar: " << v1[n].punts_retirar << endl << endl;}
}

void AnadirInfoE2(v_multa&v1){

    t_multa nuevo;
        string enter;

        cout << "Introduce el nuevo código de la infracción: ";
        cin >> nuevo.codi;
        getline(cin, enter); // Para evitar que la siguiente entrada de datos no se interrumpa por el enter de la primera entrada
        cout << "Introduce la descripción corta en catalán: ";
        getline(cin, nuevo.desccurtaCA);
        cout << "Introduce la descripción corta en castellano: ";
        getline(cin, nuevo.desccurtaES);
        cout << "Introduce la descripción larga en catalán: ";
        getline(cin, nuevo.descllargaCA);
        cout << "Introduce la descripción larga en castellano: ";
        getline(cin, nuevo.descllargaES);
        cout << "Introduce la normativa y el importe nominal: ";
        cin >> nuevo.norm >> nuevo.import;
        cout << "Introduce el tipo y los puntos a retirar: ";
        cin >> nuevo.tipus >> nuevo.punts_retirar;

        v1.push_back(nuevo);
        ModificarFicheroE2(v1);
        cout << endl << "Infracción añadida correctamente." << endl << endl;

}

void EliminarInfoE2(v_multa&v1){

    bool trobat = false;
    int codigo;
    cout << "Introduce el código de la multa a eliminar: ";
    cin >> codigo;

    for (int i = 0; i < v1.size() && !trobat; i++){
        if (v1[i].codi == codigo){
            trobat = true;
            for (int k = i; k < v1.size()-1; k++){ //Eliminar y reorganizar el vector
                v1[k] = v1[k+1];}
                v1.pop_back();} //eliminem l'últim que queda repetit
                ModificarFicheroE2(v1);
             }
    if (trobat) cout << endl << "Infracción eliminada correctamente" << endl << endl;
    else cout << endl << "No se ha podido encontrar la infracción a eliminar" << endl << endl;
}

void ModificarInfoE2(v_multa&v1){
    t_multa mod;
    bool trobat = false, salir = false;
    int n = 0, codigo, opcionmod = 0;
    bool k;
    string opcionlong, basura;

    cout << "Introduce el código de la multa a modificar: ";
    cin >> codigo;
    for (int i = 0; i < v1.size() && !trobat; i++){
        if (v1[i].codi == codigo){
            trobat = true;
            n = i;}}
            if (trobat){
                while(!salir){
                    k = true;
                    cout << "¿Qué deseas modificar? (Introduce el entero)" << endl << "1. Código de la infracción " << endl << "2. Descripción (CAT) " << endl << "3. Descripción (ESP) " << endl;
                    cout << "4. Normativa " << endl << "5. Importe nominal " << endl << "6. Tipo de la infracción " << endl << "7. Puntos a retirar " << endl << "8. Salir" << endl;
                    cin >> opcionmod;
                    if (opcionmod == 1){
                        cout << "Introduce el nuevo código: ";
                        cin >> mod.codi;
                        v1[n].codi = mod.codi;}
                    else if (opcionmod == 2){
                        cout << "¿Corta o Larga? ";
                        cin >> opcionlong;
                        getline(cin, basura); //Eliminamos la basura que queda (enter)
                        while (k){
                            if (opcionlong == "Corta"){
                                cout << "Introduce tu texto: ";
                                getline(cin, mod.desccurtaCA);
                                v1[n].desccurtaCA = mod.desccurtaCA;
                                k = false;
                                    }
                            else if (opcionlong == "Larga"){
                                cout << "Introduce tu texto: ";
                                getline(cin, mod.descllargaCA);
                                v1[n].descllargaCA = mod.descllargaCA;
                                k = false;
                                    }
                            else cout << "Introduce una opción válida" << endl;}
                            }
                    else if (opcionmod == 3){
                        cout << "¿Corta o Larga? ";
                        cin >> opcionlong;
                        getline(cin, basura); //Eliminamos la basura que queda (enter)
                        while (k){
                            if (opcionlong == "Corta"){
                                cout << "Introduce tu texto: ";
                                getline(cin, mod.desccurtaES);
                                v1[n].desccurtaES = mod.desccurtaES;
                                k = false;
                                        }
                            else if (opcionlong == "Larga"){
                                cout << "Introduce tu texto: ";
                                getline(cin, mod.descllargaES);
                                v1[n].descllargaES = mod.descllargaES;
                                k = false;}}
                                             }
                    else if (opcionmod == 4){
                        cout << "Introduce la nueva normativa: ";
                        cin >> mod.norm;
                        v1[n].norm = mod.norm;}
                    else if (opcionmod == 5){
                        cout << "Introduce el nuevo importe: ";
                        cin >> mod.import;
                        v1[n].import = mod.import;}
                    else if (opcionmod == 6){
                        cout << "Introduce el nuevo tipo: ";
                        cin >> mod.tipus;
                        v1[n].tipus = mod.tipus;}
                    else if (opcionmod == 7){
                        cout << "Introduce la nueva cantidad: ";
                        cin >> mod.punts_retirar;
                        v1[n].punts_retirar = mod.punts_retirar;}
                    else if (opcionmod == 8) salir = true;
                    else cout << "Introduce una entrada válida." << endl;}

                    ModificarFicheroE2(v1);}

                    else cout << endl << "No se ha podido encontrar la infracción a modificar" << endl << endl;
}













