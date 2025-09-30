# Maišos funkcija
# Pseudokodas
```
def pad_input(input):
    # Užpildome pradinę reikšmę nuliniais baitais ir pridedame
    # pradinės reikšmės ilgį taip, kad bendras ilgis dalintųsi iš 16
    
    p = input
    padZeros = 15 - (len(input) % 16)
    for _ in range(0, padZeros):
        p.append(0x00)
    p.append(len % 256)
    return p
    
def rearrange(padded_input, org_input):
    # Sukeičiame simbolius vietomis “atsitiktine” tvarka.
    # Kaip random generatoriaus “seed” naudojame pradinės reikšmės pirmo,
    # vidurinio ir paskutinio baitų sumą XOR su pradinės reikšmės ilgiu.

    rearranged = padded_input;
    seed = org_input[0] + org_input[len(org_input)/2] + org_input[-1];
    seed = seed XOR len(org_input);
    random = mt19937(seed);
    shuffle(rearranged, random);
    return rearranged;

def mix(a, b):
    # 8 baitų blokus a ir b "sumaišome" į vieną 8 baitų bloką naudojant rotl funckiją, kuri "pasuka"
    # a[n] XOR b[(n + 4) MOD 8] operacijos rezultato bitus į kairę pusę
    # per antrame argumente nurodytą kiekį vietų (šiuo atveju 3)
    # Galutinis rezultatas visada yra 8 baitų ilgio
    
    out = []
    
    for i in range (0, 8):
        out[i] = rotl(a[i] XOR b[(i + 4) MOD 8], 3);
        
    return out

def hash(input):
    first = [ 0x20,0x03,0x01,0x08,0x20,0x03,0x01,0x08 ];
    msg = pad_input(input);
    acc = first
    msg = rearrange(msg, input);

    for (size_t i = 0; i < msg.size(); i += 8) {
        acc = mix(acc, &msg[i]);
    }

    return acc
```

# Eksperimentinis tyrimas

### Visi testavimo failai sugeneruoti ```filegen.py``` skriptu. 

## Išvedimo dydis ir deterministiškumas
```
# isvedimo dydis ir deterministiskumas
test_files/test_1char_1.txt  len=1  hash_len=8  hash=089a404208c04002 vienodi=true
test_files/test_1char_2.txt  len=1  hash_len=8  hash=08c0404294c04002 vienodi=true
test_files/test_1char_3.txt  len=1  hash_len=8  hash=08c0400200c05a02 vienodi=true
test_files/test_1char_4.txt  len=1  hash_len=8  hash=08c040a108c84002 vienodi=true
test_files/test_1char_5.txt  len=1  hash_len=8  hash=08c0404208c0e902 vienodi=true
test_files/test_1char_6.txt  len=1  hash_len=8  hash=48c08c0208c04002 vienodi=true
test_files/test_1char_7.txt  len=1  hash_len=8  hash=08c05a0208c84002 vienodi=true
test_files/test_1char_8.txt  len=1  hash_len=8  hash=08c048027ac04002 vienodi=true
test_files/test_1char_9.txt  len=1  hash_len=8  hash=08c0de4208c04002 vienodi=true
test_files/test_1char_10.txt  len=1  hash_len=8  hash=08c04002088c4042 vienodi=true
test_files/test_1000chars_1.txt  len=1000  hash_len=8  hash=e874b8788bd36748 vienodi=true
test_files/test_1000chars_2.txt  len=1000  hash_len=8  hash=51c6b3974494bbb8 vienodi=true
test_files/test_1000chars_3.txt  len=1000  hash_len=8  hash=6c5e222d8daad576 vienodi=true
test_files/test_1000chars_4.txt  len=1000  hash_len=8  hash=046caa410465dd9b vienodi=true
test_files/test_1000chars_5.txt  len=1000  hash_len=8  hash=1c935bb0446b7e78 vienodi=true
test_files/test_1000chars_6.txt  len=1000  hash_len=8  hash=c06eeef8eaeeb5f1 vienodi=true
test_files/test_1000chars_7.txt  len=1000  hash_len=8  hash=45fe920cd2a6968c vienodi=true
test_files/test_1000chars_8.txt  len=1000  hash_len=8  hash=84e172848486be01 vienodi=true
test_files/test_1000chars_9.txt  len=1000  hash_len=8  hash=b6a6835ba355a61a vienodi=true
test_files/test_1000chars_10.txt  len=1000  hash_len=8  hash=ac81a1f8e6de05f6 vienodi=true
test_files/test_1000chars_chardiff_1.txt  len=1000  hash_len=8  hash=9956ec22d5635d8f vienodi=true
test_files/test_1000chars_chardiff_2.txt  len=1000  hash_len=8  hash=9956ec227d635d8f vienodi=true
test_files/test_1000chars_chardiff_3.txt  len=1000  hash_len=8  hash=9956ec22ed635d8f vienodi=true
test_files/test_1000chars_chardiff_4.txt  len=1000  hash_len=8  hash=9956ec2228635d8f vienodi=true
test_files/test_1000chars_chardiff_5.txt  len=1000  hash_len=8  hash=9956ec22a9635d8f vienodi=true
test_files/test_1000chars_chardiff_6.txt  len=1000  hash_len=8  hash=9956ec22b9635d8f vienodi=true
test_files/test_1000chars_chardiff_7.txt  len=1000  hash_len=8  hash=9956ec223d635d8f vienodi=true
test_files/test_1000chars_chardiff_8.txt  len=1000  hash_len=8  hash=9956ec2249635d8f vienodi=true
test_files/test_1000chars_chardiff_9.txt  len=1000  hash_len=8  hash=9956ec2224635d8f vienodi=true
test_files/test_1000chars_chardiff_10.txt  len=1000  hash_len=8  hash=9956ec2269635d8f vienodi=true
```
- Hash'o ilgis vienodas visais atvejais.
- Kiekvienas failas hash'inamas 2 kartus, abu hash'ai vienodi.

## Efektyvumas
```
# efektyvumas hash
  eiluciu_kiekis=1  avg_ms=0.021
  eiluciu_kiekis=2  avg_ms=0.021
  eiluciu_kiekis=4  avg_ms=0.023
  eiluciu_kiekis=8  avg_ms=0.048
  eiluciu_kiekis=16  avg_ms=0.081
  eiluciu_kiekis=32  avg_ms=0.309
  eiluciu_kiekis=64  avg_ms=0.291
  eiluciu_kiekis=128  avg_ms=0.894
  eiluciu_kiekis=256  avg_ms=1.864
  eiluciu_kiekis=512  avg_ms=4.865
  eiluciu_kiekis=1024  avg_ms=6.164

# efektyvumas SHA256
  eiluciu_kiekis=1  avg_ms=0.001
  eiluciu_kiekis=2  avg_ms=0.001
  eiluciu_kiekis=4  avg_ms=0.000
  eiluciu_kiekis=8  avg_ms=0.000
  eiluciu_kiekis=16  avg_ms=0.001
  eiluciu_kiekis=32  avg_ms=0.001
  eiluciu_kiekis=64  avg_ms=0.002
  eiluciu_kiekis=128  avg_ms=0.005
  eiluciu_kiekis=256  avg_ms=0.010
  eiluciu_kiekis=512  avg_ms=0.029
  eiluciu_kiekis=1024  avg_ms=0.055
  ```
- Kiekvienas skaičius failo ```/test_files/konstitucija.txt``` eilučių (1, 2, 4...) hash'inamos po 10 kartų, išvedamas vidurkis. Matuojama milisekundėmis.
- SHA256 žymiai greitesnis.

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
  hamming distance / bits: avg_percentage=10.489%  min=1.562%  max=76.562%
  hamming distance / hex: avg_percentage=20.939%  min=6.250%  max=100.000%

# lavinos efektas sha256
  hamming distance / bits: avg_percentage=49.519%  min=35%  max=65%
  hamming distance / hex: avg_percentage=93.242%  min=78%  max=100%
```
- SHA256 atveju, 2 vienu simboliu besiskiriančių porų hash'ai vidutiniškai skiriasi daugiau.
- SHA256 minimalios reikšmės taip pat žymiai didesnės.

## Negrįžtamumas
```
# negriztamumas
  input=08774002089a3337
  input + salt1=15595c4e845be349
  input + salt2=557140b5acc06902
```

## Išvados
### Stiprybės
- Maža kolizijų tikimybė.
- Funkcija veikia pakankamai greitai, nors SHA256 veikia daug greičiau.

### Silpnybės
- Lavinos efektas - kai kuriais atvejais, bitų lygmeniu 2 porų hash'ai gali skirtis tik ~1.6%
- Hash'as yra tik 64 bitų dydžio, todėl yra didesnė tikimybė kolizijoms.
- pad_input funkcijoje, pradinės reikšmės ilgis saugomas tik 8 bituose, tai reiškia, kad bet kurių 2 pradinių reikšmių, kurių ilgio, dalijant iš 256, liekana yra vienoda, ilgio baitas bus vienodas.

### vs. SHA256
- SHA256 funkcija veikia greičiau
- Lavinos efektas mano funkcijoje silpnesnis.

