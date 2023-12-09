use neinbee::part_b;

fn main() {
    let input = std::fs::read_to_string("input.txt").unwrap();
    let mut sequences = Vec::new();
    for line in input.lines() {
        let sequence: Vec<_> = line
            .split_ascii_whitespace()
            .map(|num| num.parse::<i64>().unwrap())
            .collect();
        sequences.push(sequence);
    }
    let total_sum = part_b(sequences);
    println!("{total_sum}");
}
