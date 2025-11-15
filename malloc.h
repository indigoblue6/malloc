#ifndef MALLOC_H
#define MALLOC_H

#include <cstddef>

// メモリブロックのヘッダー構造
struct BlockHeader {
    size_t size;           // ブロックのサイズ（ヘッダーを除く）
    bool is_free;          // ブロックが空いているかどうか
    BlockHeader* next;     // 次のブロックへのポインタ
};

// カスタムmalloc関数
void* my_malloc(size_t size);

// カスタムfree関数
void my_free(void* ptr);

// カスタムcalloc関数
void* my_calloc(size_t num, size_t size);

// カスタムrealloc関数
void* my_realloc(void* ptr, size_t new_size);

// デバッグ用: メモリブロックの状態を表示
void print_memory_blocks();

#endif // MALLOC_H
