pub const ARRAY_SIZE: usize = 2000;
use crate::{DEPTH, N, SIZE};
use std::slice::{from_raw_parts, from_raw_parts_mut};
use unroll::unroll_for_loops;

use const_for::const_for;

const SLICE_SIZE: usize = 128;
const SLICE_SIZE1: usize = SLICE_SIZE - 1;

/*pub struct Langford {
    arrays: [[u64; ARRAY_SIZE]; N],
    arrays_sizes: [usize; N],
}*/

pub struct Langford {
    arrays: Vec<Vec<u64>>,
    arrays_sizes: [usize; N],
}

impl Langford {
    fn new() -> Self {
        Self {
            arrays: vec![vec![0u64; ARRAY_SIZE]; N],
            arrays_sizes: [0; N],
        }
    }
    pub fn from_slice(states: &[u64], index: usize) -> Self {
        assert!(ARRAY_SIZE >= states.len());
        let mut ret = Self::new();
        let len = states.len();
        for i in 0..len {
            ret.arrays[index][i] = states[i];
        }
        ret.arrays_sizes[index] = len;
        ret
    }
    #[unroll_for_loops]
    pub fn explore(&mut self) -> u32 {
        let mut sum = 0;
        loop {
            /*println!(
                "index to compute: {} len:{}",
                N - index,
                self.arrays_sizes[index]
            );
            sleep(Duration::from_millis(500));*/
            let index = self.get_new_index();
            if index == usize::MAX {
                return sum;
            }
            let arr_size = self.arrays_sizes[index];
            if arr_size < SLICE_SIZE {
                if index == { N - 1 } {
                    let mut temp: u32 = 0;
                    for i in 0..arr_size {
                        let state = self.arrays[index][i];
                        temp += ((state & (0b101 << state.trailing_ones())) == 0) as u32;
                    }
                    sum += temp;
                    self.arrays_sizes[index] = 0;
                    continue;
                }
                let (read, mut write) = self.split_in_two_sized(index);
                let mut total_added = 0;
                for i in 0..arr_size {
                    let added = FUNCTIONS[N - index](write, read[i]);
                    total_added += added;
                    write = &mut write[added..];
                }
                self.arrays_sizes[index] = 0;
                self.arrays_sizes[index + 1] += total_added;
                continue;
            }
            if index != { N - 1 } {
                let (read, mut write) = self.split_in_two(index);
                let mut total_added = 0;
                for i in 0..SLICE_SIZE {
                    let added = FUNCTIONS[N - index](write, read[i]);
                    total_added += added;
                    write = &mut write[added..];
                }
                self.arrays_sizes[index] -= SLICE_SIZE;
                self.arrays_sizes[index + 1] += total_added;
                continue;
            }
            let read = &self.arrays[index][(self.arrays_sizes[index] - SLICE_SIZE)..];
            let mut temp = 0;
            for i in 0..SLICE_SIZE {
                temp += ((read[i] & (0b101 << read[i].trailing_ones())) == 0) as u32;
            }
            self.arrays_sizes[index] -= SLICE_SIZE;
            sum += temp;
            //println!("State of sizes:{:?}", &self.arrays_sizes);
        }
    }
    fn split_in_two<'a>(&self, index: usize) -> (&[u64], &mut [u64]) {
        let idx = self.arrays_sizes[index] - SLICE_SIZE;
        let ptr = self.arrays[index].as_ptr();
        let idy = self.arrays_sizes[index + 1];
        let write_ptr = self.arrays[index + 1].as_ptr() as *mut u64;
        let write_slice = unsafe { from_raw_parts_mut(write_ptr.add(idy), ARRAY_SIZE - idy) };
        let read_slice = unsafe { from_raw_parts(ptr.add(idx), SLICE_SIZE) };
        (read_slice, write_slice)
    }
    fn split_in_two_sized<'a>(&mut self, index: usize) -> (&[u64], &mut [u64]) {
        let idx = self.arrays_sizes[index];
        let ptr = self.arrays[index].as_ptr();
        let idy = self.arrays_sizes[index + 1];
        let write_ptr = self.arrays[index + 1].as_mut_ptr();
        let write_slice = unsafe { from_raw_parts_mut(write_ptr.add(idy), ARRAY_SIZE - idy) };
        let read_slice = unsafe { from_raw_parts(ptr, idx) };
        (read_slice, write_slice)
    }
    #[unroll_for_loops]
    fn get_new_index(&self) -> usize {
        for i in (DEPTH..N).rev() {
            if self.arrays_sizes[i] > SLICE_SIZE1 {
                return i;
            }
        }
        for i in DEPTH..N {
            if self.arrays_sizes[i] > 0 {
                return i;
            }
        }
        return usize::MAX;
    }
}
pub fn compute_round<const X: usize>(stack: &mut [u64], state: u64) -> usize {
    let test_base: u64 = PRE_REPR[X];
    let mut size = 0;
    for i in 0..(SIZE - 1 - X) {
        let test = test_base << i;
        if test & state == 0 {
            //aucun 1 ne match --> dispo
            stack[size] = state | test;
            size += 1;
        }
    }
    size
}
const PRE_REPR: [u64; N + 1] = {
    let mut ret = [0; N + 1];
    const_for!(i in 0..(N+1)=>{
        ret[i] = (1u64 << (i + 1)) | (1u64)
    });
    ret
};
pub const FUNCTIONS: [for<'a> fn(&'a mut [u64], u64) -> usize; 18] = [
    compute_round::<0>,
    compute_round::<1>,
    compute_round::<2>,
    compute_round::<3>,
    compute_round::<4>,
    compute_round::<5>,
    compute_round::<6>,
    compute_round::<7>,
    compute_round::<8>,
    compute_round::<9>,
    compute_round::<10>,
    compute_round::<11>,
    compute_round::<12>,
    compute_round::<13>,
    compute_round::<14>,
    compute_round::<15>,
    compute_round::<16>,
    compute_round::<17>,
];
