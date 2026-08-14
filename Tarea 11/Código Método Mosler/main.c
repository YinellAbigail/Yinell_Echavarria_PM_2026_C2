#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_TEXTO 100
#define FILENAME_DEFAULT "riesgos.json"

#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_ENTER 13

// Estructura del Riesgo
typedef struct {
    char nombre[MAX_TEXTO];
    char descripcion[MAX_TEXTO];

    // Fase 2: Criterios (Escala 1 - 5)
    int F; // Función
    int S; // Sustitución
    int P; // Profundidad
    int E; // Extensión
    int A; // Agresión
    int V; // Vulnerabilidad

    // Fase 3 & 4: Resultados
    int I;  // Impacto (F * S)
    int C;  // Carácter del riesgo (I + P)
    int PR; // Probabilidad (A * V)
    int ER; // Riesgo Estimado (C * PR)
    char clase[30];
} Riesgo;

typedef struct {
    Riesgo *arreglo;
    int cantidad;
    int capacidad;
} ListaRiesgos;

char archivo_actual[MAX_TEXTO] = FILENAME_DEFAULT;

// Declaración de funciones
void inicializar_lista(ListaRiesgos *lista);
void liberar_lista(ListaRiesgos *lista);
void redimensionar_lista(ListaRiesgos *lista, int nueva_capacidad);
void calcular_fases_mosler(Riesgo *r);
void agregar_riesgo(ListaRiesgos *lista, Riesgo r);

// Persistencia
void guardar_en_json(ListaRiesgos *lista, const char *filename);
void cargar_desde_json(ListaRiesgos *lista, const char *filename);
void guardar_en_xml(ListaRiesgos *lista, const char *filename);

void limpiar_buffer();

// Interfaz
void setColor(int colorFondo, int colorTexto);
void gotoxy(int x, int y);
void ocultarCursor();
void mostrarCursor();
void dibujarCuadro(int x1, int y1, int x2, int y2);
int mostrarMenuInteractivo(const char *titulo, const char *opciones[], int numOpciones);
void pausar();

void ui_crear_riesgo(ListaRiesgos *lista);
void ui_modificar_riesgo(ListaRiesgos *lista);
void ui_calcular_todos(ListaRiesgos *lista);
void ui_imprimir_riesgos(ListaRiesgos *lista);
void ui_borrar_riesgo(ListaRiesgos *lista);
void ui_seleccionar_archivo(ListaRiesgos *lista);
void ui_crear_archivo(ListaRiesgos *lista);
void ui_exportar_xml(ListaRiesgos *lista);

int main() {
    ocultarCursor();
    SetConsoleTitle("Sistema de Gestion de Riesgos - Metodo Mosler (JSON/XML)");

    ListaRiesgos lista;
    inicializar_lista(&lista);
    cargar_desde_json(&lista, archivo_actual);

    const char *opciones[] = {
        "1. Crear riesgo",
        "2. Modificar parametros del riesgo",
        "3. Calcular riesgos",
        "4. Imprimir los riesgos",
        "5. Borrar riesgo",
        "6. Seleccionar un archivo especifico",
        "7. Crear un archivo especifico",
        "8. Guardar en archivo JSON",
        "9. Exportar a formato XML",
        "10. Salir"
    };
    int numOpciones = 10;
    int opcion = 1;

    do {
        opcion = mostrarMenuInteractivo("SISTEMA DE GESTION DE RIESGOS - METODO MOSLER", opciones, numOpciones);

        switch (opcion) {
            case 1: ui_crear_riesgo(&lista); break;
            case 2: ui_modificar_riesgo(&lista); break;
            case 3: ui_calcular_todos(&lista); break;
            case 4: ui_imprimir_riesgos(&lista); break;
            case 5: ui_borrar_riesgo(&lista); break;
            case 6: ui_seleccionar_archivo(&lista); break;
            case 7: ui_crear_archivo(&lista); break;
            case 8:
                guardar_en_json(&lista, archivo_actual);
                system("cls");
                dibujarCuadro(10, 5, 70, 9);
                gotoxy(12, 7);
                setColor(0, 10);
                printf("[OK] Datos guardados en JSON correctamente en '%s'", archivo_actual);
                pausar();
                break;
            case 9:
                ui_exportar_xml(&lista);
                break;
            case 10:
                system("cls");
                setColor(0, 14);
                gotoxy(25, 10);
                printf("Saliendo del programa...");
                setColor(0, 7);
                break;
        }
    } while (opcion != 10);

    liberar_lista(&lista);
    return 0;
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void setColor(int colorFondo, int colorTexto) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (colorFondo << 4) | colorTexto);
}

void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ocultarCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(consoleHandle, &info);
}

void mostrarCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {20, TRUE};
    SetConsoleCursorInfo(consoleHandle, &info);
}

void dibujarCuadro(int x1, int y1, int x2, int y2) {
    setColor(0, 11);
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y1); printf("-");
        gotoxy(i, y2); printf("-");
    }
    for (int i = y1; i <= y2; i++) {
        gotoxy(x1, i); printf("|");
        gotoxy(x2, i); printf("|");
    }
    gotoxy(x1, y1); printf("+");
    gotoxy(x2, y1); printf("+");
    gotoxy(x1, y2); printf("+");
    gotoxy(x2, y2); printf("+");
    setColor(0, 7);
}

int mostrarMenuInteractivo(const char *titulo, const char *opciones[], int numOpciones) {
    int seleccion = 1;
    int tecla;

    while (1) {
        system("cls");
        ocultarCursor();

        dibujarCuadro(4, 1, 74, 5);
        gotoxy(12, 2); setColor(0, 14);
        printf("=== %s ===", titulo);

        gotoxy(8, 4); setColor(0, 11);
        printf("Archivo activo: [%s]", archivo_actual);

        dibujarCuadro(4, 6, 74, 8 + numOpciones);

        for (int i = 0; i < numOpciones; i++) {
            gotoxy(8, 7 + i);
            if (i + 1 == seleccion) {
                setColor(3, 0);
                printf("  -> %-50s ", opciones[i]);
                setColor(0, 7);
            } else {
                setColor(0, 15);
                printf("     %-50s ", opciones[i]);
            }
        }

        gotoxy(8, 7 + numOpciones + 1); setColor(0, 8);
        printf("Usa las flechas [Arriba/Abajo] y presiona [ENTER]");

        tecla = getch();
        if (tecla == 224) {
            tecla = getch();
            if (tecla == TECLA_ARRIBA) seleccion = (seleccion == 1) ? numOpciones : seleccion - 1;
            else if (tecla == TECLA_ABAJO) seleccion = (seleccion == numOpciones) ? 1 : seleccion + 1;
        } else if (tecla == TECLA_ENTER) {
            return seleccion;
        }
    }
}

void pausar() {
    setColor(0, 8);
    printf("\n\n Presione cualquier tecla para continuar...");
    getch();
    setColor(0, 7);
}

// --- MEMORIA DINÁMICA ---

void inicializar_lista(ListaRiesgos *lista) {
    lista->cantidad = 0;
    lista->capacidad = 4;
    lista->arreglo = (Riesgo *) malloc(lista->capacidad * sizeof(Riesgo));
}

void redimensionar_lista(ListaRiesgos *lista, int nueva_capacidad) {
    lista->arreglo = (Riesgo *) realloc(lista->arreglo, nueva_capacidad * sizeof(Riesgo));
    lista->capacidad = nueva_capacidad;
}

void liberar_lista(ListaRiesgos *lista) {
    free(lista->arreglo);
    lista->arreglo = NULL;
    lista->cantidad = 0;
    lista->capacidad = 0;
}

void calcular_fases_mosler(Riesgo *r) {
    r->I = r->F * r->S;       // Fase 3: Impacto
    r->C = r->I + r->P;       // Fase 3: Carácter del riesgo
    r->PR = r->A * r->V;      // Fase 3: Probabilidad
    r->ER = r->C * r->PR;     // Fase 4: Riesgo Estimado

    if (r->ER <= 250) strcpy(r->clase, "Muy Bajo");
    else if (r->ER <= 500) strcpy(r->clase, "Bajo");
    else if (r->ER <= 750) strcpy(r->clase, "Normal / Medio");
    else if (r->ER <= 1000) strcpy(r->clase, "Elevado");
    else strcpy(r->clase, "Muy Alto");
}

void agregar_riesgo(ListaRiesgos *lista, Riesgo r) {
    if (lista->cantidad >= lista->capacidad) {
        redimensionar_lista(lista, lista->capacidad * 2);
    }
    calcular_fases_mosler(&r);
    lista->arreglo[lista->cantidad++] = r;
}

// --- PERSISTENCIA JSON ---

void guardar_en_json(ListaRiesgos *lista, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "[\n");
    for (int i = 0; i < lista->cantidad; i++) {
        Riesgo *r = &lista->arreglo[i];
        fprintf(f, "  {\n");
        fprintf(f, "    \"nombre\": \"%s\",\n", r->nombre);
        fprintf(f, "    \"descripcion\": \"%s\",\n", r->descripcion);
        fprintf(f, "    \"F\": %d,\n", r->F);
        fprintf(f, "    \"S\": %d,\n", r->S);
        fprintf(f, "    \"P\": %d,\n", r->P);
        fprintf(f, "    \"E\": %d,\n", r->E);
        fprintf(f, "    \"A\": %d,\n", r->A);
        fprintf(f, "    \"V\": %d,\n", r->V);
        fprintf(f, "    \"I\": %d,\n", r->I);
        fprintf(f, "    \"C\": %d,\n", r->C);
        fprintf(f, "    \"PR\": %d,\n", r->PR);
        fprintf(f, "    \"ER\": %d,\n", r->ER);
        fprintf(f, "    \"clase\": \"%s\"\n", r->clase);
        fprintf(f, "  }%s\n", (i == lista->cantidad - 1) ? "" : ",");
    }
    fprintf(f, "]\n");
    fclose(f);
}

void cargar_desde_json(ListaRiesgos *lista, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;

    lista->cantidad = 0;
    char linea[512];
    Riesgo r;
    int en_objeto = 0;

    while (fgets(linea, sizeof(linea), f)) {
        if (strstr(linea, "{")) {
            en_objeto = 1;
            memset(&r, 0, sizeof(Riesgo));
        } else if (strstr(linea, "}")) {
            if (en_objeto) {
                agregar_riesgo(lista, r);
                en_objeto = 0;
            }
        } else if (en_objeto) {
            char *p;
            if ((p = strstr(linea, "\"nombre\":"))) {
                sscanf(p, "\"nombre\": \"%[^\"]\"", r.nombre);
            } else if ((p = strstr(linea, "\"descripcion\":"))) {
                sscanf(p, "\"descripcion\": \"%[^\"]\"", r.descripcion);
            } else if ((p = strstr(linea, "\"F\":"))) {
                sscanf(p, "\"F\": %d", &r.F);
            } else if ((p = strstr(linea, "\"S\":"))) {
                sscanf(p, "\"S\": %d", &r.S);
            } else if ((p = strstr(linea, "\"P\":"))) {
                sscanf(p, "\"P\": %d", &r.P);
            } else if ((p = strstr(linea, "\"E\":"))) {
                sscanf(p, "\"E\": %d", &r.E);
            } else if ((p = strstr(linea, "\"A\":"))) {
                sscanf(p, "\"A\": %d", &r.A);
            } else if ((p = strstr(linea, "\"V\":"))) {
                sscanf(p, "\"V\": %d", &r.V);
            }
        }
    }
    fclose(f);
}

// --- PERSISTENCIA XML ---

void guardar_en_xml(ListaRiesgos *lista, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<MatrizRiesgos>\n");

    for (int i = 0; i < lista->cantidad; i++) {
        Riesgo *r = &lista->arreglo[i];
        fprintf(f, "  <Riesgo id=\"%d\">\n", i + 1);
        fprintf(f, "    <Nombre>%s</Nombre>\n", r->nombre);
        fprintf(f, "    <Descripcion>%s</Descripcion>\n", r->descripcion);
        fprintf(f, "    <Criterios>\n");
        fprintf(f, "      <F>%d</F>\n", r->F);
        fprintf(f, "      <S>%d</S>\n", r->S);
        fprintf(f, "      <P>%d</P>\n", r->P);
        fprintf(f, "      <E>%d</E>\n", r->E);
        fprintf(f, "      <A>%d</A>\n", r->A);
        fprintf(f, "      <V>%d</V>\n", r->V);
        fprintf(f, "    </Criterios>\n");
        fprintf(f, "    <Resultados>\n");
        fprintf(f, "      <I>%d</I>\n", r->I);
        fprintf(f, "      <C>%d</C>\n", r->C);
        fprintf(f, "      <PR>%d</PR>\n", r->PR);
        fprintf(f, "      <ER>%d</ER>\n", r->ER);
        fprintf(f, "      <Clasificacion>%s</Clasificacion>\n", r->clase);
        fprintf(f, "    </Resultados>\n");
        fprintf(f, "  </Riesgo>\n");
    }

    fprintf(f, "</MatrizRiesgos>\n");
    fclose(f);
}

// --- MÓDULOS DE USUARIO ---

void ui_exportar_xml(ListaRiesgos *lista) {
    system("cls");
    mostrarCursor();
    dibujarCuadro(2, 1, 65, 8);
    gotoxy(4, 3); setColor(0, 14); printf("EXPORTAR DATOS A XML");

    char xml_filename[MAX_TEXTO];
    gotoxy(4, 5); setColor(0, 15); printf("Nombre del archivo XML (ej. riesgos.xml): ");

    fflush(stdin);
    fgets(xml_filename, MAX_TEXTO, stdin);
    xml_filename[strcspn(xml_filename, "\r\n")] = 0;

    if (strlen(xml_filename) == 0) strcpy(xml_filename, "riesgos.xml");

    guardar_en_xml(lista, xml_filename);

    gotoxy(4, 7); setColor(0, 10); printf("[OK] Documento XML exportado en '%s'", xml_filename);
    ocultarCursor();
    pausar();
}

void ui_crear_riesgo(ListaRiesgos *lista) {
    system("cls");
    mostrarCursor();
    dibujarCuadro(2, 1, 75, 18);

    Riesgo r;
    gotoxy(4, 2); setColor(0, 14); printf("=== CREAR NUEVO RIESGO (FASE 1 Y 2) ===");

    gotoxy(4, 4); setColor(0, 15); printf("Nombre del Riesgo: ");
    fflush(stdin);
    fgets(r.nombre, MAX_TEXTO, stdin);
    r.nombre[strcspn(r.nombre, "\r\n")] = 0;

    gotoxy(4, 6); printf("Descripcion corta: ");
    fflush(stdin);
    fgets(r.descripcion, MAX_TEXTO, stdin);
    r.descripcion[strcspn(r.descripcion, "\r\n")] = 0;

    gotoxy(4, 8); setColor(0, 11); printf("Ingrese valores entre 1 y 5 para cada criterio:");
    gotoxy(4, 10); setColor(0, 15); printf("F - Funcion (1-5): "); scanf("%d", &r.F);
    gotoxy(4, 11); printf("S - Sustitucion (1-5): "); scanf("%d", &r.S);
    gotoxy(4, 12); printf("P - Profundidad (1-5): "); scanf("%d", &r.P);
    gotoxy(4, 13); printf("E - Extension (1-5): "); scanf("%d", &r.E);
    gotoxy(4, 14); printf("A - Agresion (1-5): "); scanf("%d", &r.A);
    gotoxy(4, 15); printf("V - Vulnerabilidad (1-5): "); scanf("%d", &r.V);

    limpiar_buffer();

    agregar_riesgo(lista, r);

    gotoxy(4, 17); setColor(0, 10); printf("[OK] ¡Riesgo creado y procesado correctamente!");
    ocultarCursor();
    pausar();
}

void ui_modificar_riesgo(ListaRiesgos *lista) {
    system("cls");
    if (lista->cantidad == 0) {
        dibujarCuadro(2, 1, 60, 5);
        gotoxy(4, 3); setColor(0, 12); printf("No hay riesgos registrados para modificar.");
        pausar();
        return;
    }

    mostrarCursor();
    printf("=== MODIFICAR PARAMETROS DE RIESGO ===\n\n");
    for (int i = 0; i < lista->cantidad; i++) {
        setColor(0, 11); printf(" [%d]", i + 1);
        setColor(0, 15); printf(" %s - (%s)\n", lista->arreglo[i].nombre, lista->arreglo[i].descripcion);
    }

    int idx;
    printf("\nSeleccione el numero de riesgo a modificar: ");
    scanf("%d", &idx);
    limpiar_buffer();
    idx--;

    if (idx >= 0 && idx < lista->cantidad) {
        Riesgo *r = &lista->arreglo[idx];
        printf("\nModificando: "); setColor(0, 14); printf("%s\n", r->nombre); setColor(0, 15);
        printf("Nuevo F - Funcion (1-5) [Actual %d]: ", r->F); scanf("%d", &r->F);
        printf("Nuevo S - Sustitucion (1-5) [Actual %d]: ", r->S); scanf("%d", &r->S);
        printf("Nuevo P - Profundidad (1-5) [Actual %d]: ", r->P); scanf("%d", &r->P);
        printf("Nuevo E - Extension (1-5) [Actual %d]: ", r->E); scanf("%d", &r->E);
        printf("Nuevo A - Agresion (1-5) [Actual %d]: ", r->A); scanf("%d", &r->A);
        printf("Nuevo V - Vulnerabilidad (1-5) [Actual %d]: ", r->V); scanf("%d", &r->V);
        limpiar_buffer();

        calcular_fases_mosler(r);
        setColor(0, 10); printf("\n[OK] Parametros actualizados correctamente.");
    } else {
        setColor(0, 12); printf("\n[ERROR] Numero invalido.");
    }
    ocultarCursor();
    pausar();
}

void ui_calcular_todos(ListaRiesgos *lista) {
    system("cls");
    dibujarCuadro(2, 1, 70, 6);
    gotoxy(4, 3); setColor(0, 10);
    for (int i = 0; i < lista->cantidad; i++) {
        calcular_fases_mosler(&lista->arreglo[i]);
    }
    printf("[OK] Se han recalculado las Fases 3 y 4 para los %d riesgos.", lista->cantidad);
    pausar();
}

void ui_imprimir_riesgos(ListaRiesgos *lista) {
    system("cls");
    setColor(0, 14);
    printf("=================================================================================\n");
    printf("                   TABLA DE EVALUACION DE RIESGOS (MOSLER)                       \n");
    printf("=================================================================================\n\n");

    if (lista->cantidad == 0) {
        setColor(0, 12); printf(" No hay riesgos registrados para mostrar.\n");
        pausar();
        return;
    }

    setColor(0, 11);
    printf("%-3s | %-16s | %-18s | %-3s | %-3s | %-3s | %-5s | %-12s\n",
           "ID", "Nombre", "Descripcion", "I", "C", "PR", "ER", "Clasificacion");
    setColor(0, 8);
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < lista->cantidad; i++) {
        Riesgo *r = &lista->arreglo[i];

        if (r->ER <= 500) setColor(0, 10);      // Verde
        else if (r->ER <= 750) setColor(0, 14); // Amarillo
        else setColor(0, 12);                   // Rojo

        printf("%-3d | %-16.16s | %-18.18s | %-3d | %-3d | %-3d | %-5d | %-12s\n",
               i + 1, r->nombre, r->descripcion, r->I, r->C, r->PR, r->ER, r->clase);
    }
    setColor(0, 8);
    printf("---------------------------------------------------------------------------------\n");
    pausar();
}

void ui_borrar_riesgo(ListaRiesgos *lista) {
    system("cls");
    if (lista->cantidad == 0) {
        dibujarCuadro(2, 1, 50, 5);
        gotoxy(4, 3); setColor(0, 12); printf("No hay riesgos para eliminar.");
        pausar();
        return;
    }

    mostrarCursor();
    printf("=== BORRAR RIESGO ===\n\n");
    for (int i = 0; i < lista->cantidad; i++) {
        printf(" %d. %s\n", i + 1, lista->arreglo[i].nombre);
    }

    int idx;
    printf("\nIngrese el numero de riesgo a eliminar: ");
    scanf("%d", &idx);
    limpiar_buffer();
    idx--;

    if (idx >= 0 && idx < lista->cantidad) {
        for (int i = idx; i < lista->cantidad - 1; i++) {
            lista->arreglo[i] = lista->arreglo[i + 1];
        }
        lista->cantidad--;
        setColor(0, 10); printf("\n[OK] Riesgo eliminado exitosamente de memoria.");
    } else {
        setColor(0, 12); printf("\n[ERROR] Numero invalido.");
    }
    ocultarCursor();
    pausar();
}

void ui_seleccionar_archivo(ListaRiesgos *lista) {
    system("cls");
    mostrarCursor();
    dibujarCuadro(2, 1, 65, 7);
    gotoxy(4, 3); setColor(0, 14); printf("SELECCIONAR Y CARGAR ARCHIVO JSON");
    gotoxy(4, 5); setColor(0, 15); printf("Nombre del archivo (ej. datos.json): ");

    fflush(stdin);
    fgets(archivo_actual, MAX_TEXTO, stdin);
    archivo_actual[strcspn(archivo_actual, "\r\n")] = 0;

    cargar_desde_json(lista, archivo_actual);

    gotoxy(4, 8); setColor(0, 10); printf("[OK] Archivo '%s' cargado.", archivo_actual);
    ocultarCursor();
    pausar();
}

void ui_crear_archivo(ListaRiesgos *lista) {
    system("cls");
    mostrarCursor();
    dibujarCuadro(2, 1, 65, 7);
    gotoxy(4, 3); setColor(0, 14); printf("CREAR ARCHIVO ESPECIFICO JSON");
    gotoxy(4, 5); setColor(0, 15); printf("Nombre del nuevo archivo (ej. mis_riesgos.json): ");

    fflush(stdin);
    fgets(archivo_actual, MAX_TEXTO, stdin);
    archivo_actual[strcspn(archivo_actual, "\r\n")] = 0;

    FILE *f = fopen(archivo_actual, "w");
    if (f) {
        fprintf(f, "[\n]\n");
        fclose(f);
        gotoxy(4, 8); setColor(0, 10); printf("[OK] Archivo JSON '%s' creado correctamente.", archivo_actual);
    } else {
        gotoxy(4, 8); setColor(0, 12); printf("[ERROR] No se pudo crear el archivo.");
    }
    ocultarCursor();
    pausar();
}
