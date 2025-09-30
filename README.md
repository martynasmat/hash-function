# Maišos funkcija
# Pseudokodas
```
pseudokodas
```

# Eksperimentinis tyrimas

### Visi testavimo failai sugeneruoti ```filegen.py``` skriptu. 

## Išvedimo dydis ir deterministiškumas
```
# isvedimo dydis ir deterministiskumas
../test_files/test_1char_1.txt  len=1  hash_len=16  hash=9867c279d834a79efc2dd17234bfd055 vienodi=true
../test_files/test_1char_2.txt  len=1  hash_len=16  hash=b80daa89477f9d98771783c86d00a7ea vienodi=true
../test_files/test_1char_3.txt  len=1  hash_len=16  hash=7f81b6bb11af99e90d9674aaa67d9bfe vienodi=true
../test_files/test_1char_4.txt  len=1  hash_len=16  hash=0f6422b91d7bf4b53dd2f39b03c42268 vienodi=true
../test_files/test_1char_5.txt  len=1  hash_len=16  hash=4fbc88c7686678baab8a72944a518a3e vienodi=true
../test_files/test_1char_6.txt  len=1  hash_len=16  hash=63ff3636ccdaa06373b957c63bc6346f vienodi=true
../test_files/test_1char_7.txt  len=1  hash_len=16  hash=545545af699392345cfc4343324470b9 vienodi=true
../test_files/test_1char_8.txt  len=1  hash_len=16  hash=6000ebd44f11aa794f4ed45498ccb7a0 vienodi=true
../test_files/test_1char_9.txt  len=1  hash_len=16  hash=ef4f87a3ee1cee1aefbcef900a58b9f5 vienodi=true
../test_files/test_1char_10.txt  len=1  hash_len=16  hash=25ebf81e66c9698d27b67a31aad921a5 vienodi=true
../test_files/test_1000chars_1.txt  len=1000  hash_len=16  hash=31d3bfd34a887253806df67cae8229f1 vienodi=true
../test_files/test_1000chars_2.txt  len=1000  hash_len=16  hash=c00d4d51b05c7931972d3503997cc77c vienodi=true
../test_files/test_1000chars_3.txt  len=1000  hash_len=16  hash=0598a72be09d6388cc2857866986d58f vienodi=true
../test_files/test_1000chars_4.txt  len=1000  hash_len=16  hash=6b5fbf0b5a84a6dcd11e5dad9a6bb8f9 vienodi=true
../test_files/test_1000chars_5.txt  len=1000  hash_len=16  hash=3a89608a8433c5a7703418134e535c88 vienodi=true
../test_files/test_1000chars_6.txt  len=1000  hash_len=16  hash=a71ce91a028cf941cb7bf0795fb24ff3 vienodi=true
../test_files/test_1000chars_7.txt  len=1000  hash_len=16  hash=990816209cc7806ef9abe26bca202cf7 vienodi=true
../test_files/test_1000chars_8.txt  len=1000  hash_len=16  hash=48fd053806463bdb5f959194abaead32 vienodi=true
../test_files/test_1000chars_9.txt  len=1000  hash_len=16  hash=b530becd7106251c923785ce71963127 vienodi=true
../test_files/test_1000chars_10.txt  len=1000  hash_len=16  hash=1f32b8f775369e62cc278e2089abf3e1 vienodi=true
../test_files/test_1000chars_chardiff_1.txt  len=1000  hash_len=16  hash=c40b5e4f23c6df5191a3ac87ba75c643 vienodi=true
../test_files/test_1000chars_chardiff_2.txt  len=1000  hash_len=16  hash=7071bc994cd49135b29404367c787cd5 vienodi=true
../test_files/test_1000chars_chardiff_3.txt  len=1000  hash_len=16  hash=54ad722972683ed14a2b2087a60a4ca7 vienodi=true
../test_files/test_1000chars_chardiff_4.txt  len=1000  hash_len=16  hash=4b70215790a71af5eefc9f0f6fc606f9 vienodi=true
../test_files/test_1000chars_chardiff_5.txt  len=1000  hash_len=16  hash=eedc49eb57202662830e686b35f53fb3 vienodi=true
../test_files/test_1000chars_chardiff_6.txt  len=1000  hash_len=16  hash=e80e5c1630c44d73e7b050fdd6adfd81 vienodi=true
../test_files/test_1000chars_chardiff_7.txt  len=1000  hash_len=16  hash=3d6c6572238d820e0e9205395e799344 vienodi=true
../test_files/test_1000chars_chardiff_8.txt  len=1000  hash_len=16  hash=7f95fd9f62215bddc6c5eb2e0384040b vienodi=true
../test_files/test_1000chars_chardiff_9.txt  len=1000  hash_len=16  hash=566d7c95ba339a71e7c3fcee1a009d5d vienodi=true
../test_files/test_1000chars_chardiff_10.txt  len=1000  hash_len=16  hash=2c4ca81b09e899ce8412878fe6d4e5fb vienodi=true
```
- Hash'o ilgis vienodas visais atvejais.
- Kiekvienas failas hash'inamas 2 kartus, abu hash'ai vienodi.
- Hash'o ilgis 16 baitu, o v0.1 - 8 baitai.

## Efektyvumas
```
# efektyvumas hash
  eiluciu_kiekis=1  avg_ms=0.000
  eiluciu_kiekis=2  avg_ms=0.000
  eiluciu_kiekis=4  avg_ms=0.000
  eiluciu_kiekis=8  avg_ms=0.001
  eiluciu_kiekis=16  avg_ms=0.002
  eiluciu_kiekis=32  avg_ms=0.003
  eiluciu_kiekis=64  avg_ms=0.005
  eiluciu_kiekis=128  avg_ms=0.016
  eiluciu_kiekis=256  avg_ms=0.026
  eiluciu_kiekis=512  avg_ms=0.096
  eiluciu_kiekis=1024  avg_ms=0.111

# efektyvumas SHA256
  eiluciu_kiekis=1  avg_ms=0.000
  eiluciu_kiekis=2  avg_ms=0.001
  eiluciu_kiekis=4  avg_ms=0.001
  eiluciu_kiekis=8  avg_ms=0.001
  eiluciu_kiekis=16  avg_ms=0.001
  eiluciu_kiekis=32  avg_ms=0.001
  eiluciu_kiekis=64  avg_ms=0.002
  eiluciu_kiekis=128  avg_ms=0.005
  eiluciu_kiekis=256  avg_ms=0.010
  eiluciu_kiekis=512  avg_ms=0.044
  eiluciu_kiekis=1024  avg_ms=0.048
```
- Kiekvienas skaičius failo ```/test_files/konstitucija.txt``` eilučių (1, 2, 4...) hash'inamos po 10 kartų, išvedamas vidurkis. Matuojama milisekundėmis.
- SHA256 vis tiek greitesnis, nors v0.2 nuo SHA256 atsilieka daug maziau, nei v0.1.

![alt text](/results/image.png)

## Kolizijos
```
  len=10  pairs=100000  collisions=0
  len=100  pairs=100000  collisions=0
  len=500  pairs=100000  collisions=0
  len=1000  pairs=100000  collisions=0
```
- Per 400000 porų, kolizijų nerasta.

## Lavinos efektas
```
# lavinos efektas hash
  hamming distance / bits: avg_percentage=50.002%  min=31.250%  max=67.969%
  hamming distance / hex: avg_percentage=93.757%  min=71.875%  max=100.000%

# lavinos efektas sha256
  hamming distance / bits: avg_percentage=49.522%  min=37%  max=62%
  hamming distance / hex: avg_percentage=93.256%  min=78%  max=100%
```
- Vidutines SHA256 ir v0.2 reiksmes labai panasios - skiriasi tik per ~0.5%.
- SHA256 minimalios reikšmės didesnes ~6.5%.

## Negrįžtamumas
```
# negriztamumas
  input         = 402bd15094118b121548ec670b6607d0
  input + salt1 = 33c0ef3bece4e694b2573b772513bda3
  input + salt2 = 0ba378bd78f0790165c14e3a98741554
```

## Išvados
### Stiprybės
- Vidutiniškai, pakeitus 1 simbolį, hash'ai bit'ų lygmeniu sutampa ~10.3%, o hex'ų lygmeniu ~20.5% (Lavinos efektas).
- Maža kolizijų tikimybė.
- Funkcija veikia pakankamai greitai, nors SHA256 veikia daug greičiau.

### Silpnybės
- Lavinos efektas - kai kuriais atvejais, bitų lygmeniu 2 porų hash'ai gali skirtis tik ~1.6%
- Hash'as yra tik 64 bitų dydžio, todėl yra didesnė tikimybė kolizijoms.
- pad_input funkcijoje, pradinės reikšmės ilgis saugomas tik 8 bituose, tai reiškia, kad bet kurių 2 pradinių reikšmių, kurių ilgio, dalijant iš 256, liekana yra vienoda, ilgio baitas bus vienodas.

### vs. SHA256
- SHA256 funkcija veikia greičiau
- Lavinos efektas mano funkcijoje silpnesnis.

# Projekto struktura

```
hash-function/tree/v0.2/
├── ChatGPT-hash/           # ChatGPT 5 patobulintos maisos funkcijos failai
    ├── cli-ChatGPT.cpp     # Komandines eilutes sasaja
    ├── Hasher-ChatGPT.hpp  # Maisos funkcijos struktura
    └── testing-ChatGPT.cpp # Eksperimentinio tyrimo ir palyginimo su SHA256 kodas
├── hash/                   # v0.1 maisos funkcijos failai
    ├── cli.cpp             # Komandines eilutes sasaja
    ├── Hasher.hpp          # Maisos funkcijos struktura
    └── testing.cpp         # Eksperimentinio tyrimo ir palyginimo su SHA256 kodas
├── test_files/             # Testavimo failai 
├── filegen.py              # Testavimo failu generavimo skriptas
└── sha256.hpp              # SHA256 maisos funkcija
```