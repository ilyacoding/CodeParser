// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    let x = 5 
    if x<3 then
        printfn "ddd"
        printfn "ddda"
    if x > 3 then 
        printfn "good" 
        if x < 9 then 
            printfn "goooood"
    if x > 3 then 
        printfn "good" 
        if x < 9 then 
            printfn "goooood"
            if x > 8 then
                printfn "ggg"
            if x < 9 then
                printfn "asdasd"
    printfn "bad"
	
	
    System.Console.ReadKey(true) |> ignore 
    0 // return an integer exit code*)
