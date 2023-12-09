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
    let mut total_sum = 0;
    for sequence in sequences {
        let mut subsequences = vec![sequence.clone()];
        loop {
            let difference: Vec<_> = unsafe {
                subsequences
                    .last()
                    .unwrap_unchecked()
                    .windows(2)
                    .map(|window| {
                        let a = window[0];
                        let b = window[1];
                        b - a
                    })
                    .collect()
            };
            let is_all_zero = difference.iter().all(|num| *num == 0);
            subsequences.push(difference);
            if is_all_zero {
                break;
            }
        }
        let mut difference = 0;
        for subsequence in subsequences.iter().rev().skip(1) {
            let last = unsafe { subsequence.last().unwrap_unchecked() };
            difference += last;
        }
        total_sum += difference;
    }
    println!("{total_sum}");
}
