* INTEGRANTES:
	Hector Labraña - 201373535-5
	David Rojas - 201573502-6

* INSTRUCCIONES DE EJECUCION:

A1.- Avion [PYTHON]:
	1. Verificar que se encuentra instalado la version de python 2.7.X
	2. Verificar que se encuentra instalado gRPC para python incluyendo protobuf (GUIA: https://grpc.io/docs/quickstart/python.html)
	3. Compilar metadatos.proto: python -m grpc_tools.protoc -I./protos --python_out=. --grpc_python_out=. ./protos/metadatos.proto
	4. Ingresar a la ruta donde se encuentren los archivos de codigo fuente .py
	5. Compilar y ejecutar mediante el comando "make run" (SIN COMILLAS)
	6. Seguir las instrucciones del programa
	7. Para eliminar archivos temporales y de compilacion ejecutar "make clean" (SIN COMILLAS)

A2.- Observaciones:
	1. Si se desea ejecutar en un entorno con multiples CLI, se debe ingresar la IP y el puerto de la siguiente forma (IP:PUERTO -> 10.10.10.10:1234)
	2. Se asume un correcto uso por parte del usuario.

B1.- Pantalla de Informacion [GO]:
	1. Verificar que se encuentra instalado la version de GO 1.6.X
	2. Verificar que se encuentra instalado gRPC para GO incluyendo protobuf (GUIA: https://grpc.io/docs/quickstart/go.html)
	3. Compilar metadatos.proto: protoc -I protos/ protos/metadatos.proto --go_out=plugins=grpc:protos
	4. Verificar que la carpeta ./protos que contiene el archivo metadatos.proto se copie en el directorio de package de go-grpc (go/src/)
	5. Ingresar a la ruta donde se encuentren los archivos de codigo fuente .go
	7. Compilar y ejecutar mediante el comando "make run" (SIN COMILLAS)
	7. Seguir las instrucciones del programa
	8. Para eliminar archivos temporales y de compilacion ejecutar "make clean" (SIN COMILLAS)

B2.- Observaciones:
	1. Si se desea ejecutar en un entorno con multiples CLI, se debe ingresar la IP y el puerto de la siguiente forma (IP:PUERTO -> 10.10.10.10:1234)
	2. Se asume un correcto uso por parte del usuario.

C.- Torre de Control [C++]:
	1. Verificar que se encuentra instalado la version de C++ (g++)
	2. Verificar que se encuentra instalado gRPC para C++ incluyendo protobuf (GUIA: https://grpc.io/docs/quickstart/cpp.html)
	3. Ingresar a la ruta donde se encuentren los archivos de codigo fuente .cpp
	4. Compilar y ejecutar mediante el comando "make" (SIN COMILLAS)
	5. Seguir las instrucciones del programa
	6. Para eliminar archivos temporales y de compilacion ejecutar "make clean" (SIN COMILLAS)

C2.- Observaciones:
	1. Si se desea ejecutar en un entorno con multiples CLI, se debe ingresar la IP y el puerto de la siguiente forma (IP:PUERTO -> 10.10.10.10:1234)
	2. Se asume un correcto uso por parte del usuario.
