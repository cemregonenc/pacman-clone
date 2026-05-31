# Pac-Man Klonu 

Klasik Pac-Man arcade oyununun **C++** ve **SFML** ile yazılmış kişisel klonu. Oyuncu labirentte noktaları toplar, hayaletlerden kaçar, güç meyvesi yiyince hayaletleri yer.

> 🎨 **Yaratıcı özellik:** Pac-Man olarak ekrana **kendi fotoğrafım** çıkıyor — `assets/images/custom_pacman.png` dosyası oyuna gömülü. Yön değiştirdikçe fotoğraf da döner, üzerinde "ağız" animasyonu olur.

---

## 🎮 Oynanış

| Tuş | İşlev |
|-----|-------|
| **ENTER** | Menüden oyuna başla |
| **↑ ↓ ← →** | Pac-Man'i hareket ettir |
| **P** | Duraklat / Devam et |
| **R** | Game Over ekranında yeniden başla |
| **ESC** | Çıkış |

**Puanlama:**
- Küçük nokta: **10 puan**
- Güç meyvesi: **50 puan**
- Korkmuş hayaleti yemek: **+200 puan**

---

## ✨ Özellikler

- ✅ Klasik 28×31 Pac-Man labirenti
- ✅ Tile-tabanlı hareket (klasik arcade hissi)
- ✅ Duvar çarpışması + yan tünel (sol/sağ kenardan geçiş)
- ✅ Güç meyvesi → **frightened mode** (hayaletler maviye döner, yenebilir)
- ✅ **2 farklı yapay zeka algoritması** (aşağıda detay)
- ✅ Can sistemi (3 can) + skor + yüksek skor
- ✅ Tüm noktalar yenince **otomatik yeni seviye**
- ✅ "YENİ REKOR!" göstergesi
- ✅ Game Over'da **R ile yeniden başlat**
- ✅ Özel fotoğraf desteği (Pac-Man yerine yüz)
- ✅ Animasyonlu menü ekranı (ağız animasyonu, yanıp sönen prompt)
- ✅ Retro tarz **Jersey 10** fontu

---

## 🧠 Yapay Zeka Algoritmaları

Proje şartı en az iki farklı davranış kalıbına sahip hayalet istemektedir. Bu projede iki **farklı algoritma** ile iki hayalet hareket eder:

### 🔴 Blinky — BFS (Breadth-First Search)

**Algoritma:** Genişlik öncelikli arama. Pac-Man'in tile'ına giden **garantili en kısa yolu** bulur.

**Çalışma adımları:**
1. Hayaletin tile'ını kuyruğa koy
2. Kuyruktan tile çek, 4 komşusuna bak (↑↓←→)
3. Duvar olmayan, ziyaret edilmemiş komşuları kuyruğa ekle
4. Pac-Man'in tile'ına ulaşınca dur
5. Parent dizisi üzerinden geri izleyip **ilk hareket yönünü** bul

**Karmaşıklık:** O(V + E) — V = tile sayısı, E = komşuluk sayısı

**Avantaj:** Optimal — her zaman en kısa yolu bulur, asla takılmaz
**Dezavantaj:** Her tile merkezinde tüm labirenti tarar

### 🩷 Pinky — Hibrit (Greedy + BFS kurtarması)

**Algoritma:** Bir adım önündeki komşulardan **Manhattan mesafesi en kısa olanı** seçer. Takıldığında BFS devreye girer.

**Çalışma adımları:**
1. 4 komşuya bak, geri dönüş yasak
2. Her komşunun Pac-Man'in **4 tile önündeki hedefe** uzaklığını hesapla
3. En yakın uzaklığa sahip yönü seç
4. 3 frame üst üste aynı tile'da kaldıysa → **BFS kurtarması**

**Manhattan mesafesi:** |x₁ − x₂| + |y₁ − y₂|

**Avantaj:** O(1) — sadece 4 komşuya bakar, hızlı
**Dezavantaj:** Çıkmaz yola girebilir, geleceği göremez (BFS kurtarması bu zaafı kapatır)

**Tasarım nedeni:** İki BFS hayaleti aşırı agresif oluyordu. Pinky'nin bilinçli "miyop" davranışı oyuna **scatter** hissi katar ve dengeyi sağlar.

---

## 🛠️ Kurulum

### Gereksinimler

- **macOS** (Linux/Windows da çalışır, CMake yolları değişebilir)
- **C++17** destekli derleyici (Apple Clang, GCC, MSVC)
- **CMake** ≥ 3.16
- **SFML** 2.5 veya 2.6

### macOS'ta SFML kurulumu

```bash
brew install sfml@2
```

### Derleme ve çalıştırma

```bash
git clone https://github.com/cemregonenc/pacman-clone.git
cd pacman-clone
mkdir build && cd build
cmake ..
cmake --build .
./PacmanClone
```

---

## 📁 Klasör Yapısı

```
pacman-clone/
├── CMakeLists.txt           # Derleme yapılandırması
├── README.md
├── .gitignore
├── assets/
│   ├── fonts/
│   │   └── Jersey10.ttf     # Retro arcade fontu
│   └── images/
│       └── custom_pacman.png  # Pac-Man yerine kullanılan fotoğraf
├── include/                 # Header dosyaları
│   ├── Constants.h          # Renk paleti, tile boyutu, oyun durumları
│   ├── Game.h               # Ana oyun sınıfı (state machine + döngü)
│   ├── Maze.h               # Labirent + pellet yönetimi
│   ├── Player.h             # Pac-Man (hareket, çizim, fotoğraf)
│   └── Ghost.h              # Hayalet (BFS + Greedy, frightened mode)
└── src/                     # Implementasyon dosyaları
    ├── main.cpp
    ├── Game.cpp
    ├── Maze.cpp
    ├── Player.cpp
    └── Ghost.cpp
```

---

## 🏗️ Mimari Notlar

### Oyun döngüsü

`Game::run()` klasik **fixed timestep** desenini kullanır:

1. Geçen süre ölçülür
2. Birikmiş zaman bir frame'i aşana kadar `update()` çağrılır
3. Sonunda `render()` ile ekran çizilir

Bu sayede oyun mantığı FPS'ten **bağımsız** çalışır — hızlı/yavaş bilgisayarda aynı hızda koşar.

### State machine

`Constants::GameState` enum'u 5 durum tanımlar:
**Menu → Playing → Paused / LevelComplete / GameOver**

`Game::render()` switch ile uygun ekran metodunu çağırır.

### Tile-tabanlı hareket

Pac-Man ve hayaletler **piksel cinsinden** hareket eder ama **yön kararları sadece tile merkezlerinde** alınır. Bu klasik arcade Pac-Man hissini verir — yön tuşuna bastığında hemen değil, bir sonraki tile merkezinde döner.

---

## 🎨 Yaratıcı Özellik — Özel Fotoğraf

Oyunun en sevdiğim özelliği: `assets/images/custom_pacman.png` varsa Pac-Man yerine **bu fotoğraf** ekranda hareket eder.

- Sprite **yön vektörüne göre döner** (sağa giderken sağa bakar, sola dönünce 180° döner)
- Üzerinde **ağız animasyonu** vardır (arka plan renginde üçgenle parça kesiliyor)
- Fotoğraf yoksa klasik sarı Pac-Man çizilir (fallback)

