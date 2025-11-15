#include "malloc.h"
#include <iostream>
#include <cstring>

int main() {
    std::cout << "=== カスタムmallocテスト ===" << std::endl;
    
    // テスト1: 基本的なmalloc/free
    std::cout << "\n[テスト1] 基本的なmalloc/free" << std::endl;
    int* num1 = (int*)my_malloc(sizeof(int));
    *num1 = 42;
    std::cout << "割り当てた整数: " << *num1 << std::endl;
    print_memory_blocks();
    
    my_free(num1);
    std::cout << "メモリ解放後:" << std::endl;
    print_memory_blocks();
    
    // テスト2: 複数のmalloc
    std::cout << "\n[テスト2] 複数のmalloc" << std::endl;
    int* arr1 = (int*)my_malloc(5 * sizeof(int));
    char* str1 = (char*)my_malloc(20 * sizeof(char));
    double* dbl1 = (double*)my_malloc(sizeof(double));
    
    for (int i = 0; i < 5; i++) {
        arr1[i] = i * 10;
    }
    std::strcpy(str1, "Hello, malloc!");
    *dbl1 = 3.14159;
    
    std::cout << "配列: ";
    for (int i = 0; i < 5; i++) {
        std::cout << arr1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "文字列: " << str1 << std::endl;
    std::cout << "Double: " << *dbl1 << std::endl;
    print_memory_blocks();
    
    // テスト3: calloc
    std::cout << "\n[テスト3] calloc（ゼロ初期化）" << std::endl;
    int* arr2 = (int*)my_calloc(10, sizeof(int));
    std::cout << "calloc後の配列（すべて0であるべき）: ";
    for (int i = 0; i < 10; i++) {
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;
    
    // テスト4: realloc
    std::cout << "\n[テスト4] realloc" << std::endl;
    for (int i = 0; i < 10; i++) {
        arr2[i] = i + 1;
    }
    std::cout << "realloc前の配列: ";
    for (int i = 0; i < 10; i++) {
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;
    
    arr2 = (int*)my_realloc(arr2, 15 * sizeof(int));
    std::cout << "realloc後の配列（既存データは保持）: ";
    for (int i = 0; i < 10; i++) {
        std::cout << arr2[i] << " ";
    }
    std::cout << std::endl;
    print_memory_blocks();
    
    // テスト5: 解放と再利用
    std::cout << "\n[テスト5] 解放と再利用" << std::endl;
    my_free(arr1);
    my_free(str1);
    std::cout << "2つのブロック解放後:" << std::endl;
    print_memory_blocks();
    
    int* arr3 = (int*)my_malloc(3 * sizeof(int));
    std::cout << "新しいmalloc後（空きブロックを再利用）:" << std::endl;
    print_memory_blocks();
    
    // クリーンアップ
    my_free(dbl1);
    my_free(arr2);
    my_free(arr3);
    
    std::cout << "\n全メモリ解放後:" << std::endl;
    print_memory_blocks();
    
    std::cout << "\n=== テスト完了 ===" << std::endl;
    
    return 0;
}
