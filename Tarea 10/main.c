#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEHICULOS 100
#define ARCHIVO_DATOS "vehiculos_rd.dat"

typedef struct
{
    int id;
    char marca[30];
    char modelo[30];
    double costo_compra;
    int vida_util_anios;
    double gasto_mantenimiento_anual;
    double gasto_seguro_anual;
    double gasto_neumaticos_anual;

    double km_por_galon_ciudad;
    double km_por_galon_autopista;

    double km_anuales_ciudad;
    double km_anuales_autopista;
} Vehiculo;

typedef struct
{
    double precio_combustible_galon;
} Config;

void cargar_datos(Vehiculo vehiculos[], int *count, Config *config);
void guardar_datos(Vehiculo vehiculos[], int count, Config config);
void crear_vehiculo(Vehiculo vehiculos[], int *count);
void borrar_vehiculo(Vehiculo vehiculos[], int *count);
void modificar_vehiculo(Vehiculo vehiculos[], int count);
void modificar_precio_combustible(Config *config);
void calcular_viaje(Vehiculo vehiculos[], int count, Config config);
void mostrar_analisis_completo(Vehiculo v, Config config);
void listar_vehiculos(Vehiculo vehiculos[], int count);
int buscar_vehiculo_index(Vehiculo vehiculos[], int count, int id);

int main()
{
    Vehiculo vehiculos[MAX_VEHICULOS];
    int total_vehiculos = 0;

    Config config = {290.00};

    cargar_datos(vehiculos, &total_vehiculos, &config);

    int opcion;
    do
        {
        printf("\n======================================================\n");
        printf("   SISTEMA DE COSTOS VEHICULARES (REP. DOMINICANA)   \n");
        printf("======================================================\n");
        printf("Precio Combustible: RD$ %.2f / galon\n", config.precio_combustible_galon);
        printf("------------------------------------------------------\n");
        printf("1. Crear vehiculo (Guardar datos)\n");
        printf("2. Borrar vehiculo\n");
        printf("3. Cambiar parametros de un vehiculo\n");
        printf("4. Calcular costo de un viaje\n");
        printf("5. Modificar precio del galon de combustible\n");
        printf("6. Mostrar analisis financiero y de costo por km\n");
        printf("7. Listar vehiculos registrados\n");
        printf("0. Salir y Guardar\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                crear_vehiculo(vehiculos, &total_vehiculos);
                guardar_datos(vehiculos, total_vehiculos, config);
                break;
            case 2:
                borrar_vehiculo(vehiculos, &total_vehiculos);
                guardar_datos(vehiculos, total_vehiculos, config);
                break;
            case 3:
                modificar_vehiculo(vehiculos, total_vehiculos);
                guardar_datos(vehiculos, total_vehiculos, config);
                break;
            case 4:
                calcular_viaje(vehiculos, total_vehiculos, config);
                break;
            case 5:
                modificar_precio_combustible(&config);
                guardar_datos(vehiculos, total_vehiculos, config);
                break;
            case 6:
                {
                if (total_vehiculos == 0)
                    {
                    printf("\nNo hay vehiculos registrados.\n");
                    break;
                }
                listar_vehiculos(vehiculos, total_vehiculos);
                int id;
                printf("\nIngrese ID del vehiculo para ver desglose financiero: ");
                scanf("%d", &id);
                int idx = buscar_vehiculo_index(vehiculos, total_vehiculos, id);
                if (idx != -1)
                {
                    mostrar_analisis_completo(vehiculos[idx], config);
                } else
                {
                    printf("\nVehiculo no encontrado.\n");
                }
                break;
            }
            case 7:
                listar_vehiculos(vehiculos, total_vehiculos);
                break;
            case 0:
                guardar_datos(vehiculos, total_vehiculos, config);
                printf("\nDatos guardados exitosamente. Saliendo...\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}

void cargar_datos(Vehiculo vehiculos[], int *count, Config *config)
{
    FILE *f = fopen(ARCHIVO_DATOS, "rb");
    if (f == NULL)
        {
        *count = 0;
        return;
    }
    fread(config, sizeof(Config), 1, f);
    fread(count, sizeof(int), 1, f);
    if (*count > 0) {
        fread(vehiculos, sizeof(Vehiculo), *count, f);
    }
    fclose(f);
}

void guardar_datos(Vehiculo vehiculos[], int count, Config config)
{
    FILE *f = fopen(ARCHIVO_DATOS, "wb");
    if (f == NULL)
        {
        printf("Error al guardar archivo.\n");
        return;
    }
    fwrite(&config, sizeof(Config), 1, f);
    fwrite(&count, sizeof(int), 1, f);
    if (count > 0)
    {
        fwrite(vehiculos, sizeof(Vehiculo), count, f);
    }
    fclose(f);
}

int buscar_vehiculo_index(Vehiculo vehiculos[], int count, int id)
{
    for (int i = 0; i < count; i++)
        {
        if (vehiculos[i].id == id) return i;
    }
    return -1;
}

void listar_vehiculos(Vehiculo vehiculos[], int count)
{
    if (count == 0)
        {
        printf("\nNo hay vehiculos registrados.\n");
        return;
    }
    printf("\n--- VEHICULOS REGISTRADOS ---\n");
    for (int i = 0; i < count; i++)
    {
        printf("ID: %d | %s %s | Valor: RD$ %.2f\n",
               vehiculos[i].id, vehiculos[i].marca, vehiculos[i].modelo, vehiculos[i].costo_compra);
    }
}

void crear_vehiculo(Vehiculo vehiculos[], int *count)
{
    if (*count >= MAX_VEHICULOS)
        {
        printf("Limite alcanzado.\n");
        return;
    }

    Vehiculo v;
    v.id = (*count == 0) ? 1 : vehiculos[*count - 1].id + 1;

    printf("\n--- REGISTRO DE VEHICULO (ID: %d) ---\n", v.id);
    printf("Marca (ej. Toyota): ");
    scanf("%s", v.marca);
    printf("Modelo (ej. Corolla): ");
    scanf("%s", v.modelo);
    printf("Costo de compra (RD$): ");
    scanf("%lf", &v.costo_compra);
    printf("Vida util estimada (anios): ");
    scanf("%d", &v.vida_util_anios);
    printf("Mantenimiento anual (RD$) [Cambios aceite, frenos, etc.]: ");
    scanf("%lf", &v.gasto_mantenimiento_anual);
    printf("Seguro anual (RD$): ");
    scanf("%lf", &v.gasto_seguro_anual);
    printf("Gasto anual en gomas/neumaticos (RD$): ");
    scanf("%lf", &v.gasto_neumaticos_anual);
    printf("Rendimiento Ciudad (km / galon): ");
    scanf("%lf", &v.km_por_galon_ciudad);
    printf("Rendimiento Autopista (km / galon): ");
    scanf("%lf", &v.km_por_galon_autopista);
    printf("Kilometros estimados por anio en CIUDAD: ");
    scanf("%lf", &v.km_anuales_ciudad);
    printf("Kilometros estimados por anio en AUTOPISTA: ");
    scanf("%lf", &v.km_anuales_autopista);

    vehiculos[*count] = v;
    (*count)++;
    printf("\nVehiculo RD$ registrado y guardado exitosamente.\n");
}


void borrar_vehiculo(Vehiculo vehiculos[], int *count)
{
    if (*count == 0)
        {
        printf("\nNo hay vehiculos registradas.\n");
        return;
    }

    listar_vehiculos(vehiculos, *count);
    int id;
    printf("\nIngrese el ID del vehiculo a eliminar: ");
    scanf("%d", &id);

    int idx = buscar_vehiculo_index(vehiculos, *count, id);
    if (idx == -1)
        {
        printf("Vehiculo no encontrado.\n");
        return;
    }

    for (int i = idx; i < *count - 1; i++)
        {
        vehiculos[i] = vehiculos[i + 1];
    }
    (*count)--;
    printf("Vehiculo ID %d eliminado correctamente.\n", id);
}

void modificar_vehiculo(Vehiculo vehiculos[], int count)
{
    if (count == 0)
        {
        printf("\nNo hay vehiculos para modificar.\n");
        return;
    }

    listar_vehiculos(vehiculos, count);
    int id;
    printf("\nIngrese ID del vehiculo a modificar: ");
    scanf("%d", &id);

    int idx = buscar_vehiculo_index(vehiculos, count, id);
    if (idx == -1)
        {
        printf("Vehiculo no encontrado.\n");
        return;
    }

    Vehiculo *v = &vehiculos[idx];
    printf("\n--- MODIFICAR PARAMETROS (ID: %d) ---\n", v->id);
    printf("Nuevo costo de compra (RD$ %.2f): ", v->costo_compra);
    scanf("%lf", &v->costo_compra);
    printf("Nuevos anios de vida util (%d): ", v->vida_util_anios);
    scanf("%d", &v->vida_util_anios);
    printf("Nuevo mantenimiento anual (RD$ %.2f): ", v->gasto_mantenimiento_anual);
    scanf("%lf", &v->gasto_mantenimiento_anual);
    printf("Nuevo seguro anual (RD$ %.2f): ", v->gasto_seguro_anual);
    scanf("%lf", &v->gasto_seguro_anual);
    printf("Nuevo gasto gomas anual (RD$ %.2f): ", v->gasto_neumaticos_anual);
    scanf("%lf", &v->gasto_neumaticos_anual);
    printf("Nuevo rendimiento Ciudad (%.2f km/gal): ", v->km_por_galon_ciudad);
    scanf("%lf", &v->km_por_galon_ciudad);
    printf("Nuevo rendimiento Autopista (%.2f km/gal): ", v->km_por_galon_autopista);
    scanf("%lf", &v->km_por_galon_autopista);

    printf("Datos actualizados exitosamente.\n");
}

// 2.5 Modificar precio del combustible por galón
void modificar_precio_combustible(Config *config) {
    printf("\nPrecio actual: RD$ %.2f / galon\n", config->precio_combustible_galon);
    printf("Ingrese nuevo precio del galon (RD$): ");
    scanf("%lf", &config->precio_combustible_galon);
    printf("Precio actualizado a RD$ %.2f / galon.\n", config->precio_combustible_galon);
}

void calcular_viaje(Vehiculo vehiculos[], int count, Config config)
{
    if (count == 0)
        {
        printf("\nNo hay vehiculos para simular un viaje.\n");
        return;
    }

    listar_vehiculos(vehiculos, count);
    int id;
    printf("\nSeleccione el ID del vehiculo: ");
    scanf("%d", &id);

    int idx = buscar_vehiculo_index(vehiculos, count, id);
    if (idx == -1) {
        printf("Vehiculo no encontrado.\n");
        return;
    }

    Vehiculo v = vehiculos[idx];
    double km_ciudad, km_autopista;

    printf("Kilometros a recorrer en CIUDAD: ");
    scanf("%lf", &km_ciudad);
    printf("Kilometros a recorrer en AUTOPISTA: ");
    scanf("%lf", &km_autopista);

    double galones_ciudad = km_ciudad / v.km_por_galon_ciudad;
    double galones_autopista = km_autopista / v.km_por_galon_autopista;
    double total_galones = galones_ciudad + galones_autopista;

    double costo_combustible_ciudad = galones_ciudad * config.precio_combustible_galon;
    double costo_combustible_autopista = galones_autopista * config.precio_combustible_galon;
    double costo_combustible_total = total_galones * config.precio_combustible_galon;

    double km_totales_anuales = v.km_anuales_ciudad + v.km_anuales_autopista;
    double amortizacion_anual = v.costo_compra / v.vida_util_anios;
    double costos_fijos_anuales = amortizacion_anual + v.gasto_mantenimiento_anual + v.gasto_seguro_anual + v.gasto_neumaticos_anual;

    double costo_fijo_por_km = (km_totales_anuales > 0) ? (costos_fijos_anuales / km_totales_anuales) : 0;
    double costo_fijo_viaje = (km_ciudad + km_autopista) * costo_fijo_por_km;

    printf("\n======================================================\n");
    printf("              DETALLE DEL VIAJE (RD$)                 \n");
    printf("======================================================\n");
    printf("Vehiculo: %s %s\n", v.marca, v.modelo);
    printf("Distancia: %.2f km (Ciudad: %.2f km | Autopista: %.2f km)\n",
           km_ciudad + km_autopista, km_ciudad, km_autopista);
    printf("Consumo estimado: %.2f Galones\n", total_galones);
    printf("  - Ciudad: %.2f gal | Autopista: %.2f gal\n", galones_ciudad, galones_autopista);
    printf("------------------------------------------------------\n");
    printf("GASTO COMBUSTIBLE EN VIAJE: RD$ %.2f\n", costo_combustible_total);
    printf("  - En Ciudad: RD$ %.2f\n", costo_combustible_ciudad);
    printf("  - En Autopista: RD$ %.2f\n", costo_combustible_autopista);
    printf("------------------------------------------------------\n");
    printf("COSTO REAL DEL VIAJE (Combustible + Desgaste/Seguro/Deprec.): RD$ %.2f\n",
           costo_combustible_total + costo_fijo_viaje);
    printf("======================================================\n");
}

void mostrar_analisis_completo(Vehiculo v, Config config)
{
    double amortizacion_anual = v.costo_compra / v.vida_util_anios;
    double mantenimiento_total_vida_util = v.gasto_mantenimiento_anual * v.vida_util_anios;

    double km_totales_anuales = v.km_anuales_ciudad + v.km_anuales_autopista;

    double prop_ciudad = (km_totales_anuales > 0) ? (v.km_anuales_ciudad / km_totales_anuales) : 0.5;
    double prop_autopista = (km_totales_anuales > 0) ? (v.km_anuales_autopista / km_totales_anuales) : 0.5;

    double gal_por_km_ciudad = 1.0 / v.km_por_galon_ciudad;
    double gal_por_km_autopista = 1.0 / v.km_por_galon_autopista;
    double consumo_promedio_gal_km = (gal_por_km_ciudad * prop_ciudad) + (gal_por_km_autopista * prop_autopista);

    double costo_km_combustible_ciudad = gal_por_km_ciudad * config.precio_combustible_galon;
    double costo_km_combustible_autopista = gal_por_km_autopista * config.precio_combustible_galon;
    double costo_km_combustible_promedio = consumo_promedio_gal_km * config.precio_combustible_galon;

    double costo_maint_por_km = (km_totales_anuales > 0) ? (v.gasto_mantenimiento_anual / km_totales_anuales) : 0;
    double costo_seguro_por_km = (km_totales_anuales > 0) ? (v.gasto_seguro_anual / km_totales_anuales) : 0;
    double costo_neumaticos_por_km = (km_totales_anuales > 0) ? (v.gasto_neumaticos_anual / km_totales_anuales) : 0;
    double amortizacion_por_km = (km_totales_anuales > 0) ? (amortizacion_anual / km_totales_anuales) : 0;

    double costo_real_total_por_km = costo_km_combustible_promedio + amortizacion_por_km +
                                      costo_maint_por_km + costo_seguro_por_km + costo_neumaticos_por_km;

    printf("\n======================================================\n");
    printf("   METRICAS Y AMORTIZACION EN RD$: %s %s   \n", v.marca, v.modelo);
    printf("======================================================\n");
    printf("1. Amortizacion anual: RD$ %.2f / anio (%d anios vida util)\n", amortizacion_anual, v.vida_util_anios);
    printf("2. Mantenimiento total estimado en vida util: RD$ %.2f\n", mantenimiento_total_vida_util);
    printf("3. Consumo promedio: %.4f galones/km (Gasto: RD$ %.2f/km)\n",
           consumo_promedio_gal_km, costo_km_combustible_promedio);
    printf("4. Costo en combustible por KM:\n");
    printf("   - En Ciudad: RD$ %.2f / km\n", costo_km_combustible_ciudad);
    printf("   - En Autopista: RD$ %.2f / km\n", costo_km_combustible_autopista);
    printf("5. COSTO REAL DE USO POR KM: RD$ %.2f / km\n", costo_real_total_por_km);
    printf("   Desglose por KM:\n");
    printf("     * Combustible medio:   RD$ %.2f / km\n", costo_km_combustible_promedio);
    printf("     * Depreciacion/Amort:  RD$ %.2f / km\n", amortizacion_por_km);
    printf("     * Mantenimiento:       RD$ %.2f / km\n", costo_maint_por_km);
    printf("     * Seguro:              RD$ %.2f / km\n", costo_seguro_por_km);
    printf("     * Neumaticos:          RD$ %.2f / km\n", costo_neumaticos_por_km);
    printf("======================================================\n");
}
