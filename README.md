# Pac-Man Klonu 

Klasik Pac-Man oyununun C++ ve SFML ile yazılmış klonu.

> 🎨 **Eğlenceli özellik:** `assets/images/custom_pacman.png` dosyasına bir
> fotoğraf koyarsan Pac-Man yerine o yüz oynar!

## İlerleme

- [x] **Adım 1** — SFML penceresi, oyun döngüsü, animasyonlu menü
- [ ] Adım 2 — Labirent (klasik 28×31)
- [ ] Adım 3 — Pac-Man hareketi + özel fotoğraf desteği
- [ ] Adım 4 — Noktalar, güç meyveleri, skor
- [ ] Adım 5 — Hayaletler (BFS akıllı + basit takip)
- [ ] Adım 6 — Can, seviye, yüksek skor, korkmuş mod
- [ ] Adım 7 — Menü cilası, animasyonlar, ses

## Kurulum (macOS)

```bash
brew install sfml@2
mkdir build && cd build
cmake ..
cmake --build .
./PacmanClone
```

## Klavye

| Tuş | İşlev |
|-----|-------|
| **ENTER** | Menüden başlat |
| **P** | Duraklat / devam |
| **ESC** | Çıkış |

## Klasör yapısı

```
pacman-clone/
├── CMakeLists.txt
├── README.md
├── assets/
│   ├── fonts/        # PressStart2P.ttf (opsiyonel)
│   └── images/       # custom_pacman.png buraya
├── include/          # .h dosyaları
└── src/              # .cpp dosyaları
```