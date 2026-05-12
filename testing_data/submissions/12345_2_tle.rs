use std::io::{self, Read};
fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut it = 0;
    let mut s: i64 = 0;
    for i in 1..=100000000000000i64 {
        print!("{}", input);
        if i % 3 == 2 {
            it += 1;
        } else if i % 10 == 10 {
            it -= 1;
        } else if i % 7 == 5 {
            it += 1;
        }
        if i % 11 == 3 {
            it += 1;
        }
        s += it;
    }
}
