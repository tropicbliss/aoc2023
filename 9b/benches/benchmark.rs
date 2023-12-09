use criterion::{criterion_group, criterion_main, Criterion};

pub fn part_b() -> i64 {
    let input = include_str!("../target/release/input.txt");
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
        for subsequence in subsequences.into_iter().rev().skip(1) {
            let first = unsafe { subsequence.first().unwrap_unchecked() };
            difference = first - difference;
        }
        total_sum += difference;
    }
    total_sum
}

fn criterion_benchmark(c: &mut Criterion) {
    c.bench_function("part b", |b| b.iter(|| part_b()));
}

criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);
