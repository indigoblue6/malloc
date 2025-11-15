#include "malloc.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

// グローバル変数: ヒープの先頭ポインタ
static BlockHeader* heap_start = nullptr;

// ヘッダーサイズを計算
constexpr size_t HEADER_SIZE = sizeof(BlockHeader);

// メモリを8バイト境界にアライン
size_t align_size(size_t size) {
    return (size + 7) & ~7;
}

// 適切な空きブロックを検索（First Fit戦略）
BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = heap_start;
    
    while (current != nullptr) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return nullptr;
}

// ブロックを分割（大きすぎる場合）
void split_block(BlockHeader* block, size_t size) {
    // 分割できるだけの余裕があるか確認
    if (block->size >= size + HEADER_SIZE + 8) {
        BlockHeader* new_block = (BlockHeader*)((char*)block + HEADER_SIZE + size);
        new_block->size = block->size - size - HEADER_SIZE;
        new_block->is_free = true;
        new_block->next = block->next;
        
        block->size = size;
        block->next = new_block;
    }
}

// 新しいブロックをヒープに追加
BlockHeader* request_space(size_t size) {
    BlockHeader* block = (BlockHeader*)sbrk(HEADER_SIZE + size);
    
    if (block == (void*)-1) {
        return nullptr; // メモリ割り当て失敗
    }
    
    block->size = size;
    block->is_free = false;
    block->next = nullptr;
    
    // ヒープの先頭が未初期化の場合
    if (heap_start == nullptr) {
        heap_start = block;
    } else {
        // 最後のブロックを見つけて追加
        BlockHeader* current = heap_start;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = block;
    }
    
    return block;
}

// 隣接する空きブロックを結合
void coalesce_blocks() {
    BlockHeader* current = heap_start;
    
    while (current != nullptr && current->next != nullptr) {
        if (current->is_free && current->next->is_free) {
            current->size += HEADER_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// カスタムmalloc実装
void* my_malloc(size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    // サイズをアライン
    size = align_size(size);
    
    // 既存の空きブロックを検索
    BlockHeader* block = find_free_block(size);
    
    if (block != nullptr) {
        // 空きブロックが見つかった
        block->is_free = false;
        split_block(block, size);
        return (void*)((char*)block + HEADER_SIZE);
    }
    
    // 空きブロックが見つからなかったので新しいスペースを要求
    block = request_space(size);
    if (block == nullptr) {
        return nullptr; // メモリ割り当て失敗
    }
    
    return (void*)((char*)block + HEADER_SIZE);
}

// カスタムfree実装
void my_free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
    
    // ヘッダーを取得
    BlockHeader* block = (BlockHeader*)((char*)ptr - HEADER_SIZE);
    block->is_free = true;
    
    // 隣接する空きブロックを結合
    coalesce_blocks();
}

// カスタムcalloc実装
void* my_calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ptr = my_malloc(total_size);
    
    if (ptr != nullptr) {
        // メモリをゼロで初期化
        std::memset(ptr, 0, total_size);
    }
    
    return ptr;
}

// カスタムrealloc実装
void* my_realloc(void* ptr, size_t new_size) {
    if (ptr == nullptr) {
        return my_malloc(new_size);
    }
    
    if (new_size == 0) {
        my_free(ptr);
        return nullptr;
    }
    
    // 現在のブロックのヘッダーを取得
    BlockHeader* block = (BlockHeader*)((char*)ptr - HEADER_SIZE);
    
    // 現在のサイズで十分な場合
    if (block->size >= new_size) {
        return ptr;
    }
    
    // 新しいメモリを割り当て
    void* new_ptr = my_malloc(new_size);
    if (new_ptr == nullptr) {
        return nullptr;
    }
    
    // データをコピー
    std::memcpy(new_ptr, ptr, block->size);
    
    // 古いメモリを解放
    my_free(ptr);
    
    return new_ptr;
}

// デバッグ用: メモリブロックの状態を表示
void print_memory_blocks() {
    BlockHeader* current = heap_start;
    int block_num = 0;
    
    std::cout << "\n=== Memory Blocks ===" << std::endl;
    
    while (current != nullptr) {
        std::cout << "Block " << block_num++ << ": "
                  << "Size = " << current->size << " bytes, "
                  << "Free = " << (current->is_free ? "Yes" : "No")
                  << std::endl;
        current = current->next;
    }
    
    std::cout << "=====================\n" << std::endl;
}
