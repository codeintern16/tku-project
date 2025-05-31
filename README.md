# tku-project
📘 Course Scheduler 課表管理器
這是一個使用 C 語言撰寫的課表管理系統，支援：
📆 多時段課程（同一門課可以佔用多個時間區段）
💾 儲存/讀取 CSV 檔案
📊 垂直排課表視覺化
🧠 自動檢查衝堂與學分統計
| 編號  | 功能名稱               | 說明                                 
| --- | ------------------ | ---------------------------------- 
| `1` | Add course         | 新增課程（可包含多個上課時段）並自動檢查衝堂             
| `2` | Show total credits | 顯示所有課程與總學分                         
| `3` | Print timetable    | 印出垂直格式課表（以星期為欄、節次為列）               
| `4` | Delete course      | 根據課程 ID 刪除課程                       
| `5` | Exit               | 結束程式                               
| `6` | Save to CSV        | 將課表資料儲存成 `courses.csv`，可用 Excel 開啟 
| `7` | Load from CSV      | 從 `courses.csv` 載入先前儲存的課表   

直接載raw就可以使用了
