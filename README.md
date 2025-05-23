# Abstract

遊戲名稱：元氣騎士 Soul Knight

組員：

- 112590035 葉凱成
- 112590042 鄭錦鑫

# Game Introduction

本專案旨在還原《元氣騎士》初代版本的核心遊玩體驗，並根據實際開發需求進行適當調整與優化。此外，這游戲中共有**三個大關卡**，其中會有5個小關卡，并且每一個大關卡會有一個大Boss。
《元氣騎士》是一款 **2D 地牢射擊遊戲**，核心功能包括：

1. **角色控制**（移動、攻擊、閃避）
2. **隨機地牢主題生成** 
3. **敵人 AI（移動軌跡、攻擊方式、追蹤玩家-部分）** 
4. **武器系統（不同武器特性、子彈軌跡、合成武器）** 
5. **道具 & 天賦（回血、護盾、技能增強）** 
6. **地圖碰撞判定** 
7. **動畫 & 特效（角色動畫、爆炸、閃電等）**
8. **音效 & UI（血量、能量條、武器選擇）**

### 專案目標
 - 還原最低限度為一個大關卡，其中包括5個小關卡（冰原主題）
    - 特殊房間（商店、寶箱房間）&NPC（神秘商販、工匠、教官）
    - 大boss ==》 雪人王
      -   影片：
      [![雪人王](https://img.youtube.com/vi/sTqyGG8T1ws/0.jpg)](https://www.youtube.com/watch?v=sTqyGG8T1ws)

### 《元氣騎士》游戲視頻
[![元氣騎士影片](https://img.youtube.com/vi/CTrSVxV5OhA/0.jpg)](https://www.youtube.com/watch?v=CTrSVxV5OhA)

### 游戲流程圖+類別關係圖
點擊即可查詢詳細[游戲流程圖+類別關係圖](https://excalidraw.com/#room=14f2a41e39a4bd1c4af2,OME25KsrfJQcoMUMvtZfhg)

# Development timeline

#### **期中前複刻主要游戲流程**

- **Week 1：基礎架構與素材準備**
  - [x] 蒐集遊戲的素材
  - [x] **凱成**：游戲場景 P1
    - 初步理解 PTSD 框架及如何運用
    - 初步完成場景類別 Scene，將 App 視爲場景管理器
    - 問題：場景切換 (CodeReview 需要) 似乎應該給 Scene 去完成嗎？(目前 App::Update)
  - [x] **錦鑫**：物件架構 -屬性、狀態、攻擊、碰撞判斷 - W1
    - 透過完成Giraffe-Adventure，對PTSD框架有初步的瞭解
    - **問題：**類別繼承樹(關係圖)是要怎麽樣？
      - **解：**需要整理出所有角色的屬性之間的關係
  ---
- **Week 2：處理遊戲的封面、游戲循環主框架、物件基本屬性參數**
  - [ ] 處理遊戲封面的素材
  - [ ] 進行遊戲封面的設計
  - [ ] **凱成**：游戲場景 P2 - 完善
    - 完成主頁菜單 v1，可以切換場景至游戲大廳
    - 問題：UI 按鈕 btn 上有文字 text，text 是組合進 btn 還是聚合進 btn？會有什麽差別？
    - TODO：還在理解 PTSD 框架，Tiled 運用，鏡頭系統
    - TODO：各場景流程圖 - dd: 2025-03-01
  - [ ] **錦鑫**：整理素材包 + 物件架構 -屬性、狀態、攻擊、碰撞判斷 - W2
    - **問題：**
      a. 游戲apk解包后，素材沒分類且十分龐大，該如何整理？ 
      b. 要怎麽精準地判斷碰撞？如果用圖片大小作爲碰撞判斷，會時常出現錯誤，因爲圖片會有邊框。 
    - **解：**
      a. 寫簡單的程式根據文件名稱進行簡單分類後，後續再手動整理(依然是大工程=_=)
      b. 大致有兩種方法：
        - 方法一：**矩形碰撞檢測**（Bounding Box Collision）：  
          設定該物件的半徑，用一個矩形將其包裝起來。  
        - 方法二：**像素碰撞檢測**（Pixel Perfect Collision）：
        檢查兩張圖片對應位置的像素是否重疊，並且這樣可以完全排除圖片邊框和透明部分的影響。（難度過大）
    - **還未完成事項：**
      - 碰撞檢測的實現

  ---
- **Week 3：可測試場景地圖系統、物件基本屬性參數**
  - [ ] **凱成**：測試用地圖製作
  - [ ] **錦鑫**：角色和敵人類別架構
    - **完成事項：**
      - 角色左右移動時，將動畫/圖片鏡像化 (0306)  
    - **問題：**  
      - a）有很多種類型的小怪，我要如何根據他們的數值創建他們？ 
    - **解：**  
      - a）目前找到兩種方法 
        - 方法一：將小怪/角色的數值寫入一個json檔，並利用外部的json檔函數庫讀取 
        - 方法二：將數值csv檔案，可以更直觀且清晰地分辨或對比數值，可是需要自己寫讀取的function 
    - **還未完成事項：**
  ---
- **Week 4：小關卡製作與敵人系統**
  - [ ] **凱成**：小關卡地圖製作(冰原主題) + 物體碰撞
  - [ ] **錦鑫**：敵人攻擊模式+索敵系統 - W1
- **Week 5：關卡可互動物品、敵人攻擊邏輯**
  - [ ] **凱成**：小關卡物件邏輯 - 箱子、地刺、門、加減速
  - [ ] **錦鑫**：敵人攻擊模式+索敵系統 - W2
  ---
- **Week 6：小關卡地圖生成、敵人攻擊邏輯**
  - [ ] **凱成**：小關卡隨機地圖生成(怪物出現位置隨機)
  - [ ] **錦鑫**：天賦系統&道具系統 - W1
  ---
- **Week 7：武器、天賦+道具系統**
  - [ ] **凱成**：武器系統 + 攻擊動畫
  - [ ] **錦鑫**：天賦系統&道具系統 - W2
  ---
- **Week 8：大關卡系統、游戲音效**
  - [ ] **凱成**：大關卡結算(選擇天賦)、顯示當前關卡數、金幣系統
  - [ ] **錦鑫**：添加游戲音效
  ---
- **Week 9：優化生怪演算法、調整角色參數**
  - [ ] **凱成**：生怪演算法
  - [ ] **錦鑫**：調整對應主角、怪物參數匹對系統
---
#### 期中後優化完善已有系統及創新

- **Week 10：優化生怪演算法、調整參數 **
  - [ ] **凱成**：生怪演算法
  - [ ] **錦鑫**：調整對應主角、怪物參數匹對系統
  ---
- **Week 11：完善游戲玩法、改進敵人 AI**
  - [ ] **凱成**：特殊房 - 工匠、 雕像、 教官、傭兵等互動邏輯 - W1
  - [ ] **錦鑫**：改進敵人 AI 索敵/走動邏輯 - W1
  ---
- **Week 12：完善游戲玩法、改進敵人 AI**
  - [ ] **凱成**：特殊房 - 工匠、 雕像、 教官、傭兵等互動邏輯 - W2
  - [ ] **錦鑫**：改進敵人 AI 索敵/走動邏輯 - W2
  ---
- **Week 13：調整游戲參數、改進敵人 AI**
  - [ ] **凱成**：增添關卡主題、武器與優化邏輯 - W1
  - [ ] **錦鑫**：改進敵人 AI 索敵/走動邏輯 - W3
  ---
- **Week 14：增加更多樣的關卡、敵人、武器、天賦及其邏輯**
  - [ ] **凱成**：增添關卡主題、武器與優化邏輯 - W2
  - [ ] **錦鑫**：優化並更新天賦系統 - W1
  ---
- **Week 15：增加更多樣的關卡、敵人、武器、天賦及其邏輯**
  - [ ] **凱成**：增添關卡主題、武器與優化邏輯 - W3
  - [ ] **錦鑫**：優化並更新天賦系統 - W2
  ---
- **Week 16：時間允許，隊友想挑戰極限**
  - [ ] **凱成**：優化程式碼+聯機功能
  - [ ] **錦鑫**：優化程式碼+存檔功能
  ---
- **Week 17：時間允許，隊友想挑戰極限**
  - [ ] **凱成**：聯機功能
  - [ ] **錦鑫**：存檔功能
  ---
- **Week 18：完善與優化**
  - [ ] **凱成**：阿祖保佑
  - [ ] **錦鑫**：媽祖保佑  
    ...


 #### Commit類型：
- Feat: 新功能（feature）
- Fix: 修正 bug
- Docs: 修改文件（documentation）
-	Style: 格式變更（不影響程式邏輯，如排版、空格）
-	Refactor: 重構程式碼（不影響功能）
-	Perf: 提升效能
-	Test: 增加或修改測試
-	Chore: 其他變更，如 CI/CD 設定、依賴更新