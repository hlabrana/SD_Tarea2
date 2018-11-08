#!/usr/bin/env python
# -*- coding: utf-8 -*-

import grpc
import metadatos_pb2
import metadatos_pb2_grpc
from funciones import *
from cliente import *
import time

# Instanciar Avion
datos = IniciarAvion()

# Crear Canal Comunicación con la Torre Actual
stub = Conectar_Torre(datos)

# Enviar Datos a Torre Inicial ; Imprime el mensaje de respueta.
datos_a_torre(datos,stub)

while True:

	# Despegue
	datos = Despegar(datos,stub)
	time.sleep(10) #Tiempo de Viaje
	# Aterrizaje
	stub = Aterrizar(datos)