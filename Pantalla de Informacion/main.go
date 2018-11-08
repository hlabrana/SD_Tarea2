package main

import "fmt"

func main() {
	var opcion int
	var listaTorres []Torre
	for true {
		opcion = IngresarOpcion()
		if opcion == 1 {
			var torreEscogida Torre
			if len(listaTorres) == 0 {
				fmt.Println("\nNo hay torres agregadas")
			} else{
				torreEscogida = SeleccionarTorre(listaTorres)
				ConsultarTorre(torreEscogida)
			}
		} else{
			listaTorres = append(listaTorres,AgregarTorre())
		}
	}
}