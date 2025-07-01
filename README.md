
# 🚨 Sistem Keamanan Motor Berbasis Arduino, GPS, dan SIM800L

Sistem ini merupakan proyek keamanan untuk sepeda motor yang memungkinkan pengguna untuk **mengontrol dan memantau motor dari jarak jauh menggunakan SMS**. Dilengkapi dengan **GPS tracking**, **anti-theft alarm**, dan **relay kontrol mesin**, proyek ini cocok untuk diterapkan pada kendaraan pribadi.

---

## 🔧 Fitur Utama

- 🔁 **Kontrol Jarak Jauh:** Menyalakan dan mematikan mesin melalui SMS
- 🧲 **Starter Otomatis:** Starter bisa diaktifkan setelah kontak ON
- 📍 **Pelacakan Lokasi:** Kirim lokasi motor lewat Google Maps link via SMS
- 🚨 **Mode Anti-Pencurian:** Sensor getar aktif, memicu alarm saat motor digerakkan
- ⚡ **Kontrol Power Utama:** Memutus atau menghubungkan aliran listrik motor
- 🔐 **Keamanan Nomor:** Hanya nomor tertentu yang bisa mengirim perintah

---

## 📦 Komponen yang Dibutuhkan

| Komponen              | Jumlah | Keterangan                         |
|-----------------------|--------|------------------------------------|
| Arduino Uno/Nano      | 1      | Mikrokontroler utama               |
| SIM800L Module        | 1      | Modul GSM/GPRS                    |
| GPS Module (NEO-6M)   | 1      | Untuk pelacakan lokasi            |
| Relay 4 Channel       | 1      | Kontrol mesin, alarm, starter, dll|
| Sensor Getar (SW-420) | 1      | Deteksi getaran                   |
| Step-down LM2596      | 3      | Menurunkan tegangan aki           |
| SIM Card Aktif        | 1      | Dengan pulsa untuk SMS/Telepon    |

### Library Arduino
- `TinyGPS++`
- `SoftwareSerial`
- `AltSoftSerial`

---

## 🔌 Wiring & Pin Konfigurasi

| Arduino Pin | Komponen          | Fungsi                                     |
|-------------|-------------------|--------------------------------------------|
| 4           | Relay Starter     | Mengaktifkan starter motor                 |
| 5           | Relay Utama       | Menghidupkan/mematikan mesin               |
| 6           | Sensor Getar (DO) | Mendeteksi guncangan                       |
| 7           | Relay Power       | Memutus/menyambung power utama             |
| 8 (RX2)     | GPS TX            | Menerima data GPS via AltSoftSerial        |
| 9 (TX2)     | GPS RX            | Mengirim data ke GPS (jika diperlukan)     |
| 10 (RX)     | SIM800L TX        | Menerima data dari SIM800L                 |
| 11 (TX)     | SIM800L RX        | Mengirim data ke SIM800L                   |
| 12          | Relay Alarm       | Mengaktifkan klakson atau buzzer           |

📌 **Catatan:** Relay power dihubungkan ke jalur Normally Open (NO).

---

## 📱 Daftar Perintah SMS

| Perintah          | Fungsi                                    | Respon             |
|-------------------|--------------------------------------------|--------------------|
| `on`              | Menyalakan power utama                     | `Power ON.`        |
| `off`             | Mematikan power utama                      | `Power OFF.`       |
| `bike on`         | Menyalakan mesin motor                     | `Motor dinyalakan.`|
| `bike off`        | Mematikan mesin                            | `Motor dimatikan.` |
| `bike start`      | Mengaktifkan starter motor                 | `Memulai starter...` |
| `anti theft on`   | Aktifkan mode anti-pencurian               | `Anti-pencurian aktif.`|
| `anti theft off`  | Nonaktifkan mode anti-pencurian            | `Anti-pencurian nonaktif.`|
| `get location`    | Minta lokasi motor                         | Link Google Maps / `GPS tidak tersedia.`|

---

## 🛠️ Cara Instalasi & Penggunaan

1. **Rakit semua komponen** sesuai diagram dan pastikan tegangan SIM800L stabil (4.0V – 4.2V).
2. **Edit nomor telepon** di kode (`AUTHORIZED_PHONE`) dengan nomor kamu dalam format internasional (`+62`).
3. **Upload kode** ke Arduino menggunakan Arduino IDE.
4. **Pasang sistem ke motor**, relay terhubung ke kontak, starter, dan alarm.
5. **Uji sistem** dengan mengirim SMS sesuai daftar perintah di atas.

---

## 🗓️ Dokumentasi

Dibuat pada: **25 Juni 2025**  
Penulis: *Gunawan Talessy, Jubeda Kwairumasabandar, Irwanti Gani.*


