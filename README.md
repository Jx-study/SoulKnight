# 元氣騎士 Soul Knight (C++ Remake)
## 專案簡介 Game Introduction

本專案是《元氣騎士》的 C++ 複刻版，旨在還原其初代版本的核心玩法與體驗。這是一款 2D隨機地牢射擊遊戲，玩家將探索關卡、擊敗敵人、收集、武器與道具，挑戰強大的 Boss。

## 功能特色 (Features)
- 隨機地牢生成系統
- 多樣化的武器與道具
- 具備基本 AI 的敵人
- 角色移動、攻擊、閃避機制
- 地圖碰撞與互動元素
- 冰原主題關卡與雪人王 Boss 戰

## 專案目標
 - 還原最低限度為一個大關卡，其中包括5個小關卡（冰原主題）
    - 特殊房間（商店、寶箱房間）&NPC（神秘商販、工匠、教官）
    - 大boss ==》 雪人王
      -   影片：
      [![雪人王](https://img.youtube.com/vi/sTqyGG8T1ws/0.jpg)](https://www.youtube.com/watch?v=sTqyGG8T1ws)

### 《元氣騎士》游戲視頻
[![元氣騎士影片](https://img.youtube.com/vi/CTrSVxV5OhA/0.jpg)](https://www.youtube.com/watch?v=CTrSVxV5OhA)

### 游戲流程圖+類別關係圖
點擊即可查詢詳細[游戲流程圖+類別關係圖](https://excalidraw.com/#room=14f2a41e39a4bd1c4af2,OME25KsrfJQcoMUMvtZfhg)

## 技術棧 (Tech Stack)
- **語言:** C++
- **建置系統:** CMake
- **核心框架:** PTSD Framework
- **函式庫:** nlohmann/json (用於資料處理)

## 如何建置與執行 (Getting Started)
### 依賴項目
- C++ Compiler (e.g., GCC, Clang, MSVC)
- CMake 3.20+

### 建置與執行步驟
1. Clone the repository
```
git clone https://github.com/Jx-study/SoulKnight.git
```
2. Configure with CMake
```
cmake -B build -S .
```

3. Build
```
cmake --build build
```
4. Run
```
./build/bin/SoulKnight
```

## 組員 (Team)
- 國立臺北科技大學 112590035 葉凱成
- 國立臺北科技大學 112590042 鄭錦鑫