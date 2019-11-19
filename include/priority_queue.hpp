#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <float.h>
#include <stdio.h>
#include <utility>

#include <cuda.h>
#include "device_launch_parameters.h"
#include <cuda_runtime.h>

template<typename T>
__host__ __device__
void swap_array_els(T* array, int i, int j) {
    T tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

template <typename T, typename Obj, int QueueSize = 128> class PriorityQueue {
    public:
        __host__ __device__
        PriorityQueue() : heap_size(0) {}

        __host__ __device__
        int get_size() {
            return heap_size;
        }

        __host__ __device__
        int parent(int i) {
            return (i - 1) / 2;
        }

        __host__ __device__
        int left_child(int i) {
            return 2 * i + 1;
        }

        __host__ __device__
        int right_child(int i) {
            return 2 * i + 2;
        }

        // ~PriorityQueue() {
            // delete heap;
        // }

        __host__ __device__
        std::pair<T, Obj> get_min() {
            if (heap_size > 0) {
                return std::pair<T, Obj>(priority_heap[0], obj_heap[0]);
            }
            else {
                return std::pair<T, Obj>(
                        std::is_same<T, float>::value ? FLT_MAX : DBL_MAX, nullptr);
            }
        }

        __host__ __device__
        void min_heapify(int index) {
            int left = left_child(index);
            int right = right_child(index);
            int smallest = index;

            if (left < heap_size && priority_heap[left] < priority_heap[index])
                smallest = left;
            if (right < heap_size && priority_heap[right] < priority_heap[index])
                smallest = right;
            if (smallest != index) {
                swap_array_els(priority_heap, index, smallest);
                swap_array_els(obj_heap, index, smallest);
                min_heapify(smallest);
            }
        }

        __host__ __device__
        void insert_key(T key, Obj obj) {
            if (heap_size == QueueSize) {
                printf("The queue has exceed its maximum size\n");
                return;
            }
            heap_size++;
            int ii = heap_size - 1;
            priority_heap[ii] = key;
            obj_heap[ii] = obj;

            // Fix the min heap property if it is violated
            min_heapify(0);
            // while (ii != 0 && heap[parent(ii)] > heap[ii])
            // {
                // swap_array_els(heap, ii, parent(ii));
                // ii = parent(ii);
            // }
        }

        // void print() {
            // for (int i = 0; i < heap_size; i++) {
                // std::cout << i << ": " << heap[i] << std::endl;
            // }
        // }

        __host__ __device__
        std::pair<T, Obj> extract() {
            if (heap_size <= 0)
                return std::pair<T, Obj>(
                        std::is_same<T, float>::value ? FLT_MAX : DBL_MAX, nullptr);

            T root_prio = priority_heap[0];
            Obj root_obj = obj_heap[0];
            // Replace the root with the last element
            priority_heap[0] = priority_heap[heap_size - 1];
            obj_heap[0] = obj_heap[heap_size - 1];
            // Decrease the size of the heap
            heap_size--;

            min_heapify(0);
            return std::pair<T, Obj>(root_prio, root_obj);
        }

    private:
        T priority_heap[QueueSize];
        Obj obj_heap[QueueSize];
        int heap_size;
};

#endif // #ifndef PRIORITY_QUEUE_H
