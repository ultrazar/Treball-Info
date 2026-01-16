// PROJECTE FINAL DE INFORMATICA ESEIAAT 2025-2026
// Rafael Gras, Mykola Stefanskyy, Albert Sabadell
// Versió 1.0.0 per Rafael Gras

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct t_denuncia{ // tupla de entidad 1
    int any, mes, dia, hora, minuto;
    string subtipus_exp, mitja_impos, tipus_vehicle, nom_carrer;
    int codi_carrer, num_carrer, codi_districte, codi_barri, seccio_censal, import, infraccio_codi;
    double longitud, latitud;
    int grua; // No puede ser bool porque el valor -1 en cualquier parámetro significa "no especificado"
};

struct t_multa{ // tupla de entidad 2
    int codi, import, punts_retirar;
    string desccurtaCA, desccurtaES, descllargaCA, descllargaES, norm;
    char tipus; //M (movimiento), E (estacionamiento)
};

struct t_imposicio { // tupla de entidad 3
    string concepte;
    string valor;
    string valordesc_ca;
    string valordesc_es;
};

struct t_log {  // tupla de entidad 4 (registro de acciones)
    int id;
    string entitat;
    string operacio;
    string detall;
};

struct t_dencodigo{ // ( tupla para poder mostrar en pantalla la infraccion más recurrente )
    int codi;
    int num;
    };

struct t_denimp{ // ( tupla para poder mostrar en pantalla el medio de imposición más recurrente )
    string imp;
    int num;
};

struct t_calle{ // ( tupla para definir nombres de calles )
    int codigo;
    string nombre;
};

typedef vector<t_denuncia> v_denuncia; // Vector de entidad 1

typedef vector<t_multa> v_multa; // Vector de entidad 2

typedef vector<t_imposicio> v_imposicio; // Vector de entidad 3

typedef vector<t_log> v_log; // vector de entidad 4

typedef vector<t_dencodigo> v_dencodigo;

typedef vector<t_denimp> v_denimp;

// Los nombres de  calles, distritos y barrios se guardan en vectores separados para facilitar la introduccion de estos nombres por parte del usuario:
typedef vector<string> v_distritos; // hay 10 distritos

typedef vector<string> v_barrios; // Hay 73 barrios

typedef vector<t_calle> v_calles; // Aprox 4000-4500 calles

struct t_zonas { // Tupla que agrupa vectores de distritos, barrios, y calles, para obtener el nombre de cada uno
    v_distritos vd;
    v_barrios vb;
    v_calles vc;

};

//Parámetros:
const string FICHERO_DENUNCIAS_BARCELONA = "2024_4t_denuncies_sancions_transit_detall.txt";
const string FICHERO_CODIGO_SANCIONES = "2024_denuncies_sancions_transit_codis.txt";
const string FICHERO_SUBTIPUS_EXPEDIENT = "2024_denuncies_sancions_transit_conceptes_annex.txt";
const string FICHERO_LOGS = "logs_comandes.txt";

const bool LIMITAR_LECTURA = false; // Limita la lectura de denuncias a solo 50000 denuncias
const int BARRIOS_BCN = 74; // Hay 73 pero sumamos uno para poder hacer barrio[73]
const int DISTRITOS_BCN = 11; // ''

// Constantes (Textos menus)
const string MENU_PRINCIPAL =
"\nSelecciona el tipo de accion desea hacer:\n"
"1. Acciones básicas sobre la entidad \"1: infracciones/denuncias Barcelona\" \n"   // Entidad 1 --> Rafael Gras
"2. Acciones básicas sobre la entidad \"2: tipos de multas\" \n"                    // Entidad 2 --> Mykola Stefanskyy
"3. Acciones básicas sobre la entidad \"3: medio imposición\" \n"                   // Entidad 3 --> Albert Sabadell
"4. A. Específicas: Diagnóstico de denuncias en Barcelona \n"                       // Acción específica 1
"5. A. Específicas: Diagnóstico de denuncias en una zona específica \n"             // Acción específica 2
"6. A. Específicas: Validación de datos \n"                                         // Acción específica 3
"7. Salir del programa \n --> ";

const string MENU_ENTIDAD_1 =
"\nMenu Entidad 1: Denuncias de tráfico en Barcelona\n"
"1. Consultar, modificar i/o eliminar denuncias\n"
"2. Añadir una denuncia\n"
"3. Salir\n--> ";

const string MENU_ENTIDAD_2 =
"¿Qué quiere hacer con la entidad \"2: tipos de multas\"?\n"
"1. Consultar la información sobre un tipo de multa.\n"
"2. Añadir un nuevo tipo de multa al sistema.\n"
"3. Eliminar un tipo de multa del sistema.\n"
"4. Modificar un tipo de multa.\n"
"5. Salir\n --> ";

const string MENU_ESPECIFICAS_1 =
"¿Qué quiere hacer?\n"
"1. Consultar el mes y el día con más recaudación.\n"
"2. Consultar el número de veces que se ha requerido el uso de una grúa.\n"
"3. Consultar cuál es la denuncia más común.\n"
"4. Consultar el número de denuncias registradas por el código.\n"
"5. Consultar cuál es el medio de imposición más común.\n"
"6. Consultar el número de denuncias registradas por el medio de imposición.\n"
"7. Salir\n --> ";

const string MENU_ESPECIFICAS_2 =
"¿Qué quiere hacer?\n"
"1. Consultar códigos de distritos\n"
"2. Consultar códigos de barrios\n"
"3. Consultar códigos de calles\n"
"4. Accion específica filtrada por zona/tiempo/tipo\n"
"5. Salir\n-->";

const string MENSAJE_ENTRADA_DATOS =
"Para introducir los datos tenga en cuenta: \n"
"- Para no especificar algun parámetro numérico, simplemente escriba -1\n"
"- No escribas carácteres que no sean números ( -1 se incluye) excepto que se especifique lo contrario\n"
"- No dejes valores vacios\n ";

//Subprogramas de Mykola Stefanskyy: Menu y Entidad 2 (multas)
bool Menu(v_denuncia&v1, v_multa&v2, v_imposicio&v3, t_zonas& z);
bool MenuEntidad2(v_multa&v2);
void SalidaSinBarrasBajas(string palabra);
void BusquedaVectores1(const v_multa&v1, int&n, bool&trobat, int codigo);
void LecturaFicheroE2(v_multa&v1);
void ModificarFicheroE2(const v_multa&v1);
void ConsultaInfoE2(const v_multa&v1, int codigo);
void AnadirInfoE2(v_multa&v1);
void EliminarInfoE2(v_multa&v1);
void ModificarInfoE2(v_multa&v1);

//Subprogramas Albert Sabadell Enitdad 3 y 4

bool MenuEntidad3(v_imposicio& v3);
void LecturaFicheroE3(v_imposicio& v3);
void ModificarFicheroE3(const v_imposicio& v3);
void ConsultaInfoE3(const v_imposicio& v3);
void AnadirInfoE3(v_imposicio& v3);
void EliminarInfoE3(v_imposicio& v3);
void ModificarInfoE3(v_imposicio& v3);

void RegistrarLog(v_log& logs, string entitat, string operacio, string detall);   // Entidad de registro de cambios
void GuardarLogs(const v_log& logs);
void LecturaLogs(v_log& logs);

// Subprogramas Rafael Gras: Entidad 1 denuncias
bool MenuEntidad1(v_denuncia& v1, t_zonas& z, const v_multa& v2);
void LecturaFicheroE1(v_denuncia& v1, t_zonas& z);
void ModificarFicheroE1(const v_denuncia& v1, const t_zonas& z);
void DescripcionDenuncia(const t_denuncia& t, const t_zonas& z, const v_multa& v2);
t_denuncia GenerarDenuncia(t_zonas& z,bool denunciaNueva, const v_multa& v1);
vector<int> FiltrarDenuncias(const v_denuncia& v1, t_denuncia r); //  Filtra las denuncias que encajan con los valores de la denuncia r de referencia
void AnadirInfoE1(v_denuncia& v1, t_zonas& z, const v_multa& v2);
void ConsultaInfoE1(v_denuncia& v1, t_zonas& z, const v_multa& v2);
void EliminarInfoE1(v_denuncia& v1, const t_zonas& z, int indice);
void ModificarInfoE1(v_denuncia& v1, t_zonas& z, int indice, const v_multa& v2);
bool MenuEspecificas2(v_denuncia&v1, v_multa&v2, v_imposicio&v3, t_zonas& z);

//Funcionalidades específicas (v0-0-7 Nico)

bool MenuEspecificas1(v_denuncia&v1, v_multa&v2, v_imposicio&v3);
void MesDiaRecaudacionInfraccion(const v_denuncia&v1);
void UsoGrua(const v_denuncia&v1);
void LlenarVectorDenunciaCodigo(const v_denuncia&v1, v_dencodigo&v4);
void DenunciaMasComun(const v_denuncia&v1, const v_multa&v2);
void BuscadorDenunciasCantidad(const v_denuncia&v1);
void LlenarVectorDenunciaImp(const v_denuncia&v1, v_denimp&v5);
void MedioImposicionMasComun (const v_denuncia&v1, const v_imposicio&v3);
void BuscadorMedioImposicionCantidad(const v_denuncia&v1);
void ValidacionDatos(const v_denuncia&v1, const v_multa&v2, const v_imposicio&v3, const t_zonas&z);

// Otros
string String(int n);

int contadorLogs = 1; // contador global de logs, se tiene que mantener fuera del main para no tener que arrastrarlo en todos los subprogramas y así poder realizar un registro en cualquier momento
v_log logs;

int main(){
    setlocale(LC_ALL, "Catalan_Spain.1252"); // Esta función no estrictamente necesaria pero sirve para mostrar todos los carácteres del alfabeto español en la salida cout
    cout << "Inicializando Gestor-Denuncias-Barcelona..." << endl << endl;

    LecturaLogs(logs);
    RegistrarLog(logs,"General","Programa_inicializado","-");

    cout << "Leyendo fichero " << FICHERO_DENUNCIAS_BARCELONA << endl<< endl;
    v_denuncia v1; // Entidad 1
    t_zonas z;
    z.vb = v_barrios(BARRIOS_BCN, "");      // Vector que contiene el nombre (string) de cada barrio (73 barrios en total)
    z.vd = v_distritos(DISTRITOS_BCN,"");   // Vector que contiene el nombre de cada distrito (10 distritos en total)
    z.vc = v_calles();                      // Vector que contiene el nombre i codigo de cada calle (Hay aprox 4000-4500 calles... pero no todas tienen porque aparecer en las denuncias)

    LecturaFicheroE1(v1, z);

    cout << "Se han cargado "  << v1.size() << " denuncias de tráfico." << endl;
    cout << "Se han encontrado " << z.vc.size() << " calles distintas." << endl << endl;

    cout << "Leyendo fichero " << FICHERO_CODIGO_SANCIONES << endl << endl;
    v_multa v2;
    LecturaFicheroE2(v2);
    cout << "Se han leído " << v2.size() << " tipos de infracciones." << endl << endl;

    cout << "Leyendo fichero " << FICHERO_SUBTIPUS_EXPEDIENT << endl;
    v_imposicio v3;
    LecturaFicheroE3(v3);
    cout << "Se han leído " << v3.size() << " subtipos de expediente." << endl << endl;

    while (!Menu(v1, v2, v3, z)); // El programa se acaba cuando Menu() devuelve true (el usuario ha solicitado salir del programa)

    // Esto se ejecuta justo antes de finalizar el programa

    cout << "Guardando ficheros... " << endl;
    ModificarFicheroE1(v1, z);
    ModificarFicheroE2(v2);
    ModificarFicheroE3(v3);
    GuardarLogs(logs);
}

bool Menu(v_denuncia&v1, v_multa&v2, v_imposicio&v3, t_zonas& z) { // Devuelve true cuando el programa debe finalizar
    int opcion, opcion2;
    cout << MENU_PRINCIPAL;
    cin >> opcion;
    cout << endl;

    switch (opcion) {
        case 1: {
            while (!MenuEntidad1(v1, z, v2));
            break; }
        case 2: {
            while(!MenuEntidad2(v2));
            break;}
        case 3: {
           while (!MenuEntidad3(v3)); // Menú específico de la Entidad 3
           break;}
        case 4: {
            while (!MenuEspecificas1(v1, v2, v3)); // Menú de las específicas 1
            break;}
        case 5: {
            while (!MenuEspecificas2(v1, v2, v3, z) );
            break;}
        case 6: {
            ValidacionDatos(v1, v2, v3, z);
            break;}
        case 7: {
            cout << "¡Hasta pronto!" << endl;
            break;}
        default: {cout << "Introduce una opción válida" << endl; break; }
    }
    return opcion == 7;
}

//SUBPROGRAMAS MYKOLA STEFANSKYY (Entidad 2)

bool MenuEntidad2(v_multa&v2) { //Menú de la entidad 2
    int opcion, codigo;
    cout << MENU_ENTIDAD_2;
    cin >> opcion;
    switch(opcion) {
        case 1:{
            cout << "Introduce el código de la infracción a consultar:\n --> ";
            cin >> codigo;
            ConsultaInfoE2(v2, codigo);
            RegistrarLog(logs, "Multas", "Consulta-infraccion",String(codigo));
            break;}
        case 2: AnadirInfoE2(v2); break;
        case 3: EliminarInfoE2(v2); break;
        case 4: ModificarInfoE2(v2); break;
        case 5: break;
        default: cout << "Opción no válida" << endl; break;
        }
        return opcion == 5;
   }

void SalidaSinBarrasBajas(string palabra){ //Subprograma que permite la salida a pantalla de las oraciones sin '_' en los espacios
    for (int i = 0; i < palabra.size(); i++){
        if (palabra[i] != '_') cout << palabra[i];
        else cout << " ";
    }
}

void BusquedaVectores1(const v_multa&v1, int&n, bool&trobat, int codigo){ //Subprograma que realiza una búsqueda en el vector v_multa (está para evitar escribir varias veces lo mismo)
    for (int i = 0; i < v1.size() && !trobat; i++){
            if (v1[i].codi == codigo) {
                    trobat = true;
                    n = i;}}
}

void LecturaFicheroE2(v_multa&v1){ //Subprograma que lleva a cabo la lectura de la base de datos relacionada

    ifstream cinf(FICHERO_CODIGO_SANCIONES);
    string titulo;
    int i = 0;
    t_multa x;

    while (i < 9){cinf >> titulo; i++;} //Extraer titulos de los datos (primera linea)
    while (cinf >> x.codi >> x.desccurtaCA >> x.desccurtaES // Lectura y incorporacion al vector v1
           >> x.descllargaCA >> x.descllargaES >> x.norm >> x.import
           >> x.tipus >> x.punts_retirar){
        v1.push_back(x); //Introducimos los datos de la lectura en el vector
    }
}

void ModificarFicheroE2(const v_multa&v1){ //Subprograma que permite modificar la base de datos relacionada con la entidad 2

    ofstream couth(FICHERO_CODIGO_SANCIONES);
    //Cabecera
    couth << "Infraccio_Codi Infraccio_Desc_Curta_CA Infraccio_Desc_Curta_ES Infraccio_Desc_Llarga_CA Infraccio_Desc_Llarga_ES Normativa Import_Nominal Tipus_Infraccio Punts_A_Retirar";

    for (int i = 0; i < v1.size(); i++){
        couth << endl << v1[i].codi << " " << v1[i].desccurtaCA << " " << v1[i].desccurtaES << " "
        << v1[i].descllargaCA << " " << v1[i].descllargaES << " " << v1[i].norm << " "
        << v1[i].import << " " << v1[i].tipus << " " << v1[i].punts_retirar;
    }
}

void ConsultaInfoE2(const v_multa&v1, int codigo){ //Subprograma que permite consultar información sobre una denuncia
        int leng = 1, n = 0;
        bool trobat = false;
        char opcionlenguaje;

        BusquedaVectores1(v1, n, trobat, codigo);
        if (!trobat) cout << "No se ha encontrado información." << endl << endl;
        else {

            while (leng != 0){
                cout << "¿Cómo quiere ver las descripciones de la infracción? (En catalán (0) o en español (1))\n --> ";
                cin >> opcionlenguaje;
                if (opcionlenguaje == '0'){
                    cout << "DescCurtaCA: "; SalidaSinBarrasBajas(v1[n].desccurtaCA); cout << endl;
                    cout << "DescLlargaCA: "; SalidaSinBarrasBajas(v1[n].descllargaCA); cout << endl;
                    leng = 0;
                }
                else if (opcionlenguaje == '1'){
                    cout << "DescCurtaES: "; SalidaSinBarrasBajas(v1[n].desccurtaES); cout << endl;
                    cout << "DescLlargaES: "; SalidaSinBarrasBajas(v1[n].descllargaES); cout << endl;
                    leng = 0;
                }
                else cout << "Introduce una opción válida.";
            }

            cout << "Normativa: " << v1[n].norm << endl;
            cout << "Importe nominal: " << v1[n].import << " euros. " << endl;
            if (v1[n].tipus == 'E') cout << "Tipo: Estacionamiento" << endl;
            else cout << "Tipo: En movimiento " << endl;
            cout << "Puntos a retirar: " << v1[n].punts_retirar << endl << endl;}
}

void AnadirInfoE2(v_multa&v1){ //Subprograma que permite añadir información a la base de datos relacionada con la entidad 2

        t_multa nuevo;
        bool valido = false, codigo_repetido = false;

        while(!valido){
            codigo_repetido = false;
            cout << "Todas las oraciones se deben de introducir con los espacios sustituidos por '_'." << endl;
            cout << "Introduce el nuevo código de la infracción: \n --> ";
            cin >> nuevo.codi;
            cout << "Introduce la descripción corta en catalán: \n --> ";
            cin >> nuevo.desccurtaCA;
            cout << "Introduce la descripción corta en castellano: \n --> ";
            cin >> nuevo.desccurtaES;
            cout << "Introduce la descripción larga en catalán: \n --> ";
            cin >> nuevo.descllargaCA;
            cout << "Introduce la descripción larga en castellano: \n --> ";
            cin >> nuevo.descllargaES;
            cout << "Introduce la normativa y el importe nominal: \n --> ";
            cin >> nuevo.norm >> nuevo.import;
            cout << "Introduce el tipo y los puntos a retirar: \n --> ";
            cin >> nuevo.tipus >> nuevo.punts_retirar;

            for (int i = 0; i < v1.size() && !codigo_repetido; i++) {
                if (v1[i].codi == nuevo.codi) {
                    codigo_repetido = true;
                }
            }
            if (codigo_repetido) {
                cout << "Ya existe una infracción registrada con el código " << nuevo.codi << "." << endl;
            }
            else if (nuevo.import < 0) {
                cout << "El importe no puede ser negativo." << endl;
            }
            else if (nuevo.punts_retirar < 0 || nuevo.punts_retirar > 15) {
                cout << "Los puntos a retirar no pueden ser negativos ni mayores a 15." << endl;
            }
            else {
                valido = true;
            }

            if (!valido) {
                cout << "Por favor, introduzca los datos nuevamente." << endl;
            }
            else {
                v1.push_back(nuevo);
                RegistrarLog(logs, "Multa", "Alta", String(nuevo.codi));
                ModificarFicheroE2(v1);
                cout << endl << "Infracción añadida correctamente." << endl << endl;
            }
        }
}

void EliminarInfoE2(v_multa&v1){

    bool trobat = false;
    int codigo;
    cout << "Introduce el código de la multa a eliminar: \n --> ";
    cin >> codigo;

    for (int i = 0; i < v1.size() && !trobat; i++){
        if (v1[i].codi == codigo){
            trobat = true;
            for (int k = i; k < v1.size()-1; k++){ //Eliminar y reorganizar el vector
                v1[k] = v1[k+1];
            }
            v1.pop_back(); //Eliminamos el último que queda repetido
        }
    }
    if (trobat){
        ModificarFicheroE2(v1);
        RegistrarLog(logs, "Multa", "Eliminada", String(codigo));
        cout << endl << "Infracción eliminada correctamente" << endl << endl;
    }

    else cout << endl << "No se ha podido encontrar la infracción a eliminar" << endl << endl;
}

void ModificarInfoE2(v_multa&v1){

    t_multa mod;
    bool trobat = false, salir = false, valido = false, existe = false;
    int n = 0, codigo, opcionmod = 0;
    bool k;
    string opcionlong;

    cout << "Introduce el código de la multa a modificar:\n --> ";
    cin >> codigo;
    BusquedaVectores1(v1, n, trobat, codigo);

            if (trobat){
                while(!salir){
                    k = true;
                    valido = false;

                    cout << "¿Qué deseas modificar? (Introduce el entero)" << endl
                    << "1. Código de la infracción " << endl
                    << "2. Descripción (CAT) " << endl
                    << "3. Descripción (ESP) " << endl
                    << "4. Normativa " << endl
                    << "5. Importe nominal " << endl
                    << "6. Tipo de la infracción " << endl
                    << "7. Puntos a retirar " << endl
                    << "8. Salir \n --> ";

                    cin >> opcionmod;
                    if (opcionmod == 1){
                        while (!valido){
                            existe = false;
                            cout << "Introduce el nuevo código: \n --> ";
                            cin >> mod.codi;
                            for (int i = 0; i < v1.size() && !existe; i++)
                                if (v1[i].codi == mod.codi) existe = true;
                            if (!existe){v1[n].codi = mod.codi; valido = true;}
                            else cout << "Introduce un código que no se encuentre ya en la base de datos." << endl;
                        }
                    }
                    else if (opcionmod == 2){
                        cout << "¿Corta o Larga? \n --> ";
                        cin >> opcionlong;
                        while (k){
                            if (opcionlong == "Corta"){
                                cout << "Introduce tu texto (con barras bajas en los espacios): \n --> ";
                                cin >> mod.desccurtaCA;
                                v1[n].desccurtaCA = mod.desccurtaCA;
                                k = false;
                                }
                            else if (opcionlong == "Larga"){
                                cout << "Introduce tu texto (con barras bajas en los espacios): \n --> ";
                                cin >> mod.descllargaCA;
                                v1[n].descllargaCA = mod.descllargaCA;
                                k = false;
                                }
                            else {cout << "Introduce una opción válida" << endl; cin >> opcionlong;}
                        }
                    }
                    else if (opcionmod == 3){
                        cout << "¿Corta o Larga? \n --> ";
                        cin >> opcionlong;
                        while (k){
                            if (opcionlong == "Corta"){
                                cout << "Introduce tu texto (con barras bajas en los espacios): \n --> ";
                                cin >> mod.desccurtaES;
                                v1[n].desccurtaES = mod.desccurtaES;
                                k = false;
                            }
                            else if (opcionlong == "Larga"){
                                cout << "Introduce tu texto (con barras bajas en los espacios): \n --> ";
                                cin >> mod.descllargaES;
                                v1[n].descllargaES = mod.descllargaES;
                                k = false;
                            }
                            else {cout << "Introduce una opción válida" << endl; cin >> opcionlong; }
                        }
                    }
                    else if (opcionmod == 4){
                        cout << "Introduce la nueva normativa: \n --> ";
                        cin >> mod.norm;
                        v1[n].norm = mod.norm;
                    }
                    else if (opcionmod == 5){
                        while(!valido){
                            cout << "Introduce el nuevo importe: \n --> ";
                            cin >> mod.import;
                            if (mod.import > 0) {
                                valido = true;
                                v1[n].import = mod.import;
                            }
                            else cout << "Introduce una cantidad válida." << endl;
                        }
                    }
                    else if (opcionmod == 6){
                        while (!valido){
                            cout << "Introduce el nuevo tipo: \n --> ";
                            cin >> mod.tipus;
                            if (mod.tipus == 'E' || mod.tipus == 'M'){
                                v1[n].tipus = mod.tipus;
                                valido = true;
                            }
                            else cout << "Introduce un tipo válido." << endl;
                        }
                    }
                    else if (opcionmod == 7){
                        while (!valido){
                            cout << "Introduce la nueva cantidad: \n --> ";
                            cin >> mod.punts_retirar;
                            if (mod.punts_retirar <= 15 && mod.punts_retirar >= 0){
                                v1[n].punts_retirar = mod.punts_retirar;
                                valido = true;
                            }
                            else cout << "Introduce una cantidad válida." << endl;
                        }
                    }
                    else if (opcionmod == 8) {
                        salir = true;
                        RegistrarLog(logs, "Multa", "Modificada", String(codigo));
                        ModificarFicheroE2(v1);
                    }
                    else cout << "Introduce una entrada válida." << endl;}
                }

            else cout << endl << "No se ha podido encontrar la infracción a modificar" << endl << endl;
}

// Subprogramas de Albert Sabadell

void LecturaFicheroE3(v_imposicio& v3) {

    ifstream cinf(FICHERO_SUBTIPUS_EXPEDIENT);
    t_imposicio x;
    string basura;

    // Saltamos la cabecera (4 columnas)
    cinf >> basura >> basura >> basura >> basura;

    while (cinf >> x.concepte >> x.valor >> x.valordesc_ca >> x.valordesc_es) {
        v3.push_back(x);
    }
}

void ModificarFicheroE3(const v_imposicio& v3){

    ofstream fout(FICHERO_SUBTIPUS_EXPEDIENT);

    // Cabecera
    fout << "Concepte Valor Descripcio_CA Descripcio_ES";

    for (int i = 0; i < v3.size(); i++){
        fout << endl;
        fout << v3[i].concepte << " "
             << v3[i].valor << " "
             << v3[i].valordesc_ca << " "
             << v3[i].valordesc_es;
    }
}

void AnadirInfoE3(v_imposicio& v3){
    t_imposicio nuevo;

    cout << "Introduce el concepto (sin espacios, usa _): ";
    cin >> nuevo.concepte;
    cout << "Introduce el valor (codigo unico): ";
    cin >> nuevo.valor;
    cout << "Introduce la descripcion en catalan: ";
    cin >> nuevo.valordesc_ca;
    cout << "Introduce la descripcion en castellano: ";
    cin >> nuevo.valordesc_es;

    v3.push_back(nuevo);
    ModificarFicheroE3(v3);
    RegistrarLog(logs, "Medio_imposicion", "Añadido", nuevo.concepte);

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
            RegistrarLog(logs, "Medio-imposicion", "Eliminado", codigo);
            cout << "Subtipo eliminado correctamente." << endl << endl;
        }
    }
    if (!encontrado) cout << "No se encontró el subtipo." << endl << endl;
}

void ModificarInfoE3(v_imposicio& v3){
    string codigo;
    bool encontrado = false;

    cout << "Introduce el valor (codigo) del subtipo a modificar: ";
    cin >> codigo;

    for (int i = 0; i < v3.size() && !encontrado; i++){
        if (v3[i].valor == codigo){
            encontrado = true;

            cout << "Nuevo concepto: ";
            cin >> v3[i].concepte;
            cout << "Nueva descripcion en catalan: ";
            cin >> v3[i].valordesc_ca;
            cout << "Nueva descripcion en castellano: ";
            cin >> v3[i].valordesc_es;

            ModificarFicheroE3(v3);
            RegistrarLog(logs, "Medio-imposicion", "Modificado", codigo);
            cout << "Subtipo modificado correctamente." << endl << endl;
        }
    }
    if (!encontrado)
        cout << "No se encontro el subtipo." << endl << endl;
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
            RegistrarLog(logs, "Medio-imposicion", "Consultado", codigo);
        }
    }
    if (!trobat) cout << "No se encontró información para el código " << codigo << endl << endl;
}

bool MenuEntidad3(v_imposicio&v3) {
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

//Entidad 4

void RegistrarLog(v_log&logs, string entitat, string operacio, string detall) {
    t_log l;
    l.id = contadorLogs;
    contadorLogs++;
    l.entitat = entitat;
    l.operacio = operacio;
    l.detall = detall;
    logs.push_back(l);
}

void GuardarLogs(const v_log&logs) {
    ofstream fout(FICHERO_LOGS);

    fout << "ID ENTIDAD OPERACION DETALLE" << endl;

    for (int i = 0; i < logs.size(); i++) {
        fout << logs[i].id << " "
             << logs[i].entitat << " "
             << logs[i].operacio << " "
             << logs[i].detall << endl;
    }
}

void LecturaLogs(v_log& logs) {
    ifstream fin(FICHERO_LOGS);
    t_log l;
    string b;

    fin >> b >> b >> b >> b;
    while (fin >> l.id >> l.entitat >> l.operacio >> l.detall) {
        logs.push_back(l);
        contadorLogs = l.id + 1;
    }
}

// Subprogramas de Rafael Gras

t_calle BuscarCalle(const t_zonas& z, int codigo, bool& trobat) {
    trobat=false;
    t_calle c;
    int i=z.vc.size()-1;
    while (!trobat and i>=0) {trobat = (z.vc[i].codigo==codigo); i--;};
    if (trobat) c=z.vc[i+1];
    return c;
}

void LecturaFicheroE1(v_denuncia& v1, t_zonas& z) {
    ifstream cinf(FICHERO_DENUNCIAS_BARCELONA);
    v1 = v_denuncia();
    t_denuncia t;
    char b, grua;
    int tiempo, k=1;
    string bs, nombre_distrito, nombre_barrio;
    for (int i=0; i< 18;i++) cinf >> bs; // eliminamos la primera fila (tenemos 18 columnas)
    bool continuar = true;
    while (cinf >> t.any >> b >> t.mes >> b >> t.dia >> tiempo
           >> t.codi_carrer >> t.nom_carrer >> t.num_carrer >> t.codi_districte
           >> nombre_distrito >> t.codi_barri >> nombre_barrio >> t.seccio_censal
           >> t.longitud >> t.latitud >> t.infraccio_codi >> t.subtipus_exp
           >> grua >> t.mitja_impos >> t.tipus_vehicle >> t.import and continuar ) {
        tiempo = tiempo / 100;
        t.minuto = tiempo % 100; // extraemos los minutos
        t.hora = (tiempo / 100); // extraemos las horas
        if (grua=='X') t.grua=1; // Ha conllevado grua
        else if (grua=='-') t.grua=-1; // No se ha especificado
        else           t.grua=0; // No ha conllevado grua
        bool trobat = false; // true cuando ya se ha registrado esta calle
        BuscarCalle(z,t.codi_carrer,trobat);
        if (!trobat) { // Registra una nueva calle al vector de calles
            t_calle c;
            c.codigo = t.codi_carrer;
            c.nombre = t.nom_carrer;
            z.vc.push_back(c);
        }
        if (t.codi_districte > 10 or t.codi_districte < 0) // Validación de datos
            {cout << "ERROR1" << endl; continuar=false;}
        else if (z.vd[t.codi_districte]=="") {z.vd[t.codi_districte]=nombre_distrito; };

        if (t.codi_barri > 73 or t.codi_barri < 0)  // Validación de datos
        { cout << "ERROR2" << endl; continuar=false;}
        else if (z.vb[t.codi_barri]=="") {z.vb[t.codi_barri]=nombre_barrio; };

        v1.push_back(t);

        if ((k%50000)==0) {if (LIMITAR_LECTURA) continuar=false; else cout << "Se han leído " << k << " denuncias" << endl;}
        k++;
    }
}

bool MenuEntidad1(v_denuncia& v1, t_zonas& z, const v_multa& v2) {
    int opcion;
    cout << MENU_ENTIDAD_1;
    cin >> opcion;

    switch(opcion) {
        case 1: ConsultaInfoE1(v1, z, v2); break;
        case 2: AnadirInfoE1(v1, z, v2); break;
        case 3: break;
        default: cout << "Opción no válida" << endl; break;
    }
    return opcion == 3;
}

void ModificarFicheroE1(const v_denuncia& v1, const t_zonas& z) {
    string cabecera = "Data_Infraccio Hora_Infraccio Codi_Carrer Nom_Carrer Num_Carrer Codi_Districte Nom_Districte Codi_Barri Nom_Barri Seccio_Censal Longitud_WGS84 Latitud_WGS84 Infraccio_Codi SUBTIPUS_EXPEDIENT Grua MITJA_IMPOSICIO Tipus_Vehicle_Codi Import_Nominal_€";
    ofstream fout(FICHERO_DENUNCIAS_BARCELONA);

    fout << cabecera << endl;
    int s = v1.size();
    for (int i=0; i< s; i++) {
        t_denuncia t = v1[i];
        string nom_distrito, nom_barrio;
        if (t.codi_districte > -1 and t.codi_districte < DISTRITOS_BCN) nom_distrito=z.vd[t.codi_districte];
        else nom_distrito="Indefinido";
        if (t.codi_barri > -1 and t.codi_barri < BARRIOS_BCN) nom_barrio=z.vb[t.codi_barri];
        else nom_barrio="Indefinido";

        fout << t.any << '-' << t.mes << '-' << t.dia << ' ' << t.hora << (t.minuto/10) << (t.minuto%10) << "00"
        << ' ' << t.codi_carrer << ' ' << t.nom_carrer << ' ' << t.num_carrer << ' ' << t.codi_districte << ' '
        << nom_distrito << ' ' << t.codi_barri << ' ' << nom_barrio << ' ' << t.seccio_censal << ' '
        << t.longitud << ' ' << t.latitud << ' ' << t.infraccio_codi << ' ' << t.subtipus_exp << ' ';
        if (t.grua==1) fout << 'X';
        else if (t.grua==0) fout << '_';
        else fout << '-'; // Indefinido
        fout << ' ' << t.mitja_impos << ' ' << t.tipus_vehicle << ' ' << t.import << endl;
    }
}

void DescripcionDenuncia(const t_denuncia& t, const t_zonas& z, const v_multa& v2) {
    string nom_districte, nom_barri, grua;
    if (t.codi_districte==-1) nom_districte="Indefinido";
    else                      nom_districte=z.vd[t.codi_districte];
    if (t.codi_barri==-1)     nom_barri="Indefinido";
    else                      nom_barri=z.vb[t.codi_barri];
    if (t.grua==1)            grua = "Sí";
    else if (t.grua==0)       grua = "No";
    else                      grua = "No especificado";
    cout << endl << "(Los valores en -1 significan un valor indefinido)" << endl;
    cout << "Fecha: ";
    if (t.any!=-1) cout << t.any << "/";
    else cout  << "-/";
    if (t.mes!=-1) cout << t.mes << "/";
    else cout << "-/";
    if (t.dia!=-1) cout << t.dia;
    else cout << "-";
    cout << " a las ";
    if (t.hora!=-1) cout << t.hora << ":";
    else cout << "-:";
    if (t.minuto!=-1) cout<<t.minuto;
    else cout << "-";
    cout << endl;
    cout << "Ubicación: " << t.num_carrer << endl << t.nom_carrer << endl << "Distrito: " << nom_districte << endl << "Barrio: "
    << nom_barri << endl << "Lat: " << t.latitud << endl << "Lon:" << t.longitud << "," << endl << "S.C: " << t.seccio_censal << endl;
    cout << "Grua: " << grua << endl << "Codigo_infraccion: " << t.infraccio_codi << endl << "Importe: " << t.import << endl << "Subtipo_expediente: " << t.subtipus_exp << endl << "Medio_imposicion: "
    << t.mitja_impos << endl << "Tipo_vehiculo: " << t.tipus_vehicle << endl;
    bool existe_codigo_infraccion; int n_infraccion;
    BusquedaVectores1(v2,n_infraccion, existe_codigo_infraccion, t.infraccio_codi);
    if (existe_codigo_infraccion) {
        t_multa m = v2[n_infraccion];
        cout << "Infraccion: " << m.desccurtaES << endl;
    }

}

t_denuncia GenerarDenuncia(t_zonas&z,bool denunciaNueva, const v_multa&v1) {
    // especificar_sancion es true cuando es obligatorio especifcar sancion (no es obligatorio para filtrar sanciones)

    t_denuncia r;
    bool denuncia_valida=false;
    while (!denuncia_valida) {
        cout << endl <<  MENSAJE_ENTRADA_DATOS;
        cout << "Introduce el año, el mes, el dia, la hora y minuto, separados por espacios ( ej: 2024 11 3 13 -1) -->";
        cin >> r.any >> r.mes >> r.dia >> r.hora >> r.minuto;

        int esp;
        cout << "Desea especificar zona? (1=sí 0=No) --> "; cin >> esp;
        if (esp==1) {
            cout << "Introduce el código de calle, número de calle, código de distrito, código de barrio y sección censal (ej: 701433 -1 4 19 -1 )";
            cin >> r.codi_carrer >> r.num_carrer >> r.codi_districte >> r.codi_barri >> r.seccio_censal;
            cout << "Introduce coordenadas, Longitud y Latitud (ej 2.176944 41.423317 ) -->"; cin >> r.longitud >> r.latitud;
        } else {r.codi_carrer=-1;r.num_carrer=-1;r.codi_districte=-1;r.codi_barri=-1;r.seccio_censal=-1;r.longitud=-1;r.latitud=-1;}

        if (!denunciaNueva) {
            cout << "Desea especificar tipo de sanción? (1=Sí 0=No) --> "; cin >> esp;
        }
        if (esp==1 or denunciaNueva) {
            cout << "Introduce codigo sanción y importe nominal (ej: 1005 90) -->"; cin >> r.infraccio_codi >> r.import;
            cout << "Introduce subtipo_expediente medio_imposicion y tipus_vehicle_codi (tres strings, ej: CR PAP T ): "; cin >> r.subtipus_exp >> r.mitja_impos >> r.tipus_vehicle;
        } else  {r.infraccio_codi=-1; r.import=-1;r.subtipus_exp="-1"; r.mitja_impos="-1"; r.tipus_vehicle="-1";}

        cout << "Ha requerido de una grua? (1=Sí; 0=No; -1=No se especifíca) -->"; cin >> r.grua;

        // Validación de datos introducidos: (Algunos valores solo se validan si se esta declarando una nueva denuncia, para hacer búsquedas no es necesario)
        bool existe_codigo_infraccion; int n_infraccion;
        BusquedaVectores1(v1,n_infraccion, existe_codigo_infraccion, r.infraccio_codi);
        if (r.codi_barri >= BARRIOS_BCN) { denuncia_valida=false; cout << "Error: El codigo de barrio debe estar entre 0 y " << (BARRIOS_BCN-1) << endl; }
        else if (r.codi_districte >= DISTRITOS_BCN) {denuncia_valida=false; cout << "Error: El codigo de distrito debe estar entre 0 y " << (DISTRITOS_BCN-1) << endl; }
        else if (r.mes > 12 or r.dia > 31 or r.hora > 23 or r.minuto > 60 ) {denuncia_valida=false; cout << "Error: La fecha, hora o minuto introducido es invalido" << endl;}
        else if (denunciaNueva and (r.any<0 or r.mes<0 or r.dia<0 or r.hora<0 or r.minuto<0) ) {denuncia_valida=false; cout << "Error: Para generar una nueva denuncia es obligatorio definir la fecha, hora y minuto" << endl;}
        else if (!existe_codigo_infraccion and denunciaNueva)  {denuncia_valida=false; cout << "Error: El código de infracción introducido no está registrado, por favor, registre primero el tipo de infracción antes de generar una denuncia" << endl;}
        else if (r.grua!=1 and r.grua !=0 and r.grua != -1) {denuncia_valida=false; cout << "Error: El número introducido que define si se ha requerido grua es invalido" << endl;}
        else {
            // Cuando el código llega aquí es que la denuncia generada es válida (pero falta confirmación por parte del usuario)
            denuncia_valida=true;

            // Validación de código barrio
            if (r.codi_barri >= 0 and z.vb[r.codi_barri]=="" ) {
                cout << "El barrio con código " << r.codi_barri << " no está registrado, introduce su nombre (reemplaza los espacios por '_') -->" << endl;
                cin >> z.vb[r.codi_barri];
            }
            // Validación distrito
            if (r.codi_districte >= 0 and z.vd[r.codi_districte]=="") {
                cout << "El distrito con código " << r.codi_districte << " no está registrado, introduce su nombre (reemplaza los espacios por '_')" << endl;
                cin >> z.vd[r.codi_districte];
            }

            // Validación de código de calle
            t_calle c;
            bool existe_calle;

            if (r.codi_carrer < 0)  r.nom_carrer = "Indefinida";
            else {
                c = BuscarCalle(z,r.codi_carrer,existe_calle);
                if (existe_calle)   r.nom_carrer = c.nombre;
                else {
                    cout << "La calle con código " << r.codi_carrer << " no está registrada, introduce su nombre (reemplaza los espacios por '_') -->";
                    cin >> c.nombre;
                    c.codigo = r.codi_carrer;
                    z.vc.push_back(c);
                    r.nom_carrer = c.nombre;
                }
            }

            cout << "Se han introducido los siguientes datos: " << endl;
            DescripcionDenuncia(r,z,v1);
            string respuesta;
            cout << "Son correctos? (1=Sí 0=No) -->"; cin >> respuesta;
            if (respuesta=="1") denuncia_valida=true;
            else                denuncia_valida=false;
        }
    }
    return r;
}

vector<int> FiltrarDenuncias(const v_denuncia& v1, t_denuncia r) { // Devuelve un vector con los índices de las denuncias encontradas
    // Devuelve las denuncias filtradas: Aquellas que encajan con los valores de la denuncia r (los valores en r de -1 significan que no se especifica)
    // se filtra por espacio y tiempo
    //v_denuncia v1f;
    vector<int> v1f;
    for (int i=v1.size(); i>=0; i--) {
        t_denuncia d = v1[i];
        if ( ( r.any==-1 or d.any==r.any ) and (r.mes==-1 or d.mes==r.mes) and (r.dia==-1 or d.dia == r.dia)
            and (r.hora ==-1 or d.hora==r.hora) and (r.minuto == -1 or d.minuto==r.minuto)
            and (r.codi_barri==-1 or d.codi_barri==r.codi_barri) and (r.codi_districte==-1 or d.codi_districte==r.codi_districte)
            and (r.seccio_censal==-1 or d.seccio_censal==r.seccio_censal) and (r.codi_carrer==-1 or d.codi_carrer==r.codi_carrer)
            and (r.num_carrer==-1 or d.num_carrer==r.num_carrer) and (r.grua==-1 or d.grua==r.grua) and (r.infraccio_codi==-1 or d.infraccio_codi==r.infraccio_codi)
            ) v1f.push_back(i);
    }
    return v1f;
}

void ConsultaInfoE1(v_denuncia& v1, t_zonas& z, const v_multa& v2) {
    cout << "A continuación debes introducir los datos sobre la denuncia/denuncias que deseas consultar/modificar/eliminar: " << endl;
    t_denuncia r = GenerarDenuncia(z,false,v2);
    RegistrarLog(logs, "Denuncias", "Consulta", "-");
    vector<int> v1f = FiltrarDenuncias(v1,r); // Devuelve un vector con indices de v1
    cout << "Se han encontrado " << v1f.size() << " denuncias" << endl;

    if (v1f.size() > 0) {
        bool salir=false;
        int i=0;
        int opcion;
        while (!salir) {
            cout << endl << "Información sobre denuncia " << i+1 << "/" << v1f.size() << ": ";
            DescripcionDenuncia(v1[v1f[i]], z, v2);
            cout << "Que desea hacer? " << endl
            << "1. Salir" << endl
            << "2. Modificar esta denuncia" << endl
            << "3. Eliminar esta denuncia" << endl;
            if ( v1f.size() > 1) cout << "4. Pasar a la siguiente denuncia" << endl;
            cout << "-->"; cin >> opcion;
            if (opcion==1) salir=true;
            else if (opcion==2) {
                ModificarInfoE1(v1, z, v1f[i], v2 );
                v1f =  FiltrarDenuncias(v1,r); // Actualizar vector de índices v1f
            } else if (opcion==3) {
                EliminarInfoE1(v1,z,v1f[i]);
                v1f =  FiltrarDenuncias(v1,r); // Actualizar vector de índices v1f
            } else if (opcion==4) {
                i++;
            }
            if (v1f.size()==0) salir=true;
            else if (i>=v1f.size()) i=0;
        }
    }
}

void AnadirInfoE1(v_denuncia& v1, t_zonas& z, const v_multa& v2) {
    cout << "A continuación debes introducir los datos de la denuncia que desea generar: " << endl;
    t_denuncia d = GenerarDenuncia(z, true, v2);
    v1.push_back(d);
    RegistrarLog(logs, "Denuncia", "Alta", String(d.infraccio_codi));
    ModificarFicheroE1(v1,z);
    cout << "Denuncia registrada." << endl;
}

void EliminarInfoE1(v_denuncia& v1, const t_zonas& z, int indice) {
    if (v1.size() >= 2 and indice < v1.size() and indice > 0){

        RegistrarLog(logs, "Denuncia", "Eliminada",
        String(v1[indice].infraccio_codi));

        int i=indice;
        int f=v1.size()-2;
        while (i<=f) {
            v1[i] = v1[i+1];
            i++;
        }
        v1.pop_back();
    }
    ModificarFicheroE1(v1,z);
}

void ModificarInfoE1(v_denuncia& v1, t_zonas& z, int i, const v_multa& v2) {
    cout << endl <<  MENSAJE_ENTRADA_DATOS;
    bool salir=false;
    int opcion;
    while (!salir) {
        t_denuncia d = v1[i];
        cout << "Que desea modificar?" << endl
        <<      "1. Fecha " << endl
        <<      "2. Ubicación" << endl
        <<      "3. Infracción" << endl
        <<      "4. Otros valores" << endl
        <<      "5. Finalizar" << endl
        <<       "-->";
        cin >> opcion;
        bool entrada_valida=false;
        if (opcion==1) {
            while (!entrada_valida) {
                cout << "Introduce año, mes, dia, hora y minuto ( Actual: " << d.any << " " << d.mes << " " << d.dia << " " << d.hora << " " << d.minuto << ") -->";
                cin >> v1[i].any >> v1[i].mes >> v1[i].dia >> v1[i].hora >> v1[i].minuto;
                if (v1[i].any < -1 or (v1[i].mes!= -1 and v1[i].mes < 1) or v1[i].mes > 12 or v1[i].dia > 31 or (v1[i].dia!=-1 and v1[i].dia<1)
                    or v1[i].hora > 23 or v1[i].hora < -1 or v1[i].minuto > 59 or v1[i].minuto < -1) {
                    cout << "Entrada invalida." << endl; entrada_valida=false;
                } else entrada_valida=true;
            }
        } else if (opcion==2) {
            while (!entrada_valida) {
                cout << "Introduce el codigo de calle, numero de calle, codigo de distrito, codigo de barrio y sección censal (Actual: " << d.codi_carrer << " " << d.num_carrer << " " << d.codi_districte << " " << d.codi_barri << " " << d.seccio_censal << " ) -->";
                cin >> v1[i].codi_carrer >> v1[i].num_carrer >> v1[i].codi_districte >> v1[i].codi_barri >> v1[i].seccio_censal;
                entrada_valida=false;
                if (v1[i].codi_barri >= BARRIOS_BCN) { cout << "Error: El codigo de barrio debe estar entre 0 y " << (BARRIOS_BCN-1) << endl; }
                else if (v1[i].codi_districte >= DISTRITOS_BCN) {cout << "Error: El codigo de distrito debe estar entre 0 y " << (DISTRITOS_BCN-1) << endl; }
                else { entrada_valida=true;
                    // Validación de código de calle
                    t_calle c; bool existe_calle;
                    if (v1[i].codi_carrer < 0)  v1[i].nom_carrer = "Indefinida";
                    else {
                        c = BuscarCalle(z,v1[i].codi_carrer,existe_calle);
                        if (existe_calle)   v1[i].nom_carrer = c.nombre;
                        else {
                            cout << "La calle con código " << v1[i].codi_carrer << " no está registrada, introduce su nombre (reemplaza los espacios por '_') -->";
                            cin >> c.nombre;
                            c.codigo = v1[i].codi_carrer;
                            z.vc.push_back(c);
                            v1[i].nom_carrer = c.nombre;
                        }
                    }
                }
            }
            cout << "Introduce coordenadas, Longitud y Latitud (Actual: " << d.longitud << " " << d.latitud << " ) -->";
            cin >> v1[i].longitud >> v1[i].latitud;
        } else if (opcion==3) {
            while (!entrada_valida) {
                cout << "Introduce codigo sanción (Actual: " << d.infraccio_codi << " ) -->";
                cin >> v1[i].infraccio_codi;
                bool existe_codigo_infraccion; int n_infraccion;
                BusquedaVectores1(v2,n_infraccion, existe_codigo_infraccion, v1[i].infraccio_codi);
                if (!existe_codigo_infraccion) {
                    cout << "El código de infracción introducido es inválido, por favor, registre primero el tipo de infracción" << endl;
                    entrada_valida=false;
                } else {v1[i].import = v2[n_infraccion].import; entrada_valida=true;}
            }
        } else if (opcion==4) {
            while (!entrada_valida) {
                cout << "Ha requerido grua? (1=Sí 0=No -1=No especifica) -->"; cin >> v1[i].grua;
                if (v1[i].grua > 1 or v1[i].grua < -1) entrada_valida=false;
                else                                   entrada_valida=true;
            }
        } else if (opcion==5) {
            cout << "La denuncia modificada ha quedado así: " << endl;
            DescripcionDenuncia(v1[i],z, v2);
            cout << "Es correcto? (1=Sí 0=No)"; cin >> opcion;
            if (opcion==1) salir=true;
            else           salir=false;
        }
    }
    ModificarFicheroE1(v1,z);
    RegistrarLog(logs, "Denuncia", "Modificado", String(v1[i].infraccio_codi));
    cout << "Denuncia modificada con éxito." << endl << endl;
}

//FUNCIONALIDADES ESPECÍFICAS

bool MenuEspecificas1(v_denuncia&v1, v_multa&v2, v_imposicio&v3){ //Menú de las funcionalidades específicas (denuncias)

    int opcion;
    cout << MENU_ESPECIFICAS_1;
    cin >> opcion;
    bool retorno=false;

    switch(opcion) {
        case 1: MesDiaRecaudacionInfraccion(v1); break;
        case 2: UsoGrua(v1); break;
        case 3: DenunciaMasComun(v1, v2); break;
        case 4: BuscadorDenunciasCantidad(v1); break;
        case 5: MedioImposicionMasComun(v1,v3); break;
        case 6: BuscadorMedioImposicionCantidad(v1); break;
        case 7: break;
        default: cout << "Opción no válida" << endl; break;
    }
    return opcion == 7;
}

void MesDiaRecaudacionInfraccion(const v_denuncia&v1){ //Este subprograma permite encontrar el mes y el día con más recaudación y, además, el mes y el día que se han cometido más infracciones.

    //Variables recaudación
    int mesant = v1[0].mes, diaant = v1[0].dia, mesrecaud, diarecaud, mesdiarecaud;
    double totaldineromes = 0, totaldinerodia = 0, mayormes = 0, mayordia = 0;

    //Variables cantidad de infracciones
    int numinfdia = 0, numinfmes = 0, masinfdia = 0, masinfmes = 0, mesinf, diainf, mesdiainf;

    for (int i = 0; i < v1.size(); i++){
        if (v1[i].dia == diaant){
            totaldinerodia = totaldinerodia + v1[i].import; //Se almacena la recaudación diaria siempre que sea del mismo día
            numinfdia++;}  //Se van añadiendo infracciones siempre que sea del mismo día
        else {
        //Cuando cambia el día, comprobamos si este ha sido el día con mayor recaudación y/o con mayor número de infracciones
            if (totaldinerodia > mayordia){
                mayordia = totaldinerodia;
                diarecaud = diaant;
                mesdiarecaud = mesant;}
            if (numinfdia > masinfdia){
                masinfdia = numinfdia;
                diainf = diaant;
                mesdiainf = mesant;}
            numinfdia = 1;
            totaldinerodia = 0;
            totaldinerodia = v1[i].import;
            diaant = v1[i].dia;}

        if (v1[i].mes == mesant){ //Proceso análogo que el anterior, pero en este caso con los meses del año
            totaldineromes = totaldineromes + v1[i].import;
            numinfmes++;}
        else {
            if (totaldineromes > mayormes){
                mayormes = totaldineromes;
                mesrecaud = mesant;}
             if (numinfmes > masinfmes){
                masinfmes = numinfmes;
                mesinf = mesant;}
            numinfmes = 1;
            totaldineromes = 0;
            totaldineromes = v1[i].import;
            mesant = v1[i].mes;}
            }
    //Comprobación del último día y mes, ya que en estos el bucle no comprueba si tiene algún elemento máximo
    if (totaldinerodia > mayordia){
        mayordia = totaldinerodia;
        diarecaud = diaant;
        mesdiarecaud = mesant;}

    if (totaldineromes > mayormes){
        mayormes = totaldineromes;
        mesrecaud = mesant;}

    if (numinfdia > masinfdia){
        masinfdia = numinfdia;
        diainf = diaant;
        mesdiainf = mesant;}

    if (numinfmes > masinfmes){
        masinfmes = numinfmes;
        mesinf = mesant;}

    cout << "Día con la mayor recaudación: " << diarecaud << "/" << mesdiarecaud << " con " << mayordia << " euros recaudados." << endl;
    cout << "Mes con la mayor recaudación: " << mesrecaud << " con " << mayormes << " euros recaudados." << endl;
    cout << "Día con el mayor número de infracciones: " << diainf << "/" << mesdiainf << " con " << masinfdia << " infracciones." << endl;
    cout << "Mes con el mayor número de infracciones: " << mesinf << " con " << masinfmes << " infracciones." << endl << endl;
    RegistrarLog(logs, "Especifica", "MesDiaConMasRecaudacion", "-");
}

void UsoGrua(const v_denuncia&v1){ //Subprograma que permite ver el número y el porcentaje de denuncias que han requerido el uso de grúa
    int numgrua = 0;
    for (int i = 0; i < v1.size(); i++){
        if (v1[i].grua == 1) numgrua++;}
    cout << numgrua << " denuncias han requerido el uso de grúa, es decir, un ";
    cout << (float(numgrua)/v1.size())*100 << "%." << endl << endl;
}

void LlenarVectorDenunciaCodigo(const v_denuncia&v1, v_dencodigo&v4){ //Subprograma que permite llenar el vector v4, en el cual se ven reflejado la denuncia (por código) y su cantidad

    bool trobat = false;
    t_dencodigo x;

    for (int i = 0; i < v1.size(); i++){
        trobat = false;
        for (int j = 0; j < v4.size() && !trobat; j++){
            if (v4[j].codi == v1[i].infraccio_codi) {
                v4[j].num++;
                trobat = true;
            }
        }
        if (!trobat){
            x.codi = v1[i].infraccio_codi;
            x.num = 1;
            v4.push_back(x);
        }
    }
}

void DenunciaMasComun(const v_denuncia&v1, const v_multa&v2){ //Mediante el proceso de llenar el vector v4, este subprograma encuentra la más común

    v_dencodigo v4;
    int mascomun = 0, denmascomun;
    string consulta;
    bool trobat = false, valido = false;

    LlenarVectorDenunciaCodigo(v1, v4);

    for (int k = 0; k < v4.size(); k++){
        if (v4[k].num > mascomun){
            mascomun = v4[k].num;
            denmascomun = v4[k].codi;
        }
    }
    cout << "La denuncia más común tiene como código " << denmascomun << " y ha tenido " << mascomun << " incidencias, ";
    cout << "hecho que representa el " << (float(mascomun)/v1.size())*100 << "% de las denuncias." << endl;
    cout << "¿Desea consultar información sobre este tipo de denuncia (S o N)?\n --> ";
    while (!valido){
        cin >> consulta;
        if (consulta == "S"){ConsultaInfoE2(v2, denmascomun); valido = true;}
        else if (consulta == "N") {cout << "Volviendo al menú... " << endl << endl; valido = true;}
        else cout << "Introduce una opción válida \n --> " << endl;
    }
}

void BuscadorDenunciasCantidad(const v_denuncia&v1){ //Subprograma que permite buscar la denuncia por código y, de esta forma, ver la cantidad de infracciones realizadas

    v_dencodigo v4;
    int cant, codigo;
    bool trobat = false;

    LlenarVectorDenunciaCodigo(v1, v4);

    cout << "Introduzca el código para consultar la cantidad de denuncias registradas:\n --> ";
    cin >> codigo;

    for (int i = 0; i < v4.size() && !trobat; i++){
        if (codigo == v4[i].codi){trobat = true; cant = v4[i].num;}
    }
    if (!trobat) cout << "No se ha encontrado la denuncia." << endl << endl;
    else cout << "Este tipo de denuncia ha tenido un total de " << cant << " denuncias registradas." << endl << endl;
}

void LlenarVectorDenunciaImp(const v_denuncia&v1, v_denimp&v5){

    bool trobat = false;
    t_denimp x;

    for (int i = 0; i < v1.size(); i++){
        trobat = false;
        for (int j = 0; j < v5.size() && !trobat; j++){
            if (v5[j].imp == v1[i].mitja_impos) {
                v5[j].num++;
                trobat = true;
            }
        }
        if (!trobat){
            x.imp = v1[i].mitja_impos;
            x.num = 1;
            v5.push_back(x);
        }
    }
}

t_imposicio Concepto(const v_imposicio& v3, string concepto) {
    t_imposicio r;
    bool trobat=false;
    for (int i=0; i<v3.size() and !trobat; i++) {
        if (v3[i].valor==concepto) {
            trobat=true;
            r=v3[i];
        }
    }
    return r;
}

void MedioImposicionMasComun (const v_denuncia&v1, const v_imposicio&v3){
    v_denimp v5;
    int mascomun = 0;
    string impmascomun;
    string impmascomun_desc;
    string consulta;
    bool trobat = false, valido = false;

    LlenarVectorDenunciaImp(v1, v5);

    for (int k = 0; k < v5.size(); k++){
        if (v5[k].num > mascomun){
            mascomun = v5[k].num;
            impmascomun = v5[k].imp;
            impmascomun_desc = Concepto(v3, impmascomun).valordesc_es;
            }
    }

    cout << "La denuncia más común tiene como medio de imposicion " << impmascomun_desc << " (" << impmascomun << ") y ha tenido " << mascomun << " incidencias, ";
    cout << "hecho que representa el " << (float(mascomun)/v1.size())*100 << "% de las denuncias." << endl;

}

void BuscadorMedioImposicionCantidad(const v_denuncia&v1){

    v_denimp v5;
    int cant;
    string imp;
    bool trobat = false;

    LlenarVectorDenunciaImp(v1, v5);

    cout << "Introduzca el medio de imposición para consultar la cantidad de denuncias registradas:\n --> ";
    cin >> imp;

    for (int i = 0; i < v5.size() && !trobat; i++){
        if (imp == v5[i].imp){trobat = true; cant = v5[i].num;}
    }
    if (!trobat) cout << "No se ha encontrado el medio de imposición." << endl << endl;
    else cout << "Este tipo de medio de imposición ha tenido un total de " << cant << " denuncias registradas." << endl << endl;
}


// Específicas Rafael Gras
bool MenuEspecificas2(v_denuncia&v1, v_multa&v2, v_imposicio&v3, t_zonas& z) {
    int opcion;
    cout << MENU_ESPECIFICAS_2;
    cin >> opcion;

    switch(opcion) {
        case 1: {
            for (int i=0; i<DISTRITOS_BCN; i++) cout << " - " << i << ": " << z.vd[i] << endl;
            break;}
        case 2: {
            for (int i=0; i<BARRIOS_BCN; i++) cout << " - " << i << ": " << z.vb[i] << endl;
            break;}
        case 3: {
            bool salir=false;
            int i=0;
            while (!salir) {
                int k=i;
                for (; k<(i+100) and k<z.vc.size(); k++) cout << " - " << z.vc[k].codigo << ": " << z.vc[k].nombre << endl; // Mostrar las siguientes 100 filas
                cout << "Se han mostrado las calles " << i << "-" << k-1 << "/" << z.vc.size() << " . Desea mostrar las siguientes 100? (S/N)\n -->";
                string r; cin >> r; salir=r=="N";
                i=k;
            }
            break;}
        case 4: {
            cout << "A continuación debes introducir los datos para filtrar las denuncias (especifica solo aquellos valores necesarios): " << endl;
            t_denuncia r = GenerarDenuncia(z,false,v2);
            RegistrarLog(logs, "Especificas-filtradas-por-zona", "-", "-");
            vector<int> indices = FiltrarDenuncias(v1,r); // Devuelve un vector con indices de v1
            cout << "Se han encontrado " << indices.size() << " denuncias" << endl;
            v_denuncia v1f;
            for (int i=0; i<indices.size(); i++) v1f.push_back(v1[indices[i]]);
            while (!MenuEspecificas1(v1f,v2,v3));
        }
        case 5: break;
        default: cout << "Opción no válida" << endl; break;
    }
    return opcion == 5;
}

// Validación de datos de la base de datos de la entidad 1

void ValidacionDatos(const v_denuncia&v1, const v_multa&v2, const v_imposicio&v3, const t_zonas&z) {

    bool error, trobat, salir = false, resp = true, errorimporte;
    string error_causa;
    char respuesta;
    int n = 0;

    for (int i = 0; i < v1.size() && !salir; i++) {
        error = false;
        error_causa = "";
        resp = true;
        errorimporte = false;

        //Validamos fechas y horas
        if ((v1[i].mes > 12 || v1[i].mes < 1) && v1[i].mes != -1) {
            error = true;
            error_causa = "Mes incorrecto";
        }
        else if ((v1[i].dia > 31 || v1[i].dia < 1) && v1[i].dia != -1) {
            error = true;
            error_causa = "Dia incorrecto";
        }
        else if ((v1[i].hora > 23 || v1[i].hora < 0) && v1[i].hora != -1) {
            error = true;
            error_causa = "Hora incorrecta";
        }
        else if ((v1[i].minuto > 59 || v1[i].minuto < 0) && v1[i].minuto != -1) {
            error = true;
            error_causa = "Minutos incorrectos";
        }

        // Validamos Zonas
        else if ( (v1[i].codi_districte >= DISTRITOS_BCN or v1[i].codi_districte < 0 ) && v1[i].codi_districte != -1) {
            error = true;
            error_causa = "Distrito inválido";
        }
        else if ( (v1[i].codi_barri >= BARRIOS_BCN or v1[i].codi_barri < 0) && v1[i].codi_barri != -1) {
             error = true;
             error_causa = "Barrio inválido";
        }

        //Validamos si existe la multa
        else if (v1[i].infraccio_codi != -1) {
            trobat = false;
            for(int k = 0; k < v2.size() && !trobat; k++) {
                if(v2[k].codi == v1[i].infraccio_codi) {
                    trobat = true;
                    n = k;
                    }
            }
            if(!trobat) {
                error = true;
                error_causa = "Código de multa no existe";
            }
            else if (v1[i].import != v2[n].import){
               error = true;
               error_causa = "Importe no concuerda con la última normativa";
               errorimporte = true;
            }
        }

        //Validamos si existe el subtipo
        if (!error && v1[i].subtipus_exp != "" && v1[i].subtipus_exp != "-1") {
            trobat = false;
            for(int k = 0; k < v3.size() && !trobat; k++) {
                if(v3[k].valor == v1[i].subtipus_exp) {
                        trobat = true;
                }
            }
            if(!trobat) {
                error = true;
                error_causa = "Subtipo expediente no existe"; }
        }

        //Validamos si existe el medio de imposición
        if (!error && v1[i].mitja_impos != "" && v1[i].mitja_impos != "-1") {
            trobat = false;
            for(int k = 0; k < v3.size() && !trobat; k++) {
                if(v3[k].valor == v1[i].mitja_impos) {
                    trobat = true;
                }
            }
            if(!trobat) {
                error = true;
                error_causa = "Medio de imposición no existe"; }
        }
        if (error) {
            cout << endl << "Hay un error en la denuncia número " << i+1 << " y tiene como causa: " << error_causa << endl;
            if (errorimporte) cout << "Su importe real es: " << v2[n].import << endl;

            DescripcionDenuncia(v1[i], z, v2); //Mostramos la denuncia completa

            cout << endl << "Quieres seguir validando los datos? (S/N):\n --> ";

            while(resp){
                cin >> respuesta;
                if (respuesta == 'N') {salir = true; resp = false;}
                else if (respuesta == 'S') {cout << "Retomando proceso... " << endl; resp = false;}
                else cout << "Introduce una respuesta válida." << endl;
            }
        }
    }
    cout << endl << "Validación finalizada." << endl;
}

// Otros:
string String(int n) {
    string r="";
    bool negativo=n<0;
    if (negativo) n*=-1;
    if (n==0) r=="0";
    else{
        while (n>0) {
            int i=n%10;
            char c = i + 48;
            r= c + r;
            n=n/10;
        }
    }
    if (negativo) r='-' + r;
    return r;
}
