#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>

#include <grpcpp/grpcpp.h>

#include "metadatos.grpc.pb.h"

#define STATUS_PERMISO_DENEGADO false
#define STATUS_PERMISO_CONCEDIDO true
#define STATUS_DATOS_RECIBIDO true
#define DEPARTING_TRACK 0
#define ARRIVING_TRACK 1
#define TOWER_NOT_FOUND "0.0.0.0:0000"
#define PLANE_NOT_FOUND "000000"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using proto::dataAvion;
using proto::responseTorre;
using proto::dataDespegue;
using proto::responseDespegue;
using proto::dataAterrizaje;
using proto::responseAterrizaje;
using proto::AvisoDespegue;
using proto::responseMensaje;
using proto::mensajes;
using proto::responseDepartures;
using proto::responseArrivals;
using proto::ObjectDepartures;
using proto::ObjectArrivals;
using proto::API;


class airPlane {
  private: 
    std::string aerolinea;
    std::string planeCode;
    int maxPeso;
    int maxFuel;
    std::string IP;
    bool dummy;
  public:
    airPlane(std::string _aerolinea, std::string code, int peso, int _maxFuel, std::string prevIP)
    // Un avion que llega de un aeropuerto a otro con sus distintas caracteristicas, en espera de pista de aterrizaje y puerta de descarga
    {
      aerolinea = _aerolinea;
      planeCode = code; 
      maxPeso = peso;
      maxFuel = _maxFuel;
      IP = prevIP;
      dummy = false;
    }

    airPlane() // Dummy
    {
      dummy = true;
    } 

    bool isDummy()
    {
      return dummy;
    }

    std::string getCode()
    {
      if (!isDummy())
        return planeCode;
      else
        return std::string(PLANE_NOT_FOUND);
    }

    std::string getAerolinea()
    {
      if (!isDummy())
        return aerolinea;
      else
        return std::string(PLANE_NOT_FOUND);
    }


};

struct Torre
{
  std::string IP;
  std::string lugar;
  Torre(std::string _IP, std::string _lugar)
  {
    IP = _IP;
    lugar = _lugar;
  }
};

struct registroPantalla
{
  std::string codigo;
  std::string lugar;
  std::string pista;
  std::string hora;
  registroPantalla(std::string _codigo, std::string _lugar, std::string _pista, std::string _hora)
  {
    codigo = _codigo;
    lugar = _lugar;
    pista = _pista;
    hora = _hora;
  }
};


class TowerServiceImpl final : public API::Service {
  private: 
  std::string nombreTorre;
  std::string IPTorre;

    /* Almacenamiento de informacion de aviones:
      airSpace: Vector del espacio aereo para espera de aviones o salida de estos.
      landingTracks: Vector de colas en las que se almacenan las colas correspondientes a cada pista de aterrizaje.
      departingTracks: Vector de colas en las que se almacenan las colas correspondientes a cada pista de salida.
      airPlanes: vector que contiene la informacion de donde se encuentra cada avion en el aeropuerto (pista de aterrizaje, salida, gate o espacio aereo)
      historicoDespegue: Almacena los aviones que han despegado desde la torre, para la pantalla de informacion
      historicoAterrizaje: Almacena los aviones que han llegado a la torre, para la pantalla de informacion
  */
    std::vector<airPlane*> airSpaces;
    /* Se crean 20 "espacios aereos" que corresponden a la altura donde debe esperar, por ejemplo aquel
    avion en el puesto 5 debe esperar a 5000 pies de altura, el puesto 0 corresponde a las pistas de aterrizaje */
    std::vector< std::queue<airPlane*> > departingTracks;
    std::vector< std::queue<airPlane*> > arrivingTracks;
    std::vector< airPlane*> historico;
    std::vector <registroPantalla*> historicoDespegue;
    std::vector <registroPantalla*> historicoAterrizaje;


    std::vector<Torre> torres;
    /* Para almacenar datos las distintas torres de control */

    int nextDTrackAvaible()
    /* Busca la pista de salida con menor cantidad de aviones en espera */
    {
      int menor = 0;
      for (int i = 0; i < departingTracks.size(); i++ )
        if (departingTracks[i].size() < menor)
          menor = i;
      return menor;
    }

    int nextATrackAvaible()
    /* Busca la pista de aterrizaje con menor cantidad de aviones en espear */
    {
      int menor = 0;
      for (int i = 0; i < arrivingTracks.size(); i++ )
        if (arrivingTracks[i].size() < menor)
          menor = i;
      return menor;      
    }
    int findAirPlace()
    {
      /* Busca un espacio aereo disponible */
      for (int i = 0; i <= 19; i++)
        if (airSpaces[i]->isDummy())
          return i;
      return -1;
    }

    int findInAirPlace(std::string code)
    /* Busca el espacio aereo utilizado por un avion */
    {
      for (int i = 0; i <= 19; i++)
        if (!airSpaces[i]->getCode().compare(code)){
          return i;
        }
      return -1;
    }

    void freeAirPlace(std::string code)
    /* Libera el espacio aereo utilizado por un avion en despegue o aterrizaje*/
    {
      for (int i = 0; i <= 19; i++)
        if (!airSpaces[i]->getCode().compare(code)){
          std::cout << "Espacio aereo del avion: " << code << " liberado" << std::endl;
          airSpaces[i]= new airPlane();
        }
    }

    int findQueue(std::string code, int typeOfTrack)
    {
      /* Buscar la pista a la que fue asignado el avion */
      /* typeOfTrack = 0 -> landingTrack */
      /* typeOfTrack = 1 -> arrivingTrack */
      std::vector< std::queue<airPlane*> > temporal_vector;
      if (typeOfTrack==DEPARTING_TRACK)
        std::vector< std::queue<airPlane*> > temporal_vector (departingTracks);
      else
        std::vector< std::queue<airPlane*> > temporal_vector (arrivingTracks);

      for (int i = 0; i < temporal_vector.size(); i++){
        std::queue<airPlane*> temporal_queue (temporal_vector[i]);
        while (!temporal_queue.empty()){
          if (temporal_queue.front()->getCode().compare(code)){
            return i;
          }
          temporal_queue.pop();
        }
      }
      return -1;

    }

    int departQueuePosition(std::string code, int track)
    /* busca la posicion de un avion en una pista de salida */
    {

      std::queue<airPlane*> temporal (departingTracks[track]);
      std::cout << std::endl << "Aviones en cola de pista de salida: " << temporal.size() << std::endl; 

      int contador = 0;
      while (!temporal.empty()){
        if (temporal.front()->getCode().compare(code)){
          contador++;
          temporal.pop();
       }
        else
          std::cout << "Puestos de espera: " << contador << std::endl;
          return contador;
      }
      std::cout << "Avion no encontrado en las colas" << std::endl;
      return -1;
    }

    int arriveQueuePosition(std::string code, int track)
    /* Busca la posicion de un avion en una pista de llegada */
    {

      std::queue<airPlane*> temporal (arrivingTracks[track]);
      std::cout << "Aviones en cola de pista de llegada: " << temporal.size() << std::endl; 


      int contador = 0;
      std::cout << temporal.empty() << " - " << temporal.size() << std::endl;
      while (!temporal.empty()){
        if (!temporal.front()->getCode().compare(code)){
          contador++;
          temporal.pop();
          std::cout << "contador: " << contador << std::endl;
        }
        else
          std::cout << "En espera: " << contador << std::endl;
          return contador;
      }
      std::cout << "Avion no encontrado en las colas" << std::endl;
      return -1;
    }



    airPlane* prevDepartingPlane(std::string code, int track)
    /* Busca el avion anterior al avion solicitado en la cola de salida de la pista solicitada */
    {
      std::queue<airPlane*> temporal (departingTracks[track]);
      airPlane* prev = new airPlane();
      while (!temporal.empty()){
        if (!temporal.front()->getCode().compare(code)){
          temporal.pop();
        }
        else{
          prev = temporal.front();
          temporal.pop();
        }
      }
      return prev;
    }


    airPlane* prevArrivingPlane(std::string code, int track)
    /* Busca el avion anterior al avion solicitado en la cola de llegada de la pista solicitada */
    {
      std::queue<airPlane*> temporal (arrivingTracks[track]);
      airPlane* prev = new airPlane();
      while (!temporal.empty()){
        if (!temporal.front()->getCode().compare(code)){          
          return prev;
        }
        else{
          prev = temporal.front();
          temporal.pop();
        }
      }
      return prev;
    }


    std::string findIP(std::string _lugar)
    /* Busca la ip asociada a una torre de control */
    {
      for (int i = 0; i < torres.size(); i++)
        if (!torres[i].lugar.compare(_lugar))
          return torres[i].IP;
      return TOWER_NOT_FOUND;
    }

    void addTower (std::string lugar, std::string IP)
    /* Agrega una torre de control al registro de torres a los que puede viajar un avion desde una torre */
    {
      torres.push_back(Torre(IP,lugar));
    }

    airPlane* buscarHistorico(std::string codigo)
    /* Busca los datos de un avion en todos los aviones que han iniciado vuelo o llegado a la torre de control */
    {
      for (int i = 0; i < historico.size(); i++){
        if (!historico[i]->getCode().compare(codigo))
          return historico[i];
      }
      return new airPlane();
    }

  public:
    void initialize(std::string nombre, std::string IP)
    /* Inicia datos utiles para la torre de control */
    {

      nombreTorre = nombre;
      IPTorre = IP;
      int lTracksNumber;
      int aTracksNumber;

      std::cout << "\nCantidad de pistas de aterrizaje: ";
      std::cin >> lTracksNumber;
      std::cout << "Cantidad de pistas de despegue: ";
      std::cin >> aTracksNumber;

      for (int i = 0; i < lTracksNumber; i++)
      {
        std::queue<airPlane*> temp;
        departingTracks.push_back(temp);
      }

      for (int i = 0; i < aTracksNumber; i++)
      {
        std::queue<airPlane*> temp;
        arrivingTracks.push_back(temp);
      }


      std::cout << "\nIngreso de otras torres de control" << std::endl << std::endl;
      bool c;
      do {
        std::string lugar;
        std::cout << "Ingrese ciudad de la torre: ";
        std::cin >> lugar;

        std::string IP;
        std::cout << "Ingrese direccion IP de la torre: ";
        std::cin >> IP;

        addTower(lugar, IP);

        char continuar;
        std::cout << "¿Desea agregar otra torre? S/N" << std::endl;
        std::cin >> continuar;
        c = (continuar=='S' || continuar == 's' || continuar == 'Y' || continuar == 'Y') ? true : false; 
      }
      while(c);

      std::cout << "Muchas gracias\n\n";

      airSpaces = std::vector<airPlane*> (20,new airPlane()); 

    }

    //RPC para agregar un avion, con sus respectivos datos a un torre de control
    Status DatosHaciatorre(ServerContext* context, const dataAvion* request, responseTorre* reply) 
    override {

      airPlane* avion = new airPlane(request->aerolinea(), request->codigo(), request->pesomax(), request->captanque(), request->iptorreinicial());
      historico.push_back(avion);
      std::cout << "Nuevo avión registrado: " << request->codigo() << std::endl;

      reply->set_estado(STATUS_DATOS_RECIBIDO);
      return Status::OK;
    }

    //RPC con el cual el avion solicita despegue, consultando la disponibilidad de las pistas
    Status SolicitarDespegue(ServerContext* context, const dataDespegue* request, responseDespegue* reply)
    override {
      std::string codigo = request->codigo();
      airPlane* avion = buscarHistorico(codigo);
      std::cout << "Despegue solicitado por: " << avion->getAerolinea() << "-" << codigo <<std::endl;

      if (!request->segundainstancia()){ //Primera vez que se pide despegar 
      /* segundaInstancia es false significa que es la primera peticion de despegue y en este caso
        se busca una pista para asignarsele al avion
      */
        int track = nextDTrackAvaible();
        std:: cout << "Pista asignada: " << track << std::endl;
        airPlane* avion = buscarHistorico(codigo);
        departingTracks[track].push(avion);
        int position = departQueuePosition(codigo, track);

        if (position==0){
          std::cout << "Despegue permitido para " << avion->getAerolinea() << "-" << codigo << std::endl;
          reply->set_estado(STATUS_PERMISO_CONCEDIDO);
          reply->set_ipdestino(findIP(request->destino()));
          reply->set_pista(track);
          int altura = findAirPlace();
          airSpaces[altura] = avion;
          reply->set_altura(altura);
          reply->set_mensaje(nombreTorre);
        }
        else{
          std::cout << "Despegue denegado para " << codigo << std::endl;
          std::cout << "Quedan " << position << " aviones antes que el" << std::endl;
          reply->set_estado(STATUS_PERMISO_DENEGADO);
          airPlane* prev = prevDepartingPlane(codigo,track);
          reply->set_mensaje(prev->getCode());
       }

      }
      else{ //Segunda vez que se pide despegar
        int track = request->pista();
        int position = departQueuePosition(codigo, track);

        if (position==0){
          std::cout << "Despegue permitido para " << avion->getAerolinea() << "-" << codigo << std::endl;
          reply->set_estado(STATUS_PERMISO_CONCEDIDO);
          reply->set_ipdestino(findIP(request->destino()));
          reply->set_pista(track);
          int altura = findAirPlace();
          airSpaces[altura] = avion;
          reply->set_altura(altura);
          reply->set_mensaje(nombreTorre);
        }
        else{
          std::cout << "Despegue denegado para " << codigo << std::endl;
          std::cout << "Quedan " << position << " aviones antes que el" << std::endl;
          airPlane* prev = prevDepartingPlane(codigo,track);
          reply->set_mensaje(prev->getCode());

        }
      }


      std::cout << std::endl;
      return Status::OK;
      
    }

    //RPC con el que el avion avisa que despego y se liberan los recursos (pista y espacio aereo)
    Status AvisarDespegue(ServerContext* context, const proto::AvisoDespegue* data, proto::responseMensaje* reply)
    override {

      time_t currTime = time(NULL);
      tm *tm_local = localtime(&currTime);
      std::string hora (std::to_string(tm_local->tm_hour) + ":" + std::to_string(tm_local->tm_min));
      historicoDespegue.push_back(new registroPantalla(data->codigo(),data->lugar(), std::to_string(data->pista()),hora));
      departingTracks[data->pista()].pop();
      freeAirPlace(data->codigo());
      airPlane* avion = buscarHistorico(data->codigo());
      std::cout << "Avion " << avion->getAerolinea() << "-" << avion->getCode() << " ha despegado en la pista " << data->pista() << std::endl;
      std::cout << std::endl;
      return Status::OK;
    }


    //RPC con el cual el avion solicita aterrizaje, consultando la disponibilidad de las pistas
    Status SolicitarAterrizaje(ServerContext* context, const dataAterrizaje* dataAvion, responseAterrizaje* reply)
    override {
      if (!dataAvion->segundainstancia()){ //Primera vez que se solicita aterrizaje
        airPlane* avion = new airPlane(dataAvion->aerolinea(), dataAvion->codigo(), dataAvion->pesomax(), dataAvion->captanque(), dataAvion->iptorreactual());      
        historico.push_back(avion);
        std::cout << "Nuevo avión registrado: " << avion->getAerolinea() << "-" << avion->getCode() << std::endl;

        int track = nextATrackAvaible();
        arrivingTracks[track].push(avion);
        reply->set_pista(track);
        std::cout << "Avion: " << avion->getAerolinea() << "-" << avion->getCode() << " registrado en espera de aterrizaje" << std::endl << "Pista de aterrizaje asignada: " << track << std::endl;
        if (arrivingTracks[track].size() == 1){
          reply->set_estado(STATUS_PERMISO_CONCEDIDO);
          std::cout << "Permiso de aterrizaje concedido para " << avion->getAerolinea() << "-" << avion->getCode() << std::endl;
          std::cout << "En la pista " << track << std::endl;
        }
        else{
          std::cout << "La pista asignada al avion " << avion->getAerolinea() << "-" << avion->getCode() << " se encuentra ocupada" << std::endl;

          reply->set_estado(STATUS_PERMISO_DENEGADO);
          int airSpace = findAirPlace();
          std::cout << "Debe esperar a una altura " << airSpace*1000 << " [Km]" << std::endl;
          airSpaces[airSpace] = avion;

          reply->set_altura(airSpace);
          airPlane* prev = prevDepartingPlane(avion->getCode(),track);
          reply->set_mensaje(prev->getCode());

        }
      }
      else { //Segunda vez que se solicita aterrizaje

        airPlane* avion = buscarHistorico(dataAvion->codigo());
        std::cout << "Avion " << avion->getAerolinea() << "-" << avion->getCode() << " solicita aterrizaje" << std::endl;
        int position = arriveQueuePosition(dataAvion->codigo(), dataAvion->pista());
        if (arrivingTracks[dataAvion->pista()].size() == 1){
          reply->set_estado(STATUS_PERMISO_CONCEDIDO);
          std::cout << "Permiso de aterrizaje concedido para " << avion->getAerolinea() << "-" << avion->getCode() << std::endl;
          std::cout << "En la pista " << dataAvion->pista() << std::endl;          
        }
        else{
          reply->set_estado(STATUS_PERMISO_DENEGADO);
          int airSpace = findInAirPlace(avion->getCode());
          reply->set_altura(airSpace);

          airPlane* prev = prevDepartingPlane(avion->getCode(),dataAvion->pista());
          reply->set_mensaje(prev->getCode());

          std::cout << "La pista asignada al avion " << avion->getAerolinea() << "-" << avion->getCode() << " se encuentra ocupada" << std::endl;
          std::cout << "El avion debe esperar a una altura " << airSpace*1000 << " [Km]" << std::endl;
        }

      }
      std::cout << std::endl;
      return Status::OK;
    }

    //RPC con el que el avion avisa que aterrizó y se liberan los recursos (pista y espacio aereo)
    Status AvisarAterrizaje(ServerContext* context, const proto::AvisoDespegue* data, proto::responseMensaje* reply)
    override {

      time_t currTime = time(NULL);
      tm *tm_local = localtime(&currTime);
      std::string hora (std::to_string(tm_local->tm_hour) + ":" + std::to_string(tm_local->tm_min));
      airPlane* avion = buscarHistorico(data->codigo());
      historicoAterrizaje.push_back(new registroPantalla(data->codigo(),data->lugar(), std::to_string(data->pista()),hora));
      arrivingTracks[data->pista()].pop();
      freeAirPlace(data->codigo());
      std::cout << "Avion " << avion->getAerolinea() << "-" << avion->getCode() << " ha aterrizado en la pista " << data->pista() << std::endl;
      std::cout << std::endl;

      return Status::OK;
    }    

    //RPC con el que se comprueba que existe el destino al que quiere ir el avion
    Status ComprobarDestino(ServerContext* context, const proto::dataDestino* data, proto::responseDestino* reply)
    override {
      std::string IP = findIP(data->destino());
      std::cout<< "Buscando torre: " << data->destino() << std::endl;
      if (!IP.compare(TOWER_NOT_FOUND)){
        std::cout<< "Torre no encontrada \n";
        reply->set_estado(false);
      }
      else{
        std::cout<< "Enviando dirección IP: " << IP << std::endl;
        reply->set_estado(true);
      }
      return Status::OK;
    }

    //RPC con el que la pantalla pide informacion de los vuelos que han salido de la torre
    Status Departures(ServerContext* context, const mensajes* peticion, responseDepartures* respuesta)
    override{
      for (const registroPantalla* registro: historicoDespegue){
        ObjectDepartures* reg = respuesta->add_departures();
        reg->set_codigoavion(registro->codigo);
        reg->set_destino(registro->lugar);
        reg->set_pistadespegue(registro->pista);
        reg->set_horadespegue(registro->hora);
      }
      return Status::OK;
    }

    //RPC con el que la pantalla pide informacion de los vuelos que han llega a la torre
    Status Arrivals(ServerContext* context, const mensajes* peticion, responseArrivals* respuesta)
    override{
      for (const registroPantalla* registro: historicoAterrizaje){
        ObjectArrivals* reg = respuesta->add_arrivals();
        reg->set_codigoavion(registro->codigo);
        reg->set_proveniente(registro->lugar);
        reg->set_pistallegada(registro->pista);
        reg->set_horallegada(registro->hora);
      }
      return Status::OK;
    }

};





void RunServer() {
  std::cout << "Bienvendo \nPor favor ingrese los datos iniciales" << std::endl << std::endl;
  std::string server_address;
  std::string server_name;
  std::cout << "Ingrese el nombre de la torre de control: ";
  std::cin >> server_name;
  std::cout << "Ingrese dirección IP de esta torre de control: ";
  std::cin >> server_address;

  TowerServiceImpl service;  // Si hay problemas, borré un parentesis de aca
  service.initialize(server_name,server_address);
  ServerBuilder builder;    // Y de aca
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  std::cout << "*******************************************" << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
