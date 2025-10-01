# Maišos funkcija
 ChatGPT 5 patobulinta funkcija
# Pseudokodas
```
# rotate left 64-bit
def rotl64(x: int, r: int) -> int:
    r &= 63
    return ((x << r) & ((1 << 64) - 1)) | (x >> (64 - r))

def mix_round(s: list[int]) -> None:
    # s masyvas sudarytas iš keturių 8 baitų blokų
    s[0] = (s[0] + s[1]) & 0xFFFFFFFFFFFFFFFF
    s[2] = (s[2] + s[3]) & 0xFFFFFFFFFFFFFFFF

    s[1] = rotl64(s[1], 13) ^ s[0]
    s[3] = rotl64(s[3], 16) ^ s[2]

    s[0] = (rotl64(s[0], 32) + s[3]) & 0xFFFFFFFFFFFFFFFF
    s[2] = (rotl64(s[2], 21) + s[1]) & 0xFFFFFFFFFFFFFFFF

    s[1] = rotl64(s[1], 17) ^ s[2]
    s[3] = rotl64(s[3], 32) ^ s[0]

def le64(b: bytes) -> int:
    # 8 baitai perskaitomi į 64 bitų int
    v = 0
    for i in range(8):
        v |= b[i] << (8 * i)
    return v

def absorb32(s: list[int], block32: bytes, rounds: int) -> None:
    # block32 yra 32 baitų ilgio
    m0 = le64(block32[ 0: 8])
    m1 = le64(block32[ 8:16])
    m2 = le64(block32[16:24])
    m3 = le64(block32[24:32])

    # Kiekviena s 8 baitų grupė apdorojama XOR su m0, m1, m2, m3
    s[0] ^= m0; s[1] ^= m1; s[2] ^= m2; s[3] ^= m3

    for _ in range(rounds):
        mix_round(s)

    # s vėl apdorojama XOR su m0, m1, m2, m3 kita tvarka
    s[0] ^= m2; s[1] ^= m3; s[2] ^= m0; s[3] ^= m1

def pad_sha_like_32B(msg: bytes) -> list[bytes]:
    """
    msg || 0x80 || 0x00*… kol ilgis mod 32 = 24, tada 8 baituose nurodomas ilgis.
    """
    n = len(msg)
    rem = n % 32

    buf = bytearray(msg[n - rem:])
    buf.append(0x80)

    # Pridėdami 0x00 baitai, kol len(buf) % 32 == 24
    while (len(buf) % 32) != 24:
        buf.append(0x00)

    # Pridedamas ilgis
    length_le = n.to_bytes(8, 'little')
    buf.extend(length_le)

    # Bendras buf ilgis yra 32 arba 64 baitai
    blocks = []
    for i in range(0, len(buf), 32):
        blk = bytes(buf[i:i+32])
        blocks.append(blk)
    return blocks

def hash128(data: bytes) -> bytes:
    # 1) Konstantos
    s = [
        0x243F6A8885A308D3,
        0x13198A2E03707344,
        0xA4093822299F31D0,
        0x082EFA98EC4E6C89,
    ]

    ROUNDS_PER_BLOCK = 2
    FINAL_ROUNDS = 8

    # 2) Visa įvestis "suabsorb'inama" į 32 baitus
    i = 0
    n = len(data)
    while i + 32 <= n:
        absorb32(s, data[i:i+32], ROUNDS_PER_BLOCK)
        i += 32

    # 3) Pad'inimas ir paskutinis(-iai) blokas(-ai) vėl "suabsorb'inami". Galutinis rezultatas yra 32 baitų ilgio
    padded_blocks = pad_sha_like_32B(data[i:])
    for blk in padded_blocks:
        absorb32(s, blk, ROUNDS_PER_BLOCK)

    # 4) Galutinis sumaišymas
    for _ in range(FINAL_ROUNDS):
        mix_round(s)

    # 5) Hash'as (16 baitų) = (s0 ^ s2) + (s1 ^ s3)
    h0 = (s[0] ^ s[2]) & 0xFFFFFFFFFFFFFFFF
    h1 = (s[1] ^ s[3]) & 0xFFFFFFFFFFFFFFFF
    out = h0.to_bytes(8, 'little') + h1.to_bytes(8, 'little')
    return out
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
- Hash'o ilgis 16 baitų, o v0.1 - 8 baitai.

## Efektyvumas
```
# efektyvumas v0.2
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
- SHA256 vis tiek greitesnis, nors v0.2 nuo SHA256 atsilieka daug mažiau, nei v0.1.

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
# lavinos efektas v0.2
  hamming distance / bits: avg_percentage=50.002%  min=31.250%  max=67.969%
  hamming distance / hex: avg_percentage=93.757%  min=71.875%  max=100.000%

# lavinos efektas sha256
  hamming distance / bits: avg_percentage=49.522%  min=37%  max=62%
  hamming distance / hex: avg_percentage=93.256%  min=78%  max=100%
```
- Vidutinės SHA256 ir v0.2 reikšmės labai panašios - skiriasi tik per ~0.5%.
- SHA256 minimalios reikšmės didesnės ~6.5%.

## Negrįžtamumas
```
# negriztamumas
  input         = 402bd15094118b121548ec670b6607d0
  input + salt1 = 33c0ef3bece4e694b2573b772513bda3
  input + salt2 = 0ba378bd78f0790165c14e3a98741554
```
- Pridėjus salt, hash'o reikšmė vidutiniškai pasikeičia bent ~50% bit'ų lygmeniu (pagal lavinos efekto testus)

## Išvados
### Stiprybės
- Maža kolizijų tikimybė
- Funkcija veikia greitai
- Lavinos efekto testo minimalios, vidutinės ir maksimalios reikšmės arba lenkia, arba nedaug atsilieka nuo SHA256

### Silpnybės
- Greičiu atsilieka nuo SHA256
- Galutinis hash'as yra 128 bitų dydžio (SHA256 - 256 bitai). Tai padidina kolizijų tikimybę.
  
### vs. SHA256
- SHA256 funkcija veikia greičiau
- SHA256 hash'as 256 bitų ilgio, o v0.2 - 128 bitai
- 

# Projekto struktūra

```
hash-function/v0.2/
├── ChatGPT-hash/             # ChatGPT 5 patobulintos maišos funkcijos failai
    ├── cli-ChatGPT.cpp       # Komandinės eilutės sąsaja
    ├── Hasher-ChatGPT.hpp    # Maišos funkcijos struktūra
    └── testing-ChatGPT.cpp   # Eksperimentinio tyrimo ir palyginimo su SHA256 kodas
├── hash/                     # v0.1 maišos funkcijos failai
    ├── cli.cpp               # Komandinės eilutės sąsaja
    ├── Hasher.hpp            # Maišos funkcijos struktūra
    └── testing.cpp           # Eksperimentinio tyrimo ir palyginimo su SHA256 kodas
├── test_files/               # Testavimo failai 
├── filegen.py                # Testavimo failų generavimo skriptas
└── sha256.hpp                # SHA256 maišos funkcija
```



