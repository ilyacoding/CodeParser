open System
[<EntryPoint>]

let main argv = 
    let line = Console.ReadLine()
    let mutable i = 0.0
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
    else
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
    else
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
    //printf "%d" x
    System.Console.ReadKey(true) |> ignore
    0 
