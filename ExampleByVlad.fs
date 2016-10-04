[<EntryPoint>]
let main argv = 
    let mutable x = 10;
    if x > 5 then
        x <- x - 3
    printf "%d" x
    System.Console.ReadKey(true) |> ignore
    0 
