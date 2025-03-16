# Parlante Bluetooth y Radio FM con ESP32

Este proyecto consiste en un parlante que puede funcionar en dos modos: **Bluetooth** y **Radio FM**. El sistema está controlado por un **ESP32** y utiliza un módulo de radio **TEA5767** para la funcionalidad de radio FM. El parlante permite alternar entre los modos Bluetooth y Radio FM mediante un botón, y la frecuencia de la radio se ajusta mediante un encoder rotatorio. La información de la frecuencia actual se muestra en una pantalla LCD.

## Características

- **Modo Bluetooth**: Conecta tu dispositivo móvil para reproducir música o cualquier otro audio.
- **Modo Radio FM**: Sintoniza estaciones de radio FM en un rango de 87.5 MHz a 108.0 MHz.
- **Interfaz de usuario**: 
  - **Encoder rotatorio**: Ajusta la frecuencia de la radio FM.
  - **Botón**: Alterna entre los modos Bluetooth y Radio FM.
  - **Pantalla LCD**: Muestra la frecuencia actual de la radio FM.

## Componentes utilizados

- **ESP32**: Microcontrolador principal.
- **Módulo de radio TEA5767**: Para la recepción de radio FM.
- **Pantalla LCD I2C**: Para mostrar la frecuencia actual.
- **Encoder rotatorio**: Para ajustar la frecuencia de la radio.
- **Botón**: Para cambiar entre modos Bluetooth y Radio FM.
- **Módulo Bluetooth**: Para la conexión con dispositivos móviles.
- **Amplificador de audio**: Para amplificar la señal de audio hacia el parlante.

## Esquema de conexión

A continuación se describe la conexión de los componentes principales:

- **ESP32**:
  - `CLK` (Encoder) -> `GPIO5`
  - `DT` (Encoder) -> `GPIO18`
  - `SW` (Encoder) -> `GPIO19`
  - `KEY_Bluetooth` -> `GPIO27`
  - `KEY_Radio` -> `GPIO14`
  - `SDA` (I2C) -> `GPIO21`
  - `SCL` (I2C) -> `GPIO22`

- **TEA5767**:
  - Conectado al bus I2C (SDA y SCL).

- **LCD I2C**:
  - Conectado al bus I2C (SDA y SCL).

- **Encoder rotatorio**:
  - `CLK` -> `GPIO5`
  - `DT` -> `GPIO18`
  - `SW` -> `GPIO19`

- **Módulo Bluetooth**:
  - `KEY_Bluetooth` -> `GPIO27`

- **Módulo de Radio FM**:
  - `KEY_Radio` -> `GPIO14`

## Instalación y configuración

1. **Clonar el repositorio**:
   ```bash
   git clone https://github.com/tuusuario/turepositorio.git
   cd turepositorio
