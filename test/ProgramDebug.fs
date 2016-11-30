open System
[<EntryPoint>]

let func y = 
        let mutable super = 0
        super <- super + y
    super

let main argv = 
    let line = Console.ReadLine()
    let mutable i = 0.0
    let mutable x = 0
    let mutable len = 0
    let mutable super = 0
    let notmut = 0
    //let VAR = 9
    len <- line.Length
    match len with
    | 1 -> x <- 10
    | 2 -> x <- 20
    | 3 -> x <- 30
    | _ -> x <- 40
    match super with
    | 1 -> super<- 4
    printfn "%d" x
    //if VAR = 9 then
    //    x <- 1
    if x = 10 then
        printfn("X will be ten")
    elif x = 20 then
        printfn("X will be twenty")
    elif x = 30 then
        printfn("X will be thirty")
    else
        printfn("X will be fourty")
    if x = 10 then
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
    if x = 20 then
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
    elif x = 30 then
        let mutable A = 0
        let mutable B = 10
        let mutable rez = 0
        //let super = 3
        //super<-1000
        i <- 0.0
        while i < 10.0 do
            rez <- A * B
            printfn "%d" rez
            A <- A + 1
            B <- B + 1
            i <- i + 1.0
    elif x = 40 then
        if x <> 50 then
            if x = 40 then
                printfn("GoodWork")
    //printf "%d" x
    System.Console.ReadKey(true) |> ignore
    0
