
use std::env;
use std::fs;
use std::process;
use std::error::Error;

fn main() {
    // call `collect()` on an iterator to turn it into a collection
    let args: Vec<String> = env::args().collect();

    // Result<T, E>, it allows us to define custom, non-panic! error handling
    // unwrap_or_else: if the `Result` is an `Ok` value, it's similar to `unwrap`,
    // if `Err` value, this method calls the code in the closure
    let config = Config::new(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {}", err);
        process::exit(1);
    });

    println!("Searching for {}", config.query);
    println!("In file {}", config.filename);

    // `run` returns `()` in the success case, and we only care about error
    // so we don't need `unwrap_or_else` to return the unwrapped value
    if let Err(e) = run(config) {
        println!("Application error: {}", e);
        process::exit(1);
    }

}

fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    let mut results = Vec::new();

    for line in contents.lines() {
        if line.contains(query) {
            results.push(line);
        }
    }

    results
}

// The trait object Box<dyn Error> means the function will return a type that
// implements the `Error` trait, but not specify what particular type it will be 
fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.filename)?;

    for line in search(&config.query, &contents) {
        println!("{}", line);
    }

    // wrap the unit type value in the `Ok` value, doesn't return a value
    Ok(())
}

struct Config {
    query: String,
    filename: String,
}

impl Config {
    fn new(args: &[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("not enough arguments");  //static string
        }
        let query = args[1].clone();
        let filename = args[2].clone();
        Ok(Config { query, filename })
    }
}
