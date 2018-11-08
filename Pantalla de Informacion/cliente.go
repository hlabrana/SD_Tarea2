package main

import (
	"log"
	"time"
	"fmt"
	"os"

	"golang.org/x/net/context"
	"google.golang.org/grpc"
	pb "protos"
)

func ConsultarTorre (torre Torre){
	// Set up a connection to the server.
	conn, err := grpc.Dial(torre.ipTorre, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer conn.Close()
	stub := pb.NewAPIClient(conn)

	// Contact the server and print out its response.
	mensaje1 := "departures"
	mensaje2 := "arrivals"
	if len(os.Args) > 1 {
		mensaje1 = os.Args[1]
		mensaje2 = os.Args[1]
	}
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()
	departures, err := stub.Departures(ctx, &pb.Mensajes{Mensaje: mensaje1})
	arrivals, err := stub.Arrivals(ctx, &pb.Mensajes{Mensaje: mensaje2})

	// Imprimir la información de los vuelos por pantalla.
	fmt.Print("\n")
	fmt.Println("[Pantalla de información - ",torre.NombreTorre,"]")
	fmt.Println("Departures")
	fmt.Println("Avion 		Destino 		Pista 		Hr")
	fmt.Println("------------------------------------------------------------")
	for f:=0; f<len(departures.GetDepartures());f++{
	fmt.Println(departures.GetDepartures()[f].CodigoAvion,"		",departures.GetDepartures()[f].Destino,"		",departures.GetDepartures()[f].PistaDespegue,"		",departures.GetDepartures()[f].HoraDespegue)
	}
	fmt.Print("\n")
	fmt.Println("Arrivals")
	fmt.Println("Avion 		Proveniente 		Pista 		Hr")
	fmt.Println("------------------------------------------------------------")
	for f:=0; f<len(arrivals.GetArrivals());f++{
	fmt.Println(arrivals.GetArrivals()[f].CodigoAvion,"		",arrivals.GetArrivals()[f].Proveniente,"		",arrivals.GetArrivals()[f].PistaLlegada,"		",arrivals.GetArrivals()[f].HoraLlegada)
	}

}