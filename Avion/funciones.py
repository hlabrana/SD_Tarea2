#!/usr/bin/env python
# -*- coding: utf-8 -*-
import grpc
import metadatos_pb2
import metadatos_pb2_grpc
from cliente import *
import time

#IMPLEMENTACIÓN DE FUNCIONES RELATIVAS A LAS REGLAS DEL NEGOCIO

# Obtiene datos de Instanciación de un AVIÓN
def IniciarAvion():
    print("\nBienvenido al vuelo\n")
    aerolinea = raw_input("[Avion] Ingrese Nombre de la Aerolínea: ")
    codigo = raw_input("[Avion] Ingrese Código de Vuelo: ")
    print("\n* Parámetros de Configuración *\n")
    pesoMax = raw_input("[Avion - "+codigo+"]: Peso Máximo de carga [Kg]: ")
    CapTanque = raw_input("[Avion - "+codigo+"]: Capacidad del tanque de combustible [L]: ")
    ipTorreInicial = raw_input("[Avion - "+codigo+"]: Torre de Control inicial: ")
    datos = {'aerolinea':aerolinea,
            'codigo':codigo,
            'pesoMax':pesoMax,
            'CapTanque':CapTanque,
            'ipTorreInicial':ipTorreInicial,
            'iporigen':None}
    return datos

def Despegar(datos,stub):
    raw_input("[Avion - "+datos['codigo']+"]: Presione enter para despegar")
    destino = raw_input("[Avion - "+datos['codigo']+"]: Ingrese destino: ")
    status = comprobar_destino(destino,datos,stub)
    # status = False -> TORRE NO TIENE REGISTRADO DESTINO
    # Status = True -> DESPEGUE AUTORIZADO
    if (status.estado == False):
        print("[Avion - "+datos['codigo']+"]: ERROR: Destino desconocido por la Torre\n")
        Despegar(datos,stub)

    elif(status.estado == True):
        print("[Avion - "+datos['codigo']+"]: Pasando por el Gate...\n")
        print("[Avion - "+datos['codigo']+"]: Todos los pasajeros a bordo y combustible cargado.\n")
        print("[Avion - "+datos['codigo']+"]: Pidiendo instrucciones para despegar...\n")
        statusPermiso = permiso_despegar(destino,datos,stub)
        # statusPermiso = 0 -> NO HAY PISTAS DISPONIBLES
        # statusPermiso = 1 -> PERMISO EXITOSO
        while (statusPermiso.estado == False):
            print("[Avion - "+datos['codigo']+"]: Todas las pistas estan ocupadas, el avion predecesor es "+statusPermiso.mensaje+"\n")
            time.sleep(1)
            statusPermiso = consultar_confirmacion(datos,stub,statusPermiso.pista) #RESPUESTA DE PISTA Y ALTURA
        

        if (statusPermiso.estado == True):
            print("[Avion - "+datos['codigo']+"]: Pista "+str(statusPermiso.pista)+" asignada y altura de "+str(statusPermiso.altura)+" km.\n")
            print("[Avion - "+datos['codigo']+"]: Despegando...\n")
            time.sleep(5)
            avisar_despegue(stub,statusPermiso.pista,datos,destino)
            datos['iporigen']=statusPermiso.mensaje #GUARDA IP DE ORIGEN PARA EL MOMENTO DE ATERRIZAJE
            datos['ipTorreInicial']=statusPermiso.ipdestino #Para conectar a la nueva torre, generar un stub
            return datos


def Aterrizar(datos):
    stub = Conectar_Torre(datos) #Generar stub de conexion con la nueva torre
    print("[Avion - "+datos['codigo']+"]: Esperando pista aterrizaje...")
    statusPermiso = contactar_torre(datos,stub) #Enviar datos a torre
    while (statusPermiso.estado == False):
            print("[Avion - "+datos['codigo']+"]: Todas las pistas estan ocupadas, el avion predecesor es "+statusPermiso.mensaje+"\n")
            print("[Avion - "+datos['codigo']+"]: Altura de espera: "+str(statusPermiso.altura*1000)+" [km]\n")
            time.sleep(1)
            statusPermiso = consultarAterrizaje(datos,stub,statusPermiso.pista) #RESPUESTA DE PISTA Y ALTURA
    print("[Avion - "+datos['codigo']+"]: Aterrizando en la pista "+str(statusPermiso.pista)+"...")
    time.sleep(5)
    AvisarAterrizaje(datos,stub,statusPermiso.pista) #Avisa a torre que aterrizo con exito
    return stub
    