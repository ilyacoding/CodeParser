// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
(*[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    let x = 5 
    if x<3 then
        printfn "ddd"
        printfn "ddda"
    match personName with
    | FirstOnly(firstName) -> printf "May I call you %s?" firstName
    | LastOnly(lastName) -> printf "Are you Mr. or Ms. %s?" lastName
    | FirstLast(firstName, lastName) -> printf "Are you %s %s?" firstName lastName
    if x > 3 then 
        printfn "good" 
        for i = 1 to 10 do
         printfn "GG"
        if x < 9 then 
            printfn "goooood"
    if x > 3 then 
        printfn "good" 
        if x < 5 then
            if x < 7 then
                if x < 5 then
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                    printfn "NOT"
                elif x > 6 then
                    printf "BLABLA"
                    for i = 1 to 4 do
                     if i < x then
                      printf "DAS IST GOOD"
                      if x > 190 then
                                 printfn "GGVLD"
                                 for i 1.100 do
                                     abs 10
        if x < 9 then 
            printfn "goooood"
            if x > 8 then
                printfn "ggg"
            if x < 9 then
                printfn "asdasd"
            if x > 10 then
                printfn "good"
            elif x < 7 then
                  printf "BLA"
            elif x < 7 then
                    printf "BLA"
            elif x < 7 then
                    printf "BLA"
            elif x < 7 then
                    printf "BLA"
    printfn "bad"
    System.Console.ReadKey(true) |> ignore 
    0 // return an integer exit code
	
[<EntryPoint>]
let main argv = 
    let mutable x = 10;
    if x > 5 then
        x <- x - 3
    printf "%d" x
    System.Console.ReadKey(true) |> ignore
    0 *)

open System
[<EntryPoint>]
let main argv = 
    let line = Console.ReadLine()
   (* let mutable i = 0.0
    let mutable x = 0
    let mutable len = 0
    len <- line.Length
    match len with
    | 1 -> x <- 10
    | 2 -> x <- 20
    | 3 -> x <- 30
    | _ -> x <- 40
    printfn "%d" x
    if x > 10 then
        printfn("bolshe desyati")
    else*)
        printfn("menshe")
    if x > 10 then
        i <- -2.0
        while i <= 2.0 do 
            let mutable rez = 0.0
            if (i <= 2.0) && (i > 1.0) then
                rez <- exp(i)
            if (i <= 1.0) && (i > 0.0) then
                rez <- log(i)
            if (i <= 0.0) && (i >= -2.0) then
                rez <- sin(i) + i * cos(i)
            printfn " %f %f" i rez
            i <- i + 0.2
  (* else
        i <- -1.0
        while i <= 3.0 do
            let mutable rez = 0.0
            if (i <= 3.0) && (i > 1.0) then
                rez <- exp(i) - 1.0
            if (i <= 1.0) && (i > 0.5) then
                rez <- sin(i) ** 2.0
            if (i <= 0.5) && (i >= -1.0) then
                rez <- 1.0
            printfn "%f %f" i rez
            i <- i + 0.25
    //printf "%d" x*)
    System.Console.ReadKey(true) |> ignore
    0 