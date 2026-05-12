use std::io::Read;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let x: usize = input.split_whitespace().next().unwrap_or("0").parse().unwrap_or(0);
    
    let size = std::cmp::max(100, x * 10 * 1024 * 1024);
    let mut v = vec![0u8; size];
    let mut i = 0;
    while i < size {
        v[i] = (i % 256) as u8;
        i += 100;
    }
    println!("{}", v[size / 2]);
}