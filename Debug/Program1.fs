open System
[<EntryPoint>]

let main argv = 
    let line = Console.ReadLine()
    let mutable i = 0.0
    match len with
    | 1 -> x <- 10
    | 2 -> x <- 20
    | 3 -> x <- 30
    //| _ -> x <- 40
    | _ -> x <- 40
    printfn "%d" x
    if x = 10 then
        printfn("X will be ten")
    elif x = 20 then
        printfn("X will be twenty")
    elif x = 30 then
        printfn("X will be thirty")
    else
        printfn("X will be fourty")
		
	(*
	0if x = 10
		1printfn("X will be ten")
	0else 
		1if x = 20
			2printfn("X will be twenty")
		1else
			2if x = 30
				3printfn("X will be thirty")
			2else
				3printfn("X will be fourty")
			
	
	
	*)
    System.Console.ReadKey(true) |> ignore
    0