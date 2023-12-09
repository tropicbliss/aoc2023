pub fn part_b(sequences: Vec<Vec<i64>>) -> i64 {
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
            let first = unsafe { subsequence.first().unwrap_unchecked() };
            difference = first - difference;
        }
        total_sum += difference;
    }
    total_sum
}
