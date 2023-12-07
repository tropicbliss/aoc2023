use std::{cmp::Ordering, collections::HashMap};

#[repr(u8)]
#[derive(Hash, PartialEq, Eq, Clone, Copy)]
enum Card {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Joker,
    Queen,
    King,
    Ace,
}

impl TryFrom<u8> for Card {
    type Error = &'static str;

    fn try_from(ch: u8) -> Result<Self, Self::Error> {
        match ch {
            b'2' => Ok(Self::Two),
            b'3' => Ok(Self::Three),
            b'4' => Ok(Self::Four),
            b'5' => Ok(Self::Five),
            b'6' => Ok(Self::Six),
            b'7' => Ok(Self::Seven),
            b'8' => Ok(Self::Eight),
            b'9' => Ok(Self::Nine),
            b'T' => Ok(Self::Ten),
            b'J' => Ok(Self::Joker),
            b'Q' => Ok(Self::Queen),
            b'K' => Ok(Self::King),
            b'A' => Ok(Self::Ace),
            _ => Err("Invalid character"),
        }
    }
}

#[repr(u8)]
#[derive(PartialEq)]
enum HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
}

#[derive(PartialEq, Eq, Clone)]
struct Hand {
    cards: [Card; 5],
}

impl Hand {
    fn new(cards: [Card; 5]) -> Self {
        Self { cards }
    }

    fn get_hand_type(&self) -> HandType {
        let mut counter: HashMap<Card, usize> = HashMap::with_capacity(5);
        for card in &self.cards {
            *counter.entry(*card).or_insert(0) += 1;
        }
        let values: Vec<usize> = counter.into_values().collect();
        if values.contains(&5) {
            return HandType::FiveOfAKind;
        }
        if values.contains(&4) {
            return HandType::FourOfAKind;
        }
        if values.contains(&3) && values.contains(&2) {
            return HandType::FullHouse;
        }
        if values.contains(&3) {
            return HandType::ThreeOfAKind;
        }
        if values.iter().filter(|v| **v == 2).count() == 2 {
            return HandType::TwoPair;
        }
        if values.contains(&2) {
            return HandType::OnePair;
        }
        HandType::HighCard
    }

    fn compare_card_strength(&self, other: &Self) -> Ordering {
        for (this, other) in self.cards.iter().zip(other.cards.iter()) {
            let this_strength = *this as u8;
            let other_strength = *other as u8;
            if this_strength > other_strength {
                return Ordering::Greater;
            }
            if this_strength < other_strength {
                return Ordering::Less;
            }
        }
        Ordering::Equal
    }
}

impl PartialOrd for Hand {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Hand {
    fn cmp(&self, other: &Self) -> Ordering {
        let this_hand_type = self.get_hand_type() as u8;
        let other_hand_type = other.get_hand_type() as u8;
        if this_hand_type > other_hand_type {
            return Ordering::Greater;
        }
        if this_hand_type < other_hand_type {
            return Ordering::Less;
        }
        self.compare_card_strength(other)
    }
}

struct Bid {
    hand: Hand,
    bid_amount: u32,
}

impl Bid {
    fn new(hand: Hand, bid_amount: u32) -> Self {
        Self { hand, bid_amount }
    }
}

fn main() {
    let file_contents = std::fs::read_to_string("input.txt").unwrap();
    let mut bids = Vec::new();
    for line in file_contents.lines() {
        if let Some((hand, bid)) = line.split_once(' ') {
            let bid_amount: u32 = bid.parse().unwrap();
            let mut cards = [Card::Ace; 5];
            for (idx, ch) in hand.bytes().enumerate() {
                let card: Card = ch.try_into().unwrap();
                cards[idx] = card;
            }
            let bid = Bid::new(Hand::new(cards), bid_amount);
            bids.push(bid);
        }
    }
    bids.sort_unstable_by_key(|bid| bid.hand.clone());
    let mut total_winnings = 0;
    for (idx, bid) in bids.into_iter().enumerate() {
        let multiplier = idx + 1;
        let winning = multiplier * bid.bid_amount as usize;
        total_winnings += winning;
    }
    println!("{total_winnings}");
}
