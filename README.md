# PTSD Template

This is a [PTSD](https://github.com/ntut-open-source-club/practical-tools-for-simple-design) framework template for students taking OOPL2024s.

## Quick Start

1. Use this template to create a new repository
   ![github screenshot](https://github.com/ntut-rick/ptsd-template/assets/126899559/ef62242f-03ed-481d-b858-12b730c09beb)

2. Clone your repository

   ```bash
   git clone YOUR_GIT_URL --recursive
   ```

3. Build your project

  > [!WARNING]
  > Please build your project in `Debug` because our `Release` path is broken D:
   
   ```sh
   cmake -DCMAKE_BUILD_TYPE=Debug -B build # -G Ninja
   ```
   better read [PTSD README](https://github.com/ntut-open-source-club/practical-tools-for-simple-design)

# Abstract

遊戲名稱：元氣騎士 Soul Knight

組員：

- 112590035 葉凱成
- 112590042 鄭錦鑫

# Game Introduction

本專案旨在還原《元氣騎士》初代版本的核心遊玩體驗，並根據實際開發需求進行適當調整與優化。 
《元氣騎士》是一款 **2D 地牢射擊遊戲**，核心功能包括：

1. **角色控制**（移動、攻擊、閃避）- 錦鑫
2. **隨機地牢主題生成** - 凱成
3. **敵人 AI（移動軌跡、攻擊方式、追蹤玩家-部分）** - 錦鑫
4. **武器系統（不同武器特性、子彈軌跡、合成武器）** - 凱成
5. **道具 & 天賦（回血、護盾、技能增強）** - 錦鑫
6. **地圖碰撞判定** - 凱成
7. **動畫 & 特效（角色動畫、爆炸、閃電等）**
8. **音效 & UI（血量、能量條、武器選擇）**

# Development timeline

- Week 1：基礎架構與素材準備
  - [ ] 蒐集遊戲的素材
  - [ ] **凱成**：游戲場景 P1 - 主菜單、游戲循環、分數結算畫面……
  - [ ] **錦鑫**：物件架構 -屬性、狀態、攻擊、碰撞判斷 P1
- Week 2：處理遊戲的封面、游戲循環主框架、物件基本屬性參數
  - [ ] 處理遊戲封面的素材
  - [ ] 進行遊戲封面的設計
  - [ ] **凱成**：游戲場景 P2 - 完善
  - [ ] **錦鑫**：物件架構 -屬性、狀態、攻擊、碰撞判斷 P2
- Week 3：可測試場景地圖系統、物件基本屬性參數
  - [ ] **凱成**：測試用地圖製作
  - [ ] **錦鑫**：敵人攻擊搜尋邏輯與架構
  - [ ] **錦鑫**：物件架構 -屬性、狀態、攻擊、碰撞判斷 P3
- Week 4：小關卡與敵人系統
  - [ ] **凱成**：小關卡地圖製作 - 冰原、城堡、火山
  - [ ] **錦鑫**：角色和敵人類別架構
- Week 5：撞墻、武器、敵人攻擊邏輯
  - [ ] **凱成**：小關卡地圖製作 - 碰撞物體
  - [ ] **凱成**：武器系統
  - [ ] **錦鑫**：敵人攻擊索敵系統 P1
- Week 6：關卡可互動物品、敵人攻擊邏輯
  - [ ] **凱成**：小關卡物件邏輯 - 箱子、地刺、門、加減速
  - [ ] **錦鑫**：敵人攻擊索敵系統 P2
- Week 7：小關卡地圖生成、天賦、道具系統
  - [ ] **凱成**：小關卡隨機地圖生成
  - [ ] **錦鑫**：天賦系統&道具系統 P1
- Week 8：大關卡系統、天賦道具系統  
  - [ ] **凱成**：大關卡結算、當前關卡數、金幣系統
  - [ ] **錦鑫**：天賦系統&道具系統 P2
- Week 9：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 10：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 11：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 12：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 13：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 14：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
  - Week 15：XXX  
  - [ ] **凱成**：XXX
  - [ ] **錦鑫**：XXX 
