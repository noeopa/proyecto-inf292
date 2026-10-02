# Proyecto Optimización (INF-292)

Generador de instancias para el proyecto de **Selección y Optimización de Materiales para Eficiencia Energética Residencial**.

El programa genera automáticamente instancias pequeñas, medianas y grandes, considerando dimensiones de la vivienda, elementos constructivos, elementos fijos y optimizables, catálogos de alternativas, costos, transmitancias térmicas y un valor máximo de transmitancia global `Umax`.

Se generan en total:

- 5 instancias pequeñas.
- 5 instancias medianas.
- 5 instancias grandes.

En total se obtienen **15 instancias válidas**.

---

## Clases

### Alternativa

Representa una solución constructiva disponible dentro del catálogo de una categoría.

Cada alternativa contiene:

- Nombre.
- Transmitancia térmica `U`.
- Costo por metro cuadrado `costoM2`.

---

### Elemento

Representa un elemento constructivo de la vivienda.

Las categorías consideradas son:

- Muro.
- Techo.
- Piso.
- Ventana.
- Puerta.

Cada elemento contiene información como:

- Nombre.
- Categoría.
- Área.
- Indicación de si es fijo u optimizable.

Si el elemento es fijo, posee directamente:

- `Ufijo`.
- `costoFijoM2`.

Si el elemento es optimizable, utiliza el catálogo correspondiente a su categoría.

---

### Instancia

Representa una vivienda completa.

Contiene información como:

- Tipo de instancia.
- Semilla utilizada.
- Largo.
- Ancho.
- Altura.
- Superficie.
- Volumen.
- Valor de `Umax`.
- Conjunto de elementos de la vivienda.

Los tipos de instancia posibles son:

- Pequeña.
- Mediana.
- Grande.

---

## Funciones

A continuación se explica de forma general qué realiza cada una de las principales funciones del generador.

### generarDimensiones()

**Tipo:** `void`

Genera aleatoriamente las dimensiones físicas de una vivienda.

Se generan:

- Largo.
- Ancho.
- Altura.

A partir de estos valores se calculan:

- Superficie.
- Volumen interior.

Los rangos utilizados son:

- Largo: entre 8.0 y 15.0 m.
- Ancho: entre 6.0 y 12.0 m.
- Altura: entre 2.3 y 2.8 m.

La superficie se calcula como:

```text
superficie = largo * ancho
```

El volumen se calcula como:

```text
volumen = superficie * altura
```

---

### generarUmax()

**Retorna:** un número.

Calcula el valor máximo de transmitancia térmica global permitido para la instancia.

Primero se calcula la menor transmitancia térmica global alcanzable utilizando:

- El valor `Ufijo` en los elementos fijos.
- La alternativa con menor `U` en los elementos optimizables.

Luego se genera un margen aleatorio entre `0.05` y `0.30`.

Finalmente:

```text
Umax = UminFactible + margen
```

De esta forma se garantiza que la instancia generada posea al menos una solución factible.

---

### elegirCategorias()

**Retorna:** arreglo de categorías.

Determina qué categorías estarán presentes en la instancia.

#### Instancias pequeñas

Se utilizan entre 3 y 4 categorías.

La categoría `Muro` siempre está presente.

Las demás categorías se seleccionan aleatoriamente entre:

- Techo.
- Piso.
- Ventana.
- Puerta.

#### Instancias medianas y grandes

Se utilizan las cinco categorías:

- Muro.
- Techo.
- Piso.
- Ventana.
- Puerta.

---

### generarCantidadAlternativas()

**Retorna:** un número.

Genera aleatoriamente la cantidad de alternativas que tendrá un catálogo dependiendo del tamaño de la instancia.

Los rangos utilizados son:

- Pequeña: entre 5 y 10 alternativas.
- Mediana: entre 11 y 25 alternativas.
- Grande: entre 26 y 50 alternativas.

---

### generarU()

**Retorna:** un número.

Genera una transmitancia térmica `U` dependiendo de la categoría del elemento.

Los rangos utilizados son:

- Muro: entre 0.3 y 1.5 W/m²K.
- Techo: entre 0.2 y 1.2 W/m²K.
- Piso: entre 0.3 y 1.4 W/m²K.
- Ventana: entre 1.0 y 5.5 W/m²K.
- Puerta: entre 1.0 y 3.5 W/m²K.

---

### generarCosto()

**Retorna:** un número.

Calcula el costo por metro cuadrado a partir de la categoría del elemento y su valor de transmitancia térmica `U`.

La relación utilizada asegura que:

- Un menor valor de `U` representa un mejor aislamiento térmico.
- Un menor valor de `U` implica un mayor costo.

Las expresiones utilizadas son:

#### Muro

```text
C = 10000 + 12000 / U
```

#### Techo

```text
C = 12000 + 10000 / U
```

#### Piso

```text
C = 9000 + 11000 / U
```

#### Ventana

```text
C = 50000 + 70000 / U
```

#### Puerta

```text
C = 30000 + 40000 / U
```

---

### generarCatalogo()

**Retorna:** un conjunto de alternativas.

Genera un catálogo de soluciones constructivas para una categoría.

Cada alternativa contiene:

- Nombre.
- Valor de transmitancia térmica `U`.
- Costo por metro cuadrado.

El catálogo se genera una sola vez por categoría dentro de cada instancia.

Si existen varios elementos optimizables pertenecientes a la misma categoría, todos utilizan el mismo catálogo.

Por ejemplo:

- Muro Norte.
- Muro Sur.
- Muro Este.
- Muro Oeste.

Si estos elementos son optimizables, todos utilizan el mismo catálogo correspondiente a la categoría `Muro`.

---

### generarElementoFijo()

**Tipo:** `void`

Configura un elemento como fijo.

Como la solución constructiva del elemento ya está definida, no necesita un catálogo de alternativas.

Se generan directamente:

- `Ufijo`.
- `costoFijoM2`.

Estos valores permanecen constantes durante la optimización.

---

### generarElementoOptimizable()

**Tipo:** `void`

Configura un elemento como optimizable.

La función recibe un catálogo previamente generado para su categoría y lo asigna al elemento.

El catálogo no se vuelve a generar si ya existe uno para la misma categoría.

---

### asignarAreas()

Calcula y asigna el área correspondiente a cada elemento constructivo.

#### Muros

Primero se calculan las áreas brutas de los muros.

Para los muros Norte y Sur:

```text
area = ancho * altura
```

Para los muros Este y Oeste:

```text
area = largo * altura
```

Posteriormente se descuenta el área correspondiente a ventanas y puertas.

#### Techo

El área del techo corresponde a la superficie de la vivienda:

```text
areaTecho = largo * ancho
```

#### Piso

El área del piso corresponde a la superficie de la vivienda:

```text
areaPiso = largo * ancho
```

#### Ventanas

El área total de ventanas corresponde a un porcentaje aleatorio entre 10% y 25% del área bruta total de los muros.

```text
areaVentanasTotal =
porcentajeVentanas * areaMurosTotal
```

Luego se divide en partes iguales entre la cantidad de ventanas generadas.

#### Puertas

Cada puerta posee un área aleatoria entre:

```text
1.5 m² y 3.0 m²
```

El área total de puertas corresponde a la suma de las áreas individuales.

#### Descuento de aberturas en los muros

El área total de aberturas se calcula como:

```text
areaAberturasTotal =
areaVentanasTotal + areaPuertasTotal
```

Posteriormente, esta área se distribuye proporcionalmente según el área bruta de cada muro.

Para cada muro:

```text
proporcionMuro =
areaBrutaMuro / areaMurosTotal
```

Luego:

```text
areaAberturasMuro =
areaAberturasTotal * proporcionMuro
```

Finalmente:

```text
areaEfectivaMuro =
areaBrutaMuro - areaAberturasMuro
```

Esto permite evitar que el área de ventanas y puertas sea contabilizada dos veces.

---

### generarInstancia()

Genera una instancia completa a partir de:

- Tipo de instancia.
- Semilla.

El procedimiento general es:

1. Generar las dimensiones de la vivienda.
2. Seleccionar las categorías.
3. Crear los elementos.
4. Determinar cuáles serán fijos y cuáles optimizables.
5. Generar los catálogos necesarios.
6. Reutilizar el mismo catálogo para elementos optimizables de una misma categoría.
7. Asignar las áreas.
8. Calcular un valor factible de `Umax`.

El uso de una semilla permite reproducir una instancia determinada.

---

### validarInstancia()

**Retorna:** `true` o `false`.

Comprueba que la instancia generada cumpla con las condiciones definidas para el proyecto.

Entre otras cosas, se verifica:

- Que las dimensiones sean positivas.
- Que las áreas sean positivas.
- Que el número de categorías sea correcto.
- Que la cantidad de elementos optimizables se encuentre dentro del rango correspondiente.
- Que la cantidad de alternativas sea válida.
- Que los valores de `U` sean positivos.
- Que los costos sean positivos.
- Que exista una relación inversa entre `U` y costo.
- Que exista al menos una solución capaz de cumplir `Umax`.

Si una instancia no cumple alguna condición, se descarta y se genera una nueva utilizando otra semilla.

---

## Cantidad de elementos optimizables

La cantidad de elementos optimizables depende del tamaño de la instancia.

- Pequeña: entre 2 y 4 elementos optimizables.
- Mediana: entre 5 y 8 elementos optimizables.
- Grande: entre 9 y 15 elementos optimizables.

Los demás elementos se consideran fijos.

---

## Generación de elementos

La cantidad de elementos generados depende de la categoría y del tamaño de la instancia.

### Muros

Se generan siempre 4 muros:

- Muro Norte.
- Muro Sur.
- Muro Este.
- Muro Oeste.

### Techo

Se genera 1 techo.

### Piso

Se genera 1 piso.

### Ventanas

La cantidad depende del tamaño de la instancia.

En las instancias grandes se permite una mayor cantidad de ventanas para aumentar el número total posible de elementos.

### Puertas

La cantidad también puede variar según el tamaño de la instancia.

---

## Consideraciones lógicas

### Catálogos por categoría

Cada categoría optimizable posee un único catálogo dentro de una instancia.

Por ejemplo, si existen cuatro muros optimizables:

- Muro Norte.
- Muro Sur.
- Muro Este.
- Muro Oeste.

El catálogo de la categoría `Muro` se genera una sola vez y se reutiliza en los cuatro elementos.

Si una categoría contiene únicamente elementos fijos, no es necesario generar un catálogo para ella.

---

### Elementos fijos y optimizables

Los elementos se dividen en dos grupos:

- Elementos fijos.
- Elementos optimizables.

Los elementos fijos poseen directamente un valor de transmitancia térmica y un costo.

Los elementos optimizables deben seleccionar una alternativa desde el catálogo de su categoría.

Cada elemento pertenece únicamente a uno de estos dos grupos.

---

### Factibilidad

Las instancias generadas deben ser factibles.

Para garantizarlo, se calcula la menor transmitancia térmica global alcanzable.

En los elementos optimizables se utiliza la alternativa de menor `U`.

En los elementos fijos se utiliza su valor `Ufijo`.

Luego se define:

```text
Umax = UminFactible + margen
```

donde:

```text
margen ∈ [0.05, 0.30]
```

De esta forma existe al menos una combinación capaz de satisfacer la restricción térmica.

---

## Archivos generados

El programa genera 15 archivos de instancia.

```text
instancia_pequena_1.txt
instancia_pequena_2.txt
instancia_pequena_3.txt
instancia_pequena_4.txt
instancia_pequena_5.txt

instancia_mediana_1.txt
instancia_mediana_2.txt
instancia_mediana_3.txt
instancia_mediana_4.txt
instancia_mediana_5.txt

instancia_grande_1.txt
instancia_grande_2.txt
instancia_grande_3.txt
instancia_grande_4.txt
instancia_grande_5.txt
```

Cada archivo contiene:

- Tipo de instancia.
- Semilla.
- Dimensiones.
- Superficie.
- Volumen.
- `Umax`.
- Catálogos por categoría.
- Información de cada elemento.
- Tipo de elemento.
- Área.
- Datos de elementos fijos.
- Catálogo utilizado por los elementos optimizables.
- Resumen de la instancia.

Los catálogos se muestran una sola vez por categoría para evitar repetir la misma información.

---

## Generación de las 15 instancias

El programa intenta generar 5 instancias válidas para cada tamaño.

El proceso utilizado es:

1. Se genera una instancia utilizando una semilla.
2. La instancia se valida.
3. Si es válida, se almacena.
4. Si no es válida, se descarta.
5. Se utiliza la siguiente semilla.
6. El proceso continúa hasta obtener 5 instancias válidas de cada tamaño.

Al finalizar se obtienen:

```text
5 pequeñas
5 medianas
5 grandes
```

Total:

```text
15 instancias válidas
```

---

## Cómo ejecutar

### 1. Preparar los archivos

Descargue todos los archivos del proyecto y colóquelos en la misma carpeta.

Abra una terminal dentro de esa carpeta.

### 2. Compilar

Ejecute:

```bash
make
```

Esto genera un ejecutable llamado:

```text
gen
```

### 3. Ejecutar el generador

Ejecute:

```bash
./gen
```

El programa generará automáticamente las 15 instancias válidas.

### 4. Eliminar el ejecutable

Para borrar el ejecutable:

```bash
make clear
```

---

## Generación de catálogo de alternativas

![Catálogo](imagenes/catalogo.png)

---

## Generación de elementos

![Elementos](imagenes/optifijos.png)
