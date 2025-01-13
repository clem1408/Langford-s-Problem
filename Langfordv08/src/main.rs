use const_for::const_for;
use rayon::{iter::ParallelIterator, slice::ParallelSlice};
use std::{collections::HashSet, time::Instant, usize};
mod array_based;
use array_based::{Langford, ARRAY_SIZE};

const N: usize = 16;
const SIZE: usize = 2 * N;
const DEPTH: usize = 4;
const PRE_REPR: [u64; N + 1] = {
    let mut ret = [0; N + 1];
    const_for!(i in 0..(N+1)=>{
        ret[i] = (1u64 << (i + 1)) | (1u64)
    });
    ret
};
fn main() {
    let vec = get_global_work(DEPTH);
    let now = Instant::now();
    let res = vec
        .par_chunks(ARRAY_SIZE)
        .map(|s| {
            let mut lang = Langford::from_slice(s, DEPTH + 1);
            lang.explore()
        })
        .sum::<u32>();
    println!(
        "The newly compute sum: {} in {}ms",
        res,
        now.elapsed().as_millis()
    );
}
#[inline]
pub fn compute_round<const X: usize>(stack: &mut [(u64, usize)], state: u64) -> usize {
    if X == 0 {
        return 0;
    }
    let test_base: u64 = PRE_REPR[X];
    let mut size = 0;
    for i in 0..(SIZE - 1 - X) {
        let test = test_base << i;
        if test & state == 0 {
            //aucun 1 ne match --> dispo
            stack[size] = (state | test, X - 1);
            size += 1;
        }
    }
    size
}

fn explore(items: &mut Vec<u64>, to_explore: u64, depth: usize) {
    let n = N - DEPTH + depth;
    let to_add = PRE_REPR[n];
    for i in 0..({ SIZE - 1 } - n) {
        let temp = to_add << i;
        if to_explore & temp == 0 {
            if depth == 0 {
                items.push(temp | to_explore)
            } else {
                explore(items, to_explore | temp, depth - 1);
            }
        }
    }
}
#[inline]
fn get_global_work(depth: usize) -> Vec<u64> {
    let now = std::time::Instant::now();
    let mut vec = vec![];
    explore(&mut vec, 0u64, depth);
    let mut map: HashSet<u64> = HashSet::with_capacity(vec.len());
    vec.retain(|&a| {
        if !map.contains(&(a.reverse_bits() >> { 64 - SIZE })) {
            map.insert(a);
            true
        } else {
            false
        }
    });
    println!(
        "State to compute: {}, time to generate: {}ms",
        vec.len(),
        now.elapsed().as_millis()
    );
    vec
}
