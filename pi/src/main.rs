fn greeting() -> String {
    "Hello".to_string()
}

fn main() {
    println!("{}, world!", greeting());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        assert_eq!("Hello", greeting());
    }
}
