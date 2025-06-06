# 🚨 Sistema de Monitoramento de Emergência com IoT
O projeto consiste em um sistema de monitoramento de emergência que utiliza dispositivos IoT para enviar alertas com localização em tempo real, visualizados através de um dashboard centralizado.

## 👥 Integrantes
* Celso Canaveze Teixeira Pinto - RM556118

* Sofia Domingues Gonçalves - RM554920

* Thiago Moreno Matheus - RM554507

## 📱 Visão Geral - Sistema de Emergência via ESP32
* Cada dispositivo de emergência possui um identificador único (ID) e envia sua localização via IoT

* Um ESP32 simula o dispositivo de emergência com botão de alerta

* O sistema publica alertas no tópico MQTT emergency/device/alerts

* Um dashboard Node-RED visualiza os alertas em tempo real com mapa interativo

## 🧠 Lógica do Projeto
* O ESP32 simula um dispositivo de emergência com:

    * Botão para acionar alertas

    * Geração de coordenadas aleatórias no Brasil

    * Conexão WiFi e MQTT

* Quando o botão é pressionado:

    1. Gera um novo ID de dispositivo e localização

    2. Publica alerta no tópico MQTT com:

        * ID do dispositivo

        * Motivo do alerta

        * Coordenadas geográficas

        * Timestamp

    3. Fornece feedback visual com LED

## 🔘 Interação Local
* Botão físico no ESP32:

    * Ao ser pressionado, dispara um alerta de emergência

    * Gera novas coordenadas para simular movimento

    * Fornece feedback visual com LED piscando

## 🚨 Tipos de Alertas
* Todos os alertas são do tipo "Botão de emergência pressionado"

* O sistema pode ser expandido para diferentes tipos de emergência

## 🔌 Componentes Usados
* ESP32 (simulado no Wokwi)

* LED para feedback visual (pino 2)

* Botão físico para acionar alertas (pino 5)

* Conexão Wi-Fi

* Broker MQTT público: broker.hivemq.com

* Node-RED para dashboard de monitoramento

## 📡 Tópicos MQTT Utilizados
* emergency/device/alerts - Para publicação de alertas de emergência

* emergency/device/status - Para publicação do status dos dispositivos

* emergency/device/commands - Para receber comandos (não implementado no dispositivo)

## 📊 Dashboard Node-RED
O dashboard inclui:

* Lista de dispositivos ativos com:

    * ID do dispositivo

    * Status/motivo do alerta

    * Coordenadas geográficas

    * Timestamp da última atualização

* Mapa interativo com localização dos dispositivos

* Botão "Localizar" para centralizar o mapa no dispositivo selecionada

* Atualização em tempo real dos status e localizações

## 🛠️ Tecnologias Utilizadas
* Hardware (simulado):

    * ESP32

    * LED (pino 2)

    * Botão (pino 5)

* Software:

    * Wokwi Simulator para simulação do ESP32

    * Protocolo MQTT para comunicação

    * Node-RED para dashboard de monitoramento

    * Worldmap no Node-RED para visualização geográfica

## 📋 Exemplo de Saída Serial
    [INFO] Sistema iniciado - Dispositivo de Emergência  
    Wi-Fi conectado!  
    Endereço IP: 192.168.0.105  
    Novo dispositivo gerado: ID device-abc123-1  
    Coordenadas: -23.5505, -46.6333  
    [MQTT] Alerta publicado em emergency/device/alerts  
    [ALERTA] LED piscando 3 vezes  