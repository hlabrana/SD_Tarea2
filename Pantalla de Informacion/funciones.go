package main

import "fmt"

// Pide al usuario consultar por un aeropuerto o ingresar nuevo aeropuerto
func IngresarOpcion () (int){
	var opcion int
	fmt.Print("\nIngrese Opción:\n\n")
	fmt.Print("[1] Consultar Aeropuerto\n")
	fmt.Print("[2] Agregar Torre de Control\n")
	fmt.Print("\nOpción: ")
	fmt.Scan(&opcion)
	if opcion!=1 && opcion!=2 {
		fmt.Print("Opción Inválida\n")
		return 0
	} else {
		return opcion
	}
}

func AgregarTorre () (Torre){
	var torre Torre
	fmt.Print("\nIngrese Nombre Aeropuerto: ")
	fmt.Scan(&torre.NombreTorre)
	fmt.Print("Ingrese Dirección IP de la torre: ")
	fmt.Scan(&torre.ipTorre)
	fmt.Print("Ingresado Con Éxito\n")
	return torre
}

func SeleccionarTorre (listaTorres []Torre) (Torre){
	var opcion int
	fmt.Print("\nIngrese Torre de Control a Consultar:\n\n")
	for f:= 0; f< len(listaTorres); f++{
		fmt.Print("[",f,"] ",listaTorres[f].NombreTorre,"\n")
	}
	fmt.Print("\nOpción: ")
	fmt.Scan(&opcion)
	if opcion>=0 && opcion<=len(listaTorres){
		return listaTorres[opcion]
	} else {
		fmt.Print("\nOpción Inválida\n")
		return SeleccionarTorre(listaTorres)
	}
}

