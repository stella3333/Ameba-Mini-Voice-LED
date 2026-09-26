# AMB82-MINI 語音控制 LED

以瀏覽器語音辨識轉換控制語句，透過 USB Serial 傳送至 AMB82-MINI。網頁上的 LED 指示只依據開發板回傳的 `STATE` 更新。

## 專案檔案

- `index.html`：語音辨識、序列埠連線、狀態顯示及事件紀錄。
- `ameba_mini_led/ameba_mini_led.ino`：AMB82-MINI 韌體。

## 設定與啟動

1. 在 Arduino IDE 開啟 `ameba_mini_led/ameba_mini_led.ino`，選擇 AMB82-MINI 對應的板卡及 COM 埠。
2. 本專案使用官方 Pinmap：藍燈 `D23`（PF9、`LED_B`），綠燈 `D24`（PE6、`LED_G`）。若板載 LED 的有效電位相反，將 `LED_ACTIVE_LOW` 改為 `true`。
3. 上傳韌體。需要確認啟動訊息時，可暫時開啟鮑率 `115200` 的序列監控器，確認 `READY AMEBA_MINI_LED` 後關閉監控器。序列監控器和網頁不能同時占用同一個 COM 埠。
4. 保持 USB 連接，以 Chrome 或 Edge 開啟網頁。若直接開啟 HTML 無法使用 Web Serial，請在工作區根目錄執行 `py -m http.server 8000`，瀏覽 `http://localhost:8000`。
5. 按「連接 Ameba Mini」，選取開發板的 USB 序列埠；連線成功後按「開始聆聽」。用網頁時請確認沒有其他程式占用該 COM 埠。

## 語音指令

中文模式：

- 「左邊開燈」：只開藍燈。
- 「右邊開燈」：只開綠燈。
- 「左邊關燈」：只關藍燈。
- 「右邊關燈」：只關綠燈。
- 「閃爍三次」或「閃爍 3 次」：藍、綠燈同步閃爍三次，最後熄滅。
- 「通訊中斷」：啟動網頁端的通訊中斷模擬，以測試離線時不執行 LED 指令。

English 模式：

- `turn on the left` / `turn off the left`
- `turn on the right` / `turn off the right`
- `blink`
- `communication interrupted`：啟動網頁端通訊中斷模擬。

左右 LED 是獨立控制；開啟其中一燈不會關閉另一燈。非控制語句不會傳送 LED 命令。韌體亦支援序列命令 `ALL_OFF` 與 `STATUS`，但網頁沒有手動「全部關閉」按鈕。

## 通訊中斷行為

- 說「通訊中斷」是軟體模擬，不會真的拔除 USB。網頁顯示模擬中斷後，說左右開燈或關燈都不會送出命令，事件紀錄會標示未執行。
- 實際拔除 USB 時，序列讀寫錯誤會標示通訊中斷；此時重新接上 USB，再按「連接 Ameba Mini」重新選擇序列埠。沒有「通訊恢復」語音命令。
- 命令送出後若 3 秒內沒有收到開發板 `STATE` 回覆，網頁顯示通訊逾時。只有收到 `STATE` 才更新介面燈號。

