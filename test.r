fn main() {
    let name := "R Language"
    let version := 0.1
    
    if version > 0 {
        println("Hello, #{name}!")
    }
    
    match version {
        0 => "zero",
        0.1 => "point one",
        _ => "other"
    }
    
    for i in 0..10 {
        println(i)
    }
    
    return 0
}
