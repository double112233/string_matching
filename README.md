# Fast Approximate String Matching

本 repo 是根據 [Fast Approximate String Matching with Suffix Arrays and A* Parsing](https://aclanthology.org/2010.amta-papers.2.pdf) 來實作。

執行 `g++ -std=c++11 main.cpp; ./a.out` 即可使用。

輸入包含兩行，第一行為一個字串 $S$，第二行為一個字串 $P$，程式碼會輸出 $S$ 之中和  $P$ edit distance 最低的子字串。

本篇的實作會盡量貼近論文裡的 psuedo code。差異在於論文裡面將 $S$ 拆成了數個 segment $S_1S_2S_3\ldots S_z$，但 `main.cpp` 並不會進行 segment 的分割，而是直接取所有可能的子字串當作 segment。

## 潛藏的 Problem

這份 paper 用在 priority 的 heuristic 是有問題的。

因為他只要判到 cost > ceiling_cost 就會停下來，但是事實上在 priority 更低的情況下可能會有別組滿足 cost < ceiling_cost，所以有時候會找不到答案。

例子可以參考 `2.in`。

## Testdata 解釋

### 1.in

可以肉眼追蹤過程的測資。

### 2.in

可以肉眼追蹤過程的測資。正確性有問題的例子。

### 3.in

$S$ 的長度是 $10^5$，$P$ 的長度是 $3$。

應該要找到超過 $100$ 個答案，否則就是錯的。

## 測試

`gen.py` 可以簡單的生成測試資料。

在第一行輸入兩個正整數 $N, M$，兩者以空白分開，就會輸出兩行，第一行是長度為 $N$ 的隨機字串，第二行是長度為 $M$ 的隨機字串。
