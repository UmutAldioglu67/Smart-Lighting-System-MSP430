# Smart-Lighting-System-MSP430
"MSP430G2553 tabanlı, dış kütüphane kullanılmadan bare-metal (register seviyesi) geliştirilmiş, PIR ve LDR sensör geri bildirimli otonom akıllı aydınlatma sistemi."

# Otonom Akıllı Aydınlatma Sistemi (MSP430)

Bu proje, ortamdaki hareket ve ışık seviyesini (PIR ve LDR) sürekli olarak analiz edip enerji tasarrufu sağlamak amacıyla aydınlatma gücünü otonom olarak ayarlayan donanım merkezli bir gömülü sistem projesidir.

Projenin temel amacı, piyasadaki hazır güç modülleri (breakout board) kullanım alışkanlıklarını bir kenara bırakarak; voltaj regülasyonundan güç anahtarlamaya kadar tüm elektriksel altyapının özel tasarlanmış **çift taraflı bir PCB** üzerinde komponent seviyesinde (component-level) birleştirilmesidir. Yazılım mimarisinde ise hiçbir soyutlama kütüphanesi (HAL veya standart kütüphane) kullanılmamış, doğrudan mikrodenetleyici register'larına müdahale edilmiştir.

---

## 🛠️ Donanım Mimarisi ve Çift Taraflı PCB Tasarımı

Sistemin en güçlü yönü, elektriksel bütünlüğü ve kararlılığı maksimize etmek amacıyla baştan sona özel olarak tasarlanan donanım altyapısıdır. Piyasada sıkça tercih edilen hazır modüllerden (Bluetooth ve OLED ekran hariç) tamamen kaçınılmış, sistem donanım seviyesinde sıfırdan kurgulanmıştır.

* **Çift Taraflı (Double-Sided) Özelleştirilmiş PCB:** Sinyal yolları ve yüksek akım çeken güç hatları, elektromanyetik girişimi (EMI) minimize edecek şekilde iki katmanlı olarak yönlendirilmiştir.
* **Tümleşik Voltaj Regülasyonu:** Harici bir güç modülü kullanılmaksızın, giriş gerilimini mikrodenetleyicinin ve sensörlerin ihtiyaç duyduğu kararlı 3.3V seviyesine düşüren LM1117 lineer regülatör devresi doğrudan PCB üzerine gömülmüştür.
* **Komponent Seviyesinde Tasarım:** Yüksek güçlü LED'i sürmek için hazır modüller yerine, mikrodenetleyici pinini koruyan ve yüksek akım anahtarlaması yapabilen NPN Transistör (BCP56) tabanlı *low-side switch* sürücü devresi PCB üzerinde bağımsız komponentlerle dizayn edilmiştir.
* **Fiziksel Dayanım:** Ortam şartlarına ve elektromekanik titreşimlere karşı dirençli bir endüstriyel prototip elde edilmiştir.

---

## 💻 Yazılım Mimarisi (Bare-Metal)

Sistemin yazılımı, 512 Byte RAM kısıtlamasına sahip MSP430G2553 mikrodenetleyicisinin tüm kaynaklarını %100 kontrol edebilmek amacıyla tamamen "Bare-Metal" (kütüphanesiz) olarak C dilinde geliştirilmiştir.

* **Register-Level Programlama:** I2C, UART, ADC ve Timer donanımları hazır kütüphanelerle değil, doğrudan işlemci kaydedicileri (register) üzerinden bayt bayt yapılandırılmıştır.
* **Özel I2C ve OLED Sürücüsü:** SSD1306 OLED ekranı kontrol etmek için donanımsal I2C protokolü ve **5x7 ASCII font matrisi** sıfırdan yazılarak bellekte yer kaplamaması için doğrudan Flash belleğe (ROM) gömülmüştür.
* **Donanımsal Kesme (Interrupt) Yönetimi:** PIR sensöründen gelen hareket verileri ana döngüde (polling) beklenmek yerine, Port 2 üzerinden asenkron donanım kesmesiyle (ISR) yakalanarak sistemin tepki süresi (latency) ideal seviyeye çekilmiştir.
* **Donanımsal PWM Üretimi:** İşlemciyi döngüsel gecikmelerle (delay) yormamak adına, Timer_A modülü "Reset/Set" modunda (OUTMOD_7) çalıştırılarak pürüzsüz ve CPU'dan bağımsız bir 1 kHz PWM sinyali elde edilmiştir.

---

## ⚙️ Temel Özellikler ve Otonom Fonksiyonlar

* **Asenkron Hareket Algılama:** PIR sensörü bir insan veya hareket algıladığı an donanımsal kesme ile sistemi uyararak aydınlatma karar algoritmasını tetikler.
* **Kademeli (Ters Orantılı) Aydınlatma:** Ortamdaki ışık (LDR) seviyesi düştükçe, PWM kontrollü yapay aydınlatma modülü otomatik olarak gücünü ayarlar. Sistem karanlıkta **%90**, loş ortamda **%30** parlaklıkta çalışır; aydınlıkta veya hareket bittiğinde **%0** seviyesine inerek tam kapanır.
* **Gerçek Zamanlı Telemetri:** ADC üzerinden okunan güncel LDR verileri ve sistem durumları Bluetooth (UART) üzerinden kablosuz olarak telefon terminaline aktarılır (Veri kirliliğini önlemek için 3 saniyelik periyotlarla).
* **Lokal Arayüz:** İşlenen tüm veriler, cihaz üzerindeki OLED ekranda anlık olarak görüntülenir.

---

## 🚀 Kullanılan Teknolojiler ve Bileşenler

* **Mikrodenetleyici:** Texas Instruments MSP430G2553
* **Sensör Ağı:** LDR (Ortam Işığı Sensörü), HC-SR501 PIR (Hareket Sensörü)
* **Aktüatörler:** Yüksek Güçlü LED (BCP56 Transistör ve Timer_A PWM destekli)
* **Haberleşme:** HC-05 Bluetooth Modülü (Sadece modül olarak kullanılmıştır)
* **Görüntüleme:** 0.96" SSD1306 OLED (I2C) (Sadece modül olarak kullanılmıştır)
* **Geliştirme Ortamı:** Code Composer Studio (CCS)
* **PCB Tasarım:** Kicad
  
---

👨‍💻 Geliştirici
Umut Aldıoğlu
Kocaeli Üniversitesi - Elektronik & Haberleşme Mühendisliği
